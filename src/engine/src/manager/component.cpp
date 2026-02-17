#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btVector3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "../sys.hpp"
#include "json/value.h"
#include <functional>


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

                ::physics::getWorld()->addRigidBody(this->body);
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
                        this->shape = new btBoxShape(halfExtents);
                    } else if(type == "sphere") {
                        float radius = collisionShape["radius"].asFloat();
                        this->shape = new btSphereShape(radius);
                    } else if(type == "capsule") {
                        float radius = collisionShape["radius"].asFloat();
                        float height = collisionShape["height"].asFloat();
                        this->shape = new btCapsuleShape(radius, height);
                    }
                } else {
                    std::cout << "This " << collisionShape["type"].asString() << " isn't supported by static-body-component\n";
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
                        this->shape = new btBoxShape(halfExtents);
                    } else if(type == "sphere") {
                        float radius = collisionShape["radius"].asFloat();
                        this->shape = new btSphereShape(radius);
                    } else if(type == "capsule") {
                        float radius = collisionShape["radius"].asFloat();
                        float height = collisionShape["height"].asFloat();
                        this->shape = new btCapsuleShape(radius, height);
                    }
                } else {
                    std::cout << "This " << collisionShape["type"].asString() << " isn't supported by static-body-component\n";
                }
            }
        }
    }
}