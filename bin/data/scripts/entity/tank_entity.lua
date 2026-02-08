--[[
    This is an example lua script for the engine....
]]

rotSpeed = 128.0
movementSpeed = 32.0

transform = nil
shootEntity = nil

function init()
    -- This function is called once every
    transform = manager_entity_getTransform(entity)
end


function update(delta)
    -- This function is 
    r = input_getKeyPressedAxis(KEYS_L, KEYS_J)
    m = input_getKeyPressedAxis(KEYS_I, KEYS_K)

    yrot = manager_transform_getRotationY(transform)

    yrot = yrot + (r * rotSpeed * delta)

    manager_transform_setRotationY(transform, yrot)

    yrad = -math.rad(yrot)

    x, y, z = manager_transform_getPosition(transform)

    x = x + (math.sin(yrad) * movementSpeed * delta * m)
    z = z - (math.cos(yrad) * movementSpeed * delta * m)

    manager_transform_setPosition(transform, x, y, z)
end


function release()
    -- This is were you'll need to release user data
    shootEntity = nil
    transform = nil
end