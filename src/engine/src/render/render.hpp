#ifndef RENDER_HPP
#define RENDER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

#define MAX_LIGHTS 128

namespace render {
    // This will be used to build shader for the render
    namespace glw {
        // Shaders
        struct Shader {
            uint32_t id = 0;

            void init(GLenum type, std::string path);
            void release();
        };
        struct Program;

        // Uniform
        struct Uniform {
            Program* program = nullptr;
            std::map<std::string, uint32_t> uniforms;

            void init(Program* program);
            void release();

            void createUniform(std::string name);

            // Integer
            void uniform1i(std::string name, int32_t x);
            void uniform2i(std::string name, int32_t x, int32_t y);
            void uniform3i(std::string name, int32_t x, int32_t y, int32_t z);
            void uniform4i(std::string name, int32_t x, int32_t y, int32_t z, int32_t w);
            // Float
            void uniform1f(std::string name, float x);
            void uniform2f(std::string name, float x, float y);
            void uniform3f(std::string name, float x, float y, float z);
            void uniform4f(std::string name, float x, float y, float z, float w);
            // Matricies
            void uniformMat2(std::string name, const glm::mat2& m);
            void uniformMat3(std::string name, const glm::mat3& m);
            void uniformMat4(std::string name, const glm::mat4& m);
        };

        // Attributes
        struct Attribute {
            Program* program = nullptr;
            uint32_t id = 0;

            std::map<std::string, uint32_t> attributes;

            void init(Program* program);
            void release();

            void bind();
            void unbind();

            void createAttribute(std::string name, uint32_t id);

            void enableAttribute(std::string name);
            void enableAttribute(std::string name, uint32_t index);
            void disableAttribute(std::string name);
            void disableAttribute(std::string name, uint32_t index);

            void attributePointer(std::string name, int size, GLenum type);
            void attributePointer(std::string name, uint32_t index, int size, GLenum type, uint32_t offset, const void* pointer);

            void attributeDivisor(std::string name, uint32_t index, uint32_t divisor);
        };

        struct UniformBlock {
            Program* program = nullptr;
            std::map<std::string, uint32_t> uniformBlocks;

            void init(Program* program);
            void release();
            void createUniformBlock(std::string name, uint32_t index);
        };

        // Program
        struct Program {
            uint32_t id = 0;
            std::vector<Shader*> shaders;
            Uniform uniforms;
            Attribute attributes;
            UniformBlock uniformBlock;

            void init(std::vector<Shader*> shaders);
            void release();

            void bind();
            void unbind();


        };

        // VertexBuffer
        struct VertexBuffer {
            uint32_t id = 0;
            std::vector<float> list;

            void clear();
            void add1f(float x);
            void add2f(float x, float y);
            void add3f(float x, float y, float z);
            void add4f(float x, float y, float z, float w);
            void addVec4(const glm::vec4& v);
            void addMat4(const glm::mat4& m);

            void init();
            void release();
            void update();

            void bind();
            void unbind();

            size_t typeSize();
            size_t count();
            size_t dataSize();
        };

        // IndexBuffer
        struct IndexBuffer {
            uint32_t id = 0;
            std::vector<uint32_t> list;

            void clear();
            void add1ui(uint32_t x);
            void add2ui(uint32_t x, uint32_t y);
            void add3ui(uint32_t x, uint32_t y, uint32_t z);
            void add4ui(uint32_t x, uint32_t y, uint32_t z, uint32_t w);

            void init();
            void release();
            void update();

            void bind();
            void unbind();

            size_t typeSize();
            size_t count();
            size_t dataSize();  
        };

        // UniformBuffers
        template<typename T>
        struct UniformBuffer {
            uint32_t id = 0;
            T value;

            void init() {
                glGenBuffers(1, &this->id);
            }

            void release() {
                glDeleteBuffers(1, &this->id);
            }

            void update() {
                this->bind();
                glBufferData(GL_UNIFORM_BUFFER, typeSize(), &value, GL_DYNAMIC_DRAW);
                this->unbind();
            }

