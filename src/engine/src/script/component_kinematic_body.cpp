#include "../sys.hpp"


namespace script {

    // Kinematic Body Section
    void manager_component_kinematic_body_load_library(lua_State* l) {
        // int manager_component_kinematic_body_hasComponent(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_hasComponent", manager_component_kinematic_body_hasComponent);
        // int manager_component_kinematic_body_getComponent(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_getComponent", manager_component_kinematic_body_getComponent);
        // int manager_component_kinematic_body_getWorldTransformOrigin(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_getWorldTransformOrigin", manager_component_kinematic_body_getWorldTransformOrigin);
        // int manager_component_kinematic_body_setWorldTransformOrigin(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_setWorldTransformOrigin", manager_component_kinematic_body_setWorldTransformOrigin);
        // int manager_component_kinematic_body_getWorldTransformRotation(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_getWorldTransformRotation", manager_component_kinematic_body_getWorldTransformRotation);
        // int manager_component_kinematic_body_setWorldTransformRotation(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_setWorldTransformRotation", manager_component_kinematic_body_setWorldTransformRotation);
        // int manager_component_kinematic_body_isController(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_isController", manager_component_kinematic_body_isController);
        // int manager_component_kinematic_body_setIsController(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_setIsController", manager_component_kinematic_body_setIsController);
        // int manager_component_kinematic_body_getLinearVelocity(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_getLinearVelocity", manager_component_kinematic_body_getLinearVelocity);
        // int manager_component_kinematic_body_setLinearVelocity(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_setLinearVelocity", manager_component_kinematic_body_setLinearVelocity);
        // int manager_component_kinematic_body_moveAndSlide(lua_State* l);
        lua_register(l, "manager_component_kinematic_body_moveAndSlide", manager_component_kinematic_body_moveAndSlide);
        // manager_component_kinematic_body_isOnFloor
        lua_register(l, "manager_component_kinematic_body_isOnFloor", manager_component_kinematic_body_isOnFloor);
    }

    int manager_component_kinematic_body_hasComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushboolean(l, entity->components.find("kinematic-body-component") != entity->components.end());
        return 1;
    }

    int manager_component_kinematic_body_getComponent(lua_State* l) {
        manager::Entity* entity = (manager::Entity*)lua_touserdata(l, 1);
        lua_pushlightuserdata(l, entity->components.at("kinematic-body-component"));
        return 1;
    }

    int manager_component_kinematic_body_getWorldTransformOrigin(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);

        btTransform tran;
        body->body->getMotionState()->getWorldTransform(tran);

        lua_pushnumber(l, tran.getOrigin().x());
        lua_pushnumber(l, tran.getOrigin().y());
        lua_pushnumber(l, tran.getOrigin().z());
        return 3;
    }

    int manager_component_kinematic_body_setWorldTransformOrigin(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);
        float x = lua_tonumber(l, 2);
        float y = lua_tonumber(l, 3);
        float z = lua_tonumber(l, 4);

        btTransform tran;
        body->body->getMotionState()->getWorldTransform(tran);

        tran.setOrigin(btVector3(x, y, z));
        body->body->getMotionState()->setWorldTransform(tran);
        //body->body->getWorldTransform().setOrigin(btVector3(x, y, z));
        return 0;
    }

    int manager_component_kinematic_body_getWorldTransformRotation(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);

        btTransform tran;
        body->body->getMotionState()->getWorldTransform(tran);

        float angle = tran.getRotation().getAngle();
        btVector3 axis = tran.getRotation().getAxis();

        axis = axis * angle;

        axis[0] = btDegrees(axis.x());
        axis[1] = btDegrees(axis.y());
        axis[2] = btDegrees(axis.z());

        lua_pushnumber(l,axis[0]);
        lua_pushnumber(l,axis[1]);
        lua_pushnumber(l,axis[2]);

        return 3;
    }

    int manager_component_kinematic_body_setWorldTransformRotation(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);

        float x = lua_tonumber(l, 2);
        float y = lua_tonumber(l, 3);
        float z = lua_tonumber(l, 4);

        btVector3 axis = btVector3(
            btRadians(x),
            btRadians(y),
            btRadians(z)
        );

        float angle = axis.length();
        axis = axis.normalize();

        if(angle < 0.0001f) {
            axis = btVector3(1.0f, 0.0f, 0.0f);
        }

        btQuaternion q = btQuaternion(axis, angle);

        btTransform tran;
        body->body->getMotionState()->getWorldTransform(tran);

        tran.setRotation(q);

        body->body->getMotionState()->setWorldTransform(tran);
        return 0;
    }


    int manager_component_kinematic_body_isController(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, body->isController);
        return 1;
    }

    int manager_component_kinematic_body_setIsController(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);
        body->isController = lua_toboolean(l, 2);
        return 0;
    }

    int manager_component_kinematic_body_getLinearVelocity(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);
        lua_pushnumber(l, body->linearVelocity.x());
        lua_pushnumber(l, body->linearVelocity.y());
        lua_pushnumber(l, body->linearVelocity.z());
        return 3;
    }

    int manager_component_kinematic_body_setLinearVelocity(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);
        body->linearVelocity = btVector3(
            lua_tonumber(l, 2),
            lua_tonumber(l, 3),
            lua_tonumber(l, 4)
        );
        return 0;
    }

    int manager_component_kinematic_body_moveAndSlide(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);
        body->moveAndSlide();
        return 0;
    }

    int manager_component_kinematic_body_isOnFloor(lua_State* l) {
        manager::component::physics::KinematicBodyComponent* body = (manager::component::physics::KinematicBodyComponent*)lua_touserdata(l, 1);
        lua_pushboolean(l, body->isOnFloor());
        return 1;
    }

}