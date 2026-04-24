#ifndef SCRIPT_HPP
#define SCRIPT_HPP


#include <lua/lua.hpp>
#include <lua/lauxlib.hpp>
#include <lua/luaconf.hpp>
#include <lua/lualib.hpp>

namespace script {
    void load_library(lua_State* l);

    // app
    void app_load_library(lua_State* l);
    int app_getCaption(lua_State* l);
    int app_getWidthInteger(lua_State* l);
    int app_getHeightInteger(lua_State* l);
    int app_getWidthFloat(lua_State* l);
    int app_getHeightFloat(lua_State* l);
    int app_getAspect(lua_State* l);
    int app_getWindow(lua_State* l);
    int app_getContext(lua_State* l);
    int app_exit(lua_State* l);

    // input
    void input_load_library(lua_State* l);
    // Keyboard
    int input_isKeyReleased(lua_State* l);
    int input_isKeyPressedOnce(lua_State* l);
    int input_isKeyPressed(lua_State* l);
    int input_isKeyReleasedOnce(lua_State* l);

    int input_getKeyReleasedValue(lua_State* l);
    int input_getKeyPressedOnceValue(lua_State* l);
    int input_getKeyPressedValue(lua_State* l);
    int input_getKeyReleasedOnceValue(lua_State* l);

    int input_getKeyReleasedAxis(lua_State* l);
    int input_getKeyPressedOnceAxis(lua_State* l);
    int input_getKeyPressedAxis(lua_State* l);
    int input_getKeyReleasedOnceAxis(lua_State* l);

    // Mouse
    int input_toPosition(lua_State* l);
    int input_toVelocity(lua_State* l);
    int input_toMouseWheel(lua_State* l);
    
    int input_isMouseButtonReleased(lua_State* l);
    int input_isMouseButtonPressedOnce(lua_State* l);
    int input_isMouseButtonPressed(lua_State* l);
    int input_isMouseButtonReleasedOnce(lua_State* l);

    int input_getMouseButtonReleasedValue(lua_State* l);
    int input_getMouseButtonPressedOnceValue(lua_State* l);
    int input_getMouseButtonPressedValue(lua_State* l);
    int input_getMouseButtonReleasedOnceValue(lua_State* l);

    int input_getMouseButtonReleasedAxis(lua_State* l);
    int input_getMouseButtonPressedOnceAxis(lua_State* l);
    int input_getMouseButtonPressedAxis(lua_State* l);
    int input_getMouseButtonReleasedOnceAxis(lua_State* l);

    // Input Grab
    int input_isGrab(lua_State* l);
    int input_setGrab(lua_State* l);
    int input_toggleGrab(lua_State* l);

    // Input Mapping ( I forgot to added in wrappers for the input mapping system :( )
    void input_mapping_load_library(lua_State* l);
    int input_mapping_isMappingReleased(lua_State* l);
    int input_mapping_isMappingPressedOnce(lua_State* l);
    int input_mapping_isMappingPressed(lua_State* l);
    int input_mapping_isMappingReleasedOnce(lua_State* l);
    int input_mapping_getMappingReleasedValue(lua_State* l);
    int input_mapping_getMappingPressedOnceValue(lua_State* l);
    int input_mapping_getMappingPressedValue(lua_State* l);
    int input_mapping_getMappingReleasedOnceValue(lua_State* l);
    int input_mapping_getMappingReleasedAxis(lua_State* l);
    int input_mapping_getMappingPressedOnceAxis(lua_State* l);
    int input_mapping_getMappingPressedAxis(lua_State* l);
    int input_mapping_getMappingReleasedOnceAxis(lua_State* l);

    // Physics
    void physics_load_library(lua_State* l);
    int physics_getGravity(lua_State* l);
    int physics_setGravity(lua_State* l);

    // Sound
    void sound_load_library(lua_State* l);
    int sound_setGroupVolume(lua_State* l);
    int sound_getGroupVolume(lua_State* l);
    int sound_setMasterVolume(lua_State* l);
    int sound_getMasterVolume(lua_State* l);

    // manager
    void manager_load_library(lua_State* l);
    int manager_createEntityFromPrefab(lua_State* l);

