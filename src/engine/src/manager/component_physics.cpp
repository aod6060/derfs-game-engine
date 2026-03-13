#include "../sys.hpp"


namespace manager {
    namespace component {
        namespace physics {

            // AbstractBodyComponent
            void AbstractBodyComponent::init(Entity* entity) {
                this->entity = entity;

                this->body = this->createRigidBody(this->mass, this->entity->transform.convertToBulletTransform(), this->shape);

                // Handle Groups
                int group = 0;

                if(!groups.empty()) {
                    for(int i = 0; i < this->groups.size() - 1; i++) {
                        //group |= groups.at(i);
                        group |= this->entity->scene->global->getPhysicsGroups(this->groups.at(i));
                    }
                    group |= this->entity->scene->global->getPhysicsGroups(groups.at(groups.size() - 1));
                }
                // Handle Masks
                int mask = 0;

                if(!masks.empty()) {
                    for(int i = 0; i < this->masks.size() - 1; i++) {
                        mask |= this->entity->scene->global->getPhysicsGroups(masks.at(i));
                    }
                    mask |= this->entity->scene->global->getPhysicsGroups(masks.at(masks.size() - 1));
                }

                ::physics::getWorld()->addRigidBody(this->body, group, mask);
            }

            void AbstractBodyComponent::handleEvent(SDL_Event* e) {

            }

            void AbstractBodyComponent::update(float delta) {
                this->entity->transform.interpretBulletTransform(this->body->getCenterOfMassTransform());
            }

            void AbstractBodyComponent::preRender() {

            }

            void AbstractBodyComponent::render() {

            }

            void AbstractBodyComponent::release() {
                ::physics::getWorld()->removeRigidBody(this->body);
                delete this->body;
                this->body = nullptr;
                delete this->shape;
                this->shape = nullptr;
                this->entity = nullptr;
            }

            btCollisionShape* AbstractBodyComponent::createSphereShape(float radius) {
                return new btSphereShape(radius);
            }

            btCollisionShape* AbstractBodyComponent::createCapsuleShape(float radius, float height) {
                return new btCapsuleShape(radius, height);
            }

            btCollisionShape* AbstractBodyComponent::createBoxShape(const btVector3& halfExtents) {
                return new btBoxShape(halfExtents);
            }

            btCollisionShape* AbstractBodyComponent::createTriangleShape(std::string meshName) {

                btTriangleMesh* triangleMesh = new btTriangleMesh(false, false);

                for(int i = 0; i < assets::getMesh(meshName)->data.triangles.size(); i++) {
                    ::render::mesh::Triangle tris = assets::getMesh(meshName)->data.triangles.at(i);

                    //triangleMesh->addTriangleIndices(tris.v1, tris.v2, tris.v3);
                    

                    btVector3 v1 = btVector3(
                        assets::getMesh(meshName)->data.vertices.at(tris.v1).position.x,
                        assets::getMesh(meshName)->data.vertices.at(tris.v1).position.y,
                        assets::getMesh(meshName)->data.vertices.at(tris.v1).position.z
                    );

                    btVector3 v2 = btVector3(
                        assets::getMesh(meshName)->data.vertices.at(tris.v2).position.x,
                        assets::getMesh(meshName)->data.vertices.at(tris.v2).position.y,
                        assets::getMesh(meshName)->data.vertices.at(tris.v2).position.z
                    );

                    btVector3 v3 = btVector3(
                        assets::getMesh(meshName)->data.vertices.at(tris.v3).position.x,
                        assets::getMesh(meshName)->data.vertices.at(tris.v3).position.y,
                        assets::getMesh(meshName)->data.vertices.at(tris.v3).position.z
                    );

                    triangleMesh->addTriangle(v1, v2, v3);
                }

                btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(triangleMesh, false);

                return shape;
            }

            btCollisionShape* AbstractBodyComponent::createStaticPlaneShape(const btVector3& planeNormal, float planeConstant) {
                return new btStaticPlaneShape(planeNormal, planeConstant);
            }

