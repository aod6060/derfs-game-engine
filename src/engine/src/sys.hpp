#ifndef SYS_HPP
#define SYS_HPP

#include <cfloat>
// Once this file gets above 2000 to 3000 lines of code I'll refactor it.
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btStridingMeshInterface.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "vorbis/vorbisfile.h"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <deque>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <functional>
#include <algorithm>
#include <random>
#include <chrono>
#include <filesystem>

#include <SDL.h>
#include <SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <json/json.h>

#include <lua/lua.hpp>
#include <lua/lualib.hpp>
#include <lua/lauxlib.hpp>

#include <btBulletDynamicsCommon.h>

#include <AL/al.h>
#include <AL/alc.h>

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

namespace input {
    enum InputState {
        IS_RELEASED = 0,
        IS_PRESSED_ONCE,
        IS_PRESSED,
        IS_RELEASED_ONCE,
        IS_MAX_SIZE
    };

    // This interface is based on "SDL_scancode.h" 
    // It was renamed out of convience...
    enum Keyboard {
        KEYS_UNKNOWN = 0,
        KEYS_A = 4,
        KEYS_B = 5,
        KEYS_C = 6,
        KEYS_D = 7,
        KEYS_E = 8,
        KEYS_F = 9,
        KEYS_G = 10,
        KEYS_H = 11,
        KEYS_I = 12,
        KEYS_J = 13,
        KEYS_K = 14,
        KEYS_L = 15,
        KEYS_M = 16,
        KEYS_N = 17,
        KEYS_O = 18,
        KEYS_P = 19,
        KEYS_Q = 20,
        KEYS_R = 21,
        KEYS_S = 22,
        KEYS_T = 23,
        KEYS_U = 24,
        KEYS_V = 25,
        KEYS_W = 26,
        KEYS_X = 27,
        KEYS_Y = 28,
        KEYS_Z = 29,
        KEYS_1 = 30,
        KEYS_2 = 31,
        KEYS_3 = 32,
        KEYS_4 = 33,
        KEYS_5 = 34,
        KEYS_6 = 35,
        KEYS_7 = 36,
        KEYS_8 = 37,
        KEYS_9 = 38,
        KEYS_0 = 39,
        KEYS_RETURN = 40,
        KEYS_ESCAPE = 41,
        KEYS_BACKSPACE = 42,
        KEYS_TAB = 43,
        KEYS_SPACE = 44,
        KEYS_MINUS = 45,
        KEYS_EQUALS = 46,
        KEYS_LEFTBRACKET = 47,
        KEYS_RIGHTBRACKET = 48,
        KEYS_BACKSLASH = 49,
        KEYS_NONUSHASH = 50,
        KEYS_SEMICOLON = 51,
        KEYS_APOSTROPHE = 52,
        KEYS_GRAVE = 53,
        KEYS_COMMA = 54,
        KEYS_PERIOD = 55,
        KEYS_SLASH = 56,
        KEYS_CAPSLOCK = 57,
        KEYS_F1 = 58,
        KEYS_F2 = 59,
        KEYS_F3 = 60,
        KEYS_F4 = 61,
        KEYS_F5 = 62,
        KEYS_F6 = 63,
        KEYS_F7 = 64,
        KEYS_F8 = 65,
        KEYS_F9 = 66,
        KEYS_F10 = 67,
        KEYS_F11 = 68,
        KEYS_F12 = 69,
        KEYS_PRINTSCREEN = 70,
        KEYS_SCROLLLOCK = 71,
        KEYS_PAUSE = 72,
        KEYS_INSERT = 73,
        KEYS_HOME = 74,
        KEYS_PAGEUP = 75,
        KEYS_DELETE = 76,
        KEYS_END = 77,
        KEYS_PAGEDOWN = 78,
        KEYS_RIGHT = 79,
        KEYS_LEFT = 80,
        KEYS_DOWN = 81,
        KEYS_UP = 82,
        KEYS_NUMLOCKCLEAR = 83,
        KEYS_KP_DIVIDE = 84,
        KEYS_KP_MULTIPLY = 85,
        KEYS_KP_MINUS = 86,
        KEYS_KP_PLUS = 87,
        KEYS_KP_ENTER = 88,
        KEYS_KP_1 = 89,
        KEYS_KP_2 = 90,
        KEYS_KP_3 = 91,
        KEYS_KP_4 = 92,
        KEYS_KP_5 = 93,
        KEYS_KP_6 = 94,
        KEYS_KP_7 = 95,
        KEYS_KP_8 = 96,
        KEYS_KP_9 = 97,
        KEYS_KP_0 = 98,
        KEYS_KP_PERIOD = 99,
        KEYS_NONUSBACKSLASH = 100,
        KEYS_APPLICATION = 101,
        KEYS_POWER = 102,
        KEYS_KP_EQUALS = 103,
        KEYS_F13 = 104,
        KEYS_F14 = 105,
        KEYS_F15 = 106,
        KEYS_F16 = 107,
        KEYS_F17 = 108,
        KEYS_F18 = 109,
        KEYS_F19 = 110,
        KEYS_F20 = 111,
        KEYS_F21 = 112,
        KEYS_F22 = 113,
        KEYS_F23 = 114,
        KEYS_F24 = 115,
        KEYS_EXECUTE = 116,
        KEYS_HELP = 117,
        KEYS_MENU = 118,
        KEYS_SELECT = 119,
        KEYS_STOP = 120,
        KEYS_AGAIN = 121,
        KEYS_UNDO = 122,
        KEYS_CUT = 123,
        KEYS_COPY = 124,
        KEYS_PASTE = 125,
        KEYS_FIND = 126,
        KEYS_MUTE = 127,
        KEYS_VOLUMEUP = 128,
        KEYS_VOLUMEDOWN = 129,
        KEYS_KP_COMMA = 133,
        KEYS_KP_EQUALSAS400 = 134,
        KEYS_INTERNATIONAL1 = 135,
        KEYS_INTERNATIONAL2 = 136,
        KEYS_INTERNATIONAL3 = 137,
        KEYS_INTERNATIONAL4 = 138,
        KEYS_INTERNATIONAL5 = 139,
        KEYS_INTERNATIONAL6 = 140,
        KEYS_INTERNATIONAL7 = 141,
        KEYS_INTERNATIONAL8 = 142,
        KEYS_INTERNATIONAL9 = 143,
        KEYS_LANG1 = 144,
        KEYS_LANG2 = 145,
        KEYS_LANG3 = 146,
        KEYS_LANG4 = 147,
        KEYS_LANG5 = 148,
        KEYS_LANG6 = 149,
        KEYS_LANG7 = 150,
        KEYS_LANG8 = 151,
        KEYS_LANG9 = 152,
        KEYS_ALTERASE = 153,
        KEYS_SYSREQ = 154,
        KEYS_CANCEL = 155,
        KEYS_CLEAR = 156,
        KEYS_PRIOR = 157,
        KEYS_RETURN2 = 158,
        KEYS_SEPARATOR = 159,
        KEYS_OUT = 160,
        KEYS_OPER = 161,
        KEYS_CLEARAGAIN = 162,
        KEYS_CRSEL = 163,
        KEYS_EXSEL = 164,
        KEYS_KP_00 = 176,
        KEYS_KP_000 = 177,
        KEYS_THOUSANDSSEPARATOR = 178,
        KEYS_DECIMALSEPARATOR = 179,
        KEYS_CURRENCYUNIT = 180,
        KEYS_CURRENCYSUBUNIT = 181,
        KEYS_KP_LEFTPAREN = 182,
        KEYS_KP_RIGHTPAREN = 183,
        KEYS_KP_LEFTBRACE = 184,
        KEYS_KP_RIGHTBRACE = 185,
        KEYS_KP_TAB = 186,
        KEYS_KP_BACKSPACE = 187,
        KEYS_KP_A = 188,
        KEYS_KP_B = 189,
        KEYS_KP_C = 190,
        KEYS_KP_D = 191,
        KEYS_KP_E = 192,
        KEYS_KP_F = 193,
        KEYS_KP_XOR = 194,
        KEYS_KP_POWER = 195,
        KEYS_KP_PERCENT = 196,
        KEYS_KP_LESS = 197,
        KEYS_KP_GREATER = 198,
        KEYS_KP_AMPERSAND = 199,
        KEYS_KP_DBLAMPERSAND = 200,
        KEYS_KP_VERTICALBAR = 201,
        KEYS_KP_DBLVERTICALBAR = 202,
        KEYS_KP_COLON = 203,
        KEYS_KP_HASH = 204,
        KEYS_KP_SPACE = 205,
        KEYS_KP_AT = 206,
        KEYS_KP_EXCLAM = 207,
        KEYS_KP_MEMSTORE = 208,
        KEYS_KP_MEMRECALL = 209,
        KEYS_KP_MEMCLEAR = 210,
        KEYS_KP_MEMADD = 211,
        KEYS_KP_MEMSUBTRACT = 212,
        KEYS_KP_MEMMULTIPLY = 213,
        KEYS_KP_MEMDIVIDE = 214,
        KEYS_KP_PLUSMINUS = 215,
        KEYS_KP_CLEAR = 216,
        KEYS_KP_CLEARENTRY = 217,
        KEYS_KP_BINARY = 218,
        KEYS_KP_OCTAL = 219,
        KEYS_KP_DECIMAL = 220,
        KEYS_KP_HEXADECIMAL = 221,
        KEYS_LCTRL = 224,
        KEYS_LSHIFT = 225,
        KEYS_LALT = 226,
        KEYS_LGUI = 227,
        KEYS_RCTRL = 228,
        KEYS_RSHIFT = 229,
        KEYS_RALT = 230,
        KEYS_RGUI = 231,
        KEYS_MODE = 257,
        KEYS_AUDIONEXT = 258,
        KEYS_AUDIOPREV = 259,
        KEYS_AUDIOSTOP = 260,
        KEYS_AUDIOPLAY = 261,
        KEYS_AUDIOMUTE = 262,
        KEYS_MEDIASELECT = 263,
        KEYS_WWW = 264,
        KEYS_MAIL = 265,
        KEYS_CALCULATOR = 266,
        KEYS_COMPUTER = 267,
        KEYS_AC_SEARCH = 268,
        KEYS_AC_HOME = 269,
        KEYS_AC_BACK = 270,
        KEYS_AC_FORWARD = 271,
        KEYS_AC_STOP = 272,
        KEYS_AC_REFRESH = 273,
        KEYS_AC_BOOKMARKS = 274,
        KEYS_BRIGHTNESSDOWN = 275,
        KEYS_BRIGHTNESSUP = 276,
        KEYS_DISPLAYSWITCH = 277,
        KEYS_KBDILLUMTOGGLE = 278,
        KEYS_KBDILLUMDOWN = 279,
        KEYS_KBDILLUMUP = 280,
        KEYS_EJECT = 281,
        KEYS_SLEEP = 282,
        KEYS_APP1 = 283,
        KEYS_APP2 = 284,
        KEYS_AUDIOREWIND = 285,
        KEYS_AUDIOFASTFORWARD = 286,
        KEYS_SOFTLEFT = 287,
        KEYS_SOFTRIGHT = 288,
        KEYS_CALL = 289,
        KEYS_ENDCALL = 290,
        KEYS_MAX_SIZE = 512
    };

