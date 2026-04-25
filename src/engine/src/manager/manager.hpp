#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <SDL.h>
#include <json/json.h>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>
#include <lua/lua.hpp>
#include <functional>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "../input/input.hpp"
#include "../render/render.hpp"
#include "../sound/sound.hpp"

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

    // I've going to completely redo this in C++ instead of lua
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

    // I want the global object to support more then one behavior. 
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

            struct LightComponent : public IComponent {
                Entity* entity = nullptr;

                /*
                glm::vec3 albedo;
                float ambient;
                float diffuse;
                float specular;
                bool isOn;
                */
                ::render::shader::lighting::Light light;

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

#endif