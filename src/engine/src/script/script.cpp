#include "../sys.hpp"


namespace script {
    void load_library(lua_State* l) {
        // app
        app_load_library(l);

        // input
        input_load_library(l);
        
        // manager
        manager_load_library(l);

        // util
        util_random_load_library(l);
    }
}