            btRigidBody* AbstractBodyComponent::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* collisionShape) {
                btVector3 localInertial(0, 0, 0);
                collisionShape->calculateLocalInertia(mass, localInertial);
                btDefaultMotionState* ms = new btDefaultMotionState(startTransform);
                btRigidBody::btRigidBodyConstructionInfo cinfo(mass, ms, collisionShape, localInertial);
                btRigidBody* body = new btRigidBody(cinfo);
                body->setUserIndex(-1);
                return body;
            }
            
            btRigidBody* AbstractBodyComponent::createStaticRigidBody(const btTransform& startTransform, btCollisionShape* collisionShape) {
                btDefaultMotionState* ms = new btDefaultMotionState(startTransform);
                btRigidBody::btRigidBodyConstructionInfo cinfo(0, ms, collisionShape);
                btRigidBody* body = new btRigidBody(cinfo);
                body->setUserIndex(-1);
                return body;
            }
                
            // StaticBodyComponent
            void StaticBodyComponent::load(Json::Value value) {
                // collision-shape
                Json::Value collisionShape = value["collision-shape"];
                if(this->collisionShapeTypes.find(collisionShape["type"].asString()) != collisionShapeTypes.end()) {
                    std::string type = collisionShape["type"].asString();

                    if(type == "static-plane") {
                        Json::Value planeNormalValue = collisionShape["plane-normal"];
                        btVector3 planeNormal = btVector3(
                            planeNormalValue["x"].asFloat(),
                            planeNormalValue["y"].asFloat(),
                            planeNormalValue["z"].asFloat()
                        );
                        float planeConstant = collisionShape["plane-constant"].asFloat();

                        this->shape = new btStaticPlaneShape(planeNormal, planeConstant);

                    } else if(type == "box") {
                        Json::Value halfExtentsValue = collisionShape["half-extends"];
                        btVector3 halfExtents = btVector3(
                            halfExtentsValue["x"].asFloat(),
                            halfExtentsValue["y"].asFloat(),
                            halfExtentsValue["z"].asFloat()
                        );
                        this->shape = this->createBoxShape(halfExtents);
                    } else if(type == "sphere") {
                        float radius = collisionShape["radius"].asFloat();
                        this->shape = this->createSphereShape(radius);
                    } else if(type == "capsule") {
                        float radius = collisionShape["radius"].asFloat();
                        float height = collisionShape["height"].asFloat();
                        this->shape = this->createCapsuleShape(radius, height);
                    } else if(type == "triangle-mesh") {
                        std::string mesh = collisionShape["mesh"].asString();
                        this->shape = this->createTriangleShape(mesh);
                    }
                } else {
                    std::cout << "This " << collisionShape["type"].asString() << " isn't supported by static-body-component\n";
                }
                // Groups
                Json::Value _groups = value["groups"];
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        this->groups.push_back(g);
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                if(!_masks.empty()) {
                    for(int i = 0; i < _masks.size(); i++) {
                        std::string m = _masks[i].asString();
                        this->masks.push_back(m);
                    }
                }
            }

            // DynamicBodyComponent
            void DynamicBodyComponent::load(Json::Value value) {
                // Get Mass
                this->mass = value["mass"].asFloat();
                // collision-shape
                Json::Value collisionShape = value["collision-shape"];

                if(this->collisionShapeTypes.find(collisionShape["type"].asString()) != collisionShapeTypes.end()) {
                    std::string type = collisionShape["type"].asString();
                    if(type == "box") {
                        Json::Value halfExtentsValue = collisionShape["half-extends"];
                        btVector3 halfExtents = btVector3(
                            halfExtentsValue["x"].asFloat(),
                            halfExtentsValue["y"].asFloat(),
                            halfExtentsValue["z"].asFloat()
                        );
                        this->shape = this->createBoxShape(halfExtents);
                    } else if(type == "sphere") {
                        float radius = collisionShape["radius"].asFloat();
                        this->shape = this->createSphereShape(radius);
                    } else if(type == "capsule") {
                        float radius = collisionShape["radius"].asFloat();
                        float height = collisionShape["height"].asFloat();
                        this->shape = this->createCapsuleShape(radius, height);
                    }
                } else {
                    std::cout << "This " << collisionShape["type"].asString() << " isn't supported by static-body-component\n";
                }
                // Groups
                Json::Value _groups = value["groups"];
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        this->groups.push_back(g);
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                if(!_masks.empty()) {
                    for(int i = 0; i < _masks.size(); i++) {
                        std::string m = _masks[i].asString();
                        this->masks.push_back(m);
                    }
                }
            }


