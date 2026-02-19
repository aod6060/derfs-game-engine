--[[
    This is an example lua script for the engine....
]]

bodyComponent = nil

time = 0.0
maxTime = 1.0

toggleForce = false

function init()
    -- This function is called once every
    bodyComponent = manager_entity_getDynamicBodyComponent(entity)
end


function update(delta)
    -- This function is

    
    if time >= maxTime then
        time = 0.0

        if manager_component_body_getActivationState(bodyComponent) == BODY_ACTIVE_TAG then
            print("Body: is active")
        elseif manager_component_body_getActivationState(bodyComponent) == BODY_ISLAND_SLEEPING then
            print("Body: is sleeping")
        elseif manager_component_body_getActivationState(bodyComponent) == BODY_WANTS_DEACTIVATION then
            print("Body: is wants to deactivate")
        elseif manager_component_body_getActivationState(bodyComponent) == BODY_DISABLE_DEACTIVATION then
            print("Body: is body is continueous")
        elseif manager_component_body_getActivationState(bodyComponent) == BODY_DISABLE_SIMULATION then
            print("Body: is body isn't being simulated")
        elseif manager_component_body_getActivationState(bodyComponent) == BODY_FIXED_BASE_MULTI_BODY then
            print("Body: is a fixed based multi body")
        else
            print("Body: is in an unknown state :|")
        end
    else
        time = time + delta
    end
    
    --[[
    if input_isKeyPressedOnce(KEYS_LEFT) then
        if toggleForce then
            --manager_component_body_clearForces(bodyComponent)
        else
            manager_component_body_setActivateState(bodyComponent, BODY_ACTIVE_TAG)
            manager_component_body_applyTorque(bodyComponent, 0.0, 256.0, 0.0)
        end
        toggleForce = not toggleForce
    end
    ]]

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

end


function release()
    -- This is were you'll need to release user data
    bodyComponent = nil
end