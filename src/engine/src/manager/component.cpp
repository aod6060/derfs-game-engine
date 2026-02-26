#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "BulletCollision/CollisionShapes/btStridingMeshInterface.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "../sys.hpp"
#include "json/value.h"
#include <functional>
#include <iostream>


namespace manager {
    namespace component {
        static std::map<std::string, std::function<IComponent*()>> _componentSetup = 
        {
            {
                "mesh-component",
                []() {
                    return new MeshComponent();
                }
            },
            {
                "camera-component",
                []() {
                    return new CameraComponent();
                }
            },
            {
                "static-body-component",
                []() {
                    return new physics::StaticBodyComponent();
                }
            },
            {
                "dynamic-body-component",
                []() {
                    return new physics::DynamicBodyComponent();
                }
            },
            {
                "kinematic-body-component",
                []() {
                    return new physics::KinematicBodyComponent();
                }
            },
            {
                "trigger-component",
                []() {
                    return new physics::TriggerComponent();
                }
            }
        };

        void componentFactory(Entity* entity, std::string type, Json::Value value) {
            if(_componentSetup.find(type) != _componentSetup.end()) {
                //_componentSetup.at(type)(entity, type, value);
                entity->components[type] = _componentSetup.at(type)();
                entity->components.at(type)->load(value);
                return;
            } else {
                std::cout << "\"" << type << "\" isn't a component type!\n";
                return;
            }
        }

        // CameraComponent
        void CameraComponent::init(Entity* entity) {
            this->entity = entity;
        }

        void CameraComponent::handleEvent(SDL_Event* e) {

        }

        void CameraComponent::update(float delta) {

        }
        
        glm::mat4 CameraComponent::toView() {
            glm::vec3 r = glm::radians(this->entity->transform.rotation);
            float angle = glm::length(r);
            glm::vec3 nr = glm::normalize(r);
            if(angle < 0.001f) {
                nr = glm::vec3(1.0f, 0.0f, 0.0f);
            }
            return
                glm::rotate(glm::mat4(1.0f), angle, nr) *
                glm::translate(glm::mat4(1.0f), -this->entity->transform.position);
        }

        glm::mat4 CameraComponent::toViewWithParent() {
            return  toView() * glm::inverse(this->entity->transform.toParentMatrix(this->entity->parent));
        }


        void CameraComponent::preRender() {
            render::getMainShader()->setProjection(glm::perspective(glm::radians(this->fov), app::getAspect(), this->znear, this->zfar));

            if(this->entity->hasParent()) {
                render::getMainShader()->setView(toViewWithParent());
            } else {
                render::getMainShader()->setView(this->toView());
            }
        }

        void CameraComponent::render() {

        }

        void CameraComponent::release() {
            this->entity = nullptr;
        }

        void CameraComponent::load(Json::Value value) {
            this->fov = value["fov"].asFloat();
            this->znear = value["znear"].asFloat();
            this->zfar = value["zfar"].asFloat();
        }

        // MeshComponent
        void MeshComponent::init(Entity* entity) {
            this->entity = entity;
        }

        void MeshComponent::handleEvent(SDL_Event* e) {

        }

        void MeshComponent::update(float delta) {

        }

        void MeshComponent::preRender() {

        }

        void MeshComponent::render() {

            if(this->entity->hasParent()) {
                render::getMainShader()->setModel(this->entity->transform.toParentMatrix(this->entity->parent) * this->entity->transform.toModel());
            } else {
                render::getMainShader()->setModel(this->entity->transform.toModel());
            }
            render::getMainShader()->setUVScale(this->uvScale);
            assets::getTexture2D(this->texture)->bind(GL_TEXTURE0);
            render::getMainShader()->bindVertexArray();
            assets::getMesh(this->mesh)->vertices.bind();
            render::getMainShader()->verticePointer();
            assets::getMesh(this->mesh)->texCoords.bind();
            render::getMainShader()->texCoordPointer();
            assets::getMesh(this->mesh)->indencies.bind();
            render::drawElements(GL_TRIANGLES, assets::getMesh(this->mesh)->indencies.count());
            assets::getMesh(this->mesh)->indencies.unbind();
            render::getMainShader()->unbindVertexArray();
            assets::getTexture2D(this->texture)->unbind(GL_TEXTURE0);
        }

        void MeshComponent::release() {
            this->entity = nullptr;
        }

        void MeshComponent::load(Json::Value value) {
            this->mesh = value["mesh"].asString();
            this->texture = value["texture"].asString();
            this->uvScale = value["uv-scale"].asFloat();
        }

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

                btTriangleMesh* triangleMesh = new btTriangleMesh(true, false);

                for(int i = 0; i < assets::getMesh(meshName)->data.triangles.size(); i++) {
                    render::mesh::Triangle tris = assets::getMesh(meshName)->data.triangles.at(i);

                    triangleMesh->addTriangleIndices(tris.v1, tris.v2, tris.v3);
                    

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

                btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(triangleMesh, true);

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
                std::cout << "Groups: " << _groups.size() << "\n";
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        this->groups.push_back(g);
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                std::cout << "Masks: " << _masks.size() << "\n";
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
                std::cout << "Groups: " << _groups.size() << "\n";
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        this->groups.push_back(g);
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                std::cout << "Masks: " << _masks.size() << "\n";
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
                std::cout << "Groups: " << _groups.size() << "\n";
                if(!_groups.empty()) {
                    for(int i = 0; i < _groups.size(); i++) {
                        std::string g = _groups[i].asString();
                        this->groups.push_back(g);
                    }
                }
                // Masks
                Json::Value _masks = value["masks"];
                std::cout << "Masks: " << _masks.size() << "\n";
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

                std::cout << "Group: " << group << "\n";

                int mask = 0;
                if(!masks.empty()) {
                    for(int i = 0; i < this->masks.size() - 1; i++) {
                        mask |= this->entity->scene->global->getPhysicsGroups(masks.at(i));
                    }
                    mask |= this->entity->scene->global->getPhysicsGroups(masks.at(masks.size() - 1));
                }

                std::cout << "Mask: " << mask << "\n";

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
                std::cout << "Groups: " << _groups.size() << "\n";
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
                std::cout << "Masks: " << _masks.size() << "\n";
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
        }
    }
}