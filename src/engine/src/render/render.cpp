#include "SDL_video.h"
#include "../sys.hpp"



namespace render {

    static MainShader mainShader;

    void init() {

        SDL_GL_SetSwapInterval(1);
        
        glEnable(GL_DEPTH_TEST);

        mainShader.init();
    }

    void release() {
        mainShader.release();
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

    MainShader* getMainShader() {
        return &mainShader;
    }

    // MainShader
    void MainShader::init() {
        vertexShader.init(GL_VERTEX_SHADER, "data/shaders/main.vs.glsl");
        fragmentShader.init(GL_FRAGMENT_SHADER, "data/shaders/main.fs.glsl");

        program.init({&vertexShader, &fragmentShader});

        program.bind();

        program.uniforms.createUniform("proj");
        program.uniforms.createUniform("view");
        program.uniforms.createUniform("model");
        program.uniforms.createUniform("tex0");
        program.uniforms.uniform1i("tex0", 0);
        program.uniforms.createUniform("uvScale");
        program.uniforms.uniform1f("uvScale", 1.0f);

        program.attributes.createAttribute("vertices", 0);
        program.attributes.createAttribute("texCoords", 1);

        program.attributes.bind();
        program.attributes.enableAttribute("vertices");
        program.attributes.enableAttribute("texCoords");
        program.attributes.unbind();

        program.unbind();
    }

    void MainShader::release() {
        program.release();
        fragmentShader.release();
        vertexShader.release();
    }

    void MainShader::bind() {
        program.bind();
    }

    void MainShader::unbind() {
        program.unbind();
    }

    void MainShader::bindVertexArray() {
        program.attributes.bind();
    }

    void MainShader::unbindVertexArray() {
        program.attributes.unbind();
    }

    void MainShader::setProjection(glm::mat4 proj) {
        program.uniforms.uniformMat4("proj", proj);
    }

    void MainShader::setView(glm::mat4 view) {
        program.uniforms.uniformMat4("view", view);
    }

    void MainShader::setModel(glm::mat4 model) {
        program.uniforms.uniformMat4("model", model);
    }

    void MainShader::setUVScale(float value) {
        program.uniforms.uniform1f("uvScale", value);
    }

    void MainShader::verticePointer() {
        program.attributes.attributePointer("vertices", 3, GL_FLOAT);
    }

    void MainShader::texCoordPointer() {
        program.attributes.attributePointer("texCoords", 2, GL_FLOAT);
    }

    void MainShader::drawMesh(render::mesh::Mesh* mesh) {
        this->bindVertexArray();
        mesh->vertices.bind();
        this->verticePointer();
        mesh->texCoords.bind();
        this->texCoordPointer();
        mesh->texCoords.unbind();
        mesh->indencies.bind();
        render::drawElements(GL_TRIANGLES, mesh->indencies.count());
        mesh->indencies.unbind();
        this->unbindVertexArray();
    }

}