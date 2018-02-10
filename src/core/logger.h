#pragma once
#include "core/string.hpp"
#include <memory>

namespace yuki {
class ILoggerListener {
public:
  ILoggerListener() = default;
  ILoggerListener(const ILoggerListener&) = default;
  ILoggerListener(ILoggerListener&&) = default;
  ILoggerListener& operator=(const ILoggerListener&) = default;
  ILoggerListener& operator=(ILoggerListener&&) = default;
  virtual ~ILoggerListener() = default;
  virtual void write(const String& message) = 0;
  virtual void flush() {}
};

class Logger {
public:
  explicit Logger(const String& tag);
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger& operator=(Logger&&) = delete;
  ~Logger();

  static void write(const String& message);
  static void addListener(std::shared_ptr<ILoggerListener> listener);
  static void removeListener(const std::shared_ptr<ILoggerListener>& listener);

  static Logger debug();
  static Logger error();
  static Logger fatal();
  static Logger info();
  static Logger trace();
  static Logger warning();
protected:
  Logger(Logger&&) = default;
};

inline Logger& operator<<(Logger& logger, const Char c) {
  Logger::write(String(1, c));
  return logger;
}

inline Logger& operator<<(Logger& logger, const Char* message) {
  Logger::write(message);
  return logger;
}

inline Logger& operator<<(Logger& logger, const String& message) {
  Logger::write(message);
  return logger;
}

inline Logger& operator<<(Logger& logger, const int value) {
  logger << ToString(value);
  return logger;
}

inline Logger& operator<<(Logger& logger, const long value) {
  logger << ToString(value);
  return logger;
}

inline Logger& operator<<(Logger& logger, const long long value) {
  logger << ToString(value);
  return logger;
}

inline Logger& operator<<(Logger& logger, const unsigned int value) {
  logger << ToString(value);
  return logger;
}

inline Logger& operator<<(Logger& logger, const unsigned long value) {
  logger << ToString(value);
  return logger;
}

inline Logger& operator<<(Logger& logger, const unsigned long long value) {
  logger << ToString(value);
  return logger;
}

inline Logger& operator<<(Logger& logger, const float value) {
  logger << ToString(value);
  return logger;
}

inline Logger& operator<<(Logger& logger, const double value) {
  logger << ToString(value);
  return logger;
}

inline Logger& operator<<(Logger& logger, const long double value) {
  logger << ToString(value);
  return logger;
}

template <typename T>
Logger& operator<<(Logger&& logger, T&& value) {
  return logger << std::forward<T>(value);
}
} // namespace yuki
