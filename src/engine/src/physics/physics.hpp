#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <btBulletDynamicsCommon.h>

namespace physics {
    void init();
    void update();
    void release();

    btDiscreteDynamicsWorld* getWorld();

    btVector3 getGravity();
    void setGravity(const btVector3& gravity);
}

#endif