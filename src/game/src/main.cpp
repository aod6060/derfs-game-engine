#include "game.hpp"


int main(int argc, char** argv) {
    game::GameApp app;
    app::Config config;


    game::setup(&config, &app);

    app::init(&config);
    app::update();
    app::release();
    
    return 0;
}