    enum MouseButtons {
        MBS_LEFT = 0,
        MBS_CENTER,
        MBS_RIGHT,
        MBS_MAX_SIZE
    };

    void init();
    void release();
    void handleEvent(SDL_Event* e);
    void update();

    // Keyboard
    bool isKeyReleased(Keyboard k);
    bool isKeyPressedOnce(Keyboard k);
    bool isKeyPressed(Keyboard k);
    bool isKeyReleasedOnce(Keyboard k);

    float getKeyReleasedValue(Keyboard k);
    float getKeyPressedOnceValue(Keyboard k);
    float getKeyPressedValue(Keyboard k);
    float getKeyReleasedOnceValue(Keyboard k);

    float getKeyReleasedAxis(Keyboard negative, Keyboard positive);
    float getKeyPressedOnceAxis(Keyboard negative, Keyboard positive);
    float getKeyPressedAxis(Keyboard negative, Keyboard positive);
    float getKeyReleasedOnceAxis(Keyboard negative, Keyboard positive);

    // Mouse
    glm::vec2 toPosition();
    glm::vec2 toVelocity();
    glm::vec2 toMouseWheel();
    
    bool isMouseButtonReleased(MouseButtons mb);
    bool isMouseButtonPressedOnce(MouseButtons mb);
    bool isMouseButtonPressed(MouseButtons mb);
    bool isMouseButtonReleasedOnce(MouseButtons mb);

    float getMouseButtonReleasedValue(MouseButtons mb);
    float getMouseButtonPressedOnceValue(MouseButtons mb);
    float getMouseButtonPressedValue(MouseButtons mb);
    float getMouseButtonReleasedOnceValue(MouseButtons mb);

    float getMouseButtonReleasedAxis(MouseButtons negative, MouseButtons positive);
    float getMouseButtonPressedOnceAxis(MouseButtons negative, MouseButtons positive);
    float getMouseButtonPressedAxis(MouseButtons negative, MouseButtons positive);
    float getMouseButtonReleasedOnceAxis(MouseButtons negative, MouseButtons positive);

    // Input Grab
    bool isGrab();
    void setGrab(bool grab);
    void toggleGrab();


    namespace mapping {
        struct Mapping {
            Keyboard key;
            MouseButtons mb;
            bool isMouse;
        };

        void createKeyboardMapping(Mapping& mapping, Keyboard key);
        void createMouseButtonMapping(Mapping& mapping, MouseButtons mb);

        bool isMappingReleased(Mapping& k);
        bool isMappingPressedOnce(Mapping& k);
        bool isMappingPressed(Mapping& k);
        bool isMappingReleasedOnce(Mapping& k);

        float getMappingReleasedValue(Mapping& k);
        float getMappingPressedOnceValue(Mapping& k);
        float getMappingPressedValue(Mapping& k);
        float getMappingReleasedOnceValue(Mapping& k);

        float getMappingReleasedAxis(Mapping& negative, Mapping& positive);
        float getMappingPressedOnceAxis(Mapping& negative, Mapping& positive);
        float getMappingPressedAxis(Mapping& negative, Mapping& positive);
        float getMappingReleasedOnceAxis(Mapping& negative, Mapping& positive);
    }
}

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
            void disableAttribute(std::string name);
            void attributePointer(std::string name, int size, GLenum type);
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
                enum TestType {
                    TT_REGULAR =  0,
                    TT_POSITION =  1,
                    TT_NORMALS =  2,
                    TT_ALBEDO_MATERIAL =  3,
                    TT_METAL_MATERIAL =  4,
                    TT_ROUGHNESS_MATERIAL =  5,
                    TT_EMISSIVE_MATERIAL =  6,
                    TT_LIT_MATERIAL =  7,
                    TT_ALBEDO_TC =  8,
                    TT_METAL_TC =  9,
                    TT_ROUGHNESS_TC  = 10,
                    TT_EMISSIVE_TC  = 11,
                    TT_LIT_TC  = 12,
                    TT_MAX_SIZE
                };

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

                /*
                void setProjection(glm::mat4 proj);
                void setView(glm::mat4 view);
                */
                void setModel(glm::mat4 model);
                //void setUVScale(float value);
                void setTest(int test);

                void verticesPointer();
                //void texCoordPointer();
                void normalsPointer();
                void albedoTexCoordPointer();
                void metalTexCoordPointer();
                void roughnessTexCoordPointer();
                void emissiveTexCoordPointer();
                void litTexCoordPointer();

                void drawMesh(render::mesh::Mesh* mesh);
            };

            void init();
            void release();
            MeshShader* getMeshShader();
            render::glw::UniformBuffer<Camera>* getCameraUBO();
        }

        namespace lighting {
            void init();
            void release();
        }

        namespace postprocess {
            void init();
            void release();
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

    void drawArrays(GLenum type, uint32_t vertexCount);
    void drawElements(GLenum type, uint32_t count);
}

namespace physics {
    void init();
    void update();
    void release();

    btDiscreteDynamicsWorld* getWorld();

    btVector3 getGravity();
    void setGravity(const btVector3& gravity);
}

namespace manager {
    struct Transform;
}

namespace sound {

    enum AudioDataMetaData {
        ADMD_BUFFER_COUNT = 2,
        ADMD_CHUNK_SIZE = 4096,
        ADMD_BUFFER_SIZE = 4096 * 32
    };

    enum AudioDataSeek {
        ADS_BEGIN = 0,
        ADS_END,
        ADS_MAX_SIZE
    };

    struct IAudioData {
        virtual bool init(std::string path) = 0;
        virtual void release() = 0;
        virtual long read(std::vector<char>& buffer) = 0;
        virtual int64_t maxSize() = 0;
        virtual int64_t tell() = 0;
        virtual void seek(int64_t position) = 0;
        virtual long getFrequence() = 0;
        virtual long getChannelCount() = 0;
        virtual long getBitPerSample() = 0;

        virtual ~IAudioData() {}
    };

    void init();
    void release();

    IAudioData* initAudioData(std::string path);

    // Volume Section
    void setMasterVolume(float volume);
    float getMasterVolume();

    void addVolumeGroup(std::string name, float value);
    void setGroupVolume(std::string name, float value);
    float getGroupVolume(std::string name);

    void setListenerPosition(const glm::vec3& position);
    void setListenerVelocity(const glm::vec3& velocity);
    void setListenerOrientation(const glm::mat4& rotationMatrix);

    void transformListener(manager::Transform& tran);

    namespace codec {

        struct WavAudioData : public IAudioData {
            struct WAVHeader {
                uint32_t type;
                uint32_t fileSize;
                uint32_t format;
            };

            struct WAVChunkDescription {
                uint32_t formatBlockID;
                uint32_t blockSize;
                uint16_t audioFormat;
                uint16_t channels;
                uint32_t frequency;
                uint32_t bytePerSecond;
                uint16_t bytePerBlock;
                uint16_t bitsPerSample;
            };

            struct WAVSampledData {
                uint32_t dataBlockID;
                uint32_t dataSize;
            };

            struct WAVString {
                unsigned char v1;
                unsigned char v2;
                unsigned char v3;
                unsigned char v4;

