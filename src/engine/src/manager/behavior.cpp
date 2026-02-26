#include "lua/lua.hpp"
#include "../sys.hpp"

namespace manager {


    void Behavior::init(std::string path, Entity* entity) {
        this->state = luaL_newstate();
        luaL_openlibs(this->state);
        script::load_library(this->state);
        lua_pushlightuserdata(this->state, entity);
        lua_setglobal(this->state, "entity");
        int value = luaL_dofile(this->state, path.c_str());

        if(value != LUA_OK) {
            std::string err = lua_tostring(this->state, -1);
            std::cout << err << "\n";
        }
        lua_getglobal(this->state, "init");
        lua_pcall(this->state, 0, 0, 0); 
    }

    void Behavior::init(std::string path, Scene* scene) {
        this->state = luaL_newstate();
        luaL_openlibs(this->state);
        script::load_library(this->state);
        lua_pushlightuserdata(this->state, scene);
        lua_setglobal(this->state, "scene");
        int value = luaL_dofile(this->state, path.c_str());
        if(value != LUA_OK) {
            std::string err = lua_tostring(this->state, -1);
            std::cout << err << "\n";
        }
        lua_getglobal(this->state, "init");
        lua_pcall(this->state, 0, 0, 0);
    }

    void Behavior::init(std::string path, Global* global) {
        this->state = luaL_newstate();
        luaL_openlibs(this->state);
        script::load_library(this->state);
        lua_pushlightuserdata(this->state, global);
        lua_setglobal(this->state, "global");
        int value = luaL_dofile(this->state, path.c_str());
        if(value != LUA_OK) {
            std::string err = lua_tostring(this->state, -1);
            std::cout << err << "\n";
        }
        lua_getglobal(this->state, "init");
        lua_pcall(this->state, 0, 0, 0);
    }

    void Behavior::update(float delta) {
        lua_getglobal(this->state, "update");
        lua_pushnumber(this->state, delta);
        lua_pcall(this->state, 1, 0, 0);
    }

    void Behavior::release() {
        lua_getglobal(this->state, "release");
        lua_pcall(this->state, 0, 0, 0);
        if(this->entity) {
            lua_getglobal(this->state, "entity");
            lua_pushlightuserdata(state, nullptr);
            this->entity = nullptr;
        } else if(this->scene) {
            lua_getglobal(this->state, "scene");
            lua_pushlightuserdata(state, nullptr);
            this->scene = nullptr;
        } else if(this->global) {
            lua_getglobal(this->state, "global");
            lua_pushlightuserdata(state, nullptr);
            this->global = nullptr;
        }
        lua_close(this->state);
    }

    // Getters
    bool Behavior::getBoolean(std::string name) {
        int status = lua_getglobal(this->state, name.c_str());
        bool value = lua_toboolean(this->state, -1);
        lua_pop(this->state, -1);
        return value;
    }

    int Behavior::getInteger(std::string name) {
        int status = lua_getglobal(this->state, name.c_str());
        int value = lua_tointeger(this->state, -1);
        lua_pop(this->state, -1);
        return value;
    }

    float Behavior::getNumber(std::string name) {
        int status = lua_getglobal(this->state, name.c_str());
        float value = lua_tonumber(this->state, -1);
        lua_pop(this->state, -1);
        return value;
    }

    std::string Behavior::getString(std::string name) {
        int status = lua_getglobal(this->state, name.c_str());
        std::string value = lua_tostring(this->state, -1);
        lua_pop(this->state, -1);
        return value;
    }

    // Setters
    void Behavior::setBoolean(std::string name, bool value) {
        lua_pushboolean(this->state, value);
        lua_setglobal(this->state, name.c_str());
    }

    void Behavior::setInteger(std::string name, int value) {
        lua_pushinteger(this->state, value);
        lua_setglobal(this->state, name.c_str());
    }

    void Behavior::setNumber(std::string name, float value) {
        lua_pushnumber(this->state, value);
        lua_setglobal(this->state, name.c_str());
    }

    void Behavior::setString(std::string name, std::string value) {
        lua_pushstring(this->state, value.c_str());
        lua_setglobal(this->state, name.c_str());
    }

    void Behavior::executeCallback(std::string name, const std::vector<Argument>& args) {

        lua_getglobal(this->state, name.c_str());
        for(int i = 0; i < args.size(); i++) {
            switch(args.at(i).type) {
                case Type::T_BOOL:
                    lua_pushboolean(this->state, args.at(i).bValue);
                    break;
                case Type::T_INTEGER:
                    lua_pushinteger(this->state, args.at(i).iValue);
                    break;
                case Type::T_NUMBER:
                    lua_pushnumber(this->state, args.at(i).nValue);
                    break;
                case Type::T_STRING:
                    lua_pushstring(this->state, args.at(i).sValue.c_str());
                    break;
                case Type::T_USERDATA:
                    lua_pushlightuserdata(this->state, args.at(i).uValue);
                default:
                    break;
            }
        }
        int result = lua_pcall(this->state, args.size(), 0, 0);
        if(result != LUA_OK) {
            std::cout << lua_tostring(this->state, -1) << "\n";
        }
    }
}