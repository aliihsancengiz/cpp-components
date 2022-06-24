#pragma once

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/spdlog.h>

namespace logger {

enum class LogLevel
{
    ERROR,
    WARN,
    CRITICAL,
    INFO,
    DEBUG,
    TRACE
};

enum class LogTarget
{
    CONSOLE,
    STDERR,
    FILE,
    SYSLOG
};

class LoggerConfigurator
{
  public:
    static LoggerConfigurator getInstance();
    void setLogLevel(LogLevel _level);
    void setLogTarget(LogTarget _target);
    std::shared_ptr<spdlog::logger> getLogger(std::string loggerName);

  private:
    LoggerConfigurator();
    static LogLevel level;
    static LogTarget target;
};

}  // namespace logger