            void bind() {
                glBindBuffer(GL_UNIFORM_BUFFER, this->id);
            }

            void unbind() {
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }

            size_t typeSize() {
                return sizeof(T);
            }

            void bufferRange(uint32_t index) {
                std::cout << "UBO: " << this->id << ", Index: " << index << "\n";
                glBindBufferBase(GL_UNIFORM_BUFFER, index, this->id);
            }
        };

        template<typename T>
        struct UniformBufferArray {
            uint32_t id = 0;
            uint32_t maxCount = 1;
            std::vector<T> list;

            void add(T t) {
                if(list.size() < maxCount) {
                    list.push_back(t);
                }
            }

            void clear() {
                list.clear();
            }

            size_t typeSize() {
                return sizeof(T);
            }

            size_t count() {
                return list.size();
            }

            size_t dataSize() {
                return count() * typeSize();
            }

            void init(uint32_t maxCount) {
                this->maxCount = maxCount;
                glGenBuffers(1, &id);
            }

            void release() {
                this->clear();
                glDeleteBuffers(1, &id);
            }

            void bind() {
                glBindBuffer(GL_UNIFORM_BUFFER, id);
            }

            void unbind() {
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }

            void update() {
                this->bind();
                glBufferData(GL_UNIFORM_BUFFER, this->dataSize(), list.data(), GL_DYNAMIC_DRAW);
                this->unbind();
            }

            void bufferRange(uint32_t index) {
                std::cout << "UBO: " << this->id << ", Index: " << index << "\n";
                glBindBufferBase(GL_UNIFORM_BUFFER, index, this->id);
            }
        };

        // Texture2D
        struct Texture2D {
            uint32_t id = 0;
            uint32_t width = 0;
            uint32_t height = 0;

            void init();
            void release();

            void bind(GLenum active);
            void unbind(GLenum active);

            void texImage2D(int32_t level, int32_t internalFormat, size_t width, size_t height, GLenum format, GLenum type, const void* pixels);
            void texParameter(GLenum type, int32_t value);
            void genMipmaps();

            static void createTextureFromFile(Texture2D* tex, std::string path);
        };

        // Cubemap
        struct Cubemap {
            enum Face {
                EAST = 0,
                WEST,
                UP,
                DOWN,
                NORTH,
                SOUTH,
                FACE_MAX_SIZE
            };

            uint32_t id = 0;

            void init();
            void release();

            void bind(GLenum active);
            void unbind(GLenum active);

            void texParameter(GLenum type, int32_t value);
            void texImage(
                Face face,
                int32_t level,
                int32_t internalFormat,
                size_t width,
                size_t height,
                GLenum format,
                GLenum type,
                const void* pixels
            );

            static void createTextureFromFile(Cubemap* map, std::vector<std::string> paths);
        };

        // Texture2DArrays
        struct Texture2DArray {
            uint32_t id = 0;
            uint32_t width = 0;
            uint32_t height = 0;
            uint32_t count = 1;

            void init();
            void release();
            void bind(GLenum tex);
            void unbind(GLenum tex);
            void texParameter(GLenum type, int32_t value);
            void genMipmaps();

            void texStorage3D(
                size_t levels,
                GLenum internalFormat,
                size_t width,
                size_t height,
                size_t depth
            );

            void texSubImage3D(
                int32_t level,
                int32_t xoffset,
                int32_t yoffset,
                int32_t zoffset,
                size_t width,
                size_t height,
                size_t depth,
                GLenum format,
                GLenum type,
                const void* pixels   
            );

            static void createTextureArrayFromFiles(
                Texture2DArray* tex,
                const std::vector<std::string>& paths
            );

        };

        struct RenderBuffer {
            uint32_t id = 0;
            uint32_t width = 0;
            uint32_t height = 0;

            void init();
            void release();

            void bind();
            void unbind();

            void update(GLenum type, uint32_t width, uint32_t height);

        };

        struct FrameBuffer {
            uint32_t id = 0;

            void init();
            void release();

