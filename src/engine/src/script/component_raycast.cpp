#include "../sys.hpp"
#include "lua/lua.hpp"

namespace script {
    // RayCast
    void manager_component_raycast_load_library(lua_State* l) {
        // int manager_component_raycast_hasComponent(lua_State* l);
        lua_register(l, "manager_component_raycast_hasComponent", manager_component_raycast_hasComponent);
        // int manager_component_raycast_getComponent(lua_State* l);
        lua_register(l, "manager_component_raycast_getComponent", manager_component_raycast_getComponent);
        // int manager_component_raycast_isHit(lua_State* l);
        lua_register(l, "manager_component_raycast_isHit", manager_component_raycast_isHit);
        // int manager_component_raycast_getWorldPoint(lua_State* l);
        lua_register(l, "manager_component_raycast_getWorldPoint", manager_component_raycast_getWorldPoint);
        // int manager_component_raycast_getWorldNormal(lua_State* l);
        lua_register(l, "manager_component_raycast_getWorldNormal", manager_component_raycast_getWorldNormal);
        // int manager_component_raycast_getDirection(lua_State* l);
        lua_register(l, "manager_component_raycast_getDirection", manager_component_raycast_getDirection);
        // int manager_component_raycast_setDirection(lua_State* l);
        lua_register(l, "manager_component_raycast_setDirection", manager_component_raycast_setDirection);
        // int manager_component_raycast_getScale(lua_State* l);
        lua_register(l, "manager_component_raycast_getScale", manager_component_raycast_getScale);
        // int manager_component_raycast_setScale(lua_State* l);
        lua_register(l, "manager_component_raycast_setScale", manager_component_raycast_setScale);
        // int manager_component_raycast_isDisabled(lua_State* l);
        lua_register(l, "manager_component_raycast_isDisabled", manager_component_raycast_isDisabled);
        // int manager_component_raycast_setDisabled(lua_State* l);
        lua_register(l, "manager_component_raycast_setDisabled", manager_component_raycast_setDisabled);
    }

    int manager_component_raycast_hasComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("ray-cast-component") != entity->components.end());
        return 1;
    }

    int manager_component_raycast_getComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("ray-cast-component"));
        return 1;
    }

    int manager_component_raycast_isHit(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, rayCast->isHit);
        return 1;
    }

    int manager_component_raycast_getWorldPoint(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, rayCast->worldPoint.x);
        lua_pushnumber(l, rayCast->worldPoint.y);
        lua_pushnumber(l, rayCast->worldPoint.z);
        return 3;
    }

    int manager_component_raycast_getWorldNormal(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, rayCast->worldNormal.x);
        lua_pushnumber(l, rayCast->worldNormal.y);
        lua_pushnumber(l, rayCast->worldNormal.z);
        return 3;
    }

    int manager_component_raycast_getDirection(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, rayCast->direction.x);
        lua_pushnumber(l, rayCast->direction.y);
        lua_pushnumber(l, rayCast->direction.z);
        return 3;
    }

    int manager_component_raycast_setDirection(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        rayCast->direction = glm::vec3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        return 0;
    }

    int manager_component_raycast_getScale(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, rayCast->scale);
        return 1;
    }

    int manager_component_raycast_setScale(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        rayCast->scale = lua_tonumber(l, 2);
        return 0;
    }

    int manager_component_raycast_isDisabled(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, rayCast->disabled);
        return 1;
    }

    int manager_component_raycast_setDisabled(lua_State* l) {
        manager::component::physics::RayCastComponent* rayCast = (manager::component::physics::RayCastComponent*)lua_touserdata(l, 1);
        rayCast->disabled = lua_toboolean(l, 2);
        return 0;
    }

}