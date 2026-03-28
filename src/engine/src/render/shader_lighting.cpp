#include "../sys.hpp"



namespace render {
    namespace shader {
        namespace lighting {

            LightingShader lightingShader;
            render::glw::UniformBuffer<SunLight> sunLight;

            void init() {
                int amount;

                glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &amount);

                std::cout << "Fragment Uniform Buffers: " << amount << "\n";

                sunLight.init();
                sunLight.value.isOn = GL_TRUE;
                sunLight.value.direction = glm::vec3(1.0f, 1.0f, 0.0f);
                sunLight.value.albedo = glm::vec3(1.0f);
                sunLight.value.ambient = 0.1f;
                sunLight.value.diffuse = 1.0f;
                sunLight.value.specular = 1.0f;
                sunLight.update();
                sunLight.bind();
                sunLight.bufferRange(2);
                sunLight.unbind();
                lightingShader.init();
            }

            void release() {
                lightingShader.release();
                sunLight.release();
            }

            LightingShader* getLightingShader() {
                return &lightingShader;
            }

            render::glw::UniformBuffer<SunLight>* getSunLight() {
                return &sunLight;
            }

            void LightingShader::init() {
                vertexShader.init(GL_VERTEX_SHADER, "data/shaders/standard2D.vs.glsl");
                fragmentShader.init(GL_FRAGMENT_SHADER, "data/shaders/lighting/lighting.fs.glsl");

                program.init({&vertexShader, &fragmentShader}); 
                program.bind();

                // UniformBlocks
                program.uniformBlock.createUniformBlock("Standard2DTransform", 1);
                program.uniformBlock.createUniformBlock("SunLight", 2);

                program.uniforms.createUniform("cameraPosition");
                program.uniforms.createUniform("depthBuffer");
                program.uniforms.uniform1i("depthBuffer", 0);
                program.uniforms.createUniform("positionBuffer");
                program.uniforms.uniform1i("positionBuffer", 1);
                program.uniforms.createUniform("normalBuffer");
                program.uniforms.uniform1i("normalBuffer", 2);
                program.uniforms.createUniform("albedoBuffer");
                program.uniforms.uniform1i("albedoBuffer", 3);
                program.uniforms.createUniform("mrelBuffer");
                program.uniforms.uniform1i("mrelBuffer", 4);
                // Attributes
                program.attributes.createAttribute("vertices", 0);
                program.attributes.createAttribute("texCoords", 1);

                program.attributes.bind();
                program.attributes.enableAttribute("vertices");
                program.attributes.enableAttribute("texCoords");
                program.attributes.unbind();

                program.unbind();
            }

            void LightingShader::release() {
                program.release();
                fragmentShader.release();
                vertexShader.release();
            }

            void LightingShader::bind() {
                program.bind();
            }

            void LightingShader::unbind() {
                program.unbind();
            }

            void LightingShader::setCameraPosition(glm::vec3 pos) {
                this->cameraPosition = pos;
                program.uniforms.uniform3f("cameraPosition", pos.x, pos.y, pos.z);
            }

            glm::vec3 LightingShader::getCameraPosition() {
                return this->cameraPosition;
            }
            void LightingShader::bindVertexArray() {
                program.attributes.bind();
            }

            void LightingShader::unbindVertexArray() {
                program.attributes.unbind();
            }

            void LightingShader::verticesPointer() {
                program.attributes.attributePointer("vertices", 3, GL_FLOAT);
            }

            void LightingShader::texCoordPointer() {
                program.attributes.attributePointer("texCoords", 2, GL_FLOAT);
            }

            
        }
    }
}