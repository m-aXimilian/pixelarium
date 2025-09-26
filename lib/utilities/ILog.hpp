#pragma once
#include <string>

namespace pixelarium::utils::log
{
enum class LogLevel
{
    Trace = 1 << 0,
    Debug = 1 << 1,
    Info = 1 << 2,
    Warn = 1 << 3,
    Error = 1 << 4,
};
/// @brief Interface for logging implementations.
class ILog
{
   public:
    virtual void Info(const std::string& msg) const = 0;
    virtual void Debug(const std::string& msg) const = 0;
    virtual void Warn(const std::string& msg) const = 0;
    virtual void Error(const std::string& msg) const = 0;
    virtual void ChangeLevel(LogLevel lvl) const = 0;

    virtual ~ILog() {}
};

}  // namespace pixelarium::utils::log
