#include "../sys.hpp"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

namespace physics {

    static btDefaultCollisionConfiguration* collisionConf = nullptr;
    static btCollisionDispatcher* disp = nullptr;
    static btBroadphaseInterface* broadPhase = nullptr;
    static btSequentialImpulseConstraintSolver* solver = nullptr;
    static btDiscreteDynamicsWorld* world = nullptr;

    static btGhostPairCallback* ghostPairCB = nullptr;

    static float timeStep = 1.0f / 60.0f;
    static btVector3 gravity;

    void init() {
        collisionConf = new btDefaultCollisionConfiguration();
        disp = new btCollisionDispatcher(collisionConf);
        broadPhase = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver();
        world = new btDiscreteDynamicsWorld(disp, broadPhase, solver, collisionConf);

        ghostPairCB = new btGhostPairCallback();

        world->getPairCache()->setInternalGhostPairCallback(ghostPairCB);

        world->setGravity(gravity);
    }

    void update() {
        world->stepSimulation(timeStep);
    }

    void release() {
        world->getPairCache()->setInternalGhostPairCallback(nullptr);
        delete ghostPairCB;
        delete world;
        delete solver;
        delete broadPhase;
        delete disp;
        delete collisionConf;
    }

    btDiscreteDynamicsWorld* getWorld() {
        return world;
    }

    btVector3 getGravity() {
        return gravity;
    }

    void setGravity(const btVector3& _gravity) {
        gravity = _gravity;
        if(world) {
            world->setGravity(gravity);
        }
    }

}