#pragma once

#include <memory>
#include <ostream>
#include <string>

namespace pixelarium::utils::log
{
struct LogStream
{
    LogStream();
    LogStream(const std::string& filesink);
    ~LogStream() = default;
    LogStream(LogStream&) = delete;
    LogStream(const LogStream&) = delete;
    LogStream(LogStream&&) = delete;
    LogStream& operator=(LogStream&) = delete;
    LogStream& operator=(LogStream&&) = delete;

    LogStream& operator<<(const std::string& str)
    {
        if (ostream_)
        {
            (*ostream_) << str;   
        }

        return *this;
    }

   private:
    std::unique_ptr<std::ostream> ostream_;
};
}  // namespace pixelarium::utils::log
