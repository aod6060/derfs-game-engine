--[[
    This is an example lua script for the engine....
]]


prefab = "data/prefabs/crate.prefab.json"

minX, minY, minZ = -20, 20, -20
maxX, maxY, maxZ = 20, 60, 20

amount = 100

global = nil

function init()
    -- This function is called once every
    for i = 0, amount - 1 do
        temp = manager_createEntityFromPrefab(prefab)
        manager_scene_addEntity(scene, temp)
        
        if manager_entity_hasDynamicBodyComponent(temp) then
            temp_transform = manager_entity_getTransform(temp)
            comp = manager_entity_getDynamicBodyComponent(temp)

            px = util_random_randrange(minX, maxX)
            py = util_random_randrange(minY, maxY)
            pz = util_random_randrange(minZ, maxZ)


            rx = util_random_randrange(0, 360)
            ry = util_random_randrange(0, 360)
            rz = util_random_randrange(0, 360)

            manager_transform_setPosition(temp_transform, px, py, pz)
            manager_transform_setRotation(temp_transform, rx, ry, rz)

            manager_component_body_component_updateTransform(comp, temp_transform)

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
    if input_isKeyPressedOnce(KEYS_R) then
        manager_global_changeScene(global, "data/scenes/test.scene.json")
    end
end


function release()
    -- This is were you'll need to release user data
    global = nil
end