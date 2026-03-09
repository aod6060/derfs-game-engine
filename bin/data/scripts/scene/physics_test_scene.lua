--[[
    This is an example lua script for the engine....
]]


prefab = "data/prefabs/crate.prefab.json"

minX, minY, minZ = -20, 20, -20
maxX, maxY, maxZ = 20, 60, 20

amount = 100

global = nil
cratesEntity = nil

function init()
    -- This function is called once every
    cratesEntity = manager_scene_getEntity(scene, 2)

    for i = 0, amount - 1 do
        temp = manager_createEntityFromPrefab(prefab)
        --manager_scene_addEntity(scene, temp)
        manager_entity_addChildEntity(cratesEntity, temp)
        
        if manager_component_dynamic_body_hasComponent(temp) then
            temp_transform = manager_entity_getTransform(temp)
            comp = manager_component_dynamic_body_getComponent(temp)

            px = util_random_randrange(minX, maxX)
            py = util_random_randrange(minY, maxY)
            pz = util_random_randrange(minZ, maxZ)


            rx = util_random_randrange(0, 360)
            ry = util_random_randrange(0, 360)
            rz = util_random_randrange(0, 360)

            manager_transform_setPosition(temp_transform, px, py, pz)
            manager_transform_setRotation(temp_transform, rx, ry, rz)

            manager_component_body_updateTransform(comp, temp_transform)

            comp = nil
            temp_transform = nil
        else
            manager_entity_removeEntity(temp)
        end


        temp = nil
    end

    global = manager_scene_getGlobal(scene)

end


function update(delta)
    -- This function is 
    if input_mapping_isMappingPressedOnce(global, "escape") then
        app_exit()
    end
    
    if input_isKeyPressedOnce(KEYS_R) then
        manager_global_changeScene(global, "data/scenes/test.scene.json")
    end
end


function release()
    -- This is were you'll need to release user data
    cratesEntity = nil
    global = nil
end


function test(i, n, b, s)
    print(i .. " " .. n .. " " .. tostring(b) .. " " .. s)
end