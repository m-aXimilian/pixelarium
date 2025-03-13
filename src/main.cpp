#include <iostream>

#include "AppGLFW.hpp"

int main(int argc, char** argv)
{
    std::cout << "ok\n";
    auto app = ui::AppGLFW();

    return app.Run();
}