    // behavior
    void manager_behavior_load_library(lua_State* l);
    int manager_behavior_getBoolean(lua_State* l);
    int manager_behavior_getInteger(lua_State* l);
    int manager_behavior_getNumber(lua_State* l);
    int manager_behavior_getString(lua_State* l);
    int manager_behavior_setBoolean(lua_State* l);
    int manager_behavior_setInteger(lua_State* l);
    int manager_behavior_setNumber(lua_State* l);
    int manager_behavior_setString(lua_State* l);
    int manager_behavior_executeCallback(lua_State* l);


    // entity
    void manager_entity_load_library(lua_State* l);
    int manager_entity_getScene(lua_State* l);
    int manager_entity_getTransform(lua_State* l);
    int manager_entity_getBehavior(lua_State* l);
    
    int manager_entity_removeEntity(lua_State* l);
    int manager_entity_addChildEntity(lua_State* l);
    int manager_entity_getChildEntity(lua_State* l);
    int manager_entity_getChilderenAmount(lua_State* l);

    int manager_entity_isVisible(lua_State* l);
    int manager_entity_setVisible(lua_State* l);
    
    // scene
    void manager_scene_load_library(lua_State* l);
    int manager_scene_getGlobal(lua_State* l);
    int manager_scene_getNumEntity(lua_State* l);
    int manager_scene_getEntity(lua_State* l);
    int manager_scene_getBehavior(lua_State* l);
    int manager_scene_addEntity(lua_State* l);

    // global
    void manager_global_load_library(lua_State* l);
    int manager_global_changeScene(lua_State* l);
    int manager_global_getScene(lua_State* l);
    int manager_global_getBehavior(lua_State* l);

    // transform
    void manager_transform_load_library(lua_State* l);
    int manager_transform_getPosition(lua_State* l);
    int manager_transform_setPosition(lua_State* l);

    int manager_transform_getPositionX(lua_State* l);
    int manager_transform_getPositionY(lua_State* l);
    int manager_transform_getPositionZ(lua_State* l);

    int manager_transform_setPositionX(lua_State* l);
    int manager_transform_setPositionY(lua_State* l);
    int manager_transform_setPositionZ(lua_State* l);

    int manager_transform_getRotation(lua_State* l);
    
    int manager_transform_getRotationX(lua_State* l);
    int manager_transform_getRotationY(lua_State* l);
    int manager_transform_getRotationZ(lua_State* l);

    int manager_transform_setRotationX(lua_State* l);
    int manager_transform_setRotationY(lua_State* l);
    int manager_transform_setRotationZ(lua_State* l);

    int manager_transform_setRotation(lua_State* l);
    int manager_transform_getScale(lua_State* l);
    int manager_transform_setScale(lua_State* l);

    int manager_transform_getScaleX(lua_State* l);
    int manager_transform_getScaleY(lua_State* l);
    int manager_transform_getScaleZ(lua_State* l);

    int manager_transform_setScaleX(lua_State* l);
    int manager_transform_setScaleY(lua_State* l);
    int manager_transform_setScaleZ(lua_State* l);

    int manager_transform_getGlobalPosition(lua_State* l);
    int manager_transform_getGlobalPositionX(lua_State* l);
    int manager_transform_getGlobalPositionY(lua_State* l);
    int manager_transform_getGlobalPositionZ(lua_State* l);

    // Component
    void manager_component_load_library(lua_State* l);

    // CameraComponent
    void manager_component_camera_load_library(lua_State* l);
    int manager_component_camera_hasComponent(lua_State* l);
    int manager_component_camera_getComponent(lua_State* l);
    int manager_component_camera_getEntity(lua_State* l);
    int manager_component_camera_getFOV(lua_State* l);
    int manager_component_camera_setFOV(lua_State* l);
    int manager_component_camera_getZNear(lua_State* l);
    int manager_component_camera_setZNear(lua_State* l);
    int manager_component_camera_getZFar(lua_State* l);
    int manager_component_camera_setZFar(lua_State* l);

