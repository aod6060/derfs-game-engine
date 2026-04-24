#include "script_hidden.hpp"

namespace script {

    // Sound
    void sound_load_library(lua_State* l) {
        // // Sound
        // int sound_setGroupVolume(lua_State* l);
        lua_register(l, "sound_setGroupVolume", sound_setGroupVolume);
        // int sound_getGroupVolume(lua_State* l);
        lua_register(l, "sound_getGroupVolume", sound_getGroupVolume);
        // int sound_setMasterVolume(lua_State* l);
        lua_register(l, "sound_setMasterVolume", sound_setMasterVolume);
        // int sound_getMasterVolume(lua_State* l);
        lua_register(l, "sound_getMasterVolume", sound_getMasterVolume);
    }

    int sound_setGroupVolume(lua_State* l) {
        std::string name = lua_tostring(l, 1);
        float value = lua_tonumber(l, 2);
        sound::setGroupVolume(name, value);
        return 0;
    }

    int sound_getGroupVolume(lua_State* l) {
        std::string name = lua_tostring(l, 1);
        lua_pushnumber(l, sound::getGroupVolume(name));
        return 1;
    }

    int sound_setMasterVolume(lua_State* l) {
        float value = lua_tonumber(l, 1);
        sound::setMasterVolume(value);
        return 0;
    }

    int sound_getMasterVolume(lua_State* l) {
        lua_pushnumber(l, sound::getMasterVolume());
        return 1;
    }


}