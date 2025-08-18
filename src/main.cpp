#include <iostream>
#include <memory>

#include "MyApp.hpp"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
#include "utilities/SpdLogger.hpp"

int main(int argc, char** argv)
{
    using namespace pixelarium;
    using namespace std;
    cout << "ok\n";
    unique_ptr<utils::log::ILog> logger;
#ifdef _WIN32
    logger = make_unique<utils::log::SpdLogger>(string(getenv("APPDATA")) + "/pixelarium/logfile.log", "default");
#else
    logger = make_unique<utils::log::SpdLogger>(std::string(getenv("HOME")) + "/.cache/pixelarium/log.log", "default");
#endif
    logger->Info(std::format("{}: Starting Application {}", __FUNCTION__, "Pixelarium"));

    logger->ChangeLevel(utils::log::LogLevel::Debug);
    auto image_pool{std::make_unique<resources::ImageResourcePool>()};

    pixelarium::ui::MyApp app = pixelarium::ui::MyApp(*logger, *image_pool);

    app.Start();
}
