--[[
    This is an example lua script for the engine....
]]


speed = 128.0
reverse = false

time = 0.0

minX, minY, minZ = 1.0, 1.0, 1.0
maxX, maxY, maxZ = 3.0, 2.0, 3.0

transform = nil

function init()
    -- This function is called once every
    transform = manager_entity_getTransform(entity)
end


function update(delta)
    -- This function is 
    if time > 1.0 then
        time = 0.0
        reverse = not reverse
    else
        time = time + delta
    end

    if reverse then
        manager_transform_setScale(transform, lerp(maxX, maxY, maxZ, minX, minY, minZ, time))
    else
        manager_transform_setScale(transform, lerp(minX, minY, minZ, maxX, maxY, maxZ, time))
    end

    yrot = manager_transform_getRotationY(transform)

    t = util_random_randf()

    value = 0.0

    if util_random_randboolean() then
        value = -1.0
    else
        value = 1.0
    end

    yrot = yrot + speed * delta * value

    manager_transform_setRotationY(transform, yrot)
end


function release()
    -- This is were you'll need to release user data
    transform = nil
end


function lerp(minX, minY, minZ, maxX, maxY, maxZ, t)
    x = minX + (maxX - minX) * t
    y = minY + (maxY - minY) * t
    z = minZ + (maxZ - minZ) * t
    return x, y, z
end

function lerp_float(min, max, t)
    return min + (max-min) * t
end