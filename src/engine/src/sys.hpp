#ifndef SYS_HPP
#define SYS_HPP



// Once this file gets above 2000 to 3000 lines of code I'll refactor it.

#include "json/value.h"
#include <iostream>
#include <fstream>
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

        // Program
        struct Program {
            uint32_t id = 0;
            std::vector<Shader*> shaders;
            Uniform uniforms;
            Attribute attributes;

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
    }

    namespace mesh {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
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
            render::glw::VertexBuffer texCoords;
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

    struct MainShader : public IShader {
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

        void setProjection(glm::mat4 proj);
        void setView(glm::mat4 view);
        void setModel(glm::mat4 model);
        void setUVScale(float value);

        void verticePointer();
        void texCoordPointer();

        void drawMesh(render::mesh::Mesh* mesh);
    };

    void init();
    void release();

    void clear(glm::vec4 clearColor);

    void drawArrays(GLenum type, uint32_t vertexCount);
    void drawElements(GLenum type, uint32_t count);

    MainShader* getMainShader();

}

namespace physics {


    void init();
    void release();

    uint32_t getMaxBodies();
    void setMaxBodies(uint32_t num);

    uint32_t getMaxBodyPairs();
    void setMaxBodyPairs(uint32_t num);

    uint32_t getMaxContactConstraints();
    void setMaxContactContraints(uint32_t num);


}

namespace assets {
    void init();
    void release();

    render::mesh::Mesh* getMesh(std::string name);
    render::glw::Texture2D* getTexture2D(std::string name);
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
    
    namespace component {
        struct IComponent {
            virtual void init(Entity* entity) = 0;
            virtual void handleEvent(SDL_Event* e) = 0;
            virtual void update(float delta) = 0;
            virtual void preRender() = 0;
            virtual void render() = 0;
            virtual void release() = 0;
            virtual void load(Json::Value value) = 0;
        };

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
            std::string texture;
            float uvScale;

            virtual void init(Entity* entity);
            virtual void handleEvent(SDL_Event* e);
            virtual void update(float delta);
            virtual void preRender();
            virtual void render();
            virtual void release();
            virtual void load(Json::Value value);
        };
    }

    struct Transform {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 toModel();

        void load(Json::Value v);
    };

    struct Behavior {
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

        
    };

    struct EntityDelelte {
        Entity* entity;
        std::vector<Entity*>::iterator it;
    };

    struct Entity {
        std::string type;
        std::string name;
        std::string prefabPath;
        Scene* scene = nullptr;

        Entity* parent = nullptr;
        std::vector<Entity*> childeren;
        //std::queue<EntityDelelte> entityDels;

        Transform transform;

        std::string script;
        Behavior* behavior = nullptr;

        component::CameraComponent* cameraComponent = nullptr;
        component::MeshComponent* meshComponent = nullptr;

        bool needRemoval = false;

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
        
        // @decrepracated
        glm::mat4 calculateParent();
        
        // @decrepracated
        glm::mat4 calculateParentMatrix();

        glm::mat4 toParentMatrix(Entity* entity);

        glm::vec3 getGlobalPosition();

        void addChildEntity(Entity* entity);
        Entity* getChildEntity(size_t index);
        size_t getChilderenAmount();
        void removeEntity(Entity* entity);
    };

    struct Scene {
        Global* global = nullptr;
        std::vector<Entity*> entities;
        std::queue<EntityDelelte> entityDels;
        //Camera camera;
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
    };
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

    // entity
    void manager_entity_load_library(lua_State* l);
    int manager_entity_getScene(lua_State* l);
    int manager_entity_getTransform(lua_State* l);
    int manager_entity_getBehavior(lua_State* l);
    int manager_entity_hasMeshComponent(lua_State* l);
    int manager_entity_getMeshComponent(lua_State* l);
    int manager_entity_hasCameraComponent(lua_State* l);
    int manager_entity_getCameraComponent(lua_State* l);
    int manager_entity_removeEntity(lua_State* l);
    int manager_entity_addChildEntity(lua_State* l);
    int manager_entity_getGlobalPosition(lua_State* l);
    int manager_entity_getChildEntity(lua_State* l);
    int manager_entity_getChilderenAmount(lua_State* l);

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

    // CameraComponent
    void manager_component_CameraComponent_load_library(lua_State* l);
    int manager_component_CameraComponent_getEntity(lua_State* l);
    int manager_component_CameraComponent_getFOV(lua_State* l);
    int manager_component_CameraComponent_setFOV(lua_State* l);
    int manager_component_CameraComponent_getZNear(lua_State* l);
    int manager_component_CameraComponent_setZNear(lua_State* l);
    int manager_component_CameraComponent_getZFar(lua_State* l);
    int manager_component_CameraComponent_setZFar(lua_State* l);

    // MeshComponent
    void manager_component_MeshComponent_load_library(lua_State* l);
    int manager_component_MeshComponent_getEntity(lua_State* l);
    int manager_component_MeshComponent_getMesh(lua_State* l);
    int manager_component_MeshComponent_setMesh(lua_State* l);
    int manager_component_MeshComponent_getTexture(lua_State* l);
    int manager_component_MeshComponent_setTexture(lua_State* l);
    int manager_component_MeshComponent_getUVScale(lua_State* l);
    int manager_component_MeshComponent_setUVScale(lua_State* l);

    // util_random
    void util_random_load_library(lua_State* l);
    int util_random_randboolean(lua_State* l);
    int util_random_randrange(lua_State* l);
    int util_random_randf(lua_State* l);
}

#endif