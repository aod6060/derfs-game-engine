#include "game.hpp"


namespace game {
    // GameApp 
    void GameApp::init() {
        global.global_load();
        input::init();
        render::init();
        physics::init();
        sound::init();
        assets::init();
        util::init();
        manager::init();

        // Register Behaviors
        // Entity
        manager::behavior::registerBehavior("Entity.PlayerFPS", BEHAVIOR_INSTANCE(behavior::PlayerFPSEntity));
        manager::behavior::registerBehavior("Entity.CrateTest", BEHAVIOR_INSTANCE(behavior::CrateTestEntity));
        // Scene
        manager::behavior::registerBehavior("Scene.DirectionPhysicsTest", BEHAVIOR_INSTANCE(behavior::DirectionPhysicsTestScene));
        manager::behavior::registerBehavior("Scene.PointPhysicsTest", BEHAVIOR_INSTANCE(behavior::PointPhysicsTestScene));
        manager::behavior::registerBehavior("Scene.SpotPhysicsTest", BEHAVIOR_INSTANCE(behavior::SpotPhysicsTestScene));
        global.startGame();
        global.init();
        global.postInit();
    }

    void GameApp::handleEvent(SDL_Event* e) {
        input::handleEvent(e);
        global.handleEvent(e);
    }

    void GameApp::update(float delta) {
        global.update(delta);
        input::update();
        physics::update();
    }

    void GameApp::render() {
        global.render();
    }

    void GameApp::release() {
        global.release();
        manager::release();
        util::release();
        assets::release();
        sound::release();
        physics::release();
        render::release();
        input::release();
    }


    // Setup GameApp
    void setup(app::Config* config, GameApp* app) {
        config->caption = "Derf's Game Engine";
        config->width = 1280;
        //config->width = 680;
        config->height = 720;
        //config->height = 1080;
        config->app = app;
    }
}