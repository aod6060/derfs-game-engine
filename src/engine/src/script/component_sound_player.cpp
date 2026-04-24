#include "script_hidden.hpp"


namespace script {

    
    // SoundPlayerComponent
    void manager_component_sound_player_load_library(lua_State* l) {
        // int manager_component_sound_player_hasComponent(lua_State* l);
        lua_register(l, "manager_component_sound_player_hasComponent", manager_component_sound_player_hasComponent);
        // int manager_component_sound_player_getComponent(lua_State* l);
        lua_register(l, "manager_component_sound_player_getComponent", manager_component_sound_player_getComponent);
        // int manager_component_sound_player_play(lua_State* l);
        lua_register(l, "manager_component_sound_player_play", manager_component_sound_player_play);
        // int manager_component_sound_player_pause(lua_State* l);
        lua_register(l, "manager_component_sound_player_pause", manager_component_sound_player_pause);
        // int manager_component_sound_player_stop(lua_State* l);
        lua_register(l, "manager_component_sound_player_stop", manager_component_sound_player_stop);
        // int manager_component_sound_player_setLooping(lua_State* l);
        lua_register(l, "manager_component_sound_player_setLooping", manager_component_sound_player_setLooping);
        // int manager_component_sound_player_getLooping(lua_State* l);
        lua_register(l, "manager_component_sound_player_setLooping", manager_component_sound_player_setLooping);
        // int manager_component_sound_player_setRelative(lua_State* l);
        lua_register(l, "manager_component_sound_player_setRelative", manager_component_sound_player_setRelative);
        // int manager_component_sound_player_getRelative(lua_State* l);
        lua_register(l, "manager_component_sound_player_getRelative", manager_component_sound_player_getRelative);
        // int manager_component_sound_player_setReferenceDistance(lua_State* l);
        lua_register(l, "manager_component_sound_player_setReferenceDistance", manager_component_sound_player_setReferenceDistance);
        // int manager_component_sound_player_getReferenceDistance(lua_State* l);
        lua_register(l, "manager_component_sound_player_getReferenceDistance", manager_component_sound_player_getReferenceDistance);
        // int manager_component_sound_player_setRollofFactor(lua_State* l);
        lua_register(l, "manager_component_sound_player_setRollofFactor", manager_component_sound_player_setRollofFactor);
        // int manager_component_sound_player_getRollofFactor(lua_State* l);
        lua_register(l, "manager_component_sound_player_getRollofFactor", manager_component_sound_player_getRollofFactor);
        // int manager_component_sound_player_setMaxDistance(lua_State* l);
        lua_register(l, "manager_component_sound_player_setMaxDistance", manager_component_sound_player_setMaxDistance);
        // int manager_component_sound_player_getMaxDistance(lua_State* l);
        lua_register(l, "manager_component_sound_player_getMaxDistance", manager_component_sound_player_getMaxDistance);
        // int manager_component_sound_player_setPitch(lua_State* l);
        lua_register(l, "manager_component_sound_player_setPitch", manager_component_sound_player_setPitch);
        // int manager_component_sound_player_getPitch(lua_State* l);
        lua_register(l, "manager_component_sound_player_getPitch", manager_component_sound_player_getPitch);
        //int manager_component_sound_player_isPlaying(lua_State* l);
        lua_register(l, "manager_component_sound_player_isPlaying", manager_component_sound_player_isPlaying);
    }

    int manager_component_sound_player_hasComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("sound-player-component") != entity->components.end());
        return 1; 
    }

    int manager_component_sound_player_getComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("sound-player-component"));
        return 1;
    }

    int manager_component_sound_player_play(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->play();
        return 0;
    }

    int manager_component_sound_player_pause(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->pause();
        return 0;
    }

    int manager_component_sound_player_stop(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->stop();
        return 0;
    }

    int manager_component_sound_player_setLooping(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->looping = lua_toboolean(l, 2);
        player->source.setLooping(player->looping);
        return 0;
    }

    int manager_component_sound_player_getLooping(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, player->looping);
        return 1;
    }


    int manager_component_sound_player_setRelative(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->relative = lua_toboolean(l, 2);
        player->source.setRelative(player->relative);
        return 0;
    }

    int manager_component_sound_player_getRelative(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, player->relative);
        return 0;
    }

    int manager_component_sound_player_setReferenceDistance(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->referenceDistance = lua_tonumber(l, 2);
        player->source.setReferenceDistance(player->referenceDistance);
        return 0;
    }

    int manager_component_sound_player_getReferenceDistance(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, player->referenceDistance);
        return 0;
    }

    int manager_component_sound_player_setRollofFactor(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->rolloffFactor = lua_tonumber(l, 2);
        player->source.setRolloffFactor(player->rolloffFactor);
        return 0;
    }

    int manager_component_sound_player_getRollofFactor(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, player->rolloffFactor);
        return 0;
    }

    int manager_component_sound_player_setMaxDistance(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->maxDistance = lua_tonumber(l, 2);
        player->source.setMaxDistance(player->maxDistance);
        return 0;
    }

    int manager_component_sound_player_getMaxDistance(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, player->maxDistance);
        return 0;
    }

    int manager_component_sound_player_setPitch(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        player->pitch = lua_tonumber(l, 2);
        player->source.setPitch(player->pitch);
        return 0;
    }

    int manager_component_sound_player_getPitch(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, player->pitch);
        return 0;
    }

    int manager_component_sound_player_isPlaying(lua_State* l) {
        manager::component::sound::SoundPlayerComponent* player = (manager::component::sound::SoundPlayerComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, player->source.getState() == AL_PLAYING);
        return 1;
    }
}