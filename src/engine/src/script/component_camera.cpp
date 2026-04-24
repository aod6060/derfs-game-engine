#include "script_hidden.hpp"

namespace script {

    void manager_component_camera_load_library(lua_State* l) {
        // int manager_component_camera_hasComponent(lua_State* l);
        lua_register(l, "manager_component_camera_hasComponent", manager_component_camera_hasComponent);
        // int manager_component_camera_getComponent(lua_State* l);
        lua_register(l, "manager_component_camera_getComponent", manager_component_camera_getComponent);
        // int manager_component_CameraComponent_getEntity(lua_State* l);
        lua_register(l, "manager_component_camera_getEntity", manager_component_camera_getEntity);
        // int manager_component_CameraComponent_getFOV(lua_State* l);
        lua_register(l, "manager_component_camera_getFOV", manager_component_camera_getFOV);
        // int manager_component_CameraComponent_setFOV(lua_State* l);
        lua_register(l, "manager_component_camera_setFOV", manager_component_camera_setFOV);
        // int manager_component_CameraComponent_getZNear(lua_State* l);
        lua_register(l, "manager_component_camera_getZNear", manager_component_camera_getZNear);
        // int manager_component_CameraComponent_setZNear(lua_State* l);
        lua_register(l, "manager_component_camera_setZNear", manager_component_camera_setZNear);
        // int manager_component_CameraComponent_getZFar(lua_State* l);
        lua_register(l, "manager_component_camera_getZFar", manager_component_camera_getZFar);
        // int manager_component_CameraComponent_setZFar(lua_State* l);
        lua_register(l, "manager_component_camera_setZFar", manager_component_camera_setZFar);
    }

    int manager_component_camera_hasComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("camera-component") != entity->components.end());
        return 1;  
    }

    int manager_component_camera_getComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("camera-component"));
        return 1;
    }
    
    int manager_component_camera_getEntity(lua_State* l) {
        manager::component::render::CameraComponent* comp = (manager::component::render::CameraComponent*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, comp->entity);
        return 1;
    }

    int manager_component_camera_getFOV(lua_State* l) {
        manager::component::render::CameraComponent* comp = (manager::component::render::CameraComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->fov);
        return 1;
    }

    int manager_component_camera_setFOV(lua_State* l) {
        manager::component::render::CameraComponent* comp = (manager::component::render::CameraComponent*)lua_touserdata(l, 1);
        comp->fov = lua_tonumber(l, 2);
        return 0;
    }

    int manager_component_camera_getZNear(lua_State* l) {
        manager::component::render::CameraComponent* comp = (manager::component::render::CameraComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->znear);
        return 1;
    }

    int manager_component_camera_setZNear(lua_State* l) {
        manager::component::render::CameraComponent* comp = (manager::component::render::CameraComponent*)lua_touserdata(l, 1);
        comp->znear = lua_tonumber(l, 2);
        return 0;
    }

    int manager_component_camera_getZFar(lua_State* l) {
        manager::component::render::CameraComponent* comp = (manager::component::render::CameraComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->zfar);
        return 1;
    }

    int manager_component_camera_setZFar(lua_State* l) {
        manager::component::render::CameraComponent* comp = (manager::component::render::CameraComponent*)lua_touserdata(l, 1);
        comp->zfar = lua_tonumber(l, 2);
        return 0;
    }


}