            // Kinematic Body
            void KinematicBodyComponent::init(Entity* entity) {
                this->entity = entity;

                //this->body = this->createRigidBody(1.0f, this->entity->transform.convertToBulletTransform(), this->shape);
                this->body = this->createRigidBody(0.0f, this->entity->transform.convertToBulletTransform(), this->shape);

                // Handle Groups
                int group = 0;

                if(!groups.empty()) {
                    for(int i = 0; i < this->groups.size() - 1; i++) {
                        //group |= groups.at(i);
                        group |= this->entity->scene->global->getPhysicsGroups(this->groups.at(i));
                    }
                    group |= this->entity->scene->global->getPhysicsGroups(groups.at(groups.size() - 1));
                }
                // Handle Masks
                int mask = 0;

                if(!masks.empty()) {
                    for(int i = 0; i < this->masks.size() - 1; i++) {
                        mask |= this->entity->scene->global->getPhysicsGroups(masks.at(i));
                    }
                    mask |= this->entity->scene->global->getPhysicsGroups(masks.at(masks.size() - 1));
                }

                body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
                body->setActivationState(DISABLE_DEACTIVATION);
                
                ::physics::getWorld()->addRigidBody(this->body, group, mask);

                //::physics::getWorld()->contactTest(this->body, ContactResultCallback &resultCallback)
            }

            void KinematicBodyComponent::update(float delta) {
                this->entity->transform.interpretBulletTransform(this->body->getCenterOfMassTransform());
                //btCollisionWorld::ContactResultCallback result;
            }

            void KinematicBodyComponent::load(Json::Value value) {
                // collision-shape
                Json::Value collisionShape = value["collision-shape"];
                if(this->collisionShapeTypes.find(collisionShape["type"].asString()) != collisionShapeTypes.end()) {
                    std::string type = collisionShape["type"].asString();

                    if(type == "static-plane") {
                        Json::Value planeNormalValue = collisionShape["plane-normal"];
                        btVector3 planeNormal = btVector3(
                            planeNormalValue["x"].asFloat(),
                            planeNormalValue["y"].asFloat(),
                            planeNormalValue["z"].asFloat()
                        );
                        float planeConstant = collisionShape["plane-constant"].asFloat();

                        this->shape = new btStaticPlaneShape(planeNormal, planeConstant);

                    } else if(type == "box") {
                        Json::Value halfExtentsValue = collisionShape["half-extends"];
                        btVector3 halfExtents = btVector3(
                            halfExtentsValue["x"].asFloat(),
                            halfExtentsValue["y"].asFloat(),
                            halfExtentsValue["z"].asFloat()
                        );
                        this->shape = this->createBoxShape(halfExtents);
                    } else if(type == "sphere") {
                        float radius = collisionShape["radius"].asFloat();
                        this->shape = this->createSphereShape(radius);
                    } else if(type == "capsule") {
                        float radius = collisionShape["radius"].asFloat();
                        float height = collisionShape["height"].asFloat();
                        this->shape = this->createCapsuleShape(radius, height);
                    } else if(type == "triangle-mesh") {
                        std::string mesh = collisionShape["mesh"].asString();
                        this->shape = this->createTriangleShape(mesh);
                    }
                } else {
                    std::cout << "This " << collisionShape["type"].asString() << " isn't supported by static-body-component\n";
                }
                // Groups
                Json::Value _groups = value["groups"];
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        this->groups.push_back(g);
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                if(!_masks.empty()) {
                    for(int i = 0; i < _masks.size(); i++) {
                        std::string m = _masks[i].asString();
                        this->masks.push_back(m);
                    }
                } 
            }

