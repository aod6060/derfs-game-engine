#include "render_hidden.hpp"



namespace render {
    namespace shader {
        namespace lighting {

            LightingShader lightingShader;
            ::render::glw::UniformBuffer<LightSystem> lightSystem;
            std::vector<Light> lights;

            void init() {
                lightSystem.init();
                lightSystem.bind();
                lightSystem.bufferRange(2);
                lightSystem.unbind();
                lightingShader.init();
            }

            void release() {
                lightingShader.release();
                lightSystem.release();
            }

            LightingShader* getLightingShader() {
                return &lightingShader;
            }

            void addLight(Light light) {
                lights.push_back(light);
            }

            void uploadLights() {
                // Sort
                std::sort(lights.begin(), lights.end(), [&](Light& a, Light& b) {
                    if(a.type == LightType::LT_DIRECTION) {
                        return true;
                    } else {
                        glm::vec3 cameraPos = getLightingShader()->getCameraPosition();

                        float a_dist = glm::abs(glm::length(a.position - cameraPos));
                        float b_dist = glm::abs(glm::length(b.position - cameraPos));

                        return a_dist < b_dist;
                    }
                });

                // Upload
                if(lights.size() > MAX_LIGHTS) {
                    for(int i = 0; i < MAX_LIGHTS; i++) {
                        /*
                            int type;
                        */
                        lightSystem.value.lights[i].type = lights[i].type;
                        /*
                            float ambient;
                        */
                        lightSystem.value.lights[i].ambient = lights[i].ambient;
                        /*
                            float diffuse;
                        */
                        lightSystem.value.lights[i].diffuse = lights[i].diffuse;
                        /*
                            float specular;
                        */
                        lightSystem.value.lights[i].specular = lights[i].specular;
                        // All  light type will use this
                        // Direction ~ Basically a Direction
                        // Point ~ The Position of the point light
                        // Spot ~ The Position of the spot light
                        /*
                            glm::vec3 position; 
                        */
                        lightSystem.value.lights[i].position = lights[i].position;
                        // The color of the light
                        /*
                            glm::vec3 albedo;
                        */
                        lightSystem.value.lights[i].albedo = lights[i].albedo;
                        /*
                        float radius;
                        */
                        lightSystem.value.lights[i].radius = lights[i].radius;
                        /*
                            // Spot Light Section
                            glm::vec3 spotDirection;
                        */
                        lightSystem.value.lights[i].spotDirection = lights[i].spotDirection;
                        /*
                            float spotCutOff;
                        */
                        lightSystem.value.lights[i].spotCutOff = lights[i].spotCutOff;
                    }
                    lightSystem.value.lightSize = MAX_LIGHTS;
                } else {
                    for(int i = 0; i < lights.size(); i++) {
                        /*
                            int type;
                        */
                        lightSystem.value.lights[i].type = lights[i].type;
                        /*
                            float ambient;
                        */
                        lightSystem.value.lights[i].ambient = lights[i].ambient;
                        /*
                            float diffuse;
                        */
                        lightSystem.value.lights[i].diffuse = lights[i].diffuse;
                        /*
                            float specular;
                        */
                        lightSystem.value.lights[i].specular = lights[i].specular;
                        // All  light type will use this
                        // Direction ~ Basically a Direction
                        // Point ~ The Position of the point light
                        // Spot ~ The Position of the spot light
                        /*
                            glm::vec3 position; 
                        */
                        lightSystem.value.lights[i].position = lights[i].position;
                        // The color of the light
                        /*
                            glm::vec3 albedo;
                        */
                        lightSystem.value.lights[i].albedo = lights[i].albedo;
                        /*
                        // Point Light Attenuation
                        float constant;
                        */
                        
                        //lightSystem.value.lights[i].constant = lights[i].constant;
                        /*
                        float linear;
                        */
                        //lightSystem.value.lights[i].linear = lights[i].linear;
                        /*
                        float quadratic;
                        */
                        //lightSystem.value.lights[i].quadratic = lights[i].quadratic;
                        /*
                        float radius;
                        */
                        lightSystem.value.lights[i].radius = lights[i].radius;
                        /*
                            // Spot Light Section
                            glm::vec3 spotDirection;
                        */
                        lightSystem.value.lights[i].spotDirection = lights[i].spotDirection;
                        /*
                            float spotCutOff;
                        */
                        lightSystem.value.lights[i].spotCutOff = lights[i].spotCutOff;
                    }
                    lightSystem.value.lightSize = lights.size();
                }

                lightSystem.update();
            }

            void clearLights() {
                lights.clear();
            }

            void LightingShader::init() {
                vertexShader.init(GL_VERTEX_SHADER, "data/shaders/standard2D.vs.glsl");
                fragmentShader.init(GL_FRAGMENT_SHADER, "data/shaders/lighting/lighting.fs.glsl");

                program.init({&vertexShader, &fragmentShader}); 
                program.bind();

                // UniformBlocks
                program.uniformBlock.createUniformBlock("Standard2DTransform", 1);
                program.uniformBlock.createUniformBlock("LightSystem", 2);

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