    // MeshComponent
    void manager_component_mesh_load_library(lua_State* l);
    int manager_component_mesh_hasComponent(lua_State* l);
    int manager_component_mesh_getComponent(lua_State* l);
    int manager_component_mesh_getEntity(lua_State* l);
    int manager_component_mesh_getMesh(lua_State* l);
    int manager_component_mesh_setMesh(lua_State* l);
    int manager_component_mesh_getMaterial(lua_State* l);
    int manager_component_mesh_setMaterial(lua_State* l);
    
    /*
    int manager_component_mesh_getTexture(lua_State* l);
    int manager_component_mesh_setTexture(lua_State* l);
    int manager_component_mesh_getUVScale(lua_State* l);
    int manager_component_mesh_setUVScale(lua_State* l);
    */
    // body ~ This covers all body types (dynamic, static, kinematic)
    void manager_component_body_load_library(lua_State* l);
    int manager_component_body_updateTransform(lua_State* l);
    int manager_component_body_getRotationY(lua_State* l);
    int manager_component_body_setRotationY(lua_State* l);
    int manager_component_body_setDamping(lua_State* l);
    int manager_component_body_getLinearDamping(lua_State* l);
    int manager_component_body_getAngularDamping(lua_State* l);
    int manager_component_body_getLinearSleepingThreshold(lua_State* l);
    int manager_component_body_getAngularSleepingThreshold(lua_State* l);
    int manager_component_body_applyDamping(lua_State* l);
    int manager_component_body_getLinearFactor(lua_State* l);
    int manager_component_body_setLinearFactor(lua_State* l);
    int manager_component_body_getInvMass(lua_State* l);
    int manager_component_body_getMass(lua_State* l);
    int manager_component_body_applyCentralForce(lua_State* l);
    int manager_component_body_getTotalForce(lua_State* l);
    int manager_component_body_getTotalTorque(lua_State* l);
    int manager_component_body_getInvInertiaDiagLocal(lua_State* l);
    int manager_component_body_setInvInertiaDiagLocal(lua_State* l);
    int manager_component_body_setSleepingThresholds(lua_State* l);
    int manager_component_body_applyTorque(lua_State* l);
    int manager_component_body_applyForce(lua_State* l);
    int manager_component_body_applyCentralImpulse(lua_State* l);
    int manager_component_body_applyTorqueImpulse(lua_State* l);
    int manager_component_body_applyImpulse(lua_State* l);
    int manager_component_body_applyPushImpulse(lua_State* l);
    int manager_component_body_getPushVelocity(lua_State* l);
    int manager_component_body_getTurnVelocity(lua_State* l);
    int manager_component_body_setPushVelocity(lua_State* l);
    int manager_component_body_setTurnVelocity(lua_State* l);
    int manager_component_body_applyCentralPushImpulse(lua_State* l);
    int manager_component_body_applyTorqueTurnImpulse(lua_State* l);
    int manager_component_body_clearForces(lua_State* l);
    int manager_component_body_getLinearVelocity(lua_State* l);
    int manager_component_body_getAngularVelocity(lua_State* l);
    int manager_component_body_setLinearVelocity(lua_State* l);
    int manager_component_body_setAngularVelocity(lua_State* l);
    int manager_component_body_getVelocityInLocalPoint(lua_State* l);
    int manager_component_body_getPushVelocityInLocalPoint(lua_State* l);
    int manager_component_body_setAngularFactorVector3(lua_State* l);
    int manager_component_body_setAngularFactorScalar(lua_State* l);
    // btCollisionObject.h
    int manager_component_body_isActive(lua_State* l);
    int manager_component_body_getActivationState(lua_State* l);
    int manager_component_body_setActivateState(lua_State* l);
    int manager_component_body_isStaticObject(lua_State* l);
    int manager_component_body_isKinematicObject(lua_State* l);

    // Static Body
    void manager_component_static_body_load_library(lua_State* l);
    int manager_component_static_body_hasComponent(lua_State* l);
    int manager_component_static_body_getComponent(lua_State* l);

    // Dynamic Body
    void manager_component_dynamic_body_load_library(lua_State* l);
    int manager_component_dynamic_body_hasComponent(lua_State* l);
    int manager_component_dynamic_body_getComponent(lua_State* l);

