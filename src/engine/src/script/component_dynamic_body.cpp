#include "../sys.hpp"
#include "lua/lua.hpp"

namespace script {
    // Dynamic Body
    void manager_component_dynamic_body_load_library(lua_State* l) {
        // int manager_component_dynamic_body_hasComponent(lua_State* l);
        lua_register(l, "manager_component_dynamic_body_hasComponent", manager_component_dynamic_body_hasComponent);
        // int manager_component_dynamic_body_getComponent(lua_State* l);
        lua_register(l, "manager_component_dynamic_body_getComponent", manager_component_dynamic_body_getComponent);
    }

    int manager_component_dynamic_body_hasComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("dynamic-body-component") != entity->components.end());
        return 1; 
    }

    int manager_component_dynamic_body_getComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("dynamic-body-component"));
        return 1;
    }

}