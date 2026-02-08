#include "../sys.hpp"
#include "lua/lua.hpp"


namespace script {
    void manager_load_library(lua_State *l) {
        manager_behavior_load_library(l);
        manager_entity_load_library(l);
        manager_scene_load_library(l);
        manager_global_load_library(l);
        manager_transform_load_library(l);
        manager_component_CameraComponent_load_library(l);
        manager_component_MeshComponent_load_library(l);

        // int manager_createEntityFromPrefab(lua_State* l);
        lua_register(l, "manager_createEntityFromPrefab", manager_createEntityFromPrefab);
    }

    int manager_createEntityFromPrefab(lua_State* l) {
        // path:String
        std::string path = lua_tostring(l, 1);
        manager::Entity* temp = new manager::Entity();
        temp->loadPrefab(path);
        lua_pushlightuserdata(l, temp);
        return 1;
    }
}