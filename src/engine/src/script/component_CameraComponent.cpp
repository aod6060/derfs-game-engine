#include "lua/lua.hpp"
#include "../sys.hpp"

namespace script {

    void manager_component_CameraComponent_load_library(lua_State* l) {
        // int manager_component_CameraComponent_getEntity(lua_State* l);
        lua_register(l, "manager_component_CameraComponent_getEntity", manager_component_CameraComponent_getEntity);
        // int manager_component_CameraComponent_getFOV(lua_State* l);
        lua_register(l, "manager_component_CameraComponent_getFOV", manager_component_CameraComponent_getFOV);
        // int manager_component_CameraComponent_setFOV(lua_State* l);
        lua_register(l, "manager_component_CameraComponent_setFOV", manager_component_CameraComponent_setFOV);
        // int manager_component_CameraComponent_getZNear(lua_State* l);
        lua_register(l, "manager_component_CameraComponent_getZNear", manager_component_CameraComponent_getZNear);
        // int manager_component_CameraComponent_setZNear(lua_State* l);
        lua_register(l, "manager_component_CameraComponent_setZNear", manager_component_CameraComponent_setZNear);
        // int manager_component_CameraComponent_getZFar(lua_State* l);
        lua_register(l, "manager_component_CameraComponent_getZFar", manager_component_CameraComponent_getZFar);
        // int manager_component_CameraComponent_setZFar(lua_State* l);
        lua_register(l, "manager_component_CameraComponent_setZFar", manager_component_CameraComponent_setZFar);
    }

    int manager_component_CameraComponent_getEntity(lua_State* l) {
        manager::component::CameraComponent* comp = (manager::component::CameraComponent*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, comp->entity);
        return 1;
    }

    int manager_component_CameraComponent_getFOV(lua_State* l) {
        manager::component::CameraComponent* comp = (manager::component::CameraComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->fov);
        return 1;
    }

    int manager_component_CameraComponent_setFOV(lua_State* l) {
        manager::component::CameraComponent* comp = (manager::component::CameraComponent*)lua_touserdata(l, 1);
        comp->fov = lua_tonumber(l, 2);
        return 0;
    }

    int manager_component_CameraComponent_getZNear(lua_State* l) {
        manager::component::CameraComponent* comp = (manager::component::CameraComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->znear);
        return 1;
    }

    int manager_component_CameraComponent_setZNear(lua_State* l) {
        manager::component::CameraComponent* comp = (manager::component::CameraComponent*)lua_touserdata(l, 1);
        comp->znear = lua_tonumber(l, 2);
        return 0;
    }

    int manager_component_CameraComponent_getZFar(lua_State* l) {
        manager::component::CameraComponent* comp = (manager::component::CameraComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->zfar);
        return 1;
    }

    int manager_component_CameraComponent_setZFar(lua_State* l) {
        manager::component::CameraComponent* comp = (manager::component::CameraComponent*)lua_touserdata(l, 1);
        comp->zfar = lua_tonumber(l, 2);
        return 0;
    }


}