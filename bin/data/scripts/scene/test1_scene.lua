--[[
    This is an example lua script for the engine....
]]


global = nil

function init()
    -- This function is called once every
    global = manager_scene_getGlobal(scene)
end


function update(delta)
    -- This function is

    if input_isKeyPressedOnce(KEYS_R) then
        --manager_global_changeScene(global, "data/scenes/test2v2.scene.json")
        --print("Hello, World" .. global)
        manager_global_changeScene(global, "data/scenes/test2v4.scene.json")
    end
end


function release()
    -- This is were you'll need to release user data
    global = nil
end