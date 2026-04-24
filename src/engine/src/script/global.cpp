#include "script_hidden.hpp"


namespace script {

    void manager_global_load_library(lua_State *l) {
        // int manager_global_changeScene(lua_State* l);
        lua_register(l, "manager_global_changeScene", manager_global_changeScene);
        // int manager_global_getScene(lua_State* l);
        lua_register(l, "manager_global_getScene", manager_global_getScene);
        // int manager_global_getBehavior(lua_State* l);
        lua_register(l, "manager_global_getBehavior", manager_global_getBehavior);

    }
    
    int manager_global_changeScene(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        std::string path = lua_tostring(l, 2);
        //lobal->changeScene(path);
        global->isSceneChange = true;
        global->scenePath = path;
        return 0;
    }

    int manager_global_getScene(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, global->scene);
        return 1;
    }

    int manager_global_getBehavior(lua_State* l) {
        manager::Global* global = (manager::Global*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, global->behavior);
        return 1;
    }

}