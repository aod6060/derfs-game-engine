--[[
    This is an example lua script for the engine....
]]

moveSpeed = 8.0
jumpSpeed = 10.0

transform = nil
scene = nil
sceneBehavior = nil
global = nil


yPivotEntity = nil
yPivotTransform = nil

pivotEntity = nil
pivotEntityTransform = nil

cameraEntity = nil
cameraTransform = nil

meshEntity = nil
meshTransform = nil

bodyComponent = nil


minX, minY, minZ = -20, 20, -20
maxX, maxY, maxZ = 20, 60, 20


time = 0.0


tx, ty, tz = 0.0, 0.0, 20.0
fx, fy, fz = 0.0, 1.0, 0.0

cx = tx
cy = ty
cz = tz

toggleFPS = false
animatedCamera = false
animateTime = 0.0
maxAnimeTime = 1.0

function init()
    -- This function is called once every
    --transform = manager_entity_getTransform(entity)
    scene = manager_entity_getScene(entity)
    sceneBehavior = manager_scene_getBehavior(scene)

    global = manager_scene_getGlobal(scene)

    transform = manager_entity_getTransform(entity)

    yPivotEntity = manager_entity_getChildEntity(entity, 0)
    yPivotTransform = manager_entity_getTransform(yPivotEntity)

    meshEntity = manager_entity_getChildEntity(entity, 1)
    meshTransform = manager_entity_getTransform(meshEntity)

    pivotEntity = manager_entity_getChildEntity(yPivotEntity, 0)
    pivotEntityTransform = manager_entity_getTransform(pivotEntity)

    cameraEntity = manager_entity_getChildEntity(pivotEntity, 0)
    cameraTransform = manager_entity_getTransform(cameraEntity)

    bodyComponent = manager_entity_getDynamicBodyComponent(entity)
    
    manager_component_body_setAngularFactorScalar(bodyComponent, 0.0)
    manager_component_body_setSleepingThresholds(bodyComponent, 0.0, 0.0)
    manager_component_body_setActivateState(bodyComponent, BODY_DISABLE_DEACTIVATION)
end

function update(delta)
    -- This function is
    if input_mapping_isMappingPressedOnce(global, "toggle-mousegrab") then
        input_toggleGrab()
    end

    if input_isGrab() then
        mcx, mcy = input_toVelocity()

        -- rx = manager_transform_getRotationX(pivotEntityTransform)
        -- ry = manager_transform_getRotationY(yPivotTransform)
        rx = manager_transform_getRotationX(pivotEntityTransform)
        ry = manager_transform_getRotationY(yPivotTransform)
        
        --ry = manager_component_body_getRotationY(bodyComponent)


        rx = rx - (mcy * 0.5)
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

        manager_transform_setRotationY(yPivotTransform, ry)
        manager_transform_setRotationX(pivotEntityTransform, rx)

        yrad = math.rad(ry)

        vx, vy, vz = manager_component_body_getLinearVelocity(bodyComponent)

        time = time + delta

        time = clamp(0.0, 1.0, time)

        vx = 0
        vz = 0

        movePlayer = false

        runSpeed = moveSpeed

        if input_mapping_isMappingPressed(global, "run") then
            runSpeed = runSpeed * 2.0
        end

        if input_mapping_isMappingPressed(global, "move-forward") then
            movePlayer = true
            vx = -(math.sin(yrad) * runSpeed)
            vz = -(math.cos(yrad) * runSpeed)
            time = 0.0
        end

        if input_mapping_isMappingPressed(global, "move-backward") then
            movePlayer = true
            vx = (math.sin(yrad) * runSpeed)
            vz = (math.cos(yrad) * runSpeed)
            time = 0.0
        end

        if input_mapping_isMappingPressed(global, "strafe-left") then
            movePlayer = true
            vx = vx - (math.cos(yrad) * runSpeed)
            vz = vz + math.sin(yrad) * runSpeed
        end

        if input_mapping_isMappingPressed(global, "strafe-right") then
            movePlayer = true
            vx = vx + (math.cos(yrad) * runSpeed)
            vz = vz - (math.sin(yrad) * runSpeed)

        end

        if input_mapping_isMappingPressedOnce(global, "jump") then
            movePlayer = true
            vy = jumpSpeed
        end

        if(toggleFPS) then
            manager_transform_setRotationY(meshTransform, ry)
        else
            if movePlayer then
                manager_transform_setRotationY(meshTransform, ry)
            end
        end

        if input_mapping_isMappingPressedOnce(global, "toggle-fps") then
            print(toggleFPS)
            toggleFPS = not toggleFPS
            animatedCamera = true
        end

        manager_component_body_setLinearVelocity(bodyComponent, vx, vy, vz)

        if manager_transform_getPositionY(transform) < -64.0 then
            reset()
        end
    end

    if animatedCamera then
        if toggleFPS then
            if animateTime >= maxAnimeTime then
                animatedCamera = false
                cx = fx
                cy = fy
                cz = fz
                animateTime = 0.0
            else
                animateTime = animateTime + (delta * 3.0)
                cx = lerp(tx, fx, animateTime)
                cy = lerp(ty, fy, animateTime)
                cz = lerp(tz, fz, animateTime)
            end
        else
            manager_entity_setVisible(meshEntity, true)
            if animateTime >= maxAnimeTime then
                animatedCamera = false
                cx = tx
                cy = ty
                cz = tz
                animateTime = 0.0
            else
                animateTime = animateTime + (delta * 3.0)
                cx = lerp(fx, tx, animateTime)
                cy = lerp(fy, ty, animateTime)
                cz = lerp(fz, tz, animateTime)
            end
        end

    else
        manager_entity_setVisible(meshEntity, not toggleFPS)
    end

    manager_transform_setPosition(cameraTransform, cx, cy, cz)


    if input_isKeyPressedOnce(KEYS_T) then
        print("Hello 1")
        manager_behavior_executeCallback(sceneBehavior, "test", math.floor(1), 3.14, true, "Hello, World")
        print("Hello 2")
    end
end


function release()
    -- This is were you'll need to release user data
    bodyComponent = nil
    cameraTransform = nil
    cameraEntity = nil
    meshTransform = nil
    meshEntity = nil
    pivotEntityTransform = nil
    pivotEntity = nil
    yPivotTransform = nil
    yPivotEntity = nil
    global = nil
    sceneBehavior = nil
    scene = nil
    transform = nil
end

function reset()
    px = util_random_randrange(minX, maxX)
    py = util_random_randrange(minY, maxY)
    pz = util_random_randrange(minZ, maxZ)

    manager_transform_setPosition(transform, px, py, pz)

    manager_component_body_updateTransform(bodyComponent, transform)
end

function lerp(v0, v1, t)
    return v0 + t * (v1 - v0)
end

function clamp(min, max, value)
    if(min > value) then
        return min
    end

    if(max < value) then
        return max
    end

    return value
end