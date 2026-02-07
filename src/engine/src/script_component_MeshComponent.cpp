#include "lua/lua.hpp"
#include "sys.hpp"


namespace script {
    void manager_component_MeshComponent_load_library(lua_State* l) {
        // int manager_component_MeshComponent_getEntity(lua_State* l);
        lua_register(l, "manager_component_MeshComponent_getEntity", manager_component_MeshComponent_getEntity);
        // int manager_component_MeshComponent_getMesh(lua_State* l);
        lua_register(l, "manager_component_MeshComponent_getMesh", manager_component_MeshComponent_getMesh);
        // int manager_component_MeshComponent_setMesh(lua_State* l);
        lua_register(l, "manager_component_MeshComponent_setMesh", manager_component_MeshComponent_setMesh);
        // int manager_component_MeshComponent_getTexture(lua_State* l);
        lua_register(l, "manager_component_MeshComponent_getTexture", manager_component_MeshComponent_getTexture);
        // int manager_component_MeshComponent_setTexture(lua_State* l);
        lua_register(l, "manager_component_MeshComponent_setTexture", manager_component_MeshComponent_setTexture);
        // int manager_component_MeshComponent_getUVScale(lua_State* l);
        lua_register(l, "manager_component_MeshComponent_getUVScale", manager_component_MeshComponent_getUVScale);
        // int manager_component_MeshComponent_setUVScale(lua_State* l);
        lua_register(l, "manager_component_MeshComponent_setUVScale", manager_component_MeshComponent_setUVScale);
    }

    int manager_component_MeshComponent_getEntity(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, comp->entity);
        return 1;
    }

    int manager_component_MeshComponent_getMesh(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        lua_pushstring(l, comp->mesh.c_str());
        return 1;
    }

    int manager_component_MeshComponent_setMesh(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        comp->mesh = lua_tostring(l, 2);
        return 0;
    }

    int manager_component_MeshComponent_getTexture(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        lua_pushstring(l, comp->texture.c_str());
        return 1;
    }

    int manager_component_MeshComponent_setTexture(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        comp->texture = lua_tostring(l, 2);
        return 0;
    }

    int manager_component_MeshComponent_getUVScale(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->uvScale);
        return 1;
    }

    int manager_component_MeshComponent_setUVScale(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        comp->uvScale = lua_tonumber(l, 2);
        return 0;
    }

}