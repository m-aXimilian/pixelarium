#include "SpdLogger.hpp"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

#include "ILog.hpp"

using namespace pixelarium::utils::log;

SpdLogger::SpdLogger(const std::string& file_sink, const std::string& name)
    : logger_(spdlog::basic_logger_mt(name, file_sink)), file_(file_sink), name_(name)
{
    spdlog::set_default_logger(this->logger_);
    spdlog::flush_on(spdlog::level::info);
    logger_->info("Logger initiated");
}

void SpdLogger::Info(const std::string& msg) const { this->logger_->info(msg); }
void SpdLogger::Debug(const std::string& msg) const { this->logger_->debug(msg); }
void SpdLogger::Warn(const std::string& msg) const { this->logger_->warn(msg); }
void SpdLogger::Error(const std::string& msg) const { this->logger_->error(msg); }

void SpdLogger::ChangeLevel(LogLevel lvl) const
{
    constexpr auto LogLevelToString = [](LogLevel l) -> const char*
    {
        switch (l)
        {
            case LogLevel::kTrace:
                return "Trace";
            case LogLevel::kDebug:
                return "Debug";
            case LogLevel::kInfo:
                return "Info";
            case LogLevel::kWarn:
                return "Warn";
            case LogLevel::kError:
                return "Error";
            default:
                return "Not Found";
        }
    };

    switch (lvl)
    {
        case LogLevel::kTrace:
            this->logger_->set_level(spdlog::level::trace);
            spdlog::flush_on(spdlog::level::trace);
            break;
        case LogLevel::kInfo:
            this->logger_->set_level(spdlog::level::info);
            spdlog::flush_on(spdlog::level::info);
            break;
        case LogLevel::kWarn:
            this->logger_->set_level(spdlog::level::warn);
            spdlog::flush_on(spdlog::level::warn);
            break;
        case LogLevel::kError:
            this->logger_->set_level(spdlog::level::err);
            spdlog::flush_on(spdlog::level::err);
            break;
        case LogLevel::kDebug:
        default:
            this->logger_->set_level(spdlog::level::debug);
            spdlog::flush_on(spdlog::level::debug);
    }

    // you will only get this message for log levels <= info! I.e., not for error or warning.
    this->logger_->info(
        std::format("{}: Changed log level to  {}({})", __FUNCTION__, LogLevelToString(lvl), static_cast<int>(lvl)));
}
