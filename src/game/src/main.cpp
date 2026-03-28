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
    uint32_t sizeOfSunLight = sizeof(render::shader::lighting::SunLight) + sizeof(glm::vec2);
    uint32_t sizeOfVec4 = sizeof(glm::vec4);

    std::cout << "SunLight Size: " << sizeOfSunLight << "\n";
    std::cout << "vec4 Size: " << sizeOfVec4 << "\n";

    std::cout << "sls/v4s: " << sizeOfSunLight / sizeOfVec4 << "\n";
    std::cout << "sls/v4s: " << sizeOfSunLight % sizeOfVec4 << "\n";
    */
    return 0;
}