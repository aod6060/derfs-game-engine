#include "lua/lua.hpp"
#include "../sys.hpp"


namespace script {

    void manager_transform_load_library(lua_State *l) {
        lua_register(l, "manager_transform_getPosition", manager_transform_getPosition);
        lua_register(l, "manager_transform_setPosition", manager_transform_setPosition);
        lua_register(l, "manager_transform_getPositionX", manager_transform_getPositionX);
        lua_register(l, "manager_transform_getPositionY", manager_transform_getPositionY);
        lua_register(l, "manager_transform_getPositionZ", manager_transform_getPositionZ);
        lua_register(l, "manager_transform_setPositionX", manager_transform_setPositionX);
        lua_register(l, "manager_transform_setPositionY", manager_transform_setPositionY);
        lua_register(l, "manager_transform_setPositionZ", manager_transform_setPositionZ);
        lua_register(l, "manager_transform_getRotation", manager_transform_getRotation);
        lua_register(l, "manager_transform_setRotation", manager_transform_setRotation);
        lua_register(l, "manager_transform_getRotationX", manager_transform_getRotationX);
        lua_register(l, "manager_transform_getRotationY", manager_transform_getRotationY);
        lua_register(l, "manager_transform_getRotationZ", manager_transform_getRotationZ);
        lua_register(l, "manager_transform_setRotationX", manager_transform_setRotationX);
        lua_register(l, "manager_transform_setRotationY", manager_transform_setRotationY);
        lua_register(l, "manager_transform_setRotationZ", manager_transform_setRotationZ);
        lua_register(l, "manager_transform_getScale", manager_transform_getScale);
        lua_register(l, "manager_transform_setScale", manager_transform_setScale);
        lua_register(l, "manager_transform_getScaleX", manager_transform_getScaleX);
        lua_register(l, "manager_transform_getScaleY", manager_transform_getScaleY);
        lua_register(l, "manager_transform_getScaleZ", manager_transform_getScaleZ);
        lua_register(l, "manager_transform_setScaleX", manager_transform_setScaleX);
        lua_register(l, "manager_transform_setScaleY", manager_transform_setScaleY);
        lua_register(l, "manager_transform_setScaleZ", manager_transform_setScaleZ);
    }

    int manager_transform_getPosition(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->position.x);
        lua_pushnumber(l, transform->position.y);
        lua_pushnumber(l, transform->position.z);
        return 3;
    }

    int manager_transform_setPosition(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float x = lua_tonumber(l, 2);
        float y = lua_tonumber(l, 3);
        float z = lua_tonumber(l, 4);
        transform->position = glm::vec3(x, y, z);
        return 0;
    }

    int manager_transform_getPositionX(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->position.x);
        return 1;
    }

    int manager_transform_getPositionY(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->position.y);
        return 1;
    }

    int manager_transform_getPositionZ(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->position.z);
        return 1;
    }

    int manager_transform_setPositionX(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->position.x = value;
        return 0;
    }

    int manager_transform_setPositionY(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->position.y = value;
        return 0;
    }

    int manager_transform_setPositionZ(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->position.z = value;
        return 0;
    }

    int manager_transform_getRotation(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->rotation.x);
        lua_pushnumber(l, transform->rotation.y);
        lua_pushnumber(l, transform->rotation.z);
        return 3;
    }

    int manager_transform_setRotation(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float x = lua_tonumber(l, 2);
        float y = lua_tonumber(l, 3);
        float z = lua_tonumber(l, 4);
        transform->rotation = glm::vec3(x, y, z);
        return 0;
    }

    int manager_transform_getRotationX(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->rotation.x);
        return 1;
    }

    int manager_transform_getRotationY(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->rotation.y);
        return 1;
    }

    int manager_transform_getRotationZ(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->rotation.z);
        return 1;
    }

    int manager_transform_setRotationX(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->rotation.x = value;
        return 0;
    }

    int manager_transform_setRotationY(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->rotation.y = value;
        return 0;
    }

    int manager_transform_setRotationZ(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->rotation.z = value;
        return 0;
    }

    int manager_transform_getScale(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->scale.x);
        lua_pushnumber(l, transform->scale.y);
        lua_pushnumber(l, transform->scale.z);
        return 3;
    }

    int manager_transform_setScale(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float x = lua_tonumber(l, 2);
        float y = lua_tonumber(l, 3);
        float z = lua_tonumber(l, 4);
        transform->scale = glm::vec3(x, y, z);
        return 0;
    }

    int manager_transform_getScaleX(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->scale.x);
        return 1;
    }

    int manager_transform_getScaleY(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->scale.y);
        return 1;
    }

    int manager_transform_getScaleZ(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        lua_pushnumber(l, transform->scale.z);
        return 1;
    }

    int manager_transform_setScaleX(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->scale.x = value;
        return 0;
    }

    int manager_transform_setScaleY(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->scale.y = value;
        return 0;
    }

    int manager_transform_setScaleZ(lua_State* l) {
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 1);
        float value = lua_tonumber(l, 2);
        transform->scale.z = value;
        return 0;
    }
}