#include "logger.h"
#include <algorithm>
#include <mutex>
#include <vector>

namespace yuki {
/*******************************************************************************
* class Logger
******************************************************************************/
class LoggerPrivate {
public:
  static std::mutex mutex;
  static std::vector<std::shared_ptr<ILoggerListener>> listeners;
};

std::mutex LoggerPrivate::mutex;
std::vector<std::shared_ptr<ILoggerListener>> LoggerPrivate::listeners;

Logger::Logger(const String& tag) {
  LoggerPrivate::mutex.lock();
  for (auto& listener : LoggerPrivate::listeners) {
    listener->write(tag);
    listener->write(TEXT(" "));
  }
}

Logger::~Logger() {
  for (auto& listener : LoggerPrivate::listeners) {
    listener->write(TEXT("\n"));
    listener->flush();
  }
  LoggerPrivate::mutex.unlock();
}

void Logger::write(const String& message) {
  for (auto& listener : LoggerPrivate::listeners) {
    listener->write(message);
  }
}

void Logger::addListener(std::shared_ptr<ILoggerListener> listener) {
  LoggerPrivate::listeners.push_back(std::move(listener));
}

void Logger::removeListener(const std::shared_ptr<ILoggerListener>& listener) {
  LoggerPrivate::listeners.erase(std::remove(LoggerPrivate::listeners.begin(),
                                             LoggerPrivate::listeners.end(),
                                             listener),
                                 LoggerPrivate::listeners.end());
}

Logger Logger::debug() {
  return Logger{TEXT("|DEBUG|")};
}

Logger Logger::error() {
  return Logger{TEXT("|Error|")};
}

Logger Logger::fatal() {
  return Logger{TEXT("|Fatal|")};
}

Logger Logger::trace() {
  return Logger{TEXT("|Trace|")};
}

Logger Logger::info() {
  return Logger{TEXT("|Info|")};
}

Logger Logger::warning() {
  return Logger{TEXT("|Warning|")};
}

} // namespace yuki
