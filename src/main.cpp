#include <iostream>

#include "AppGLFW.hpp"

int main(int argc, char** argv)
{
    std::cout << "ok\n";
    auto app = pixelarium::ui::AppGLFW();

    return app.Run();
}