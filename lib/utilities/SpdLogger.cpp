#include "SpdLogger.hpp"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <string>


using namespace pixelarium::utils::log;

SpdLogger::SpdLogger(const std::string& file_sink, const std::string& name)
    : _logger(spdlog::basic_logger_mt(name, file_sink)), _file(file_sink), _name(name)
{
    spdlog::set_default_logger(this->_logger);
    spdlog::flush_on(spdlog::level::info);
    _logger->info("Logger initiated");
}

void SpdLogger::Info(const std::string& msg)
{
    this->_logger->info(msg);
}
void SpdLogger::Debug(const std::string& msg)
{
    this->_logger->debug(msg);
}
void SpdLogger::Warn(const std::string& msg)
{
    this->_logger->warn(msg);
}
void SpdLogger::Error(const std::string& msg)
{
    this->_logger->error(msg);
}