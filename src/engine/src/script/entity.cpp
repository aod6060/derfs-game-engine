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
        // int manager_entity_hasStaticBodyComponent(lua_State* l);
        lua_register(l, "manager_entity_hasStaticBodyComponent", manager_entity_hasStaticBodyComponent);
        // int manager_entity_getStaticBodyComponent(lua_State* l);
        lua_register(l, "manager_entity_getStaticBodyComponent", manager_entity_getStaticBodyComponent);
        // int manager_entity_hasDynamicBodyComponent(lua_State* l);
        lua_register(l, "manager_entity_hasDynamicBodyComponent", manager_entity_hasDynamicBodyComponent);
        // int manager_entity_getDynamicBodyComponent(lua_State* l);
        lua_register(l, "manager_entity_getDynamicBodyComponent", manager_entity_getDynamicBodyComponent);
        //int manager_entity_hasKinematicBodyComponent(lua_State* l);
        lua_register(l, "manager_entity_hasKinematicBodyComponent", manager_entity_hasKinematicBodyComponent);
        //int manager_entity_getKinematicBodyComponent(lua_State* l);
        lua_register(l, "manager_entity_getKinematicBodyComponent", manager_entity_getKinematicBodyComponent);
        // int manager_entity_removeEntity(lua_State* l);
        lua_register(l, "manager_entity_removeEntity", manager_entity_removeEntity);
        // manager_entity_addChildEntity
        lua_register(l, "manager_entity_addChildEntity", manager_entity_addChildEntity);
        // int manager_entity_getChildEntity(lua_State* l);
        lua_register(l, "manager_entity_getChildEntity", manager_entity_getChildEntity);
        // int manager_entity_getChilderenAmount(lua_State* l);
        lua_register(l, "manager_entity_getChilderenAmount", manager_entity_getChilderenAmount);
        // int manager_entity_isVisible(lua_State* l);
        lua_register(l, "manager_entity_isVisible", manager_entity_isVisible);
        // int manager_entity_setVisible(lua_State* l);
        lua_register(l, "manager_entity_setVisible", manager_entity_setVisible);
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
        lua_pushboolean(l, entity->components.find("mesh-component") != entity->components.end());
        return 1;
    }

    int manager_entity_getMeshComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("mesh-component"));
        return 1;
    }

    int manager_entity_hasCameraComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("camera-component") != entity->components.end());
        return 1;
    }

    int manager_entity_getCameraComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("camera-component"));
        return 1;
    }

    int manager_entity_hasStaticBodyComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("static-body-component") != entity->components.end());
        return 1;
    }

    int manager_entity_getStaticBodyComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("static-body-component"));
        return 1;
    }
    
    int manager_entity_hasDynamicBodyComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("dynamic-body-component") != entity->components.end());
        return 1;
    }

    int manager_entity_getDynamicBodyComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("dynamic-body-component"));
        return 1;
    }

    int manager_entity_hasKinematicBodyComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("kinematic-body-component") != entity->components.end());
        return 1;
    }

    int manager_entity_getKinematicBodyComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("kinematic-body-component"));
        return 1;
    }

    int manager_entity_removeEntity(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        if(entity->hasParent()) {
            entity->parent->removeEntity(entity);
            std::cout << "Remove From Parent\n";
        } else {
            std::cout << "Remove From Scene\n";
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

    int manager_entity_getChildEntity(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        int index = lua_tointeger(l, 2);
        lua_pushlightuserdata(l, entity->getChildEntity(index));
        return 1;
    }

    int manager_entity_getChilderenAmount(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushinteger(l, entity->getChilderenAmount());
        return 1;
    }

    int manager_entity_isVisible(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->visible);
        return 1;
    }

    int manager_entity_setVisible(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        entity->visible = lua_toboolean(l, 2);
        return 0;
    }

}