#include "../sys.hpp"

namespace render {
    namespace shader {
        namespace geometry {
            static MeshShader meshShader;

            void init() {
                meshShader.init();
            }

            void release() {
                meshShader.release();
            }

            MeshShader* getMeshShader() {
                return &meshShader;
            }

            void MeshShader::init() {
                vertexShader.init(GL_VERTEX_SHADER, "data/shaders/geometry/mesh.vs.glsl");
                fragmentShader.init(GL_FRAGMENT_SHADER, "data/shaders/geometry/mesh.fs.glsl");

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

            void MeshShader::release() {
                program.release();
                fragmentShader.release();
                vertexShader.release();
            }

            void MeshShader::bind() {
                program.bind();
            }

            void MeshShader::unbind() {
                program.unbind();
            }

            void MeshShader::bindVertexArray() {
                program.attributes.bind();
            }

            void MeshShader::unbindVertexArray() {
                program.attributes.unbind();
            }

            void MeshShader::setProjection(glm::mat4 proj) {
                program.uniforms.uniformMat4("proj", proj);
            }

            void MeshShader::setView(glm::mat4 view) {
                program.uniforms.uniformMat4("view", view);
            }

            void MeshShader::setModel(glm::mat4 model) {
                program.uniforms.uniformMat4("model", model);
            }

            void MeshShader::setUVScale(float value) {
                program.uniforms.uniform1f("uvScale", value);
            }

            void MeshShader::verticePointer() {
                program.attributes.attributePointer("vertices", 3, GL_FLOAT);
            }

            void MeshShader::texCoordPointer() {
                program.attributes.attributePointer("texCoords", 2, GL_FLOAT);
            }

            void MeshShader::drawMesh(render::mesh::Mesh* mesh) {
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
    }
}