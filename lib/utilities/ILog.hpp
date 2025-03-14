#pragma once
#include <string>

namespace pixelarium::utils::log
{
class ILog
{
   public:
    virtual void Info(const std::string& msg,
                      const std::string& source = "") = 0;
    virtual void Debug(const std::string& msg,
                       const std::string& source = "") = 0;
    virtual void Warn(const std::string& msg,
                      const std::string& source = "") = 0;
    virtual void Error(const std::string& msg,
                       const std::string& source = "") = 0;

    virtual ~ILog() {}
};

}  // namespace pixelarium::utils