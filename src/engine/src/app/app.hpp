#ifndef APP_HPP
#define APP_HPP



#include <SDL3/SDL.h>
#include <string>

namespace app {

    struct IApp {
        virtual void init() = 0;
        virtual void handleEvent(SDL_Event* e) = 0;
        virtual void update(float delta) = 0;
        virtual void render() = 0;
        virtual void release() = 0;
    };

    struct Config {
        std::string caption;
        uint32_t width;
        uint32_t height;
        IApp* app;
    };

    void init(Config* config);
    void update();
    void release();

    std::string getCaption();
    uint32_t getWidthInteger();
    uint32_t getHeightInteger();
    float getWidthFloat();
    float getHeightFloat();
    float getAspect();
    SDL_Window* getWindow();
    SDL_GLContext getContext();
    void exit();
    
}


#endif