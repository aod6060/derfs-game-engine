--[[
    This is an example lua script for the engine....
]]


transform = nil

function init()
    -- This function is called once every
    transform = manager_entity_getTransform(entity)
end


function update(delta)
    -- This function is
    x, y, z = manager_transform_getPosition(transform)

    if input_isKeyPressed(KEYS_LEFT) then
        x = x - (32.0 * delta)
    elseif input_isKeyPressed(KEYS_RIGHT) then
        x = x + (32.0 * delta)
    end


    if input_isKeyPressed(KEYS_UP) then
        z = z - (32.0 * delta)
    elseif input_isKeyPressed(KEYS_DOWN) then
        z = z + (32.0 * delta)
    end

    manager_transform_setPosition(transform, x, y, z)
end


function release()
    -- This is were you'll need to release user data
    transform = nil
end