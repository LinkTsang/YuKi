#pragma once
#include <algorithm>
#include <functional>
#include <vector>

class PropertyBase {
 public:
  // evaluate
  PropertyBase() = default;
  virtual void evaluate(PropertyBase* source) = 0;
  virtual ~PropertyBase() {
    for (const auto& observer : observers_) {
      auto& dependencies = observer->dependencies_;
      if (!dependencies.empty()) {
        dependencies.erase(
            std::remove(dependencies.begin(), dependencies.end(), this),
            dependencies.end());
      }
    }
  }
  // notify
  void notify() {
    for (auto observer : observers_) {
      observer->evaluate(this);
    }
  }

 protected:
  std::vector<PropertyBase*> observers_;
  std::vector<PropertyBase*> dependencies_;
};

template <typename T>
class Property : public PropertyBase {
 public:
  using Binding = std::function<T()>;
  Property() = default;
  ~Property() = default;
  template <typename U>
  Property(const Property<U>& value) {
    value_ = value.get();
  }
  Property(const T& t) : value_(t) {}
  Property(T&& value) : value_(std::move(value)) {}

  template <typename... Args>
  void bind(Binding binding, Property<Args>&... args) {
    { [[maybe_unused]] int unused[] = {0, ((void)bind(args), 0)...}; }
    binding_ = binding;
    evaluate(this);
  }

  // setters
  void operator=(const T& t) {
    value_ = t;
    notify();
  }
  void operator=(T&& t) {
    value_ = std::move(t);
    notify();
  }

  // getters
  const T& get() const { return value_; }
  const T& operator()() const { return get(); }
  operator const T&() const { return get(); }

  // bind
  template <typename U>
  void bind(Property<U>& value) {
    dependencies_.push_back(&value);
    value.observers_.push_back(this);
  }

  // evaluate
  virtual void evaluate(PropertyBase* source) override {
    if (binding_) {
      value_ = binding_();
      notify();
    }
  }

 protected:
  T value_;
  Binding binding_;
};
