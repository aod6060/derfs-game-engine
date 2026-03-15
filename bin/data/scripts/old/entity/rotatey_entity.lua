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
    x, y, z = manager_transform_getRotation(transform)

    y = y + (64.0 * delta)

    if y > 360.0 then
        y = y - 360.0
    end

    manager_transform_setRotation(transform, x, y, z)
end

function release()
    -- This is were you'll need to release user data
    transform = nil
end