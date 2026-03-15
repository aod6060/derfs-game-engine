--[[
    This is an example lua script for the engine....
]]


-- Floor
floorEntity = nil

function init()
    -- This function is called once every
    floorEntity = manager_scene_getEntity(scene, 1)
end


function update(delta)
    -- This function is 
    if floorEntity ~= nil then

        if input_isKeyPressedOnce(KEYS_R) then
            manager_entity_removeEntity(floorEntity)
        end
    end
end


function release()
    -- This is were you'll need to release user data
    floorEntity = nil
end