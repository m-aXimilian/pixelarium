#pragma once
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "ILog.hpp"
namespace pixelarium::utils::log
{
class SpdLogger : public ILog
{
   public:
    explicit SpdLogger(const std::string& file_sink, const std::string& name);

    void Info(const std::string& msg) override;
    void Debug(const std::string& msg) override;
    void Warn(const std::string& msg) override;
    void Error(const std::string& msg) override;
    void ChangeLevel(LogLevel lvl) override;

   private:
    std::shared_ptr<spdlog::logger> logger_;
    std::string file_;
    std::string name_;
};
}  // namespace pixelarium::utils::log
