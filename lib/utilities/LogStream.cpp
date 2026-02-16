#include "LogStream.hpp"

#include <fstream>
#include <iostream>
#include <memory>

namespace pixelarium::utils::log
{
LogStream::LogStream() { ostream_ = std::make_unique<std::ostream>(std::cout.rdbuf()); }

LogStream::LogStream(const std::string& sink)
{
    auto fstream = std::make_unique<std::ofstream>(sink, std::ios::app);
    if (!fstream)
    {
        throw std::runtime_error("Failed to open log stream");
    }

    ostream_ = std::move(fstream);
}

// LogStream& LogStream::operator<<(const std::string& str)
// {
//     (*ostream_) << str;
//     return *this;
// }
}  // namespace pixelarium::utils::log
