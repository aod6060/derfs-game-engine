#include "script_hidden.hpp"

namespace script {

    void input_mapping_load_library(lua_State* l) {
        // int input_mapping_isMappingReleased(lua_State* l);
        lua_register(l, "input_mapping_isMappingReleased", input_mapping_isMappingReleased);
        // int input_mapping_isMappingPressedOnce(lua_State* l);
        lua_register(l, "input_mapping_isMappingPressedOnce", input_mapping_isMappingPressedOnce);
        // int input_mapping_isMappingPressed(lua_State* l);
        lua_register(l, "input_mapping_isMappingPressed", input_mapping_isMappingPressed);
        // int input_mapping_isMappingReleasedOnce(lua_State* l);
        lua_register(l, "input_mapping_isMappingReleasedOnce", input_mapping_isMappingReleasedOnce);
        // int input_mapping_getMappingReleasedValue(lua_State* l);
        lua_register(l, "input_mapping_getMappingReleasedValue", input_mapping_getMappingReleasedValue);
        // int input_mapping_getMappingPressedOnceValue(lua_State* l);
        lua_register(l, "input_mapping_getMappingPressedOnceValue", input_mapping_getMappingPressedOnceValue);
        // int input_mapping_getMappingPressedValue(lua_State* l);
        lua_register(l, "input_mapping_getMappingPressedValue", input_mapping_getMappingPressedValue);
        // int input_mapping_getMappingReleasedOnceValue(lua_State* l);
        lua_register(l, "input_mapping_getMappingReleasedOnceValue", input_mapping_getMappingReleasedOnceValue);
        // int input_mapping_getMappingReleasedAxis(lua_State* l);
        lua_register(l, "input_mapping_getMappingReleasedAxis", input_mapping_getMappingReleasedAxis);
        // int input_mapping_getMappingPressedOnceAxis(lua_State* l);
        lua_register(l, "input_mapping_getMappingPressedOnceAxis", input_mapping_getMappingPressedOnceAxis);
        // int input_mapping_getMappingPressedAxis(lua_State* l);
        lua_register(l, "input_mapping_getMappingPressedAxis", input_mapping_getMappingPressedAxis);
        // int input_mapping_getMappingReleasedOnceAxis(lua_State* l);
        lua_register(l, "input_mapping_getMappingReleasedOnceAxis", input_mapping_getMappingReleasedOnceAxis);
    }

    int input_mapping_isMappingReleased(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        if(global->mappings.find(name) == global->mappings.end()) {
            lua_pushboolean(l, false);
            return 1;
        }
        lua_pushboolean(l, input::mapping::isMappingReleased(global->mappings.at(name)));
        return 1;
    }

    int input_mapping_isMappingPressedOnce(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        if(global->mappings.find(name) == global->mappings.end()) {
            lua_pushboolean(l, false);
            return 1;
        }
        lua_pushboolean(l, input::mapping::isMappingPressedOnce(global->mappings.at(name)));
        return 1;
    }

    int input_mapping_isMappingPressed(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        if(global->mappings.find(name) == global->mappings.end()) {
            lua_pushboolean(l, false);
            return 1;
        }
        lua_pushboolean(l, input::mapping::isMappingPressed(global->mappings.at(name)));
        return 1;
    }

    int input_mapping_isMappingReleasedOnce(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        if(global->mappings.find(name) == global->mappings.end()) {
            lua_pushboolean(l, false);
            return 1;
        }
        lua_pushboolean(l, input::mapping::isMappingReleasedOnce(global->mappings.at(name)));
        return 1;
    }

    int input_mapping_getMappingReleasedValue(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        if(global->mappings.find(name) == global->mappings.end()) {
            lua_pushnumber(l, 0.0f);
            return 1;
        }
        lua_pushnumber(l, input::mapping::getMappingReleasedValue(global->mappings.at(name)));
        return 1;
    }

    int input_mapping_getMappingPressedOnceValue(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        if(global->mappings.find(name) == global->mappings.end()) {
            lua_pushnumber(l, 0.0f);
            return 1;
        }
        lua_pushnumber(l, input::mapping::getMappingPressedOnceValue(global->mappings.at(name)));
        return 1;
    }

    int input_mapping_getMappingPressedValue(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        if(global->mappings.find(name) == global->mappings.end()) {
            lua_pushnumber(l, 0.0f);
            return 1;
        }
        lua_pushnumber(l, input::mapping::getMappingPressedValue(global->mappings.at(name)));
        return 1;
    }

    int input_mapping_getMappingReleasedOnceValue(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string name = lua_tostring(l, 2);
        if(global->mappings.find(name) == global->mappings.end()) {
            lua_pushnumber(l, 0.0f);
            return 1;
        }
        lua_pushnumber(l, input::mapping::getMappingReleasedOnceValue(global->mappings.at(name)));
        return 1;
    }
    
    int input_mapping_getMappingReleasedAxis(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string negative = lua_tostring(l, 2);
        std::string positive = lua_tostring(l, 3);
        if(global->mappings.find(negative) == global->mappings.end() || global->mappings.find(positive) == global->mappings.end()) {
            lua_pushnumber(l, 0.0);
            return 1;
        }
        lua_pushnumber(l, input::mapping::getMappingReleasedAxis(global->mappings.at(negative), global->mappings.at(positive)));
        return 1;
    }

    int input_mapping_getMappingPressedOnceAxis(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string negative = lua_tostring(l, 2);
        std::string positive = lua_tostring(l, 3);
        if(global->mappings.find(negative) == global->mappings.end() || global->mappings.find(positive) == global->mappings.end()) {
            lua_pushnumber(l, 0.0);
            return 1;
        }
        lua_pushnumber(l, input::mapping::getMappingPressedOnceAxis(global->mappings.at(negative), global->mappings.at(positive)));
        return 1;
    }

    int input_mapping_getMappingPressedAxis(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string negative = lua_tostring(l, 2);
        std::string positive = lua_tostring(l, 3);
        if(global->mappings.find(negative) == global->mappings.end() || global->mappings.find(positive) == global->mappings.end()) {
            lua_pushnumber(l, 0.0);
            return 1;
        }
        lua_pushnumber(l, input::mapping::getMappingPressedAxis(global->mappings.at(negative), global->mappings.at(positive)));
        return 1;
    }

    int input_mapping_getMappingReleasedOnceAxis(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string negative = lua_tostring(l, 2);
        std::string positive = lua_tostring(l, 3);
        if(global->mappings.find(negative) == global->mappings.end() || global->mappings.find(positive) == global->mappings.end()) {
            lua_pushnumber(l, 0.0);
            return 1;
        }
        lua_pushnumber(l, input::mapping::getMappingReleasedOnceAxis(global->mappings.at(negative), global->mappings.at(positive)));
        return 1;
    }


}