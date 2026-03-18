#include "../sys.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/gtc/matrix_inverse.hpp"

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
                program.uniforms.createUniform("normalMatrix");
                program.uniforms.createUniform("test");
                program.uniforms.uniform1i("test", TestType::TT_REGULAR);

                // uniform sampler2D albedoMaterial;
                program.uniforms.createUniform("albedoMaterial");
                program.uniforms.uniform1i("albedoMaterial", 0);
                // uniform sampler2D metalMaterial;
                program.uniforms.createUniform("metalMaterial");
                program.uniforms.uniform1i("metalMaterial", 1);
                // uniform sampler2D roughnessMaterial;
                program.uniforms.createUniform("roughnessMaterial");
                program.uniforms.uniform1i("roughnessMaterial", 2);
                // uniform sampler2D emissiveMaterial;
                program.uniforms.createUniform("emissiveMaterial");
                program.uniforms.uniform1i("emissiveMaterial", 3);
                // uniform sampler2D litMaterial;
                program.uniforms.createUniform("litMaterial");
                program.uniforms.uniform1i("litMaterial", 4);


                program.attributes.createAttribute("vertices", 0);
                // layout(location=1) in vec3 normals;
                program.attributes.createAttribute("normals", 1);
                // layout(location=2) in vec2 albedoTexCoords;
                program.attributes.createAttribute("albedoTexCoords", 2);
                // layout(location=3) in vec2 metalTexCoords;
                program.attributes.createAttribute("metalTexCoords", 3);
                // layout(location=4) in vec2 roughnessTexCoords;
                program.attributes.createAttribute("roughnessTexCoords", 4);
                // layout(location=5) in vec2 emissiveTexCoords;
                program.attributes.createAttribute("emissiveTexCoords", 5);
                // layout(location=6) in vec2 litTexCoords;
                program.attributes.createAttribute("litTexCoords", 6);
                //program.attributes.createAttribute("texCoords", 1);

                program.attributes.bind();
                program.attributes.enableAttribute("vertices");
                // layout(location=1) in vec3 normals;
                program.attributes.enableAttribute("normals");
                // layout(location=2) in vec2 albedoTexCoords;
                program.attributes.enableAttribute("albedoTexCoords");
                // layout(location=3) in vec2 metalTexCoords;
                program.attributes.enableAttribute("metalTexCoords");
                // layout(location=4) in vec2 roughnessTexCoords;
                program.attributes.enableAttribute("roughnessTexCoords");
                // layout(location=5) in vec2 emissiveTexCoords;
                program.attributes.enableAttribute("emissiveTexCoords");
                // layout(location=6) in vec2 litTexCoords;
                program.attributes.enableAttribute("litTexCoords");

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
                program.uniforms.uniformMat4("normalMatrix", glm::inverseTranspose(model));
            }

            void MeshShader::setTest(int test) {
                program.uniforms.uniform1i("test", test);
            }

            void MeshShader::verticesPointer() {
                program.attributes.attributePointer("vertices", 3, GL_FLOAT);
            }

            void MeshShader::normalsPointer() {
                program.attributes.attributePointer("normals", 3, GL_FLOAT);
            }

            void MeshShader::albedoTexCoordPointer() {
                program.attributes.attributePointer("albedoTexCoords", 2, GL_FLOAT);
            }

            void MeshShader::metalTexCoordPointer() {
                program.attributes.attributePointer("metalTexCoords", 2, GL_FLOAT);
            }

            void MeshShader::roughnessTexCoordPointer() {
                program.attributes.attributePointer("roughnessTexCoords", 2, GL_FLOAT);
            }

            void MeshShader::emissiveTexCoordPointer() {
                program.attributes.attributePointer("emissiveTexCoords", 2, GL_FLOAT);
            }

            void MeshShader::litTexCoordPointer() {
                program.attributes.attributePointer("litTexCoords", 2, GL_FLOAT);
            }


            void MeshShader::drawMesh(render::mesh::Mesh* mesh) {
                this->bindVertexArray();

                mesh->vertices.bind();
                this->verticesPointer();
                mesh->vertices.unbind();
                
                // Normals
                mesh->normals.bind();
                this->normalsPointer();
                mesh->normals.unbind();

                // AlbedoTexCoords
                mesh->albedoTexCoords.bind();
                this->albedoTexCoordPointer();
                mesh->albedoTexCoords.unbind();

                // MetalTexCoords
                mesh->metalTexCoords.bind();
                this->metalTexCoordPointer();
                mesh->metalTexCoords.unbind();

                // RoughnessTexCoords
                mesh->roughnessTexCoords.bind();
                this->roughnessTexCoordPointer();
                mesh->roughnessTexCoords.unbind();

                // EmissiveTexCoords
                mesh->emissiveTexCoords.bind();
                this->emissiveTexCoordPointer();
                mesh->emissiveTexCoords.unbind();

                // LitTexCoords
                mesh->litTexCoords.bind();
                this->litTexCoordPointer();
                mesh->litTexCoords.unbind();

                mesh->indencies.bind();
                render::drawElements(GL_TRIANGLES, mesh->indencies.count());
                mesh->indencies.unbind();
                
                this->unbindVertexArray();
            }
        }
    }
}