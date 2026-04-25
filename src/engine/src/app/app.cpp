#include "app_hidden.hpp"
#include <sstream>


namespace app {

    static Config* g_config = nullptr;
    static SDL_Window* g_window = nullptr;
    static SDL_GLContext g_context = nullptr;
    static bool g_isRunning = true;


    float maxTime = 1.0f;
    float currTime = 0.0f;

    void init(Config* config) {
        g_config = config;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        g_window = SDL_CreateWindow(g_config->caption.c_str(), g_config->width, g_config->height, SDL_WINDOW_OPENGL);
        g_context = SDL_GL_CreateContext(g_window);
        glewInit();

        if(g_config->app) {
            g_config->app->init();
        }
    }

    void update() {

        SDL_Event event;

        uint32_t pre_time = SDL_GetTicks();
        uint32_t curr_time = 0;
        float delta = 0.0f;

        while(g_isRunning) {
            // Handle Timing
            curr_time = SDL_GetTicks();
            delta = (curr_time - pre_time) / 1000.0f;
            pre_time = curr_time;

            if(maxTime <= currTime) {
                currTime = 0.0f;
                //std::cout << "FPS: " << (1.0f / delta) << "\n";
                std::stringstream ss;
                ss << g_config->caption << " FPS: (" << (1.0f / delta) << ")";
                SDL_SetWindowTitle(g_window, ss.str().c_str());
            } else {
                currTime += delta;
            }

            // Handle Event Loop
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_EVENT_QUIT) {
                    app::exit();
                }

                // Handle Calls to app interface
                if(g_config->app) {
                    g_config->app->handleEvent(&event);
                }
            }

            // Handle Calls to app interface
            if(g_config->app) {
                g_config->app->update(delta);
                g_config->app->render();
            }

            // Swap Widnow
            SDL_GL_SwapWindow(g_window);
        }
    }

    void release() {
        if(g_config->app) {
            g_config->app->release();
        }
        //SDL_GL_DeleteContext(g_context);
        SDL_GL_DestroyContext(g_context);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
    }

    std::string getCaption() {
        return g_config->caption;
    }

    uint32_t getWidthInteger() {
        return g_config->width;
    }

    uint32_t getHeightInteger() {
        return g_config->height;
    }

    float getWidthFloat() {
        return (float)getWidthInteger();
    }

    float getHeightFloat() {
        return (float)getHeightInteger();
    }

    float getAspect() {
        return getWidthFloat() / getHeightFloat();
    }

    SDL_Window* getWindow() {
        return g_window;
    }

    SDL_GLContext getContext() {
        return g_context;
    }

    void exit() {
        g_isRunning = false;
    }

}