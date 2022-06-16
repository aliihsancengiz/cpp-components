#include "Logger.hpp"

namespace logger
{

    LogLevel LoggerConfigurator::level = LogLevel::ERROR;
    LogTarget LoggerConfigurator::target = LogTarget::CONSOLE;

    LoggerConfigurator::LoggerConfigurator()
    {
        spdlog::set_level(spdlog::level::err);
        spdlog::set_pattern("[%D] [%H:%M:%S] [%l] [%n] %v");
    }

    LoggerConfigurator LoggerConfigurator::getInstance()
    {
        static LoggerConfigurator loggerConfigurator;
        return loggerConfigurator;
    }
    void LoggerConfigurator::setLogLevel(LogLevel _level)
    {
        level = _level;
        switch (level)
        {
        case LogLevel::WARN:
            spdlog::set_level(spdlog::level::warn);
            break;

        case LogLevel::CRITICAL:
            spdlog::set_level(spdlog::level::critical);
            break;

        case LogLevel::DEBUG:
            spdlog::set_level(spdlog::level::debug);
            break;

        case LogLevel::TRACE:
            spdlog::set_level(spdlog::level::trace);
            break;

        case LogLevel::ERROR:
            spdlog::set_level(spdlog::level::err);
            break;

        case LogLevel::INFO:
            spdlog::set_level(spdlog::level::info);
            break;
        default:
            break;
        }
    }
    void LoggerConfigurator::setLogTarget(LogTarget _target)
    {
        target = _target;
    }

    std::shared_ptr<spdlog::logger> LoggerConfigurator::getLogger(std::string loggerName)
    {
        if (target == LogTarget::CONSOLE)
        {
            return spdlog::stdout_color_mt(loggerName);
        }
        else if (target == LogTarget::FILE)
        {
            return spdlog::basic_logger_mt(loggerName, "basic-log.txt");
        }
        else if (target == LogTarget::STDERR)
        {
            return spdlog::stderr_color_mt(loggerName);
        }
        else if (target == LogTarget::SYSLOG)
        {
            return spdlog::syslog_logger_mt(loggerName, "example", LOG_PID);
        }

        return spdlog::get(loggerName);
    }

} // namespace logger
