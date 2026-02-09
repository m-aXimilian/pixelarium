#pragma once

#include <memory>
#include <mutex>
#include <string>

#include "ILog.hpp"
#include "LogStream.hpp"

namespace pixelarium::utils::log
{
class PixelariumLogger : public ILog
{
   public:
    PixelariumLogger(const std::string& name)
        : pixelarium::utils::log::PixelariumLogger(name, std::make_unique<LogStream>()) {}
    PixelariumLogger(const std::string& name, const std::string& filesink) : pixelarium::utils::log::PixelariumLogger(name, std::make_unique<LogStream>(filesink)) {}
    explicit PixelariumLogger(const std::string& name, std::unique_ptr<LogStream> str) : name_(name), log_stream_(std::move(str)) {}
    ~PixelariumLogger() = default;

    void Info(const std::string& msg) const override { this->Write(LogLevel::kInfo, msg); }
    void Debug(const std::string& msg) const override { this->Write(LogLevel::kDebug, msg); }
    void Warn(const std::string& msg) const override { this->Write(LogLevel::kWarn, msg); }
    void Error(const std::string& msg) const override { this->Write(LogLevel::kError, msg); }
    void ChangeLevel(LogLevel lvl) const override { this->level_ = lvl; }

   private:
    void Write(LogLevel, const std::string&) const;

   private:
    std::mutex mutable mutex_;
    std::string name_;
    // std::string file_sink_;
    std::unique_ptr<LogStream> log_stream_;
    LogLevel mutable level_{LogLevel::kDebug};
};
}  // namespace pixelarium::utils::log
