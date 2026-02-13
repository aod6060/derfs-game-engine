--[[
    This is an example lua script for the engine....
]]

rotSpeed = 128.0
movementSpeed = 32.0

transform = nil
shootEntity = nil
shootEntityTransform = nil

scene = nil
projectile = nil

function init()
    -- This function is called once every
    transform = manager_entity_getTransform(entity)
    shootEntity = manager_entity_getChildEntity(entity, 0)
    shootEntityTransform = manager_entity_getTransform(shootEntity)
    scene = manager_entity_getScene(entity)
    projectile = manager_scene_getEntity(scene, 2)
end


function update(delta)
    if input_isKeyPressed(KEYS_U) then
        shoot()
    end

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
    projectile = nil
    scene = nil
    shootEntityTransform = nil
    shootEntity = nil
    transform = nil
end


function normalize(x, y, z)
    l = math.sqrt(x * x + y * y + z * z)
    return x / l, y / l, z / l
end

function shoot()
    x, y, z = manager_transform_getGlobalPosition(transform)
    sx, sy, sz = manager_transform_getGlobalPosition(shootEntityTransform)

    dx = sx - x
    dy = sy - y
    dz = sz - z

    dx, dy, dz = normalize(dx, dy, dz)

    temp = manager_createEntityFromPrefab("data/prefabs/ball.prefab.json")
    manager_entity_addChildEntity(projectile, temp)

    temp_transform = manager_entity_getTransform(temp)
    manager_transform_setPosition(temp_transform, sx, sy, sz)
    temp_behavior = manager_entity_getBehavior(temp)
    manager_behavior_setNumber(temp_behavior, "dirX", dx)
    manager_behavior_setNumber(temp_behavior, "dirY", dy)
    manager_behavior_setNumber(temp_behavior, "dirZ", dz)

    temp_behavior = nil
    temp_transform = nil
    temp = nil
end