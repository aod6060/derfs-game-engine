--[[
    This is an example lua script for the engine....
]]

speed = 32.0

dirX = 0.0
dirY = 0.0
dirZ = 0.0

time = 0.0
maxTime = 2.0

transform = nil

function init()
    -- This function is called once every
    transform = manager_entity_getTransform(entity)
end


function update(delta)

    if time >= maxTime then
        manager_entity_removeEntity(entity)
    else
        time = time + delta
    end
    -- This function is
    x, y, z = manager_transform_getPosition(transform)

    x = x + speed * delta * dirX
    y = y + speed * delta * dirY
    z = z + speed * delta * dirZ

    manager_transform_setPosition(transform, x, y, z)
end


function release()
    -- This is were you'll need to release user data
    transform = nil
end