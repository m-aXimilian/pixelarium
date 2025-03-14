#pragma once
#include "ILog.hpp"
namespace pixelarium::utils::log
{
class SpdLogger : public ILog
{
   public:
    SpdLogger(const std::string& file_sink);

    void Info(const std::string& msg,
              const std::string& source = "") override;
    void Debug(const std::string& msg,
               const std::string& source = "") override;
    void Warn(const std::string& msg,
              const std::string& source = "") override;
    void Error(const std::string& msg,
                       const std::string& source = "") override;
};
}  // namespace pixelarium::utils::log