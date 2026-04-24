#include "render_hidden.hpp"

namespace render {
    namespace shader {
        namespace geometry {
            static render::glw::UniformBuffer<Camera> camera;

            static MeshShader meshShader;

            void init() {
                camera.init();
                camera.value.proj = glm::mat4(1.0f);
                camera.value.view = glm::mat4(1.0f);
                camera.update();
                camera.bind();
                camera.bufferRange(0);
                camera.unbind();
                meshShader.init();
            }

            void release() {
                meshShader.release();
                camera.release();
            }

            MeshShader* getMeshShader() {
                return &meshShader;
            }

            render::glw::UniformBuffer<Camera>* getCameraUBO() {
                return &camera;
            }

            void MeshShader::init() {
                vertexShader.init(GL_VERTEX_SHADER, "data/shaders/geometry/mesh.vs.glsl");
                fragmentShader.init(GL_FRAGMENT_SHADER, "data/shaders/geometry/mesh.fs.glsl");

                program.init({&vertexShader, &fragmentShader});

                program.bind();

                // UniformBlock
                program.uniformBlock.createUniformBlock("Camera", 0);

                // Uniforms
                program.uniforms.createUniform("model");
                program.uniforms.createUniform("normalMatrix");
                program.uniforms.createUniform("test");

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
                program.attributes.createAttribute("model", 7);

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
                program.attributes.enableAttribute("model", 0);
                program.attributes.enableAttribute("model", 1);
                program.attributes.enableAttribute("model", 2);
                program.attributes.enableAttribute("model", 3);
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

            void MeshShader::modelMatrixPointer() {
                uint32_t vec4Size = sizeof(glm::vec4);
                program.attributes.attributePointer("model", 0, 4, GL_FLOAT, 4 * vec4Size, (const void*)0);
                program.attributes.attributePointer("model", 1, 4, GL_FLOAT, 4 * vec4Size, (const void*)(vec4Size));
                program.attributes.attributePointer("model", 2, 4, GL_FLOAT, 4 * vec4Size, (const void*)(vec4Size * 2));
                program.attributes.attributePointer("model", 3, 4, GL_FLOAT, 4 * vec4Size, (const void*)(vec4Size * 3));
                program.attributes.attributeDivisor("model", 0, 1);
                program.attributes.attributeDivisor("model", 1, 1);
                program.attributes.attributeDivisor("model", 2, 1);
                program.attributes.attributeDivisor("model", 3, 1);
            }

            void MeshShader::drawMesh(render::mesh::Mesh* mesh, render::glw::VertexBuffer* model) {
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

                model->bind();
                this->modelMatrixPointer();
                model->unbind();

                mesh->indencies.bind();
                //render::drawElements(GL_TRIANGLES, mesh->indencies.count());
                render::drawElementsInstance(GL_TRIANGLES, mesh->indencies.count(), model->count() / 16);
                mesh->indencies.unbind();
                
                this->unbindVertexArray();
            }
        }
    }
}