#include "../sys.hpp"
#include "LinearMath/btVector3.h"
#include "lua/lua.hpp"


namespace script {
    void physics_load_library(lua_State* l) {
        // int physics_getGravity(lua_State* l);
        lua_register(l, "physics_getGravity", physics_getGravity);
        // int physics_setGravity(lua_State* l);
        lua_register(l, "physics_setGravity", physics_setGravity);
    }

    int physics_getGravity(lua_State* l) {
        btVector3 gravity = physics::getGravity();
        lua_pushnumber(l, gravity.x());
        lua_pushnumber(l, gravity.y());
        lua_pushnumber(l, gravity.z());
        return 3;
    }

    int physics_setGravity(lua_State* l) {
        btVector3 gravity = btVector3(
            lua_tonumber(l, 1),
            lua_tonumber(l, 2),
            lua_tonumber(l, 3)
        );
        physics::setGravity(gravity);
        return 0;
    }


}