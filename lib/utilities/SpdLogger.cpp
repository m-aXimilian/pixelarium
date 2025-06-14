#include "SpdLogger.hpp"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <sstream>
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

void SpdLogger::Info(const std::string& msg)
{
    this->logger_->info(msg);
}
void SpdLogger::Debug(const std::string& msg)
{
    this->logger_->debug(msg);
}
void SpdLogger::Warn(const std::string& msg)
{
    this->logger_->warn(msg);
}
void SpdLogger::Error(const std::string& msg) { this->logger_->error(msg); }

void SpdLogger::ChangeLevel(LogLevel lvl)
{
    std::stringstream st{};
    st << std::format("with argument {}", static_cast<int>(lvl));
    switch (lvl)
    {
        case LogLevel::Trace:
            this->logger_->set_level(spdlog::level::trace);
            spdlog::flush_on(spdlog::level::trace);
            st << "Trace";
            break;
        case LogLevel::Info:
            this->logger_->set_level(spdlog::level::info);
            spdlog::flush_on(spdlog::level::info);
            st << "Info";
            break;
        case LogLevel::Warn:
            this->logger_->set_level(spdlog::level::warn);
            spdlog::flush_on(spdlog::level::warn);
            st << "Warn";
            break;
        case LogLevel::Error:
            this->logger_->set_level(spdlog::level::err);
            spdlog::flush_on(spdlog::level::err);
            st << "Error";
            break;
        case LogLevel::Debug:
        default:
            this->logger_->set_level(spdlog::level::debug);
            spdlog::flush_on(spdlog::level::debug);
            st << "Debug";
    }

    // you will only get this message for log levels <= info! I.e., not for error or warning.
    this->logger_->info(std::format("{}: Changed log level {}", __FUNCTION__, st.str()).c_str());
}
