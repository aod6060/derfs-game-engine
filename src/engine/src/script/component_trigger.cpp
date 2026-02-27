#include "../sys.hpp"
#include "lua/lua.hpp"

namespace script {

    // Trigger
    void manager_component_trigger_load_library(lua_State* l) {
        // int manager_component_trigger_hasComponent(lua_State* l);
        lua_register(l, "manager_component_trigger_hasComponent", manager_component_trigger_hasComponent);
        // int manager_component_trigger_getComponent(lua_State* l);
        lua_register(l, "manager_component_trigger_getComponent", manager_component_trigger_getComponent);
        // int manager_component_trigger_addEntityEnter(lua_State* l);
        lua_register(l, "manager_component_trigger_addEntityEnter", manager_component_trigger_addEntityEnter);
        // int manager_component_trigger_removeEntityEnter(lua_State* l);
        lua_register(l, "manager_component_trigger_removeEntityEnter", manager_component_trigger_removeEntityEnter);
        // int manager_component_trigger_addEntityExit(lua_State* l);
        lua_register(l, "manager_component_trigger_addEntityExit", manager_component_trigger_addEntityExit);
        // int manager_component_trigger_removeEntityExit(lua_State* l);
        lua_register(l, "manager_component_trigger_removeEntityExit", manager_component_trigger_removeEntityExit);
    }

    int manager_component_trigger_hasComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("trigger-component") != entity->components.end());
        return 1;      
    }

    int manager_component_trigger_getComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("trigger-component"));
        return 1;
    }

    int manager_component_trigger_addEntityEnter(lua_State* l) {
        manager::component::physics::TriggerComponent* trigger = (manager::component::physics::TriggerComponent*)lua_touserdata(l, 1);
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 2);
        std::string name = lua_tostring(l, 3);
        trigger->entityEnter.behavior = behavior;
        trigger->entityEnter.name = name;
        return 0;        
    }

    int manager_component_trigger_removeEntityEnter(lua_State* l) {
        manager::component::physics::TriggerComponent* trigger = (manager::component::physics::TriggerComponent*)lua_touserdata(l, 1);
        trigger->entityEnter.behavior = nullptr;
        trigger->entityEnter.name = "";
        return 0;
    }
    
    int manager_component_trigger_addEntityExit(lua_State* l) {
        manager::component::physics::TriggerComponent* trigger = (manager::component::physics::TriggerComponent*)lua_touserdata(l, 1);
        manager::Behavior* behavior = (manager::Behavior*)lua_touserdata(l, 2);
        std::string name = lua_tostring(l, 3);
        trigger->entityExit.behavior = behavior;
        trigger->entityExit.name = name;
        return 0;
    }

    int manager_component_trigger_removeEntityExit(lua_State* l) {
        manager::component::physics::TriggerComponent* trigger = (manager::component::physics::TriggerComponent*)lua_touserdata(l, 1);
        trigger->entityExit.behavior = nullptr;
        trigger->entityExit.name = "";
        return 0;
    }
}