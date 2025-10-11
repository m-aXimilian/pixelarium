#include <memory>
#include <string>

#include "DefaultApp.hpp"
#include "resources/resource.hpp"
#include "utilities/ILog.hpp"
#include "utilities/SpdLogger.hpp"

using namespace pixelarium;
using namespace std;
using Log = utils::log::ILog;
using Pool = resources::ImageResourcePool;

// setup a logger
#ifdef _WIN32
unique_ptr<Log> logger =
    make_unique<utils::log::SpdLogger>(string(getenv("APPDATA")) + "/pixelarium/custom_app.log", "default");
#else
unique_ptr<Log> logger =
    make_unique<utils::log::SpdLogger>(string(getenv("HOME")) + "/.cache/pixelarium/custom_app.log", "default");
#endif

// create a custom app inheriting from the library's default app
class MyApp : public application::DefaultApp
{
   public:
    MyApp(const Log& log, Pool& pool) : application::DefaultApp(log, pool) {}

    // override some of the defaults member functions
    void Run() override;
    void MenuBarOptionsColumn1() override {};
    void MenuBarOptionsColumn2() override {};
};

int main()
{
    // some initial log message
    logger->Info(std::format("{}: Starting Application {}", __FUNCTION__, "Pixelarium"));

    // instantiate an image pool for the application
    auto image_pool{std::make_unique<resources::ImageResourcePool>()};

    // create a custom application, inject its dependencies and start it
    auto app = MyApp(*logger, *image_pool);

    app.Start();
}

void MyApp::Run()
{
    this->gallery_.RenderGallery();
    this->gallery_.RenderImages();
}
