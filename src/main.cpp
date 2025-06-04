#include <iostream>
#include <memory>

#include "AppGLFW.hpp"
#include "resources/resource.hpp"
#include "uiresources.h"
#include "utilities/ILog.hpp"
#include "utilities/SpdLogger.hpp"

int main(int argc, char** argv)
{
    using namespace pixelarium::utils::log;
    using namespace std;
    cout << "ok\n";
    unique_ptr<ILog> logger;
#ifdef _WIN32
      logger = make_unique<SpdLogger>(string(getenv("APPDATA")) + "/pixelarium/logfile.log", "default");
#else
      logger = make_unique<SpdLogger>(std::string(getenv("HOME")) + "/.cache/pixelarium/log.log", "default");
#endif

    auto app = pixelarium::ui::AppGLFW(logger);

    auto image_pool{std::make_unique<pixelarium::resources::ImageResourcePool>()};

    logger->Info(std::format("Starting Application {}", PIXELARIUM_TITLE));
    logger->Error("Starting Application");
    return app.Run();
}
