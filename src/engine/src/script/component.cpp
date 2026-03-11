#include "../sys.hpp"


namespace script {
    void manager_component_load_library(lua_State* l) {       
        manager_component_camera_load_library(l);
        manager_component_mesh_load_library(l);
        manager_component_body_load_library(l);
        manager_component_static_body_load_library(l);
        manager_component_dynamic_body_load_library(l);
        manager_component_kinematic_body_load_library(l);
        manager_component_trigger_load_library(l);
        manager_component_push_arm_load_library(l);
        manager_component_raycast_load_library(l);
        manager_component_sound_player_load_library(l);
    }
}