                std::string toString() {
                    std::stringstream ss;
                    ss << v1 << v2 << v3 << v4;
                    return ss.str();
                }
            };

            struct WAVFile {
                WAVHeader header;
                WAVChunkDescription chunkDescription;
                WAVSampledData sampleData;
            };


            WAVFile waveFile;

            size_t start = 0;
            size_t end = 0;

            //std::ifstream stream;
            FILE* stream;

            virtual bool init(std::string path);
            virtual void release();
            virtual long read(std::vector<char>& buffer);
            virtual int64_t maxSize();
            virtual int64_t tell();
            virtual void seek(int64_t position);
            virtual long getFrequence();
            virtual long getChannelCount();
            virtual long getBitPerSample();
        };


        struct OggAudioData : public IAudioData {
            FILE* fp = nullptr;
            OggVorbis_File file;
            vorbis_info* info;

            virtual bool init(std::string path);
            virtual void release();
            virtual long read(std::vector<char>& buffer);
            virtual int64_t maxSize();
            virtual int64_t tell();
            virtual void seek(int64_t position);
            virtual long getFrequence();
            virtual long getChannelCount();
            virtual long getBitPerSample();
        };


    }

    namespace alw {
        struct Buffer;

        struct Source {
            ALuint id = 0;

            void init();
            void release();

            void setPosition(glm::vec3 position);
            glm::vec3 getPosition();

            void setVelocity(glm::vec3 velocity);
            glm::vec3 getVelocity();

            void setVolume(float volume);
            float getVolume();

            void setRelative(bool value);
            bool isRelative();

            void setType(ALint type);
            ALint getType();

            void setLooping(bool value);
            bool isLooping();

            void setBuffer(Buffer* buffer);

            int getBufferedQueued();

            int getBufferedProcessed();

            void setMinVolume(float value);
            float getMinVolume();

            void setMaxVolume(float value);
            float getMaxVolume();

            void setReferenceDistance(float value);
            float getReferenceDistance();

            void setRolloffFactor(float value);
            float getRolloffFactor();

            void setMaxDistance(float value);
            float getMaxDistance();

            void setPitch(float value);
            float getPitch();

            void setDirection(const glm::vec3& direction);
            glm::vec3 getDirection();

            void setConeInnerAngle(float angle); // Value Between 0.0f, 360.0f
            float getConeInnerAngle();

            void setConeOuterAngle(float angle);
            float getConeOuterAngle();

            void setConeOuterVolume(float volume);
            float getConeOuterVolume();

            void setSecOffset(float sec);
            float getSecOffset();

            void setSampleOffset(float sample);
            float getSampleOffset();

            void setByteOffset(ALint offset);
            ALint getByteOffset();

            void sourceQueueBuffers(std::vector<ALuint>& buffers);
            void sourceUnqueueBuffers(std::vector<ALuint>& buffers);

            ALenum getState();

            void play();
            void pause();
            void stop();
            void rewind();
        };

        struct Buffer {
            ALuint id = 0;

            void init();
            void release();

            void setFrequency(int freq);
            int getFrequency();

            void setSize(int size);
            int getSize();

            void setBits(int bits); // Either 8 or 16
            int getBits(); // Either 8 or 16

            void setChannels(int channel); // 1 mono or 2 sterio
            int getChannels(); // 1 mono or 2 sterio

            void bufferData(ALenum format, void* data, size_t size, ALsizei frequency);

        };
    }
}

namespace assets {

    template<typename T>
    struct Asset {
        uint64_t id = 0;
        T value;
    };

    void init();
    void release();

    uint64_t getMeshID(std::string name);
    render::mesh::Mesh* getMesh(std::string name);

    uint64_t getTexture2DID(std::string name);
    render::glw::Texture2D* getTexture2D(std::string name);

    uint64_t getSoundID(std::string name);
    sound::IAudioData* getSound(std::string name);

    uint64_t getMaterialID(std::string name);
    render::Material* getMaterial(std::string name);
}

namespace util {
    void init();
    void release();

    namespace random {
        void init();
        void release();
        bool randboolean();
        int32_t randrange(int32_t min, int32_t max);
        float randf(); // 0.0f -> 1.0f
    }
}

namespace manager {
    struct Entity;
    struct Scene;
    struct Global;
    struct Behavior;

    namespace component {
        struct IComponent {
            virtual void init(Entity* entity) = 0;
            virtual void handleEvent(SDL_Event* e) = 0;
            virtual void update(float delta) = 0;
            virtual void preRender() = 0;
            virtual void render() = 0;
            virtual void release() = 0;
            virtual void load(Json::Value value) = 0;

            virtual ~IComponent() {}

        };
    }

    struct Transform {
        Entity* entity = nullptr;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        struct Axis {
            glm::vec3 axis;
            float angle;

            void setup(glm::vec3 rotations);
            glm::vec3 convert();
        };

        void init(Entity* entity);
        void release();

        glm::mat4 toParentMatrix(Entity* entity);
        glm::vec3 getGlobalPosition();

        // This will convert to a btTransform
        btTransform convertToBulletTransform();
        void interpretBulletTransform(const btTransform& transform);

        glm::vec3 getTransformedPosition();
        glm::vec3 getTransformedRotation();
        void setTransformedPosition(glm::vec3 position);
        void setTransformedRotation(glm::vec3 rotation);

        btVector3 toBulletVector3(glm::vec3 v);
        glm::vec3 toGLMVector3(const btVector3& v);

        glm::mat4 toParentTranslateMatrix(Entity* entity);
        glm::mat4 toParentRotationMatrix(Entity* entity);

        glm::mat4 toGlobalRotaionMatrix();

        Axis toAxis(glm::vec3 rotation);

        glm::mat4 toModel();

        void load(Json::Value v);
    };

    struct Behavior {
        enum Type {
            T_BOOL = 0,
            T_INTEGER,
            T_NUMBER,
            T_STRING,
            T_USERDATA
        };

        struct Argument {
            Type type;
            bool bValue;
            int iValue;
            float nValue;
            void* uValue = nullptr;
            std::string sValue;
        };

        struct Callback {
            Behavior* behavior = nullptr;
            std::string name;
        };

        lua_State* state = nullptr;
        Entity* entity = nullptr;
        Scene* scene = nullptr;
        Global* global = nullptr;

        void init(std::string path, Entity* entity);
        void init(std::string path, Scene* scene);
        void init(std::string path, Global* global);
        void update(float delta);
        void release();

        // Getters
        bool getBoolean(std::string name);
        int getInteger(std::string name);
        float getNumber(std::string name);
        std::string getString(std::string name);

        // Setters
        void setBoolean(std::string name, bool value);
        void setInteger(std::string name, int value);
        void setNumber(std::string name, float value);
        void setString(std::string name, std::string value);

        void executeCallback(std::string name, const std::vector<Argument>& args);

    };

    struct Entity {
        std::string type;
        std::string name;
        std::string prefabPath;
        Scene* scene = nullptr;

        Entity* parent = nullptr;
        std::vector<Entity*> childeren;

        Transform transform;

        std::string script;
        Behavior* behavior = nullptr;

        std::map<std::string, component::IComponent*> components;

        bool needRemoval = false;

        bool visible = true;

        void init(Scene* scene);
        void handleEvent(SDL_Event* e);
        void update(float delta);
        void preRender();
        void render();
        void release();

        void handleEntity(Json::Value value);
        void loadPrefab(std::string path);
        void load(Json::Value value);

        bool hasParent();

        void addChildEntity(Entity* entity);
        Entity* getChildEntity(size_t index);
        size_t getChilderenAmount();
        void removeEntity(Entity* entity);

        void componentIterator(std::function<void(component::IComponent* comp)>);
    };

    struct Scene {
        Global* global = nullptr;
        std::vector<Entity*> entities;

        std::string script;
        Behavior* behavior = nullptr;

        void init(Global* global);
        void handleEvent(SDL_Event* e);
        void update(float delta);
        void render();
        void release();

        void load(Json::Value value);

        void addEntity(Entity* entity);
        void removeEntity(Entity* entity);
    };

    struct Global {
        Scene* scene = nullptr;
        
        std::string script;
        Behavior* behavior = nullptr;
        
        bool isSceneChange = false;
        std::string scenePath;

        std::string defaultScenePath;

        std::map<std::string, int> groups;
        int groupIndex = 1;

