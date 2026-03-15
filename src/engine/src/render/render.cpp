#include "SDL_video.h"
#include "../sys.hpp"



namespace render {

    //static MainShader mainShader;

    void init() {

        SDL_GL_SetSwapInterval(1);
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        //mainShader.init();

        shader::prepass::init();
        shader::geometry::init();
        shader::lighting::init();
        shader::postprocess::init();
    }

    void release() {
        shader::postprocess::release();
        shader::lighting::release();
        shader::geometry::release();
        shader::prepass::release();
        //mainShader.release();
    }

    void clear(glm::vec4 clearColor) {
        glViewport(0, 0, app::getWidthInteger(), app::getHeightInteger());
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void drawArrays(GLenum type, uint32_t vertexCount) {
        glDrawArrays(type, 0, vertexCount);
    }

    void drawElements(GLenum type, uint32_t count) {
        glDrawElements(type, count, GL_UNSIGNED_INT, nullptr);
    }
}