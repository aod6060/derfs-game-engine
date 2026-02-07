#include "sys.hpp"


namespace script {
    void manager_load_library(lua_State *l) {
        manager_behavior_load_library(l);
        manager_entity_load_library(l);
        manager_scene_load_library(l);
        manager_global_load_library(l);
        manager_transform_load_library(l);
        manager_component_CameraComponent_load_library(l);
        manager_component_MeshComponent_load_library(l);
    }
}