        std::map<std::string, input::Keyboard> keyboard = {
        {"KEYS_UNKNOWN", input::Keyboard::KEYS_UNKNOWN},
        {"KEYS_A", input::Keyboard::KEYS_A},
        {"KEYS_B", input::Keyboard::KEYS_B},
        {"KEYS_C", input::Keyboard::KEYS_C},
        {"KEYS_D", input::Keyboard::KEYS_D},
        {"KEYS_E", input::Keyboard::KEYS_E},
        {"KEYS_F", input::Keyboard::KEYS_F},
        {"KEYS_G", input::Keyboard::KEYS_G},
        {"KEYS_H", input::Keyboard::KEYS_H},
        {"KEYS_I", input::Keyboard::KEYS_I},
        {"KEYS_J", input::Keyboard::KEYS_J},
        {"KEYS_K", input::Keyboard::KEYS_K},
        {"KEYS_L", input::Keyboard::KEYS_L},
        {"KEYS_M", input::Keyboard::KEYS_M},
        {"KEYS_N", input::Keyboard::KEYS_N},
        {"KEYS_O", input::Keyboard::KEYS_O},
        {"KEYS_P", input::Keyboard::KEYS_P},
        {"KEYS_Q", input::Keyboard::KEYS_Q},
        {"KEYS_R", input::Keyboard::KEYS_R},
        {"KEYS_S", input::Keyboard::KEYS_S},
        {"KEYS_T", input::Keyboard::KEYS_T},
        {"KEYS_U", input::Keyboard::KEYS_U},
        {"KEYS_V", input::Keyboard::KEYS_V},
        {"KEYS_W", input::Keyboard::KEYS_W},
        {"KEYS_X", input::Keyboard::KEYS_X},
        {"KEYS_Y", input::Keyboard::KEYS_Y},
        {"KEYS_Z", input::Keyboard::KEYS_Z},
        {"KEYS_1", input::Keyboard::KEYS_1},
        {"KEYS_2", input::Keyboard::KEYS_2},
        {"KEYS_3", input::Keyboard::KEYS_3},
        {"KEYS_4", input::Keyboard::KEYS_4},
        {"KEYS_5", input::Keyboard::KEYS_5},
        {"KEYS_6", input::Keyboard::KEYS_6},
        {"KEYS_7", input::Keyboard::KEYS_7},
        {"KEYS_8", input::Keyboard::KEYS_8},
        {"KEYS_9", input::Keyboard::KEYS_9},
        {"KEYS_0", input::Keyboard::KEYS_0},
        {"KEYS_RETURN", input::Keyboard::KEYS_RETURN},
        {"KEYS_ESCAPE", input::Keyboard::KEYS_ESCAPE},
        {"KEYS_BACKSPACE", input::Keyboard::KEYS_BACKSPACE},
        {"KEYS_TAB", input::Keyboard::KEYS_TAB},
        {"KEYS_SPACE", input::Keyboard::KEYS_SPACE},
        {"KEYS_MINUS", input::Keyboard::KEYS_MINUS},
        {"KEYS_EQUALS", input::Keyboard::KEYS_EQUALS},
        {"KEYS_LEFTBRACKET", input::Keyboard::KEYS_LEFTBRACKET},
        {"KEYS_RIGHTBRACKET", input::Keyboard::KEYS_RIGHTBRACKET},
        {"KEYS_BACKSLASH", input::Keyboard::KEYS_BACKSLASH},
        {"KEYS_NONUSHASH", input::Keyboard::KEYS_NONUSHASH},
        {"KEYS_SEMICOLON", input::Keyboard::KEYS_SEMICOLON},
        {"KEYS_APOSTROPHE", input::Keyboard::KEYS_APOSTROPHE},
        {"KEYS_GRAVE", input::Keyboard::KEYS_GRAVE},
        {"KEYS_COMMA", input::Keyboard::KEYS_COMMA},
        {"KEYS_PERIOD", input::Keyboard::KEYS_PERIOD},
        {"KEYS_SLASH", input::Keyboard::KEYS_SLASH},
        {"KEYS_CAPSLOCK", input::Keyboard::KEYS_CAPSLOCK},
        {"KEYS_F1", input::Keyboard::KEYS_F1},
        {"KEYS_F2", input::Keyboard::KEYS_F2},
        {"KEYS_F3", input::Keyboard::KEYS_F3},
        {"KEYS_F4", input::Keyboard::KEYS_F4},
        {"KEYS_F5", input::Keyboard::KEYS_F5},
        {"KEYS_F6", input::Keyboard::KEYS_F6},
        {"KEYS_F7", input::Keyboard::KEYS_F7},
        {"KEYS_F8", input::Keyboard::KEYS_F8},
        {"KEYS_F9", input::Keyboard::KEYS_F9},
        {"KEYS_F10", input::Keyboard::KEYS_F10},
        {"KEYS_F11", input::Keyboard::KEYS_F11},
        {"KEYS_F12", input::Keyboard::KEYS_F12},
        {"KEYS_PRINTSCREEN", input::Keyboard::KEYS_PRINTSCREEN},
        {"KEYS_SCROLLLOCK", input::Keyboard::KEYS_SCROLLLOCK},
        {"KEYS_PAUSE", input::Keyboard::KEYS_PAUSE},
        {"KEYS_INSERT", input::Keyboard::KEYS_INSERT},
        {"KEYS_HOME", input::Keyboard::KEYS_HOME},
        {"KEYS_PAGEUP", input::Keyboard::KEYS_PAGEUP},
        {"KEYS_DELETE", input::Keyboard::KEYS_DELETE},
        {"KEYS_END", input::Keyboard::KEYS_END},
        {"KEYS_PAGEDOWN", input::Keyboard::KEYS_PAGEDOWN},
        {"KEYS_RIGHT", input::Keyboard::KEYS_RIGHT},
        {"KEYS_LEFT", input::Keyboard::KEYS_LEFT},
        {"KEYS_DOWN", input::Keyboard::KEYS_DOWN},
        {"KEYS_UP", input::Keyboard::KEYS_UP},
        {"KEYS_NUMLOCKCLEAR", input::Keyboard::KEYS_NUMLOCKCLEAR},
        {"KEYS_KP_DIVIDE", input::Keyboard::KEYS_KP_DIVIDE},
        {"KEYS_KP_MULTIPLY", input::Keyboard::KEYS_KP_MULTIPLY},
        {"KEYS_KP_MINUS", input::Keyboard::KEYS_KP_MINUS},
        {"KEYS_KP_PLUS", input::Keyboard::KEYS_KP_PLUS},
        {"KEYS_KP_ENTER", input::Keyboard::KEYS_KP_ENTER},
        {"KEYS_KP_1", input::Keyboard::KEYS_KP_1},
        {"KEYS_KP_2", input::Keyboard::KEYS_KP_2},
        {"KEYS_KP_3", input::Keyboard::KEYS_KP_3},
        {"KEYS_KP_4", input::Keyboard::KEYS_KP_4},
        {"KEYS_KP_5", input::Keyboard::KEYS_KP_5},
        {"KEYS_KP_6", input::Keyboard::KEYS_KP_6},
        {"KEYS_KP_7", input::Keyboard::KEYS_KP_7},
        {"KEYS_KP_8", input::Keyboard::KEYS_KP_8},
        {"KEYS_KP_9", input::Keyboard::KEYS_KP_9},
        {"KEYS_KP_0", input::Keyboard::KEYS_KP_0},
        {"KEYS_KP_PERIOD", input::Keyboard::KEYS_KP_PERIOD},
        {"KEYS_NONUSBACKSLASH", input::Keyboard::KEYS_NONUSBACKSLASH},
        {"KEYS_APPLICATION", input::Keyboard::KEYS_APPLICATION},
        {"KEYS_POWER", input::Keyboard::KEYS_POWER},
        {"KEYS_KP_EQUALS", input::Keyboard::KEYS_KP_EQUALS},
        {"KEYS_F13", input::Keyboard::KEYS_F13},
        {"KEYS_F14", input::Keyboard::KEYS_F14},
        {"KEYS_F15", input::Keyboard::KEYS_F15},
        {"KEYS_F16", input::Keyboard::KEYS_F16},
        {"KEYS_F17", input::Keyboard::KEYS_F17},
        {"KEYS_F18", input::Keyboard::KEYS_F18},
        {"KEYS_F19", input::Keyboard::KEYS_F19},
        {"KEYS_F20", input::Keyboard::KEYS_F20},
        {"KEYS_F21", input::Keyboard::KEYS_F21},
        {"KEYS_F22", input::Keyboard::KEYS_F22},
        {"KEYS_F23", input::Keyboard::KEYS_F23},
        {"KEYS_F24", input::Keyboard::KEYS_F24},
        {"KEYS_EXECUTE", input::Keyboard::KEYS_EXECUTE},
        {"KEYS_HELP", input::Keyboard::KEYS_HELP},
        {"KEYS_MENU", input::Keyboard::KEYS_MENU},
        {"KEYS_SELECT", input::Keyboard::KEYS_SELECT},
        {"KEYS_STOP", input::Keyboard::KEYS_STOP},
        {"KEYS_AGAIN", input::Keyboard::KEYS_AGAIN},
        {"KEYS_UNDO", input::Keyboard::KEYS_UNDO},
        {"KEYS_CUT", input::Keyboard::KEYS_CUT},
        {"KEYS_COPY", input::Keyboard::KEYS_COPY},
        {"KEYS_PASTE", input::Keyboard::KEYS_PASTE},
        {"KEYS_FIND", input::Keyboard::KEYS_FIND},
        {"KEYS_MUTE", input::Keyboard::KEYS_MUTE},
        {"KEYS_VOLUMEUP", input::Keyboard::KEYS_VOLUMEUP},
        {"KEYS_VOLUMEDOWN", input::Keyboard::KEYS_VOLUMEDOWN},
        {"KEYS_KP_COMMA", input::Keyboard::KEYS_KP_COMMA},
        {"KEYS_KP_EQUALSAS400", input::Keyboard::KEYS_KP_EQUALSAS400},
        {"KEYS_INTERNATIONAL1", input::Keyboard::KEYS_INTERNATIONAL1},
        {"KEYS_INTERNATIONAL2", input::Keyboard::KEYS_INTERNATIONAL2},
        {"KEYS_INTERNATIONAL3", input::Keyboard::KEYS_INTERNATIONAL3},
        {"KEYS_INTERNATIONAL4", input::Keyboard::KEYS_INTERNATIONAL4},
        {"KEYS_INTERNATIONAL5", input::Keyboard::KEYS_INTERNATIONAL5},
        {"KEYS_INTERNATIONAL6", input::Keyboard::KEYS_INTERNATIONAL6},
        {"KEYS_INTERNATIONAL7", input::Keyboard::KEYS_INTERNATIONAL7},
        {"KEYS_INTERNATIONAL8", input::Keyboard::KEYS_INTERNATIONAL8},
        {"KEYS_INTERNATIONAL9", input::Keyboard::KEYS_INTERNATIONAL9},
        {"KEYS_LANG1", input::Keyboard::KEYS_LANG1},
        {"KEYS_LANG2", input::Keyboard::KEYS_LANG2},
        {"KEYS_LANG3", input::Keyboard::KEYS_LANG3},
        {"KEYS_LANG4", input::Keyboard::KEYS_LANG4},
        {"KEYS_LANG5", input::Keyboard::KEYS_LANG5},
        {"KEYS_LANG6", input::Keyboard::KEYS_LANG6},
        {"KEYS_LANG7", input::Keyboard::KEYS_LANG7},
        {"KEYS_LANG8", input::Keyboard::KEYS_LANG8},
        {"KEYS_LANG9", input::Keyboard::KEYS_LANG9},
        {"KEYS_ALTERASE", input::Keyboard::KEYS_ALTERASE},
        {"KEYS_SYSREQ", input::Keyboard::KEYS_SYSREQ},
        {"KEYS_CANCEL", input::Keyboard::KEYS_CANCEL},
        {"KEYS_CLEAR", input::Keyboard::KEYS_CLEAR},
        {"KEYS_PRIOR", input::Keyboard::KEYS_PRIOR},
        {"KEYS_RETURN2", input::Keyboard::KEYS_RETURN2},
        {"KEYS_SEPARATOR", input::Keyboard::KEYS_SEPARATOR},
        {"KEYS_OUT", input::Keyboard::KEYS_OUT},
        {"KEYS_OPER", input::Keyboard::KEYS_OPER},
        {"KEYS_CLEARAGAIN", input::Keyboard::KEYS_CLEARAGAIN},
        {"KEYS_CRSEL", input::Keyboard::KEYS_CRSEL},
        {"KEYS_EXSEL", input::Keyboard::KEYS_EXSEL},
        {"KEYS_KP_00", input::Keyboard::KEYS_KP_00},
        {"KEYS_KP_000", input::Keyboard::KEYS_KP_000},
        {"KEYS_THOUSANDSSEPARATOR", input::Keyboard::KEYS_THOUSANDSSEPARATOR},
        {"KEYS_DECIMALSEPARATOR", input::Keyboard::KEYS_DECIMALSEPARATOR},
        {"KEYS_CURRENCYUNIT", input::Keyboard::KEYS_CURRENCYUNIT},
        {"KEYS_CURRENCYSUBUNIT", input::Keyboard::KEYS_CURRENCYSUBUNIT},
        {"KEYS_KP_LEFTPAREN", input::Keyboard::KEYS_KP_LEFTPAREN},
        {"KEYS_KP_RIGHTPAREN", input::Keyboard::KEYS_KP_RIGHTPAREN},
        {"KEYS_KP_LEFTBRACE", input::Keyboard::KEYS_KP_LEFTBRACE},
        {"KEYS_KP_RIGHTBRACE", input::Keyboard::KEYS_KP_RIGHTBRACE},
        {"KEYS_KP_TAB", input::Keyboard::KEYS_KP_TAB},
        {"KEYS_KP_BACKSPACE", input::Keyboard::KEYS_KP_BACKSPACE},
        {"KEYS_KP_A", input::Keyboard::KEYS_KP_A},
        {"KEYS_KP_B", input::Keyboard::KEYS_KP_B},
        {"KEYS_KP_C", input::Keyboard::KEYS_KP_C},
        {"KEYS_KP_D", input::Keyboard::KEYS_KP_D},
        {"KEYS_KP_E", input::Keyboard::KEYS_KP_E},
        {"KEYS_KP_F", input::Keyboard::KEYS_KP_F},
        {"KEYS_KP_XOR", input::Keyboard::KEYS_KP_XOR},
        {"KEYS_KP_POWER", input::Keyboard::KEYS_KP_POWER},
        {"KEYS_KP_PERCENT", input::Keyboard::KEYS_KP_PERCENT},
        {"KEYS_KP_LESS", input::Keyboard::KEYS_KP_LESS},
        {"KEYS_KP_GREATER", input::Keyboard::KEYS_KP_GREATER},
        {"KEYS_KP_AMPERSAND", input::Keyboard::KEYS_KP_AMPERSAND},
        {"KEYS_KP_DBLAMPERSAND", input::Keyboard::KEYS_KP_DBLAMPERSAND},
        {"KEYS_KP_VERTICALBAR", input::Keyboard::KEYS_KP_VERTICALBAR},
        {"KEYS_KP_DBLVERTICALBAR", input::Keyboard::KEYS_KP_DBLVERTICALBAR},
        {"KEYS_KP_COLON", input::Keyboard::KEYS_KP_COLON},
        {"KEYS_KP_HASH", input::Keyboard::KEYS_KP_HASH},
        {"KEYS_KP_SPACE", input::Keyboard::KEYS_KP_SPACE},
        {"KEYS_KP_AT", input::Keyboard::KEYS_KP_AT},
        {"KEYS_KP_EXCLAM", input::Keyboard::KEYS_KP_EXCLAM},
        {"KEYS_KP_MEMSTORE", input::Keyboard::KEYS_KP_MEMSTORE},
        {"KEYS_KP_MEMRECALL", input::Keyboard::KEYS_KP_MEMRECALL},
        {"KEYS_KP_MEMCLEAR", input::Keyboard::KEYS_KP_MEMCLEAR},
        {"KEYS_KP_MEMADD", input::Keyboard::KEYS_KP_MEMADD},
        {"KEYS_KP_MEMSUBTRACT", input::Keyboard::KEYS_KP_MEMSUBTRACT},
        {"KEYS_KP_MEMMULTIPLY", input::Keyboard::KEYS_KP_MEMMULTIPLY},
        {"KEYS_KP_MEMDIVIDE", input::Keyboard::KEYS_KP_MEMDIVIDE},
        {"KEYS_KP_PLUSMINUS", input::Keyboard::KEYS_KP_PLUSMINUS},
        {"KEYS_KP_CLEAR", input::Keyboard::KEYS_KP_CLEAR},
        {"KEYS_KP_CLEARENTRY", input::Keyboard::KEYS_KP_CLEARENTRY},
        {"KEYS_KP_BINARY", input::Keyboard::KEYS_KP_BINARY},
        {"KEYS_KP_OCTAL", input::Keyboard::KEYS_KP_OCTAL},
        {"KEYS_KP_DECIMAL", input::Keyboard::KEYS_KP_DECIMAL},
        {"KEYS_KP_HEXADECIMAL", input::Keyboard::KEYS_KP_HEXADECIMAL},
        {"KEYS_LCTRL", input::Keyboard::KEYS_LCTRL},
        {"KEYS_LSHIFT", input::Keyboard::KEYS_LSHIFT},
        {"KEYS_LALT", input::Keyboard::KEYS_LALT},
        {"KEYS_LGUI", input::Keyboard::KEYS_LGUI},
        {"KEYS_RCTRL", input::Keyboard::KEYS_RCTRL},
        {"KEYS_RSHIFT", input::Keyboard::KEYS_RSHIFT},
        {"KEYS_RALT", input::Keyboard::KEYS_RALT},
        {"KEYS_RGUI", input::Keyboard::KEYS_RGUI},
        {"KEYS_MODE", input::Keyboard::KEYS_MODE},
        {"KEYS_AUDIONEXT", input::Keyboard::KEYS_AUDIONEXT},
        {"KEYS_AUDIOPREV", input::Keyboard::KEYS_AUDIOPREV},
        {"KEYS_AUDIOSTOP", input::Keyboard::KEYS_AUDIOSTOP},
        {"KEYS_AUDIOPLAY", input::Keyboard::KEYS_AUDIOPLAY},
        {"KEYS_AUDIOMUTE", input::Keyboard::KEYS_AUDIOMUTE},
        {"KEYS_MEDIASELECT", input::Keyboard::KEYS_MEDIASELECT},
        {"KEYS_WWW", input::Keyboard::KEYS_WWW},
        {"KEYS_MAIL", input::Keyboard::KEYS_MAIL},
        {"KEYS_CALCULATOR", input::Keyboard::KEYS_CALCULATOR},
        {"KEYS_COMPUTER", input::Keyboard::KEYS_COMPUTER},
        {"KEYS_AC_SEARCH", input::Keyboard::KEYS_AC_SEARCH},
        {"KEYS_AC_HOME", input::Keyboard::KEYS_AC_HOME},
        {"KEYS_AC_BACK", input::Keyboard::KEYS_AC_BACK},
        {"KEYS_AC_FORWARD", input::Keyboard::KEYS_AC_FORWARD},
        {"KEYS_AC_STOP", input::Keyboard::KEYS_AC_STOP},
        {"KEYS_AC_REFRESH", input::Keyboard::KEYS_AC_REFRESH},
        {"KEYS_AC_BOOKMARKS", input::Keyboard::KEYS_AC_BOOKMARKS},
        {"KEYS_BRIGHTNESSDOWN", input::Keyboard::KEYS_BRIGHTNESSDOWN},
        {"KEYS_BRIGHTNESSUP", input::Keyboard::KEYS_BRIGHTNESSUP},
        {"KEYS_DISPLAYSWITCH", input::Keyboard::KEYS_DISPLAYSWITCH},
        {"KEYS_KBDILLUMTOGGLE", input::Keyboard::KEYS_KBDILLUMTOGGLE},
        {"KEYS_KBDILLUMDOWN", input::Keyboard::KEYS_KBDILLUMDOWN},
        {"KEYS_KBDILLUMUP", input::Keyboard::KEYS_KBDILLUMUP},
        {"KEYS_EJECT", input::Keyboard::KEYS_EJECT},
        {"KEYS_SLEEP", input::Keyboard::KEYS_SLEEP},
        {"KEYS_APP1", input::Keyboard::KEYS_APP1},
        {"KEYS_APP2", input::Keyboard::KEYS_APP2},
        {"KEYS_AUDIOREWIND", input::Keyboard::KEYS_AUDIOREWIND},
        {"KEYS_AUDIOFASTFORWARD", input::Keyboard::KEYS_AUDIOFASTFORWARD},
        {"KEYS_SOFTLEFT", input::Keyboard::KEYS_SOFTLEFT},
        {"KEYS_SOFTRIGHT", input::Keyboard::KEYS_SOFTRIGHT},
        {"KEYS_CALL", input::Keyboard::KEYS_CALL},
        {"KEYS_ENDCALL", input::Keyboard::KEYS_ENDCALL}
        };