            void KinematicBodyComponent::moveAndSlide() {
                // Get Bodies Motion State Transform
                btTransform tran;
                this->body->getMotionState()->getWorldTransform(tran);
                btVector3 pos = tran.getOrigin();
                pos += linearVelocity;
                tran.setOrigin(pos);
                this->body->getMotionState()->setWorldTransform(tran);
            }

            bool KinematicBodyComponent::isOnFloor() {

                KinematicBodyContactResultCallback callback;

                ::physics::getWorld()->contactTest(this->body, callback);

                if(callback.hit) {
                    btVector3 ndir = (callback.point - btVector3(0.0, 0.0, 0.0)).normalize();
                    return ndir.y() < 0.0f;
                }
                
                return false;
            }
            
            btScalar KinematicBodyComponent::KinematicBodyContactResultCallback::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
                if(colObj1Wrap->getCollisionObject()->isStaticOrKinematicObject()) {
                    this->hit = true;
                    this->point = cp.m_localPointA;
                    this->normal = cp.m_normalWorldOnB;
                    this->dist = cp.getDistance();
                }
                return 0;
            }


            // Trigger
            void TriggerComponent::init(Entity* entity) {
                std::cout << "Start of TriggerComponent::init\n";
                this->entity = entity;

                this->ghostObject = new btGhostObject();
                this->ghostObject->setCollisionShape(this->shape);
                this->ghostObject->setWorldTransform(this->entity->transform.convertToBulletTransform());
                this->ghostObject->setUserIndex(-1);
                this->ghostObject->setCollisionFlags(this->ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

                int group = 0;
                if(!groups.empty()) {
                    for(int i = 0; i < this->groups.size() - 1; i++) {
                        //group |= groups.at(i);
                        group |= this->entity->scene->global->getPhysicsGroups(this->groups.at(i));
                    }
                    group |= this->entity->scene->global->getPhysicsGroups(groups.at(groups.size() - 1));
                }

                int mask = 0;
                if(!masks.empty()) {
                    for(int i = 0; i < this->masks.size() - 1; i++) {
                        mask |= this->entity->scene->global->getPhysicsGroups(masks.at(i));
                    }
                    mask |= this->entity->scene->global->getPhysicsGroups(masks.at(masks.size() - 1));
                }

                ::physics::getWorld()->addCollisionObject(this->ghostObject, group, mask);
                std::cout << "End of TriggerComponent::init\n";
            }
            
            void TriggerComponent::handleEvent(SDL_Event* e) {

            }
            
            void TriggerComponent::update(float delta) {

                if(ghostObject->getNumOverlappingObjects() > 0) {
                    for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++) {
                        if(this->entityEnter.behavior != nullptr) {
                            this->entityEnter.behavior->executeCallback(this->entityEnter.name, {});
                        }
                    }
                } else {
                    if(this->entityExit.behavior != nullptr) {
                        // For now there won't be any arguments
                        this->entityExit.behavior->executeCallback(this->entityExit.name, {});
                    }
                }
            }
            
            void TriggerComponent::preRender() {

            }
            
            void TriggerComponent::render() {

            }
            
            void TriggerComponent::release() {
                ::physics::getWorld()->removeCollisionObject(this->ghostObject);
                if(this->entityEnter.behavior) {
                    this->entityEnter.behavior = nullptr;
                }
                if(this->entityExit.behavior) {
                    this->entityExit.behavior = nullptr;
                }
                ::physics::getWorld()->removeCollisionObject(this->ghostObject);
                delete this->ghostObject;
                this->ghostObject = nullptr;
                delete this->shape;
                this->shape = nullptr;
                this->entity = nullptr;
            }
            
