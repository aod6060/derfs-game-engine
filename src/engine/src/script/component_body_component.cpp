#include "../sys.hpp"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btScalar.h"
#include "LinearMath/btTransform.h"
#include "lua/lua.hpp"

namespace script {
    void manager_component_body_component_load_library(lua_State* l) {
        // #define ACTIVE_TAG 1
        lua_pushinteger(l, ACTIVE_TAG);
        lua_setglobal(l, "BODY_ACTIVE_TAG");
        // #define ISLAND_SLEEPING 2
        lua_pushinteger(l, ISLAND_SLEEPING);
        lua_setglobal(l, "BODY_ISLAND_SLEEPING");
        // #define WANTS_DEACTIVATION 3
        lua_pushinteger(l, WANTS_DEACTIVATION);
        lua_setglobal(l, "BODY_WANTS_DEACTIVATION");
        // #define DISABLE_DEACTIVATION 4
        lua_pushinteger(l, DISABLE_DEACTIVATION);
        lua_setglobal(l, "BODY_DISABLE_DEACTIVATION");
        // #define DISABLE_SIMULATION 5
        lua_pushinteger(l, DISABLE_SIMULATION);
        lua_setglobal(l, "BODY_DISABLE_SIMULATION");
        // #define FIXED_BASE_MULTI_BODY 6
        lua_pushinteger(l, FIXED_BASE_MULTI_BODY);
        lua_setglobal(l, "BODY_FIXED_BASE_MULTI_BODY");

        // int manager_component_DynamicBodyComponent_updateTransform(lua_State* l);
        lua_register(l, "manager_component_body_updateTransform", manager_component_body_updateTransform);
        // int manager_component_body_getRotationY(lua_State* l);
        lua_register(l, "manager_component_body_getRotationY", manager_component_body_getRotationY);
        // int manager_component_body_setRotationY(lua_State* l);
        lua_register(l, "manager_component_body_setRotationY", manager_component_body_setRotationY);
        // BodyComponent
        // int manager_component_body_setDamping(lua_State* l);
        lua_register(l, "manager_component_body_setDamping", manager_component_body_setDamping);
        // int manager_component_body_getLinearDamping(lua_State* l);
        lua_register(l, "manager_component_body_getLinearDamping", manager_component_body_getLinearDamping);
        // int manager_component_body_getAngularDamping(lua_State* l);
        lua_register(l, "manager_component_body_getAngularDamping", manager_component_body_getAngularDamping);
        // int manager_component_body_getLinearSleepingThreshold(lua_State* l);
        lua_register(l, "manager_component_body_getLinearSleepingThreshold", manager_component_body_getLinearSleepingThreshold);
        // int manager_component_body_getAngularSleepingThreshold(lua_State* l);
        lua_register(l, "manager_component_body_getAngularSleepingThreshold", manager_component_body_getAngularSleepingThreshold);
        // int manager_component_body_applyDamping(lua_State* l);
        lua_register(l, "manager_component_body_applyDamping", manager_component_body_applyDamping);
        // int manager_component_body_getLinearFactor(lua_State* l);
        lua_register(l, "manager_component_body_getLinearFactor", manager_component_body_getLinearFactor);
        // int manager_component_body_setLinearFactor(lua_State* l);
        lua_register(l, "manager_component_body_setLinearFactor", manager_component_body_setLinearFactor);
        // int manager_component_body_getInvMass(lua_State* l);
        lua_register(l, "manager_component_body_getInvMass", manager_component_body_getInvMass);
        // int manager_component_body_getMass(lua_State* l);
        lua_register(l, "manager_component_body_getMass", manager_component_body_getMass);
        // int manager_component_body_applyCentralForce(lua_State* l);
        lua_register(l, "manager_component_body_applyCentralForce", manager_component_body_applyCentralForce);
        // int manager_component_body_getTotalForce(lua_State* l);
        lua_register(l, "manager_component_body_getTotalForce", manager_component_body_getTotalForce);
        // int manager_component_body_getTotalTorque(lua_State* l);
        lua_register(l, "manager_component_body_getTotalTorque", manager_component_body_getTotalTorque);
        // int manager_component_body_getInvInertiaDiagLocal(lua_State* l);
        lua_register(l, "manager_component_body_getInvInertiaDiagLocal", manager_component_body_getInvInertiaDiagLocal);
        // int manager_component_body_setInvInertiaDiagLocal(lua_State* l);
        lua_register(l, "manager_component_body_setInvInertiaDiagLocal", manager_component_body_setInvInertiaDiagLocal);
        // int manager_component_body_setSleepingThresholds(lua_State* l);
        lua_register(l, "manager_component_body_setSleepingThresholds", manager_component_body_setSleepingThresholds);
        // int manager_component_body_applyTorque(lua_State* l);
        lua_register(l, "manager_component_body_applyTorque", manager_component_body_applyTorque);
        // int manager_component_body_applyForce(lua_State* l);
        lua_register(l, "manager_component_body_applyForce", manager_component_body_applyForce);
        // int manager_component_body_applyCentralImpulse(lua_State* l);
        lua_register(l, "manager_component_body_applyCentralImpulse", manager_component_body_applyCentralImpulse);
        // int manager_component_body_applyTorqueImpulse(lua_State* l);
        lua_register(l, "manager_component_body_applyTorqueImpulse", manager_component_body_applyTorqueImpulse);
        // int manager_component_body_applyImpulse(lua_State* l);
        lua_register(l, "manager_component_body_applyImpulse", manager_component_body_applyImpulse);
        // int manager_component_body_applyPushImpulse(lua_State* l);
        lua_register(l, "manager_component_body_applyPushImpulse", manager_component_body_applyPushImpulse);
        // int manager_component_body_getPushVelocity(lua_State* l);
        lua_register(l, "manager_component_body_getPushVelocity", manager_component_body_getPushVelocity);
        // int manager_component_body_getTurnVelocity(lua_State* l);
        lua_register(l, "manager_component_body_getTurnVelocity", manager_component_body_getTurnVelocity);
        // int manager_component_body_setPushVelocity(lua_State* l);
        lua_register(l, "manager_component_body_setPushVelocity", manager_component_body_setPushVelocity);
        // int manager_component_body_setTurnVelocity(lua_State* l);
        lua_register(l, "manager_component_body_setTurnVelocity", manager_component_body_setTurnVelocity);
        // int manager_component_body_applyCentralPushImpulse(lua_State* l);
        lua_register(l, "manager_component_body_applyCentralPushImpulse", manager_component_body_applyCentralPushImpulse);
        // int manager_component_body_applyTorqueTurnImpulse(lua_State* l);
        lua_register(l, "manager_component_body_applyTorqueTurnImpulse", manager_component_body_applyTorqueTurnImpulse);
        // int manager_component_body_clearForces(lua_State* l);
        lua_register(l, "manager_component_body_clearForces", manager_component_body_clearForces);
        // int manager_component_body_getLinearVelocity(lua_State* l);
        lua_register(l, "manager_component_body_getLinearVelocity", manager_component_body_getLinearVelocity);
        // int manager_component_body_getAngularVelocity(lua_State* l);
        lua_register(l, "manager_component_body_getAngularVelocity", manager_component_body_getAngularVelocity);
        // int manager_component_body_setLinearVelocity(lua_State* l);
        lua_register(l, "manager_component_body_setLinearVelocity", manager_component_body_setLinearVelocity);
        // int manager_component_body_setAngularVelocity(lua_State* l);
        lua_register(l, "manager_component_body_setAngularVelocity", manager_component_body_setAngularVelocity);
        // int manager_component_body_getVelocityInLocalPoint(lua_State* l);
        lua_register(l, "manager_component_body_getVelocityInLocalPoint", manager_component_body_getVelocityInLocalPoint);
        // int manager_component_body_getPushVelocityInLocalPoint(lua_State* l);
        lua_register(l, "manager_component_body_getPushVelocityInLocalPoint", manager_component_body_getPushVelocityInLocalPoint);
        // int manager_component_body_setAngularFactorVector3(lua_State* l);
        lua_register(l, "manager_component_body_setAngularFactorVector3", manager_component_body_setAngularFactorVector3);
        // int manager_component_body_setAngularFactorScalar(lua_State* l);
        lua_register(l, "manager_component_body_setAngularFactorScalar", manager_component_body_setAngularFactorScalar);
        // btCollisionObject.h
        // int manager_component_body_isActive(lua_State* l);
        lua_register(l, "manager_component_body_isActive", manager_component_body_isActive);
        // int manager_component_body_getActivationState(lua_State* l);
        lua_register(l, "manager_component_body_getActivationState", manager_component_body_getActivationState);
        // int manager_component_body_setActivateState(lua_State* l);
        lua_register(l, "manager_component_body_setActivateState", manager_component_body_setActivateState);
        // int manager_component_body_isStaticObject(lua_State* l);
        lua_register(l, "manager_component_body_isStaticObject", manager_component_body_isStaticObject);
        // int manager_component_body_isKinematicObject(lua_State* l);
        lua_register(l, "manager_component_body_isKinematicObject", manager_component_body_isKinematicObject);
    }

