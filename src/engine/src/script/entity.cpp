#include "lua/lua.hpp"
#include "../sys.hpp"


namespace script {

    // entity
    void manager_entity_load_library(lua_State* l) {
        //int manager_entity_getScene(lua_State* l);
        lua_register(l, "manager_entity_getScene", manager_entity_getScene);
        //int manager_entity_getTransform(lua_State* l);
        lua_register(l, "manager_entity_getTransform", manager_entity_getTransform);
        // int manager_entity_getBehavior(lua_State* l);
        lua_register(l, "manager_entity_getBehavior", manager_entity_getBehavior);
        // int manager_entity_hasMeshComponent(lua_State* l);
        lua_register(l, "manager_entity_hasMeshComponent", manager_entity_hasMeshComponent);
        // int manager_entity_getMeshComponent(lua_State* l);
        lua_register(l, "manager_entity_getMeshComponent", manager_entity_getMeshComponent);
        // int manager_entity_hasCameraComponent(lua_State* l);
        lua_register(l, "manager_entity_hasCameraComponent", manager_entity_hasCameraComponent);
        // int manager_entity_getCameraComponent(lua_State* l);
        lua_register(l, "manager_entity_getCameraComponent", manager_entity_getCameraComponent);
        // int manager_entity_removeEntity(lua_State* l);
        lua_register(l, "manager_entity_removeEntity", manager_entity_removeEntity);
        // manager_entity_addChildEntity
        lua_register(l, "manager_entity_addChildEntity", manager_entity_addChildEntity);
    }

    int manager_entity_getScene(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->scene);
        return 1;
    }

    int manager_entity_getTransform(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, &entity->transform);
        return 1;
    }

    int manager_entity_getBehavior(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->behavior);
        return 1;
    }

    int manager_entity_hasMeshComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->meshComponent != nullptr);
        return 1;
    }

    int manager_entity_getMeshComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->meshComponent);
        return 1;
    }

    int manager_entity_hasCameraComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->cameraComponent != nullptr);
        return 1;
    }

    int manager_entity_getCameraComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->cameraComponent);
        return 1;
    }

    int manager_entity_removeEntity(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        if(entity->hasParent()) {
            entity->parent->removeEntity(entity);
        } else {
            entity->scene->removeEntity(entity);
        }
        return 0;
    }

    int manager_entity_addChildEntity(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        manager::Entity* child = (manager::Entity*)lua_touserdata(l, 2);
        entity->addChildEntity(child);
        return 0;
    }
}