        std::map<std::string, input::MouseButtons> mouseBouttons = {
        {"MBS_LEFT", input::MouseButtons::MBS_LEFT},
        {"MBS_CENTER", input::MouseButtons::MBS_CENTER},
        {"MBS_RIGHT", input::MouseButtons::MBS_RIGHT}
        };

        std::map<std::string, input::mapping::Mapping> mappings;

        void init();
        void handleEvent(SDL_Event* e);
        void update(float delta);
        void render();
        void release();

        Json::Value open_json(std::string path);

        void load(std::string path);
        void changeScene(std::string path);

        void global_load();

        void startGame();

        int getPhysicsGroups(std::string name);
    };

    namespace component {
        void componentFactory(Entity* entity, std::string type, Json::Value value);

        namespace render {
            struct CameraComponent : public IComponent {
                Entity* entity = nullptr;
                float fov;
                float znear;
                float zfar;

                glm::mat4 toView();
                glm::mat4 toViewWithParent();

                virtual void init(Entity* entity);
                virtual void handleEvent(SDL_Event* e);
                virtual void update(float delta);
                virtual void preRender();
                virtual void render();
                virtual void release();
                virtual void load(Json::Value value);
            };

            struct MeshComponent : public IComponent {
                Entity* entity = nullptr;
                std::string mesh;
                std::string material;
                //std::string texture;
                //float uvScale;

