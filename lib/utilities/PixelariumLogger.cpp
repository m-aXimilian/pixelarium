#include "PixelariumLogger.hpp"

#include <chrono>
#include <fstream>
#include <memory>
#include <ostream>
#include <stdexcept>

using namespace pixelarium::utils::log;

constexpr auto LogLevelToString(LogLevel lvl) -> std::string
{
    switch (lvl)
    {
        case LogLevel::kTrace:
            return "Trace";
        case LogLevel::kDebug:
            return "Debug";
        case LogLevel::kInfo:
            return "Info";
        case LogLevel::kWarn:
            return "Warning";
        case LogLevel::kError:
            return "Error";
    }
}

struct PixelariumLogger::LogStream
{
    LogStream(const std::string& sink)
    {
        out_stream_ = std::ofstream(sink, std::ios::app);
        if (!out_stream_)
        {
            throw std::runtime_error("Failed to open log stream");
        }
    }

    ~LogStream()
    {
        if (out_stream_.is_open())
        {
            out_stream_.close();
        }
    }

    LogStream& operator<<(const std::string& str)
    {
        this->out_stream_ << str;
        return *this;
    }

   private:
    std::ofstream out_stream_;
};

PixelariumLogger::PixelariumLogger(const std::string& name, const std::string& file_sink)
    : name_(name), file_sink_(file_sink)
{
    log_stream_ = std::make_unique<LogStream>(file_sink_);
}

PixelariumLogger::~PixelariumLogger() = default;

auto PixelariumLogger::Write(LogLevel lvl, const std::string& msg) const -> void
{
    if (lvl < this->level_)
    {
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &time_t);
#else
    localtime_r(&time_t, &tm);
#endif

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    auto timestamp = std::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}", tm.tm_year + 1900, tm.tm_mon + 1,
                                 tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ms.count());

    {
        std::lock_guard<std::mutex> guard(mutex_);
        *log_stream_ << std::format( "[{}] [{}] [{}] {}\n", timestamp, name_, LogLevelToString(lvl), msg);
    }
}
