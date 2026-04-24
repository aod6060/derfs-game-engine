#include "script_hidden.hpp"

namespace script {

    // PushArm
    void manager_component_push_arm_load_library(lua_State* l) {
        // int manager_component_push_arm_hasComponent(lua_State* l);
        lua_register(l, "manager_component_push_arm_hasComponent", manager_component_push_arm_hasComponent);
        // int manager_component_push_arm_getComponent(lua_State* l);
        lua_register(l, "manager_component_push_arm_getComponent", manager_component_push_arm_getComponent);
        // int manager_component_push_arm_getDistance(lua_State* l);
        lua_register(l, "manager_component_push_arm_getDistance", manager_component_push_arm_getDistance);
        // int manager_component_push_arm_setDistance(lua_State* l);
        lua_register(l, "manager_component_push_arm_setDistance", manager_component_push_arm_setDistance);
    }

    int manager_component_push_arm_hasComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("push-arm-component") != entity->components.end());
        return 1;
    }

    int manager_component_push_arm_getComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("push-arm-component"));
        return 1;
    }

    int manager_component_push_arm_getDistance(lua_State* l) {
        manager::component::physics::PushArmComponent* pushArm = (manager::component::physics::PushArmComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, pushArm->distance);
        return 1;
    }

    int manager_component_push_arm_setDistance(lua_State* l) {
        manager::component::physics::PushArmComponent* pushArm = (manager::component::physics::PushArmComponent*)lua_touserdata(l, 1);
        pushArm->distance = lua_tonumber(l, 2);
        return 0;
    }


}