            void TriggerComponent::load(Json::Value value) {
                std::cout << "Start of TriggerComponent.\n";
                // collision-shape
                Json::Value collisionShape = value["collision-shape"];
                if(this->collisionShapeTypes.find(collisionShape["type"].asString()) != collisionShapeTypes.end()) {
                    std::string type = collisionShape["type"].asString();

                    if(type == "box") {
                        Json::Value halfExtentsValue = collisionShape["half-extends"];
                        btVector3 halfExtents = btVector3(
                            halfExtentsValue["x"].asFloat(),
                            halfExtentsValue["y"].asFloat(),
                            halfExtentsValue["z"].asFloat()
                        );
                        this->shape = this->createBoxShape(halfExtents);
                    } else if(type == "sphere") {
                        float radius = collisionShape["radius"].asFloat();
                        this->shape = this->createSphereShape(radius);
                    } else if(type == "capsule") {
                        float radius = collisionShape["radius"].asFloat();
                        float height = collisionShape["height"].asFloat();
                        this->shape = this->createCapsuleShape(radius, height);
                    }
                } else {
                    std::cout << "This " << collisionShape["type"].asString() << " isn't supported by static-body-component\n";
                }

                // Groups
                Json::Value _groups = value["groups"];
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        std::cout << g << "\n";
                        this->groups.push_back(g);
                        std::cout << g << "\n";
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                if(!_masks.empty()) {
                    for(int i = 0; i < _masks.size(); i++) {
                        std::string m = _masks[i].asString();
                        this->masks.push_back(m);
                    }
                }

                std::cout << "End of TriggerComponent.\n";
            }

            btCollisionShape* TriggerComponent::createSphereShape(float radius) {
                return new btSphereShape(radius);
            }

            btCollisionShape* TriggerComponent::createCapsuleShape(float radius, float height) {
                return new btCapsuleShape(radius, height);
            }

            btCollisionShape* TriggerComponent::createBoxShape(const btVector3& halfExtents) {
                return new btBoxShape(halfExtents);
            }


            // PushArmComponent
            void PushArmComponent::init(Entity* entity) {
                this->entity = entity;

                int group = 0;
                if(!groups.empty()) {
                    for(int i = 0; i < this->groups.size() - 1; i++) {
                        //group |= groups.at(i);
                        group |= this->entity->scene->global->getPhysicsGroups(this->groups.at(i));
                    }
                    group |= this->entity->scene->global->getPhysicsGroups(groups.at(groups.size() - 1));
                }

                this->filterGroup = group;

                int mask = 0;
                if(!masks.empty()) {
                    for(int i = 0; i < this->masks.size() - 1; i++) {
                        mask |= this->entity->scene->global->getPhysicsGroups(masks.at(i));
                    }
                    mask |= this->entity->scene->global->getPhysicsGroups(masks.at(masks.size() - 1));
                }

                this->filterMask = mask;
            }

            void PushArmComponent::handleEvent(SDL_Event* e) {

            }

            void PushArmComponent::update(float delta) {
                if(this->entity->getChilderenAmount() > 0) {
                    manager::Entity* child = this->entity->getChildEntity(0);

                    btVector3 from = this->entity->transform.toBulletVector3(this->entity->transform.getTransformedPosition());

                    glm::mat4 m = this->entity->transform.toGlobalRotaionMatrix();
                    btVector3 dir = btVector3(
                        m[2][0],
                        m[2][1],
                        m[2][2]
                    );

                    btVector3 to = from + (this->entity->transform.toBulletVector3(glm::vec3(dir.x(), dir.y(), dir.z())) * this->distance);

                    btCollisionWorld::ClosestRayResultCallback rayResult = btCollisionWorld::ClosestRayResultCallback(from, to);
                    rayResult.m_collisionFilterGroup = filterGroup;
                    rayResult.m_collisionFilterMask = filterMask;

                    ::physics::getWorld()->rayTest(from, to, rayResult);

                    if(rayResult.hasHit()) {

                        glm::vec3 p = child->transform.toGLMVector3(rayResult.m_hitPointWorld);

                        glm::mat4 m = glm::inverse(child->transform.toParentTranslateMatrix(child->parent) * child->transform.toParentRotationMatrix(child->parent));

                        glm::vec4 p4 = m * glm::vec4(p.x, p.y, p.z, 1.0f);

                        //std::cout << p4.x << ", " << p4.y << ", " << p4.z << "\n";

                        child->transform.position = glm::vec3(0.0f, 0.5f, p4.z);

                    } else {
                        child->transform.position = glm::vec3(0.0f, 0.5f, distance);   
                    }

                    child = nullptr;
                }
            }

            void PushArmComponent::preRender() {

            }