    // Kinematic Body
    void manager_component_kinematic_body_load_library(lua_State* l);
    int manager_component_kinematic_body_hasComponent(lua_State* l);
    int manager_component_kinematic_body_getComponent(lua_State* l);
    int manager_component_kinematic_body_getWorldTransformOrigin(lua_State* l);
    int manager_component_kinematic_body_setWorldTransformOrigin(lua_State* l);
    int manager_component_kinematic_body_getWorldTransformRotation(lua_State* l);
    int manager_component_kinematic_body_setWorldTransformRotation(lua_State* l);
    int manager_component_kinematic_body_isController(lua_State* l);
    int manager_component_kinematic_body_setIsController(lua_State* l);
    int manager_component_kinematic_body_getLinearVelocity(lua_State* l);
    int manager_component_kinematic_body_setLinearVelocity(lua_State* l);
    int manager_component_kinematic_body_moveAndSlide(lua_State* l);
    int manager_component_kinematic_body_isOnFloor(lua_State* l);


    // Trigger
    void manager_component_trigger_load_library(lua_State* l);
    int manager_component_trigger_hasComponent(lua_State* l);
    int manager_component_trigger_getComponent(lua_State* l);
    int manager_component_trigger_addEntityEnter(lua_State* l);
    int manager_component_trigger_removeEntityEnter(lua_State* l);
    int manager_component_trigger_addEntityExit(lua_State* l);
    int manager_component_trigger_removeEntityExit(lua_State* l);

    // PushArm
    void manager_component_push_arm_load_library(lua_State* l);
    int manager_component_push_arm_hasComponent(lua_State* l);
    int manager_component_push_arm_getComponent(lua_State* l);
    int manager_component_push_arm_getDistance(lua_State* l);
    int manager_component_push_arm_setDistance(lua_State* l);

    // RayCast
    void manager_component_raycast_load_library(lua_State* l);
    int manager_component_raycast_hasComponent(lua_State* l);
    int manager_component_raycast_getComponent(lua_State* l);
    int manager_component_raycast_isHit(lua_State* l);
    int manager_component_raycast_getWorldPoint(lua_State* l);
    int manager_component_raycast_getWorldNormal(lua_State* l);
    int manager_component_raycast_getDirection(lua_State* l);
    int manager_component_raycast_setDirection(lua_State* l);
    int manager_component_raycast_getScale(lua_State* l);
    int manager_component_raycast_setScale(lua_State* l);
    int manager_component_raycast_isDisabled(lua_State* l);
    int manager_component_raycast_setDisabled(lua_State* l);
    
    // SoundPlayerComponent
    void manager_component_sound_player_load_library(lua_State* l);
    int manager_component_sound_player_hasComponent(lua_State* l);
    int manager_component_sound_player_getComponent(lua_State* l);
    int manager_component_sound_player_play(lua_State* l);
    int manager_component_sound_player_pause(lua_State* l);
    int manager_component_sound_player_stop(lua_State* l);
    int manager_component_sound_player_setLooping(lua_State* l);
    int manager_component_sound_player_getLooping(lua_State* l);
    int manager_component_sound_player_setRelative(lua_State* l);
    int manager_component_sound_player_getRelative(lua_State* l);
    int manager_component_sound_player_setReferenceDistance(lua_State* l);
    int manager_component_sound_player_getReferenceDistance(lua_State* l);
    int manager_component_sound_player_setRollofFactor(lua_State* l);
    int manager_component_sound_player_getRollofFactor(lua_State* l);
    int manager_component_sound_player_setMaxDistance(lua_State* l);
    int manager_component_sound_player_getMaxDistance(lua_State* l);
    int manager_component_sound_player_setPitch(lua_State* l);
    int manager_component_sound_player_getPitch(lua_State* l);
    int manager_component_sound_player_isPlaying(lua_State* l);
    
    // util_random
    void util_random_load_library(lua_State* l);
    int util_random_randboolean(lua_State* l);
    int util_random_randrange(lua_State* l);
    int util_random_randf(lua_State* l);
}

#endif