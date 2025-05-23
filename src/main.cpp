#include <iostream>
#include <memory>

#include "AppGLFW.hpp"
#include "utilities/ILog.hpp"
#include "utilities/SpdLogger.hpp"
#include "uiresources.h"

int main(int argc, char** argv)
{
    using namespace pixelarium::utils::log;
    using namespace std;
    cout << "ok\n";

    unique_ptr<ILog> logger = make_unique<SpdLogger>(string(getenv("APPDATA")) + "/pixelarium/logfile.log", "default");

    auto app = pixelarium::ui::AppGLFW(logger);
    // auto app = pixelarium::ui::AppGLFW();

    logger->Info(std::format("Starting Application {}", PIXELARIUM_TITLE));
    logger->Error("Starting Application");
    return app.Run();
}