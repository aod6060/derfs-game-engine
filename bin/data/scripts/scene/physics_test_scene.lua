--[[
    This is an example lua script for the engine....
]]


prefab = "data/prefabs/crate.prefab.json"

minX, minY, minZ = -20, 20, -20
maxX, maxY, maxZ = 20, 60, 20

amount = 20


function init()
    -- This function is called once every
    for i = 0, amount do
        temp = manager_createEntityFromPrefab(prefab)
        manager_entity_addEntity(scene, temp)
        temp_transform = manager_entity_getTransform(temp)

        
        temp = nil
    end
end


function update(delta)
    -- This function is 
end


function release()
    -- This is were you'll need to release user data
end