#include "game.hpp"


namespace game {
    // GameApp 
    void GameApp::init() {
        // This one will chge
        global.global_load();
        input::init();
        render::init();
        physics::init();
        sound::init();
        assets::init();
        util::init();
        //global.load("data/scenes/testv2.scene.json");
        global.startGame();
        global.init();
    }

    void GameApp::handleEvent(SDL_Event* e) {
        input::handleEvent(e);
        global.handleEvent(e);
    }

    void GameApp::update(float delta) {

        if(maxTime <= time) {
            time = 0.0f;
            std::cout << "FPS: " << (1.0f / delta) << "\n";
        } else {
            time += delta;
        }

        // This is the test section of the code.
        if(input::isKeyPressedOnce(input::Keyboard::KEYS_LALT)) {
            /*
            if(this->testType > render::shader::geometry::MeshShader::TestType::TT_MAX_SIZE - 1) {
                this->testType = 0;
            } else {
                this->testType += 1;

            }
            */

            this->testType += 1;
            if(this->testType > render::shader::geometry::MeshShader::TestType::TT_MAX_SIZE - 1) {
                this->testType = 0;
            }
        }

        render::shader::geometry::getMeshShader()->bind();
        render::shader::geometry::getMeshShader()->setTest(this->testType);
        render::shader::geometry::getMeshShader()->unbind();

        global.update(delta);
        input::update();
        physics::update();
    }

    void GameApp::render() {
        global.render();
    }

    void GameApp::release() {
        global.release();
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