            void bind();
            void unbind();

            void attachDepthBuffer(Texture2D* tex);
            void attachDepthBuffer(RenderBuffer* renderBuffer);

            void attachColorBuffer(Texture2D* tex, GLenum attachment);

            void drawBuffers(const std::vector<GLenum>& attachment);

            bool wasCreated();
        };
    }

    namespace mesh {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            //glm::vec2 texCoord;
            glm::vec2 albedoTexCoord;
            glm::vec2 metalTexCoord;
            glm::vec2 roughnessTexCoord;
            glm::vec2 emissiveTexCoord;
            glm::vec2 litTexCoord;
        };

        struct Triangle {
            uint32_t v1;
            uint32_t v2;
            uint32_t v3;
        };

        struct MeshData {
            std::vector<Vertex> vertices;
            std::vector<Triangle> triangles;
        };

        struct Mesh {
            MeshData data;

            render::glw::VertexBuffer vertices;
            render::glw::VertexBuffer normals;
            //render::glw::VertexBuffer texCoords;
            render::glw::VertexBuffer albedoTexCoords;
            render::glw::VertexBuffer metalTexCoords;
            render::glw::VertexBuffer roughnessTexCoords;
            render::glw::VertexBuffer emissiveTexCoords;
            render::glw::VertexBuffer litTexCoords;
            
            render::glw::IndexBuffer indencies;

