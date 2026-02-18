--[[
    This is an example lua script for the engine....
]]

moveSpeed = 32.0

transform = nil
scene = nil
global = nil

pivotEntity = nil
pivotEntityTransform = nil

function init()
    -- This function is called once every
    transform = manager_entity_getTransform(entity)
    scene = manager_entity_getScene(entity)
    global = manager_scene_getGlobal(scene)

    pivotEntity = manager_entity_getChildEntity(entity, 0)
    pivotEntityTransform = manager_entity_getTransform(pivotEntity)

end


function update(delta)
    -- This function is
    if input_mapping_isMappingPressedOnce(global, "toggle-mousegrab") then
        input_toggleGrab()
    end

    if input_isGrab() then
        mcx, mcy = input_toVelocity()

        --rx, ry, rz = manager_transform_getRotation(transform)
        rx = manager_transform_getRotationX(pivotEntityTransform)
        ry = manager_transform_getRotationY(transform)

        rx = rx + (mcy * 0.5)
        ry = ry - (mcx * 0.5)


        if rx < -90.0 then
            rx = -90.0
        elseif rx > 90.0 then
            rx = 90.0
        end

        if ry < -360.0 then
            ry = ry + 360.0
        elseif ry > 360.0 then
            ry = ry - 360.0
        end

        --manager_transform_setRotation(transform, rx, ry, rz)
        manager_transform_setRotationY(transform, ry)
        manager_transform_setRotationX(pivotEntityTransform, rx)

        yrad = math.rad(ry)

        px, py, pz = manager_transform_getPosition(transform)
        
        if input_isKeyPressed(KEYS_W) then
            px = px - (math.sin(yrad) * delta * moveSpeed)
            pz = pz - (math.cos(yrad) * delta * moveSpeed)
        end

        if input_isKeyPressed(KEYS_S) then
            px = px + (math.sin(yrad) * delta * moveSpeed)
            pz = pz + (math.cos(yrad) * delta * moveSpeed)
        end

        if input_isKeyPressed(KEYS_A) then
            px = px - (math.cos(yrad) * delta * moveSpeed)
            pz = pz + (math.sin(yrad) * delta * moveSpeed)
        end

        if input_isKeyPressed(KEYS_D) then
            px = px + (math.cos(yrad) * delta * moveSpeed)
            pz = pz - (math.sin(yrad) * delta * moveSpeed)
        end

        if input_isKeyPressed(KEYS_LSHIFT) then
            py = py - delta * moveSpeed
        end

        if input_isKeyPressed(KEYS_SPACE) then
            py = py + delta * moveSpeed
        end

        manager_transform_setPosition(transform, px, py, pz)

    end
end


function release()
    -- This is were you'll need to release user data
    pivotEntityTransform = nil
    pivotEntity = nil
    global = nil
    scene = nil
    transform = nil
end