                virtual void init(Entity* entity);
                virtual void handleEvent(SDL_Event* e);
                virtual void update(float delta);
                virtual void preRender();
                virtual void render();
                virtual void release();
                virtual void load(Json::Value value);
            };
        }

        namespace physics {

            struct AbstractBodyComponent : public IComponent {
                Entity* entity = nullptr;
                btRigidBody* body = nullptr;
                btCollisionShape* shape = nullptr;

                float mass = 0.0;
                
                virtual void init(Entity* entity);
                virtual void handleEvent(SDL_Event* e);
                virtual void update(float delta);
                virtual void preRender();
                virtual void render();
                virtual void release();

                virtual void load(Json::Value value) = 0;

                std::vector<std::string> groups;
                std::vector<std::string> masks;

                btCollisionShape* createSphereShape(float radius);
                btCollisionShape* createCapsuleShape(float radius, float height);
                btCollisionShape* createBoxShape(const btVector3& halfExtents);
                btCollisionShape* createStaticPlaneShape(const btVector3& planeNormal, float planeConstant);
                btCollisionShape* createTriangleShape(std::string meshName);

                btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* collisionShape);

                btRigidBody* createStaticRigidBody(const btTransform& startTransform, btCollisionShape* collisionShape);

            };

            struct StaticBodyComponent : public AbstractBodyComponent {
                std::map<std::string, bool> collisionShapeTypes = {
                    {"static-plane", true},
                    {"box", true},
                    {"sphere", true},
                    {"capsule", true},
                    {"triangle-mesh", true}
                };

                virtual void load(Json::Value value);
            };

            struct DynamicBodyComponent : public AbstractBodyComponent {
                std::map<std::string, bool> collisionShapeTypes = {
                    {"static-plane", false},
                    {"box", true},
                    {"sphere", true},
                    {"capsule", true},
                    {"triangle-mesh", false}
                };

                virtual void load(Json::Value value);
            };

            struct KinematicBodyComponent : public AbstractBodyComponent {
                std::map<std::string, bool> collisionShapeTypes = {
                    {"static-plane", false},
                    {"box", true},
                    {"sphere", true},
                    {"capsule", true},
                    {"triangle-mesh", false}
                };

                struct KinematicBodyContactResultCallback : public btCollisionWorld::ContactResultCallback {
                    bool hit = false;
                    float dist = 0.0f;
                    btVector3 point;
                    btVector3 normal;

                    virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
                };

                // Basiclly you'll be using this as a movable object. If not then its a more static asset that
                // requires updating the worldTransform directly.
                bool isController = false;
                btVector3 linearVelocity = btVector3(0.0f, 0.0f, 0.0f);

                bool onFloor = false;

                virtual void init(Entity* entity);
                virtual void update(float delta);

                virtual void load(Json::Value value);

                void moveAndSlide();

                bool isOnFloor();
            };

            struct TriggerComponent : public IComponent {
                std::map<std::string, bool> collisionShapeTypes = {
                    {"box", true},
                    {"sphere", true},
                    {"capsule", true},
                };

                Entity* entity = nullptr;
                btGhostObject* ghostObject = nullptr;
                btCollisionShape* shape = nullptr;

                std::vector<std::string> groups;
                std::vector<std::string> masks;

                // Behavior Entity Enter
                Behavior::Callback entityEnter;

                // Behavior Entity Exit
                Behavior::Callback entityExit;

                bool isEntered = false;

                virtual void init(Entity* entity);
                
                virtual void handleEvent(SDL_Event* e);
                
                virtual void update(float delta);
                
                virtual void preRender();
                
                virtual void render();
                
                virtual void release();
                
                virtual void load(Json::Value value);

                btCollisionShape* createSphereShape(float radius);

                btCollisionShape* createCapsuleShape(float radius, float height);

                btCollisionShape* createBoxShape(const btVector3& halfExtents);

            };

            struct PushArmComponent : public IComponent {
                Entity* entity = nullptr;
                float distance = 0.0f;
                std::vector<std::string> groups;
                std::vector<std::string> masks;

                int filterGroup = 0;
                int filterMask = 0;
                
                virtual void init(Entity* entity);
                virtual void handleEvent(SDL_Event* e);
                virtual void update(float delta);
                virtual void preRender();
                virtual void render();
                virtual void release();
                virtual void load(Json::Value value);
            };

            struct RayCastComponent : public IComponent {
                Entity* entity = nullptr;
                glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
                float scale = 0.0f;
                bool disabled = false;
                std::vector<std::string> groups;
                std::vector<std::string> masks;

                int filterGroup = 0;
                int filterMask = 0;

                // These values will be returned via lua api
                bool isHit = false;
                glm::vec3 worldPoint = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 worldNormal = glm::vec3(0.0f, 0.0f, 0.0f);

                virtual void init(Entity* entity);
                virtual void handleEvent(SDL_Event* e);
                virtual void update(float delta);
                virtual void preRender();
                virtual void render();
                virtual void release();
                virtual void load(Json::Value value);
            };
        }

        namespace sound {
            struct SoundListenerComponent : public IComponent {
                Entity* entity = nullptr;

                virtual void init(Entity* entity);
                virtual void handleEvent(SDL_Event* e);
                virtual void update(float delta);
                virtual void preRender();
                virtual void render();
                virtual void release();
                virtual void load(Json::Value value);
            };

            struct MusicStreamPlayerComponent : public IComponent {
                const int BUFFER_COUNT = 2;
                const int BUFFER_SIZE = 4096 * 32;
                const int CHUNK_SIZE = 4096;

                Entity* entity = nullptr;
                std::string audioDataName;
                std::string volumeGroup;

                std::vector<::sound::alw::Buffer> buffers;
                ::sound::alw::Source sounce;

                enum ChunkType {
                    CT_START = 0,
                    CT_DATA,
                    CT_END,
                    CT_MAX_SIZE
                };

