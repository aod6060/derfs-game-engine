#include "lua/lua.hpp"
#include "../sys.hpp"


namespace script {
    void manager_behavior_load_library(lua_State* l) {
        // int manager_behavior_getBoolean(lua_State* l);
        lua_register(l, "manager_behavior_getBoolean", manager_behavior_getBoolean);
        // int manager_behavior_getInteger(lua_State* l);
        lua_register(l, "manager_behavior_getInteger", manager_behavior_getInteger);
        // int manager_behavior_getNumber(lua_State* l);
        lua_register(l, "manager_behavior_getNumber", manager_behavior_getNumber);
        // int manager_behavior_getString(lua_State* l);
        lua_register(l, "manager_behavior_getString", manager_behavior_getString);
        // int manager_behavior_setBoolean(lua_State* l);
        lua_register(l, "manager_behavior_setBoolean", manager_behavior_setBoolean);
        // int manager_behavior_setInteger(lua_State* l);
        lua_register(l, "manager_behavior_setInteger", manager_behavior_setInteger);
        // int manager_behavior_setNumber(lua_State* l);
        lua_register(l, "manager_behavior_setNumber", manager_behavior_setNumber);
        // int manager_behavior_setString(lua_State* l);
        lua_register(l, "manager_behavior_setString", manager_behavior_setString);
    }

    int manager_behavior_getBoolean(lua_State* l) {
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        lua_pushboolean(l, behavior->getBoolean(name));
        return 1;
    }

    int manager_behavior_getInteger(lua_State* l) {
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        lua_pushinteger(l, behavior->getInteger(name));
        return 1;
    }

    int manager_behavior_getNumber(lua_State* l) {
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        lua_pushnumber(l, behavior->getNumber(name));
        return 1;
    }

    int manager_behavior_getString(lua_State* l) {
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        lua_pushstring(l, behavior->getString(name).c_str());
        return 1;
    }

    int manager_behavior_setBoolean(lua_State* l) {
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        behavior->setBoolean(name, lua_toboolean(l, 3));
        return 0;
    }

    int manager_behavior_setInteger(lua_State* l) {
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        behavior->setInteger(name, lua_tointeger(l, 3));
        return 0;
    }

    int manager_behavior_setNumber(lua_State* l) {
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        behavior->setNumber(name, lua_tonumber(l, 3));
        return 0;
    }

    int manager_behavior_setString(lua_State* l) {
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        behavior->setString(name, lua_tostring(l, 3));
        return 0;
    }


}