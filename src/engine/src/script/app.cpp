#include "lua/lua.hpp"
#include "../sys.hpp"


namespace script {

    // app
    void app_load_library(lua_State* l) {
        lua_register(l, "app_getCaption", app_getCaption);
        lua_register(l, "app_getWidthInteger", app_getWidthInteger);
        lua_register(l, "app_getHeightInteger", app_getHeightInteger);
        lua_register(l, "app_getWidthFloat", app_getWidthFloat);
        lua_register(l, "app_getHeightFloat", app_getHeightFloat);
        lua_register(l, "app_getAspect", app_getAspect);
        lua_register(l, "app_exit", app_exit);
    }

    int app_getCaption(lua_State* l) {
        lua_pushstring(l, app::getCaption().c_str());
        return 1;
    }

    int app_getWidthInteger(lua_State* l) {
        lua_pushinteger(l, app::getWidthInteger());
        return 1;
    }

    int app_getHeightInteger(lua_State* l) {
        lua_pushinteger(l, app::getHeightInteger());
        return 1;
    }

    int app_getWidthFloat(lua_State* l) {
        lua_pushnumber(l, app::getWidthFloat());
        return 1;
    }

    int app_getHeightFloat(lua_State* l) {
        lua_pushnumber(l, app::getHeightFloat());
        return 1;
    }

    int app_getAspect(lua_State* l) {
        lua_pushnumber(l, app::getAspect());
        return 1;
    }

    int app_exit(lua_State* l) {
        app::exit();
        return 0;
    }


}