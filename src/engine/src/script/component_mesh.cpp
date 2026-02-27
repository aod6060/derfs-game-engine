#include "lua/lua.hpp"
#include "../sys.hpp"


namespace script {
    void manager_component_mesh_load_library(lua_State* l) {
        // int manager_component_mesh_hasComponent(lua_State* l);
        lua_register(l, "manager_component_mesh_hasComponent", manager_component_mesh_hasComponent);
        // int manager_component_mesh_getComponent(lua_State* l);
        lua_register(l, "manager_component_mesh_getComponent", manager_component_mesh_getComponent);
        // int manager_component_MeshComponent_getEntity(lua_State* l);
        lua_register(l, "manager_component_mesh_getEntity", manager_component_mesh_getEntity);
        // int manager_component_MeshComponent_getMesh(lua_State* l);
        lua_register(l, "manager_component_mesh_getMesh", manager_component_mesh_getMesh);
        // int manager_component_MeshComponent_setMesh(lua_State* l);
        lua_register(l, "manager_component_mesh_setMesh", manager_component_mesh_setMesh);
        // int manager_component_MeshComponent_getTexture(lua_State* l);
        lua_register(l, "manager_component_mesh_getTexture", manager_component_mesh_getTexture);
        // int manager_component_MeshComponent_setTexture(lua_State* l);
        lua_register(l, "manager_component_mesh_setTexture", manager_component_mesh_setTexture);
        // int manager_component_MeshComponent_getUVScale(lua_State* l);
        lua_register(l, "manager_component_mesh_getUVScale", manager_component_mesh_getUVScale);
        // int manager_component_MeshComponent_setUVScale(lua_State* l);
        lua_register(l, "manager_component_mesh_setUVScale", manager_component_mesh_setUVScale);
    }

    int manager_component_mesh_hasComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("mesh-component") != entity->components.end());
        return 1;
    }
    
    int manager_component_mesh_getComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("mesh-component"));
        return 1;
    }

    int manager_component_mesh_getEntity(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, comp->entity);
        return 1;
    }

    int manager_component_mesh_getMesh(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        lua_pushstring(l, comp->mesh.c_str());
        return 1;
    }

    int manager_component_mesh_setMesh(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        comp->mesh = lua_tostring(l, 2);
        return 0;
    }

    int manager_component_mesh_getTexture(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        lua_pushstring(l, comp->texture.c_str());
        return 1;
    }

    int manager_component_mesh_setTexture(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        comp->texture = lua_tostring(l, 2);
        return 0;
    }

    int manager_component_mesh_getUVScale(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->uvScale);
        return 1;
    }

    int manager_component_mesh_setUVScale(lua_State* l) {
        manager::component::MeshComponent* comp = (manager::component::MeshComponent*)lua_touserdata(l, 1);
        comp->uvScale = lua_tonumber(l, 2);
        return 0;
    }

}