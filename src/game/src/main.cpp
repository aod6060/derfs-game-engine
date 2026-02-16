#include "game.hpp"


int main(int argc, char** argv) {
    game::GameApp app;
    app::Config config;


    game::setup(&config, &app);

    app::init(&config);
    app::update();
    app::release();

    /*
    // Arbutrary rotations in degress
    glm::vec3 rot = glm::vec3(32.0f, 24.0f, 128.0f);

    std::cout << "Original: " << rot.x << ", " << rot.y << ", " << rot.z << "\n";

    // Arbutrary rotation in radians
    glm::vec3 rrot = glm::radians(rot);

    std::cout << "Radians: " << rrot.x << ", " << rrot.y << ", " << rrot.z << "\n";

    float angle = glm::length(rrot);

    std::cout << "Angle: Radian > " << angle << " Degrees > " << glm::degrees(angle) << "\n";

    glm::vec3 nrrot = glm::normalize(rrot);

    std::cout << "Normalize Radians: " << nrrot.x << ", " << nrrot.y << ", " << nrrot.z << "\n";

    // Test: 32, 24, 128
    glm::vec3 test = glm::degrees(nrrot * angle);

    std::cout << "Test: " << test.x << ", " << test.y << ", " << test.z << "\n";
    */

    return 0;
}