            void PushArmComponent::render() {

            }

            void PushArmComponent::release() {
                this->entity = nullptr;
            }

            void PushArmComponent::load(Json::Value value) {
                // Distance
                this->distance = value["distance"].asFloat();
                // Groups
                Json::Value _groups = value["groups"];
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        std::cout << g << "\n";
                        this->groups.push_back(g);
                        std::cout << g << "\n";
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                if(!_masks.empty()) {
                    for(int i = 0; i < _masks.size(); i++) {
                        std::string m = _masks[i].asString();
                        this->masks.push_back(m);
                    }
                }
            }

            // RayCastComponent
            void RayCastComponent::init(Entity* entity) {
                std::cout << "RayCastComponent Init: \n";
                this->entity = entity;
                int group = 0;
                if(!groups.empty()) {
                    for(int i = 0; i < this->groups.size() - 1; i++) {
                        //group |= groups.at(i);
                        group |= this->entity->scene->global->getPhysicsGroups(this->groups.at(i));
                    }
                    group |= this->entity->scene->global->getPhysicsGroups(groups.at(groups.size() - 1));
                }
                this->filterGroup = group;

                int mask = 0;
                if(!masks.empty()) {
                    for(int i = 0; i < this->masks.size() - 1; i++) {
                        mask |= this->entity->scene->global->getPhysicsGroups(masks.at(i));
                    }
                    mask |= this->entity->scene->global->getPhysicsGroups(masks.at(masks.size() - 1));
                }

                this->filterMask = mask;
            }

            void RayCastComponent::handleEvent(SDL_Event* e) {
                
            }

            void RayCastComponent::update(float delta) {

                if(!this->disabled) {
                    btVector3 from = this->entity->transform.toBulletVector3(this->entity->transform.getTransformedPosition());
                    //std::cout << "From: " << from.x() << ", " << from.y() << ", " << from.z() << "\n";

                    glm::mat4 m = this->entity->transform.toGlobalRotaionMatrix();

                    glm::vec4 to_vec4 = m * glm::vec4(direction.x, direction.y, direction.z, 0.0f);

                    btVector3 to = this->entity->transform.toBulletVector3(glm::vec3(to_vec4.x, to_vec4.y, to_vec4.z) * scale) + from;
                    //std::cout << "To: " << to.x() << ", " << to.y() << ", " << to.z() << "\n";

                    btCollisionWorld::ClosestRayResultCallback rayResult = btCollisionWorld::ClosestRayResultCallback(from, to);
                    rayResult.m_collisionFilterGroup = filterGroup;
                    rayResult.m_collisionFilterMask = filterMask;

                    ::physics::getWorld()->rayTest(from, to, rayResult);

                    if(rayResult.hasHit()) {
                        this->isHit = true;
                        this->worldPoint = this->entity->transform.toGLMVector3(rayResult.m_hitPointWorld);
                        this->worldNormal = this->entity->transform.toGLMVector3(rayResult.m_hitNormalWorld);
                    } else {
                        this->isHit = false;
                        this->worldNormal = glm::vec3(0.0f);
                        this->worldPoint = glm::vec3(0.0f);
                    }
                } else {
                    this->isHit = false;
                }
            }

            void RayCastComponent::preRender() {
                
            }

            void RayCastComponent::render() {
                
            }

            void RayCastComponent::release() {
                this->entity = nullptr;
            }

            void RayCastComponent::load(Json::Value value) {
                Json::Value _direction = value["direction"];
                this->direction = glm::vec3(
                    _direction["x"].asFloat(),
                    _direction["y"].asFloat(),
                    _direction["z"].asFloat()
                );
                this->scale = value["scale"].asFloat();
                // Groups
                this->disabled = value["disabled"].asBool();
                Json::Value _groups = value["groups"];
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        std::cout << g << "\n";
                        this->groups.push_back(g);
                        std::cout << g << "\n";
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                if(!_masks.empty()) {
                    for(int i = 0; i < _masks.size(); i++) {
                        std::string m = _masks[i].asString();
                        this->masks.push_back(m);
                    }
                }
            }

        }
    }
}