                enum StreamState {
                    SS_PLAY = 0,
                    SS_STOP,
                    SS_PAUSE,
                    SS_MAX_SIZE
                };

                struct Chunk {
                    ChunkType type;
                    int len;
                    std::vector<char> data;
                };

                struct Buffer {
                    int len;
                    std::vector<char> data;
                };

                std::deque<Chunk> chunks;
                std::vector<char> bufferData;

                int64_t currentPosition = 0;
                int state = AL_STOPPED;
                bool endOfFile = false;
                bool endOfQueue = false;
                int length = 0;
                bool start = true;
                int processingPointer = 0;
                int playingPointer = 0;
                int processingIncrementor = 0;
                int playingIncrementor = 0;
                bool playlast = false;
                bool playing = true;

                StreamState ssState = StreamState::SS_STOP;


                // Public Interface Variables that need lua Wrappers
                bool looping = false;
                bool autoPlay = false;
                bool relative = false;
                float referenceDistance = 1.0f;
                float rolloffFactor = 1.0f;
                float maxDistance = std::numeric_limits<float>::max();

                virtual void init(Entity* entity);
                virtual void handleEvent(SDL_Event* e);
                virtual void update(float delta);
                virtual void preRender();
                virtual void render();
                virtual void release();
                virtual void load(Json::Value value);

                void play();
                void pause();
                void stop();

                bool isLooping();
                void setLooping(bool value);

                bool isAutoPlay();
                void setAutoPlay(bool value);

                bool isRelative();
                void setRelative(bool value);

                void stream_data();
                void process_current_buffer();
                void check_current_buffer_state();
                void play_current_buffer();

                void reset();
            };

            struct SoundPlayerComponent : public IComponent {
                Entity* entity = nullptr;

                ::sound::alw::Buffer buffer;
                ::sound::alw::Source source;

                std::string audioDataName;
                std::string volumeGroup;
                bool autoPlay = false;
                bool looping = false;
                bool relative = false;
                float referenceDistance = 1.0f;
                float rolloffFactor = 1.0f;
                float maxDistance = std::numeric_limits<float>::max();
                float pitch = 1.0f;

                struct Chunk {
                    int len;
                    std::vector<char> data;
                };

                virtual void init(Entity* entity);
                virtual void handleEvent(SDL_Event* e);
                virtual void update(float delta);
                virtual void preRender();
                virtual void render();
                virtual void release();
                virtual void load(Json::Value value);

                void play();
                void pause();
                void stop();

            };
        }
    }

}

namespace script {
    void load_library(lua_State* l);

    // app
    void app_load_library(lua_State* l);
    int app_getCaption(lua_State* l);
    int app_getWidthInteger(lua_State* l);
    int app_getHeightInteger(lua_State* l);
    int app_getWidthFloat(lua_State* l);
    int app_getHeightFloat(lua_State* l);
    int app_getAspect(lua_State* l);
    int app_getWindow(lua_State* l);
    int app_getContext(lua_State* l);
    int app_exit(lua_State* l);

    // input
    void input_load_library(lua_State* l);
    // Keyboard
    int input_isKeyReleased(lua_State* l);
    int input_isKeyPressedOnce(lua_State* l);
    int input_isKeyPressed(lua_State* l);
    int input_isKeyReleasedOnce(lua_State* l);

    int input_getKeyReleasedValue(lua_State* l);
    int input_getKeyPressedOnceValue(lua_State* l);
    int input_getKeyPressedValue(lua_State* l);
    int input_getKeyReleasedOnceValue(lua_State* l);

    int input_getKeyReleasedAxis(lua_State* l);
    int input_getKeyPressedOnceAxis(lua_State* l);
    int input_getKeyPressedAxis(lua_State* l);
    int input_getKeyReleasedOnceAxis(lua_State* l);

    // Mouse
    int input_toPosition(lua_State* l);
    int input_toVelocity(lua_State* l);
    int input_toMouseWheel(lua_State* l);
    
    int input_isMouseButtonReleased(lua_State* l);
    int input_isMouseButtonPressedOnce(lua_State* l);
    int input_isMouseButtonPressed(lua_State* l);
    int input_isMouseButtonReleasedOnce(lua_State* l);

    int input_getMouseButtonReleasedValue(lua_State* l);
    int input_getMouseButtonPressedOnceValue(lua_State* l);
    int input_getMouseButtonPressedValue(lua_State* l);
    int input_getMouseButtonReleasedOnceValue(lua_State* l);

    int input_getMouseButtonReleasedAxis(lua_State* l);
    int input_getMouseButtonPressedOnceAxis(lua_State* l);
    int input_getMouseButtonPressedAxis(lua_State* l);
    int input_getMouseButtonReleasedOnceAxis(lua_State* l);

    // Input Grab
    int input_isGrab(lua_State* l);
    int input_setGrab(lua_State* l);
    int input_toggleGrab(lua_State* l);

    // Input Mapping ( I forgot to added in wrappers for the input mapping system :( )
    void input_mapping_load_library(lua_State* l);
    int input_mapping_isMappingReleased(lua_State* l);
    int input_mapping_isMappingPressedOnce(lua_State* l);
    int input_mapping_isMappingPressed(lua_State* l);
    int input_mapping_isMappingReleasedOnce(lua_State* l);
    int input_mapping_getMappingReleasedValue(lua_State* l);
    int input_mapping_getMappingPressedOnceValue(lua_State* l);
    int input_mapping_getMappingPressedValue(lua_State* l);
    int input_mapping_getMappingReleasedOnceValue(lua_State* l);
    int input_mapping_getMappingReleasedAxis(lua_State* l);
    int input_mapping_getMappingPressedOnceAxis(lua_State* l);
    int input_mapping_getMappingPressedAxis(lua_State* l);
    int input_mapping_getMappingReleasedOnceAxis(lua_State* l);

    // Physics
    void physics_load_library(lua_State* l);
    int physics_getGravity(lua_State* l);
    int physics_setGravity(lua_State* l);

    // Sound
    void sound_load_library(lua_State* l);
    int sound_setGroupVolume(lua_State* l);
    int sound_getGroupVolume(lua_State* l);
    int sound_setMasterVolume(lua_State* l);
    int sound_getMasterVolume(lua_State* l);

    // manager
    void manager_load_library(lua_State* l);
    int manager_createEntityFromPrefab(lua_State* l);

    // behavior
    void manager_behavior_load_library(lua_State* l);
    int manager_behavior_getBoolean(lua_State* l);
    int manager_behavior_getInteger(lua_State* l);
    int manager_behavior_getNumber(lua_State* l);
    int manager_behavior_getString(lua_State* l);
    int manager_behavior_setBoolean(lua_State* l);
    int manager_behavior_setInteger(lua_State* l);
    int manager_behavior_setNumber(lua_State* l);
    int manager_behavior_setString(lua_State* l);
    int manager_behavior_executeCallback(lua_State* l);


    // entity
    void manager_entity_load_library(lua_State* l);
    int manager_entity_getScene(lua_State* l);
    int manager_entity_getTransform(lua_State* l);
    int manager_entity_getBehavior(lua_State* l);
    
    int manager_entity_removeEntity(lua_State* l);
    int manager_entity_addChildEntity(lua_State* l);
    int manager_entity_getChildEntity(lua_State* l);
    int manager_entity_getChilderenAmount(lua_State* l);

    int manager_entity_isVisible(lua_State* l);
    int manager_entity_setVisible(lua_State* l);
    
    // scene
    void manager_scene_load_library(lua_State* l);
    int manager_scene_getGlobal(lua_State* l);
    int manager_scene_getNumEntity(lua_State* l);
    int manager_scene_getEntity(lua_State* l);
    int manager_scene_getBehavior(lua_State* l);
    int manager_scene_addEntity(lua_State* l);

    // global
    void manager_global_load_library(lua_State* l);
    int manager_global_changeScene(lua_State* l);
    int manager_global_getScene(lua_State* l);
    int manager_global_getBehavior(lua_State* l);

    // transform
    void manager_transform_load_library(lua_State* l);
    int manager_transform_getPosition(lua_State* l);
    int manager_transform_setPosition(lua_State* l);

    int manager_transform_getPositionX(lua_State* l);
    int manager_transform_getPositionY(lua_State* l);
    int manager_transform_getPositionZ(lua_State* l);

    int manager_transform_setPositionX(lua_State* l);
    int manager_transform_setPositionY(lua_State* l);
    int manager_transform_setPositionZ(lua_State* l);

    int manager_transform_getRotation(lua_State* l);
    
    int manager_transform_getRotationX(lua_State* l);
    int manager_transform_getRotationY(lua_State* l);
    int manager_transform_getRotationZ(lua_State* l);

    int manager_transform_setRotationX(lua_State* l);
    int manager_transform_setRotationY(lua_State* l);
    int manager_transform_setRotationZ(lua_State* l);

    int manager_transform_setRotation(lua_State* l);
    int manager_transform_getScale(lua_State* l);
    int manager_transform_setScale(lua_State* l);

    int manager_transform_getScaleX(lua_State* l);
    int manager_transform_getScaleY(lua_State* l);
    int manager_transform_getScaleZ(lua_State* l);

