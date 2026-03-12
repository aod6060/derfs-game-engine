--[[
    This is an example lua script for the engine....
]]

behavior = nil
meshEntity = nil
meshTransform = nil

trigger_area = nil
sound_player = nil

is_player = false

-- Original Position
ox = 0.0
oy = 0.0
oz = 0.0
-- Original YRot
ory = 0
-- Original Scale
osx = 1
osy = 1
osz = 1


yrot_speed = 64.0

minPX, minPY, minPZ = 0.0, 0.0, 0.0
maxPX, maxPY, maxPZ = 0.0, 5.0, 0.0

minX, minY, minZ = 1.0, 1.0, 1.0
maxX, maxY, maxZ = 1.5, 1.25, 1.5
stoggle = false
time = 0.0
maxTime = 1.0

function init()
    behavior = manager_entity_getBehavior(entity)
    -- This function is called once every
    meshEntity = manager_entity_getChildEntity(entity, 0)
    meshTransform = manager_entity_getTransform(meshEntity)

    trigger_area = manager_component_trigger_getComponent(entity)
    sound_player = manager_component_sound_player_getComponent(entity)

    manager_component_trigger_addEntityEnter(trigger_area, behavior, "playerEnter")
    manager_component_trigger_addEntityExit(trigger_area, behavior, "playerExit")

    -- Save original postion, rotaiton, and scale
    ox, oy, oz = manager_transform_getPosition(meshTransform)
    ory = manager_transform_getRotationY(meshTransform)
    osx, osy, osz = manager_transform_getScale(meshTransform)


end


function update(delta)
    -- This function is

    if is_player then
        if(input_isKeyPressedOnce(KEYS_E)) then
            if(manager_component_sound_player_isPlaying(sound_player)) then
                manager_component_sound_player_stop(sound_player)
            else
                manager_component_sound_player_play(sound_player)
            end
        end
    end

    -- Animated Mesh
    if(manager_component_sound_player_isPlaying(sound_player)) then
        -- Position

        -- Rotate
        y = manager_transform_getRotationY(meshTransform)

        y = y + (yrot_speed * delta)

        manager_transform_setRotationY(meshTransform, y)
        -- Posiiton
        xp = 0.0
        yp = 0.0
        zp = 0.0
        -- Scale
        xs = 1.0
        ys = 1.0
        zs = 1.0

        if time >= maxTime then
            stoggle = not stoggle
            time = 0.0
        else
            time = time + (delta * 2.0)
        end

        if stoggle then
            xp, yp, zp = lerp(maxPX, maxPY, maxPZ, minPX, minPY, minPZ, time)
            xs, ys, zs = lerp(maxX, maxY, maxZ, minX, minY, minZ, time)
        else
            xp, yp, zp = lerp(minPX, minPY, minPZ, maxPX, maxPY, maxPZ, time)
            xs, ys, zs = lerp(minX, minY, minZ, maxX, maxY, maxZ, time)
        end

        manager_transform_setPosition(meshTransform, xp, yp, zp)
        manager_transform_setScale(meshTransform, xs, ys, zs)
    else
        manager_transform_setPosition(meshTransform, ox, oy, oz)
        manager_transform_setRotationY(meshTransform, ory)
        manager_transform_setScale(meshTransform, osx, osy, osz)
        stoggle = false
    end
end


function release()
    -- This is were you'll need to release user data
    sound_player = nil
    trigger_area = nil
    meshTransform = nil
    meshEntity = nil
    behavior = nil
end

function playerEnter()
    is_player = true
end

function playerExit()
    is_player = false
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