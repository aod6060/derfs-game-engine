--[[
    This is an example lua script for the engine....
]]

mesh = nil
trigger = nil
behavior = nil

texture_enter = "snow"
texture_exit = "angry"

function init()
    -- This function is called once every
    behavior = manager_entity_getBehavior(entity)
    trigger = manager_component_trigger_getComponent(entity)
    mesh = manager_component_mesh_getComponent(entity)

    manager_component_trigger_addEntityEnter(trigger, behavior, "player_enter")
    manager_component_trigger_addEntityExit(trigger, behavior, "player_exit")
end


function update(delta)
    -- This function is 
end


function release()
    -- This is were you'll need to release user data
    trigger = nil
    behavior = nil
end

function player_enter()
    print("Player Enter")
    manager_component_mesh_setTexture(mesh, texture_enter)
end

function player_exit()
    --print("Player Exit")
    manager_component_mesh_setTexture(mesh, texture_exit)
end