--[[
    This is an example lua script for the engine....
]]

bodyComponent = nil
transform = nil

time = 0.0
maxTime = 1.0

minX, minY, minZ = -20, 20, -20
maxX, maxY, maxZ = 20, 60, 20

function init()
    -- This function is called once every
    bodyComponent = manager_component_dynamic_body_getComponent(entity)
    transform = manager_entity_getTransform(entity)
end


function update(delta)
    -- Force
    if input_isKeyPressedOnce(KEYS_LEFT) then
        amount = 128.0
        x = (util_random_randf() * 2.0 - 1.0) * amount
        y = (util_random_randf() + 1.0) * amount
        z = (util_random_randf() * 2.0 - 1.0) * amount
        manager_component_body_setActivateState(bodyComponent, BODY_ACTIVE_TAG)
        manager_component_body_applyCentralForce(bodyComponent, x, y, z)
    end
    -- Torque
    if input_isKeyPressedOnce(KEYS_UP) then
        amount = 32.0
        x = (util_random_randf() * 2.0 - 1.0) * amount
        y = (util_random_randf() + 1.0) * amount
        z = (util_random_randf() * 2.0 - 1.0) * amount
        manager_component_body_setActivateState(bodyComponent, BODY_ACTIVE_TAG)
        manager_component_body_applyTorque(bodyComponent, x, y, z)
    end

    -- Impulse
    if input_isKeyPressedOnce(KEYS_RIGHT) then
        amount = 8.0
        x = (util_random_randf() * 2.0 - 1.0) * amount
        y = (util_random_randf() + 1.0) * amount
        z = (util_random_randf() * 2.0 - 1.0) * amount
        manager_component_body_setActivateState(bodyComponent, BODY_ACTIVE_TAG)
        manager_component_body_applyCentralImpulse(bodyComponent, x, y, z)
    end


    if manager_transform_getPositionY(transform) < -32.0 then
        reset()
    end
end


function release()
    -- This is were you'll need to release user data
    bodyComponent = nil
    transform = nil
end

function reset()
    px = util_random_randrange(minX, maxX)
    py = util_random_randrange(minY, maxY)
    pz = util_random_randrange(minZ, maxZ)


    rx = util_random_randrange(0, 360)
    ry = util_random_randrange(0, 360)
    rz = util_random_randrange(0, 360)

    manager_transform_setPosition(transform, px, py, pz)
    manager_transform_setRotation(transform, rx, ry, rz)

    manager_component_body_updateTransform(bodyComponent, transform)
end