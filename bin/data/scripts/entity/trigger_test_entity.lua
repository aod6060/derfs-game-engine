--[[
    This is an example lua script for the engine....
]]


trigger = nil
behavior = nil

function init()
    -- This function is called once every
    behavior = manager_entity_getBehavior(entity)
    trigger = manager_component_trigger_getComponent(entity)

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
end

function player_exit()
    --print("Player Exit")
end