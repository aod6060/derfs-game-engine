#include "lua/lua.hpp"
#include "../sys.hpp"


namespace script {

    void manager_scene_load_library(lua_State *l) {
        // int manager_scene_getGlobal(lua_State* l);
        lua_register(l, "manager_scene_getGlobal", manager_scene_getGlobal);
        // int manager_scene_getNumEntity(lua_State* l);
        lua_register(l, "manager_scene_getNumEntity", manager_scene_getNumEntity);
        // int manager_scene_getEntity(lua_State* l);
        lua_register(l, "manager_scene_getEntity", manager_scene_getEntity);
        // manager_scene_getBehavior
        lua_register(l, "manager_scene_getBehavior", manager_scene_getBehavior);
    }

    int manager_scene_getGlobal(lua_State* l) {
        manager::Scene* scene = (manager::Scene*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, scene->global);
        return 1;
    }

    int manager_scene_getNumEntity(lua_State* l) {
        manager::Scene* scene = (manager::Scene*)lua_touserdata(l, 1);
        lua_pushinteger(l, scene->entities.size());
        return 1;
    }

    int manager_scene_getEntity(lua_State* l) {
        manager::Scene* scene = (manager::Scene*)lua_touserdata(l, 1);
        size_t index = lua_tointeger(l, 2);
        lua_pushlightuserdata(l, scene->entities[index]);
        return 1;
    }

    int manager_scene_getBehavior(lua_State* l) {
        manager::Scene* scene = (manager::Scene*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, scene->behavior);
        return 1;
    }
}