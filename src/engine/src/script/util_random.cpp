#include "script_hidden.hpp"


namespace script {
    void util_random_load_library(lua_State* l) {
        lua_register(l, "util_random_randboolean", util_random_randboolean);
        lua_register(l, "util_random_randrange", util_random_randrange);
        lua_register(l, "util_random_randf", util_random_randf);
    }

    int util_random_randboolean(lua_State* l) {
        lua_pushboolean(l, util::random::randboolean());
        return 1;
    }

    int util_random_randrange(lua_State* l) {
        int32_t minI = lua_tointeger(l, 1);
        int32_t maxI = lua_tointeger(l, 2);
        lua_pushinteger(l, util::random::randrange(minI, maxI));
        return 1;    
    }

    int util_random_randf(lua_State* l) {
        lua_pushnumber(l, util::random::randf());
        return 1;
    }

}