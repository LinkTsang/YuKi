#pragma once
#include "core/object.h"
#include <vector>
#include <algorithm>
#include <functional>

namespace yuki {
template <typename HandlerType>
class Event;

template <typename R, typename... Args>
class Event<R(Args ...)> : public Object {
public:
  typedef R (HandlerType)(Args ...);

  Event() = default;

  template <typename F>
  void addHandler(F&& f) {
    handlers_.emplace_back(std::forward<F>(f));
  }

  void removeHandler(const std::function<HandlerType>& handler) {
    handlers_.erase(std::remove(handlers_.begin(), handlers_.end(), handler),
                    handlers_.end());
  }

  void fire(Args ... args) {
    for (const auto& h : handlers_) {
      h(args...);
    }
  }

private:
  std::vector<std::function<HandlerType>> handlers_;
};

class EventArgs : public Object {
public:
  EventArgs() = default;
};


template <typename Sender, typename EventArgs>
using EventHandler = void(Sender sender, EventArgs args);

}