            void init(std::string path);
            void release();

        };
    }

    struct IShader {
        virtual void init() = 0;
        virtual void release() = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void bindVertexArray() = 0;
        virtual void unbindVertexArray() = 0;
    };

    
    namespace shader {
        namespace prepass {
            void init();
            void release();
        }

        namespace geometry {

            struct Camera {
                glm::mat4 proj;
                glm::mat4 view;
            };

            struct MeshShader : public IShader {
                // Shader
                render::glw::Shader vertexShader;
                render::glw::Shader fragmentShader;

                // Program
                render::glw::Program program;

                virtual void init();
                virtual void release();
                virtual void bind();
                virtual void unbind();
                virtual void bindVertexArray();
                virtual void unbindVertexArray();

                void verticesPointer();
                //void texCoordPointer();
                void normalsPointer();
                void albedoTexCoordPointer();
                void metalTexCoordPointer();
                void roughnessTexCoordPointer();
                void emissiveTexCoordPointer();
                void litTexCoordPointer();

                void modelMatrixPointer();

                void drawMesh(render::mesh::Mesh* mesh, render::glw::VertexBuffer* model);
            };

            void init();
            void release();
            MeshShader* getMeshShader();
            render::glw::UniformBuffer<Camera>* getCameraUBO();
        }

        namespace lighting {
            enum LightType {
                LT_DIRECTION = 0,
                LT_POINT,
                LT_SPOT,
                LT_MAX_SIZE
            };

            struct Light {
                int type;
                float ambient;
                float diffuse;
                float specular;
                // All  light type will use this
                // Direction ~ Basically a Direction
                // Point ~ The Position of the point light
                // Spot ~ The Position of the spot light
                glm::vec3 position; 
                float unused1;
                // The color of the light
                glm::vec3 albedo;
                float unused2;
                // Point Light Attenuation
                glm::vec3 unused3;
                float radius;
                // Spot Light Section
                glm::vec3 spotDirection;
                float spotCutOff;
            };

            struct LightSystem {
                Light lights[MAX_LIGHTS];
                int lightSize;
                glm::ivec3 unused;
            };

            struct LightingShader : public IShader {
                // Shader
                render::glw::Shader vertexShader;
                render::glw::Shader fragmentShader;

                // Program
                render::glw::Program program;

                glm::vec3 cameraPosition;
                
                virtual void init();

                virtual void release();

                virtual void bind();

                virtual void unbind();

                virtual void bindVertexArray();

                virtual void unbindVertexArray();

                void setCameraPosition(glm::vec3 pos);
                glm::vec3 getCameraPosition();

                void verticesPointer();

                void texCoordPointer();

            };

            void init();
            void release();

            LightingShader* getLightingShader();

            void addLight(Light light);
            void uploadLights();
            void clearLights();
        }

        namespace postprocess {

            struct PostProcessShader : public IShader {
                std::string fragmentShaderPath;

                // Shader
                render::glw::Shader vertexShader;
                render::glw::Shader fragmentShader;

                // Program
                render::glw::Program program;

                virtual void init();

                virtual void release();

                virtual void bind();

                virtual void unbind();

                virtual void bindVertexArray();

                virtual void unbindVertexArray();
                
                void verticesPointer();

                void texCoordPointer();

                void setFragmentShaderPath(std::string path);

                virtual void buildShader() = 0;
            };

            // Copy Shader
            struct CopyPostProcessShader : public PostProcessShader {
                virtual void buildShader();
            };


            struct GaussianBlurPostProcessShader : public PostProcessShader {
                virtual void buildShader();
                //void setSampleDistance(float value);
                void setWidthDistance(float value);
                void setHeightDistance(float value);
            };

            struct CombinePostProcessShader : public PostProcessShader {
                enum CombineOP {
                    COMBINE_OP_ADD = 0,
                    COMBINE_OP_SUB,
                    COMBINE_OP_MUL,
                    COMBINE_OP_DIV,
                    COMBINE_OP_MIX,
                    COMBINE_OP_TEXTURE,
                    COMBINE_OP_CAMERA_TEXTURE,
                    COMBINE_OP_TEXTURE_CAMERA,
                    COMBINE_OP_CAMERA_TEXTURE_NORMALIZED,
                    COMBINE_OP_TEXTURE_CAMERA_NORMALIZED
                };

                virtual void buildShader();

                void setCombineOp(CombineOP op);
                // If COMBINE_OP_MIX is used it will be 
                // a Value between 0 -> 1 using the mix function
                void setMixValue(float value);

                void setCameraPosition(glm::vec3 cameraPosition);
            };

            struct ThresholdPostProcessShader : public PostProcessShader {
                virtual void buildShader();

                void setMinValue(float value);
                void setMaxValue(float value);
            };

            struct ModifiedEdgeDetectionPostProcessShader : public PostProcessShader {
                virtual void buildShader();
                //void setSampleDistance(float value);
                void setWidthDistance(float value);
                void setHeightDistance(float value);
            };

            struct InvertPostProcessShader : public PostProcessShader {
                virtual void buildShader();
            };

            struct DesaturatePostProcessShader : public PostProcessShader {
                virtual void buildShader();
                void setValue(float value);
            };

            struct AxisPostProcessShader : public PostProcessShader {
                enum Axis {
                    AXIS_X = 0,
                    AXIS_Y,
                    AXIS_Z,
                    AXIS_W
                };

                virtual void buildShader();
                void setAxis(Axis axis);
            };

            void init();
            void release();

            CopyPostProcessShader* getCopyShader();
            GaussianBlurPostProcessShader* getGaussianBlurShader();
            CombinePostProcessShader* getCombineShader();
            ThresholdPostProcessShader* getThresholdShader();
            ModifiedEdgeDetectionPostProcessShader* getModifiedEdgeDetectionShader();
            InvertPostProcessShader* getInvertShader();
            DesaturatePostProcessShader* getDesaturateShader();
            AxisPostProcessShader* getAxisShader();

        }
    }

    struct Material {
        std::string albedo;
        std::string metal;
        std::string roughness;
        std::string emissive;
        std::string lit;

        void init(std::string path);

        void bind();
        void unbind();
    };

    void init();
    void release();

    void clear(glm::vec4 clearColor);
    void clear2D(glm::vec4 clearColor);

    void drawArrays(GLenum type, uint32_t vertexCount);
    void drawElements(GLenum type, uint32_t count);
    void drawElementsInstance(GLenum type, uint32_t indexCount, uint32_t instanceCount);
    
    void submitMeshDraw(std::string mesh, std::string material, const glm::mat4& model);

    void present();
}

#endif