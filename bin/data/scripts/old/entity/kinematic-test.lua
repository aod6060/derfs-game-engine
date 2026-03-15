--[[
    This is an example lua script for the engine....
]]


bodyComponent = nil

function init()
    -- This function is called once every
    bodyComponent = manager_component_kinematic_body_getComponent(entity)

    manager_component_kinematic_body_setISController(bodyComponent, true)
end


function update(delta)
    -- This function is
    x, y, z = manager_component_kinematic_body_getWorldTransformOrigin(bodyComponent)
    vx, vy, vz = manager_component_kinematic_body_getLinearVelocity(bodyComponent)
    rx, ry, rz = manager_component_kinematic_body_getWorldTransformRotation(bodyComponent)

    speed = 8.0
    rotSpeed = 128.0

    r = input_getKeyPressedAxis(KEYS_L, KEYS_J)
    m = input_getKeyPressedAxis(KEYS_I, KEYS_K)

    ry = ry + (r * rotSpeed * delta)

    yrad = -math.rad(ry)


    if manager_component_kinematic_body_isOnFloor(bodyComponent) then
        vy = 0.0

        if(input_isKeyPressedOnce(KEYS_U)) then
            vy = 5
        end
    else
        vy = (-10 * delta)
    end

    vx = (math.sin(yrad) * speed * delta * m)
    vz = -(math.cos(yrad) * speed * delta * m)

    --[[
    if input_isKeyPressed(KEYS_J) then
        x = x - (8.0 * delta)
    end

    if(input_isKeyPressed(KEYS_L)) then
        x = x + (8.0 * delta)
    end

    if input_isKeyPressed(KEYS_I) then
        z = z - (8.0 * delta)
    end

    if(input_isKeyPressed(KEYS_K)) then
        z = z + (8.0 * delta)
    end
    ]]

    if y < -32.0 then
        manager_component_kinematic_body_setWorldTransformOrigin(bodyComponent, -20, 32.0, -20)
    end

    manager_component_kinematic_body_setLinearVelocity(bodyComponent, vx, vy, vz)
    manager_component_kinematic_body_setWorldTransformRotation(bodyComponent, rx, ry, rz)
    manager_component_kinematic_body_moveAndSlide(bodyComponent)
end


function release()
    -- This is were you'll need to release user data
    bodyComponent = nil
end