    int manager_component_body_updateTransform(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        manager::Transform* transform = (manager::Transform*)lua_touserdata(l, 2);
        comp->body->setCenterOfMassTransform(transform->convertToBulletTransform());
        return 0;
    }

    int manager_component_body_getRotationY(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* bodyComp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        float y = btDegrees(bodyComp->body->getCenterOfMassTransform().getRotation().getAxis().getY() * bodyComp->body->getCenterOfMassTransform().getRotation().getAngle());
        lua_pushnumber(l, y);
        return 1;
    }

    int manager_component_body_setRotationY(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* bodyComp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        float y = btRadians(lua_tonumber(l, 2));
        btVector3 axis = btVector3(0.0f, y, 0.0f);
        float angle = axis.length();
        btVector3 position = bodyComp->body->getCenterOfMassPosition();
        bodyComp->body->setCenterOfMassTransform(btTransform(
            btQuaternion(axis, angle),
            position
        ));
        return 0;
    }


    // btRigidBody.h
    int manager_component_body_setDamping(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        float linear_damping = lua_tonumber(l, 2);
        float angular_damping = lua_tonumber(l, 3);
        comp->body->setDamping(linear_damping, angular_damping);
        return 0;
    }

    int manager_component_body_getLinearDamping(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->body->getLinearDamping());
        return 1;
    }

    int manager_component_body_getAngularDamping(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->body->getAngularDamping());
        return 1;
    }

    int manager_component_body_getLinearSleepingThreshold(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->body->getLinearSleepingThreshold());
        return 1;
    }

    int manager_component_body_getAngularSleepingThreshold(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->body->getAngularSleepingThreshold());
        return 1;
    }

    int manager_component_body_applyDamping(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        float timeStep = lua_tonumber(l, 2);
        comp->body->applyDamping(timeStep);
        return 0;
    }

    int manager_component_body_getLinearFactor(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 linearFactor = comp->body->getLinearFactor();
        lua_pushnumber(l, linearFactor.x());
        lua_pushnumber(l, linearFactor.y());
        lua_pushnumber(l, linearFactor.z());
        return 3;
    }

    int manager_component_body_setLinearFactor(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 linearFactor = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->setLinearFactor(linearFactor);
        return 0;
    }

    int manager_component_body_getInvMass(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->body->getInvMass());
        return 1;
    }

    int manager_component_body_getMass(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, comp->body->getMass());
        return 1;
    }

    int manager_component_body_applyCentralForce(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 force = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->applyCentralForce(force);
        return 0;
    }

    int manager_component_body_getTotalForce(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 force = comp->body->getTotalForce();
        lua_pushnumber(l, force.x());
        lua_pushnumber(l, force.y());
        lua_pushnumber(l, force.z());
        return 3;
    }

    int manager_component_body_getTotalTorque(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 torque = comp->body->getTotalTorque();
        lua_pushnumber(l, torque.x());
        lua_pushnumber(l, torque.y());
        lua_pushnumber(l, torque.z());
        return 3;
    }

    int manager_component_body_getInvInertiaDiagLocal(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 inertia = comp->body->getInvInertiaDiagLocal();
        lua_pushnumber(l, inertia.x());
        lua_pushnumber(l, inertia.y());
        lua_pushnumber(l, inertia.z());
        return 3;
    }

    int manager_component_body_setInvInertiaDiagLocal(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 inertia = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->setInvInertiaDiagLocal(inertia);
        return 0;
    }

    int manager_component_body_setSleepingThresholds(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        float linear = lua_tonumber(l, 2);
        float angular = lua_tonumber(l, 3);
        comp->body->setSleepingThresholds(linear, angular);
        return 0;
    }

    int manager_component_body_applyTorque(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 torque = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->applyTorque(torque);
        return 0;
    }

    int manager_component_body_applyForce(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 force = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        btVector3 relPos = btVector3(
            lua_tonumber(l, 5),
            lua_tonumber(l, 6),
            lua_tonumber(l, 7)
        );
        comp->body->applyForce(force, relPos);
        return 0;
    }

    int manager_component_body_applyCentralImpulse(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 impulse = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->applyCentralImpulse(impulse);
        return 0;
    }

    int manager_component_body_applyTorqueImpulse(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 torque = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->applyTorqueImpulse(torque);
        return 0;
    }

    int manager_component_body_applyImpulse(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 impulse = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        btVector3 relPos = btVector3(
            lua_tonumber(l, 5),
            lua_tonumber(l, 6),
            lua_tonumber(l, 7)
        );
        comp->body->applyImpulse(impulse, relPos);
        return 0;
    }

    int manager_component_body_applyPushImpulse(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 impulse = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        btVector3 relPos = btVector3(
            lua_tonumber(l, 5),
            lua_tonumber(l, 6),
            lua_tonumber(l, 7)
        );
        comp->body->applyPushImpulse(impulse, relPos);
        return 0;
    }

    int manager_component_body_getPushVelocity(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 velocity = comp->body->getPushVelocity();
        lua_pushnumber(l, velocity.x());
        lua_pushnumber(l, velocity.y());
        lua_pushnumber(l, velocity.z());
        return 3;
    }

    int manager_component_body_getTurnVelocity(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 velocity = comp->body->getTurnVelocity();
        lua_pushnumber(l, velocity.x());
        lua_pushnumber(l, velocity.y());
        lua_pushnumber(l, velocity.z());
        return 3;
    }

    int manager_component_body_setPushVelocity(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 velocity = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->setPushVelocity(velocity);
        return 0;
    }

    int manager_component_body_setTurnVelocity(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 velocity = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->setTurnVelocity(velocity);
        return 0;
    }

    int manager_component_body_applyCentralPushImpulse(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 impulse = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->applyCentralPushImpulse(impulse);
        return 0;
    }

    int manager_component_body_applyTorqueTurnImpulse(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 torque = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->applyTorqueTurnImpulse(torque);
        return 0;
    }

    int manager_component_body_clearForces(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        comp->body->clearForces();
        return 0;
    }

    int manager_component_body_getLinearVelocity(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 velocity = comp->body->getLinearVelocity();
        lua_pushnumber(l, velocity.x());
        lua_pushnumber(l, velocity.y());
        lua_pushnumber(l, velocity.z());
        return 3;
    }

    int manager_component_body_getAngularVelocity(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 velocity = comp->body->getAngularVelocity();
        lua_pushnumber(l, velocity.x());
        lua_pushnumber(l, velocity.y());
        lua_pushnumber(l, velocity.z());
        return 3;
    }

    int manager_component_body_setLinearVelocity(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 velocity = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->setLinearVelocity(velocity);
        return 0;
    }

    int manager_component_body_setAngularVelocity(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 velocity = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->setAngularVelocity(velocity);
        return 0;
    }

    int manager_component_body_getVelocityInLocalPoint(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 relPos = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        btVector3 velocity = comp->body->getVelocityInLocalPoint(relPos);
        lua_pushnumber(l, velocity.x());
        lua_pushnumber(l, velocity.y());
        lua_pushnumber(l, velocity.z());
        return 3;
    }

    int manager_component_body_getPushVelocityInLocalPoint(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 relPos = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        btVector3 velocity = comp->body->getPushVelocityInLocalPoint(relPos);
        lua_pushnumber(l, velocity.x());
        lua_pushnumber(l, velocity.y());
        lua_pushnumber(l, velocity.z());
        return 3;
    }

    int manager_component_body_setAngularFactorVector3(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        btVector3 angFac = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        comp->body->setAngularFactor(angFac);
        return 0;
    }

    int manager_component_body_setAngularFactorScalar(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        float angFac = lua_tonumber(l, 2);
        comp->body->setAngularFactor(angFac);
        return 0;
    }


    // btCollisionObject.h
    int manager_component_body_isActive(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, comp->body->isActive());
        return 1;
    }

    int manager_component_body_getActivationState(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushinteger(l, comp->body->getActivationState());
        return 1;
    }

    int manager_component_body_setActivateState(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        comp->body->setActivationState(lua_tointeger(l, 2));
        return 0;
    }

    int manager_component_body_isStaticObject(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, comp->body->isStaticObject());
        return 1;
    }

    int manager_component_body_isKinematicObject(lua_State* l) {
        manager::component::physics::AbstractBodyComponent* comp = (manager::component::physics::AbstractBodyComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, comp->body->isKinematicObject());
        return 1;
    }

}