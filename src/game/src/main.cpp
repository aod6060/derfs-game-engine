#include "game.hpp"
#include <fstream>


int main(int argc, char** argv) {
    game::GameApp app;
    app::Config config;
    game::setup(&config, &app);
    app::init(&config);
    app::update();
    app::release();

    /*
    size_t lightSystemSize = sizeof(render::shader::lighting::LightSystem);
    size_t v4Size = sizeof(glm::vec4);

    std::cout << "lightSystemSize: " << lightSystemSize << "\n";
    std::cout << "v4Size: " << v4Size << "\n";

    std::cout << "lightSystemSize / v4Size: " << (lightSystemSize / v4Size) << "\n";
    std::cout << "lightSystemSize % v4Size: " << (lightSystemSize % v4Size) << "\n";
    */

    return 0;
}