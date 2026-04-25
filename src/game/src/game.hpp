#ifndef GAME_HPP
#define GAME_HPP

#include "../../engine/src/sys.hpp"

namespace game {

    struct GameApp : app::IApp {
        manager::Global global;

        //float time = 0.0f;
        //float maxTime = 1.0f;

        virtual void init();
        virtual void handleEvent(SDL_Event* e);
        virtual void update(float delta);
        virtual void render();
        virtual void release();
    };

    void setup(app::Config* config, GameApp* app);
}

#endif