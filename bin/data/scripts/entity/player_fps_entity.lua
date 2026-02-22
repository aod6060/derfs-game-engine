--[[
    This is an example lua script for the engine....
]]

moveSpeed = 16.0
jumpSpeed = 10.0

transform = nil
scene = nil
global = nil


yPivotEntity = nil
yPivotTransform = nil

pivotEntity = nil
pivotEntityTransform = nil

bodyComponent = nil


minX, minY, minZ = -20, 20, -20
maxX, maxY, maxZ = 20, 60, 20

function init()
    -- This function is called once every
    --transform = manager_entity_getTransform(entity)
    scene = manager_entity_getScene(entity)
    global = manager_scene_getGlobal(scene)

    transform = manager_entity_getTransform(entity)

    yPivotEntity = manager_entity_getChildEntity(entity, 0)
    yPivotTransform = manager_entity_getTransform(yPivotEntity)

    pivotEntity = manager_entity_getChildEntity(yPivotEntity, 0)
    pivotEntityTransform = manager_entity_getTransform(pivotEntity)

    bodyComponent = manager_entity_getKinematicBodyComponent(entity)
end

function update(delta)
    -- This function is
    if input_mapping_isMappingPressedOnce(global, "toggle-mousegrab") then
        input_toggleGrab()
    end

    if input_isGrab() then
        mcx, mcy = input_toVelocity()

        rx = manager_transform_getRotationX(pivotEntityTransform)
        ry = manager_transform_getRotationY(yPivotTransform)
        --ry = manager_component_body_getRotationY(bodyComponent)


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
        
        manager_transform_setRotationY(yPivotTransform, ry)
        manager_transform_setRotationX(pivotEntityTransform, rx)

        yrad = math.rad(ry)

        vx, vy, vz = manager_component_body_getLinearVelocity(bodyComponent)

        vx = 0
        vz = 0

        if input_mapping_isMappingPressed(global, "move-forward") then
            vx = -(math.sin(yrad) * moveSpeed)
            vz = -(math.cos(yrad) * moveSpeed)

        end

        if input_mapping_isMappingPressed(global, "move-backward") then
            vx = (math.sin(yrad) * moveSpeed)
            vz = (math.cos(yrad) * moveSpeed)
        end

        if input_mapping_isMappingPressed(global, "strafe-left") then
            vx = vx - (math.cos(yrad) * moveSpeed)
            vz = vz + math.sin(yrad) * moveSpeed
        end

        if input_mapping_isMappingPressed(global, "strafe-right") then
            vx = vx + (math.cos(yrad) * moveSpeed)
            vz = vz - (math.sin(yrad) * moveSpeed)
        end

        if input_mapping_isMappingPressedOnce(global, "jump") then
            vy = jumpSpeed
        end

        manager_component_body_setLinearVelocity(bodyComponent, vx, vy, vz)

        if manager_transform_getPositionY(transform) < -32.0 then
            reset()
        end
    end
end


function release()
    -- This is were you'll need to release user data
    bodyComponent = nil
    pivotEntityTransform = nil
    pivotEntity = nil
    yPivotTransform = nil
    yPivotEntity = nil
    global = nil
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