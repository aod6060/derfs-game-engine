#include "../sys.hpp"
#include "lua/lua.hpp"

namespace script {

    void manager_component_DynamicBodyComponent_load_library(lua_State* l) {
        // int manager_component_DynamicBodyComponent_updateTransform(lua_State* l);
        lua_register(l, "manager_component_DynamicBodyComponent_updateTransform", manager_component_DynamicBodyComponent_updateTransform);
    }

    int manager_component_DynamicBodyComponent_updateTransform(lua_State* l) {
        manager::component::physics::DynamicBodyComponent* comp = (manager::component::physics::DynamicBodyComponent*)lua_touserdata(l, 1);
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 2);
        comp->body->setCenterOfMassTransform(transform->convertToBulletTransform());
        return 0;
    }
}