    int manager_transform_setScaleX(lua_State* l);
    int manager_transform_setScaleY(lua_State* l);
    int manager_transform_setScaleZ(lua_State* l);

    int manager_transform_getGlobalPosition(lua_State* l);
    int manager_transform_getGlobalPositionX(lua_State* l);
    int manager_transform_getGlobalPositionY(lua_State* l);
    int manager_transform_getGlobalPositionZ(lua_State* l);

    // Component
    void manager_component_load_library(lua_State* l);

    // CameraComponent
    void manager_component_camera_load_library(lua_State* l);
    int manager_component_camera_hasComponent(lua_State* l);
    int manager_component_camera_getComponent(lua_State* l);
    int manager_component_camera_getEntity(lua_State* l);
    int manager_component_camera_getFOV(lua_State* l);
    int manager_component_camera_setFOV(lua_State* l);
    int manager_component_camera_getZNear(lua_State* l);
    int manager_component_camera_setZNear(lua_State* l);
    int manager_component_camera_getZFar(lua_State* l);
    int manager_component_camera_setZFar(lua_State* l);

    // MeshComponent
    void manager_component_mesh_load_library(lua_State* l);
    int manager_component_mesh_hasComponent(lua_State* l);
    int manager_component_mesh_getComponent(lua_State* l);
    int manager_component_mesh_getEntity(lua_State* l);
    int manager_component_mesh_getMesh(lua_State* l);
    int manager_component_mesh_setMesh(lua_State* l);
    int manager_component_mesh_getMaterial(lua_State* l);
    int manager_component_mesh_setMaterial(lua_State* l);
    
    /*
    int manager_component_mesh_getTexture(lua_State* l);
    int manager_component_mesh_setTexture(lua_State* l);
    int manager_component_mesh_getUVScale(lua_State* l);
    int manager_component_mesh_setUVScale(lua_State* l);
    */
    // body ~ This covers all body types (dynamic, static, kinematic)
    void manager_component_body_load_library(lua_State* l);
    int manager_component_body_updateTransform(lua_State* l);
    int manager_component_body_getRotationY(lua_State* l);
    int manager_component_body_setRotationY(lua_State* l);
    int manager_component_body_setDamping(lua_State* l);
    int manager_component_body_getLinearDamping(lua_State* l);
    int manager_component_body_getAngularDamping(lua_State* l);
    int manager_component_body_getLinearSleepingThreshold(lua_State* l);
    int manager_component_body_getAngularSleepingThreshold(lua_State* l);
    int manager_component_body_applyDamping(lua_State* l);
    int manager_component_body_getLinearFactor(lua_State* l);
    int manager_component_body_setLinearFactor(lua_State* l);
    int manager_component_body_getInvMass(lua_State* l);
    int manager_component_body_getMass(lua_State* l);
    int manager_component_body_applyCentralForce(lua_State* l);
    int manager_component_body_getTotalForce(lua_State* l);
    int manager_component_body_getTotalTorque(lua_State* l);
    int manager_component_body_getInvInertiaDiagLocal(lua_State* l);
    int manager_component_body_setInvInertiaDiagLocal(lua_State* l);
    int manager_component_body_setSleepingThresholds(lua_State* l);
    int manager_component_body_applyTorque(lua_State* l);
    int manager_component_body_applyForce(lua_State* l);
    int manager_component_body_applyCentralImpulse(lua_State* l);
    int manager_component_body_applyTorqueImpulse(lua_State* l);
    int manager_component_body_applyImpulse(lua_State* l);
    int manager_component_body_applyPushImpulse(lua_State* l);
    int manager_component_body_getPushVelocity(lua_State* l);
    int manager_component_body_getTurnVelocity(lua_State* l);
    int manager_component_body_setPushVelocity(lua_State* l);
    int manager_component_body_setTurnVelocity(lua_State* l);
    int manager_component_body_applyCentralPushImpulse(lua_State* l);
    int manager_component_body_applyTorqueTurnImpulse(lua_State* l);
    int manager_component_body_clearForces(lua_State* l);
    int manager_component_body_getLinearVelocity(lua_State* l);
    int manager_component_body_getAngularVelocity(lua_State* l);
    int manager_component_body_setLinearVelocity(lua_State* l);
    int manager_component_body_setAngularVelocity(lua_State* l);
    int manager_component_body_getVelocityInLocalPoint(lua_State* l);
    int manager_component_body_getPushVelocityInLocalPoint(lua_State* l);
    int manager_component_body_setAngularFactorVector3(lua_State* l);
    int manager_component_body_setAngularFactorScalar(lua_State* l);
    // btCollisionObject.h
    int manager_component_body_isActive(lua_State* l);
    int manager_component_body_getActivationState(lua_State* l);
    int manager_component_body_setActivateState(lua_State* l);
    int manager_component_body_isStaticObject(lua_State* l);
    int manager_component_body_isKinematicObject(lua_State* l);

    // Static Body
    void manager_component_static_body_load_library(lua_State* l);
    int manager_component_static_body_hasComponent(lua_State* l);
    int manager_component_static_body_getComponent(lua_State* l);

    // Dynamic Body
    void manager_component_dynamic_body_load_library(lua_State* l);
    int manager_component_dynamic_body_hasComponent(lua_State* l);
    int manager_component_dynamic_body_getComponent(lua_State* l);

    // Kinematic Body
    void manager_component_kinematic_body_load_library(lua_State* l);
    int manager_component_kinematic_body_hasComponent(lua_State* l);
    int manager_component_kinematic_body_getComponent(lua_State* l);
    int manager_component_kinematic_body_getWorldTransformOrigin(lua_State* l);
    int manager_component_kinematic_body_setWorldTransformOrigin(lua_State* l);
    int manager_component_kinematic_body_getWorldTransformRotation(lua_State* l);
    int manager_component_kinematic_body_setWorldTransformRotation(lua_State* l);
    int manager_component_kinematic_body_isController(lua_State* l);
    int manager_component_kinematic_body_setIsController(lua_State* l);
    int manager_component_kinematic_body_getLinearVelocity(lua_State* l);
    int manager_component_kinematic_body_setLinearVelocity(lua_State* l);
    int manager_component_kinematic_body_moveAndSlide(lua_State* l);
    int manager_component_kinematic_body_isOnFloor(lua_State* l);


    // Trigger
    void manager_component_trigger_load_library(lua_State* l);
    int manager_component_trigger_hasComponent(lua_State* l);
    int manager_component_trigger_getComponent(lua_State* l);
    int manager_component_trigger_addEntityEnter(lua_State* l);
    int manager_component_trigger_removeEntityEnter(lua_State* l);
    int manager_component_trigger_addEntityExit(lua_State* l);
    int manager_component_trigger_removeEntityExit(lua_State* l);

    // PushArm
    void manager_component_push_arm_load_library(lua_State* l);
    int manager_component_push_arm_hasComponent(lua_State* l);
    int manager_component_push_arm_getComponent(lua_State* l);
    int manager_component_push_arm_getDistance(lua_State* l);
    int manager_component_push_arm_setDistance(lua_State* l);

    // RayCast
    void manager_component_raycast_load_library(lua_State* l);
    int manager_component_raycast_hasComponent(lua_State* l);
    int manager_component_raycast_getComponent(lua_State* l);
    int manager_component_raycast_isHit(lua_State* l);
    int manager_component_raycast_getWorldPoint(lua_State* l);
    int manager_component_raycast_getWorldNormal(lua_State* l);
    int manager_component_raycast_getDirection(lua_State* l);
    int manager_component_raycast_setDirection(lua_State* l);
    int manager_component_raycast_getScale(lua_State* l);
    int manager_component_raycast_setScale(lua_State* l);
    int manager_component_raycast_isDisabled(lua_State* l);
    int manager_component_raycast_setDisabled(lua_State* l);
    
    // SoundPlayerComponent
    void manager_component_sound_player_load_library(lua_State* l);
    int manager_component_sound_player_hasComponent(lua_State* l);
    int manager_component_sound_player_getComponent(lua_State* l);
    int manager_component_sound_player_play(lua_State* l);
    int manager_component_sound_player_pause(lua_State* l);
    int manager_component_sound_player_stop(lua_State* l);
    int manager_component_sound_player_setLooping(lua_State* l);
    int manager_component_sound_player_getLooping(lua_State* l);
    int manager_component_sound_player_setRelative(lua_State* l);
    int manager_component_sound_player_getRelative(lua_State* l);
    int manager_component_sound_player_setReferenceDistance(lua_State* l);
    int manager_component_sound_player_getReferenceDistance(lua_State* l);
    int manager_component_sound_player_setRollofFactor(lua_State* l);
    int manager_component_sound_player_getRollofFactor(lua_State* l);
    int manager_component_sound_player_setMaxDistance(lua_State* l);
    int manager_component_sound_player_getMaxDistance(lua_State* l);
    int manager_component_sound_player_setPitch(lua_State* l);
    int manager_component_sound_player_getPitch(lua_State* l);
    int manager_component_sound_player_isPlaying(lua_State* l);
    
    // util_random
    void util_random_load_library(lua_State* l);
    int util_random_randboolean(lua_State* l);
    int util_random_randrange(lua_State* l);
    int util_random_randf(lua_State* l);
}

#endif