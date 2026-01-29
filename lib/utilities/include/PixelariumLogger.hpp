#pragma once

#include <memory>
#include <mutex>
#include <string>

#include "ILog.hpp"

namespace pixelarium::utils::log
{
class PixelariumLogger : public ILog
{
   private:
    struct LogStream;

   public:
    explicit PixelariumLogger(const std::string& name, const std::string& file_sink);
    ~PixelariumLogger();

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
    std::string file_sink_;
    std::unique_ptr<LogStream> log_stream_{};
    LogLevel mutable level_{LogLevel::kDebug};
};
}  // namespace pixelarium::utils::log
