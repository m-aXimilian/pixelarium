#include <memory>

#include "DefaultApp.hpp"
#include "ILog.hpp"
#include "SpdLogger.hpp"
#include "resource.hpp"

using namespace pixelarium;
using namespace std;
using Log = utils::log::ILog;
using Pool = resources::ImageResourcePool;

// setup a logger
#ifdef _WIN32
const auto logger{utils::log::SpdLogger(string(getenv("APPDATA")) + "/pixelarium/simple_app.log", "default")};
#else
const auto logger{
    utils::log::SpdLogger(string(getenv("HOME")) + "/.cache/pixelarium/simple_app.log", "default")};
#endif

int main()
{
    // some initial log message and default log level setting
    logger.Info(format("{}: Starting Application {}", __FUNCTION__, "Pixelarium"));
    logger.ChangeLevel(utils::log::LogLevel::kDebug);

    // instantiate an image pool for the application
    resources::ImageResourcePool image_pool;

    // create an application, inject its dependencies and start it
    auto app{application::DefaultApp(logger, image_pool)};
    app.Start();
}
