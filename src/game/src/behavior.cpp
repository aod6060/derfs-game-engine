#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btScalar.h"
#include "game.hpp"
#include "glm/gtc/random.hpp"

namespace game {
    namespace behavior {

        // Entity

        // CrateTestEntity
        void CrateTestEntity::ready() {
            this->bodyComponent = (manager::component::physics::DynamicBodyComponent*)this->entity->components.at("dynamic-body-component");
        }

        void CrateTestEntity::update(float delta) {
            if(input::isKeyPressedOnce(input::Keyboard::KEYS_LEFT)) {
                float amount = 128.0f;

                btVector3 p = btVector3(
                    (util::random::randf() * 2.0f - 1.0f) * amount,
                    (util::random::randf() + 1.0f) * amount,
                    (util::random::randf() * 2.0f - 1.0f) * amount
                );

                this->bodyComponent->body->setActivationState(ACTIVE_TAG);
                this->bodyComponent->body->applyCentralForce(p);
            }

            if(input::isKeyPressedOnce(input::Keyboard::KEYS_UP)) {
                float amount = 32.0f;

                btVector3 p = btVector3(
                    (util::random::randf() * 2.0f - 1.0f) * amount,
                    (util::random::randf() + 1.0f) * amount,
                    (util::random::randf() * 2.0f - 1.0f) * amount
                );
                
                this->bodyComponent->body->setActivationState(ACTIVE_TAG);
                this->bodyComponent->body->applyTorque(p);
            }

            if(input::isKeyPressedOnce(input::Keyboard::KEYS_RIGHT)) {
                float amount = 8.0f;

                btVector3 p = btVector3(
                    (util::random::randf() * 2.0f - 1.0f) * amount,
                    (util::random::randf() + 1.0f) * amount,
                    (util::random::randf() * 2.0f - 1.0f) * amount
                );
                
                this->bodyComponent->body->setActivationState(ACTIVE_TAG);
                this->bodyComponent->body->applyCentralImpulse(p);
            }

            if(this->entity->transform.position.y < -32.0f) {
                this->reset();
            }
            
        }

        void CrateTestEntity::release() {
            this->bodyComponent = nullptr;
        }

        void CrateTestEntity::reset() {
            glm::vec3 p = glm::linearRand(this->min, this->max);
            glm::vec3 r = glm::linearRand(glm::vec3(0.0f), glm::vec3(360.0f));

            this->entity->transform.setTransformedPosition(p);
            this->entity->transform.setTransformedRotation(r);

            this->bodyComponent->body->setCenterOfMassTransform(this->entity->transform.convertToBulletTransform());
        }

        // PlayerFPSEntity
        void PlayerFPSEntity::ready() {
            int i = 0;
            this->scene = this->entity->scene;
            this->sceneBehavior = (AbstractPhysicsTestScene*)this->scene->behavior;
            this->global = this->scene->global;
            this->rayCast = (manager::component::physics::RayCastComponent*)this->entity->components.at("ray-cast-component");
            
            this->yPivotEntity = this->entity->getChildEntity(0);
            
            this->meshEntity = this->entity->getChildEntity(1);
           
            this->pivotEntity = this->yPivotEntity->getChildEntity(0);
            this->pivotEntityPushArm = (manager::component::physics::PushArmComponent*)this->pivotEntity->components.at("push-arm-component");
            
            this->cameraEntity = pivotEntity->getChildEntity(0);
            this->bodyComponent = (manager::component::physics::DynamicBodyComponent*)entity->components.at("dynamic-body-component");
            this->bodyComponent->body->setAngularFactor(0.0f);
            this->bodyComponent->body->setSleepingThresholds(0.0f, 0.0f);
            this->bodyComponent->body->setActivationState(DISABLE_DEACTIVATION);
        }

        void PlayerFPSEntity::update(float delta) {
            if(input::mapping::isMappingPressedOnce(global->mappings.at("toggle-mousegrab"))) {
                input::toggleGrab();
            }

            if(input::isGrab()) {
                glm::vec2 mc = input::toVelocity();

                float rx = pivotEntity->transform.rotation.x;
                float ry = yPivotEntity->transform.rotation.y;

                rx = rx - (mc.y * 0.5f);
                ry = ry - (mc.x * 0.5f);

                if(rx < -90.0f) {
                    rx = -90.0f;
                } else if(rx > 90.0f) {
                    rx = 90.0f;
                }

                if(ry < -360.0f) {
                    ry += 360.0f;
                } else if(ry > 360.0f) {
                    ry -= 360.0f;
                }

                pivotEntity->transform.rotation.x = rx;
                yPivotEntity->transform.rotation.y = ry;

                float yrad = btRadians(ry);

                btVector3 v = bodyComponent->body->getLinearVelocity();

                this->time += delta;

                time = clamp(0.0f, 1.0f, time);

                v[0] = 0;
                v[2] = 0;

                bool movePlayer = false;
                float runSpeed = this->moveSpeed;

                if(input::mapping::isMappingPressed(global->mappings.at("run"))) {
                    runSpeed = runSpeed * 2.0f;    
                }

                if(input::mapping::isMappingPressed(global->mappings.at("move-forward"))) {
                    movePlayer = true;
                    v[0] = -(btSin(yrad) * runSpeed);
                    v[2] = -(btCos(yrad) * runSpeed);
                    time = 0.0f;
                }

                if(input::mapping::isMappingPressed(global->mappings.at("move-backward"))) {
                    movePlayer = true;
                    v[0] = (btSin(yrad) * runSpeed);
                    v[2] = (btCos(yrad) * runSpeed);
                    time = 0.0f;
                }

                if(input::mapping::isMappingPressed(global->mappings.at("strafe-left"))) {
                    movePlayer = true;
                    v[0] = v[0] - (btCos(yrad) * runSpeed);
                    v[2] = v[2] + (btSin(yrad) * runSpeed);
                }

                if(input::mapping::isMappingPressed(global->mappings.at("strafe-right"))) {
                    movePlayer = true;
                    v[0] = v[0] + (btCos(yrad) * runSpeed);
                    v[2] = v[2] - (btSin(yrad) * runSpeed);
                }

                if(this->rayCast->isHit && input::mapping::isMappingPressed(global->mappings.at("jump"))) {
                    movePlayer = true;
                    v[1] = jumpSpeed;
                }

                if(movePlayer) {
                    this->meshEntity->transform.rotation.y = ry;
                }

                if(toggleFPS) {
                    meshEntity->transform.rotation.y = ry;
                } else {
                    if(movePlayer) {
                        meshEntity->transform.rotation.y = ry;
                    }
                }

                if(input::mapping::isMappingPressedOnce(global->mappings.at("toggle-fps"))) {
                    std::cout << toggleFPS << "\n";
                    toggleFPS = !toggleFPS;
                    animatedCamera = true;
                }

                bodyComponent->body->setLinearVelocity(v);

                if(this->entity->transform.position.y < -64.0f) {
                    reset();
                }
            }

            float td = this->tdistance;

            if(this->pivotEntityPushArm->distance < tdistance) {
                td = this->pivotEntityPushArm->distance;
            }

            if(animatedCamera) {
                if(toggleFPS) {
                    if(animateTime >= maxAnimateTime) {
                        animatedCamera = false;
                        cdistance = fdistance;
                        animateTime = 0.0f;
                    } else {
                        animateTime += (delta * 3.0f);
                        cdistance = lerp(td, fdistance, animateTime);
                    }
                } else {
                    meshEntity->visible = true;
                    if(animateTime >= maxAnimateTime) {
                        animatedCamera = false;
                        cdistance = tdistance;
                        animateTime = 0.0f;
                    } else {
                        animateTime += (delta * 3.0f);
                        cdistance = lerp(fdistance, tdistance, animateTime);
                    }
                }
            } else {
                meshEntity->visible = !this->toggleFPS;
            }

            pivotEntityPushArm->distance = cdistance;


        }

        void PlayerFPSEntity::release() {
            this->scene = nullptr;
            this->sceneBehavior = nullptr;
            this->rayCast = nullptr;
            this->yPivotEntity = nullptr;
            this->pivotEntity = nullptr;
            this->pivotEntityPushArm = nullptr;
            this->cameraEntity = nullptr;
            this->meshEntity = nullptr;
            this->bodyComponent = nullptr;
        }

        void PlayerFPSEntity::reset() {
            glm::vec3 p = glm::linearRand(this->min, this->max);
            this->entity->transform.setTransformedPosition(p);
            this->bodyComponent->body->setCenterOfMassTransform(this->entity->transform.convertToBulletTransform());
        }

        float PlayerFPSEntity::lerp(float v0, float v1, float t) {
            return v0 + t * (v1 - v0);
        }

        float PlayerFPSEntity::clamp(float min, float max, float value) {
            if(min > value) {
                return min;
            }

            if(max < value) {
                return max;
            }

            return value;
        }

        
        // Scene
        void AbstractPhysicsTestScene::ready() {
            this->cratesEntity = this->scene->entities.at(1);

            for(int i = 0; i < this->amount; i++) {
                manager::Entity* temp = new manager::Entity();
                temp->loadPrefab(this->prefab);
                this->cratesEntity->addChildEntity(temp);
                manager::component::physics::DynamicBodyComponent* comp = (manager::component::physics::DynamicBodyComponent*)temp->components.at("dynamic-body-component");
                temp->transform.setTransformedPosition(glm::linearRand(this->min, this->max));
                temp->transform.setTransformedRotation(glm::linearRand(glm::vec3(0.0f), glm::vec3(360.0f)));
                comp->body->setCenterOfMassTransform(temp->transform.convertToBulletTransform());
                temp->postInit();
                comp = nullptr;
                temp = nullptr;
            }

            int entityAmount = amount + 2;

            std::cout << "There are " << entityAmount << " in this scene!\n";

            this->setNextScene();
        }

        void AbstractPhysicsTestScene::update(float delta) {
            if(input::mapping::isMappingPressedOnce(scene->global->mappings.at("escape"))) {
                app::exit();
            }

            if(input::isKeyPressedOnce(input::Keyboard::KEYS_R)) {
                scene->global->changeScene(this->nextScene);
            }
        }

        void AbstractPhysicsTestScene::release() {
            cratesEntity = nullptr;
        }


        void DirectionPhysicsTestScene::setNextScene() {
            this->nextScene = "data/scenes/point.scene.json";
        }

        void PointPhysicsTestScene::setNextScene() {
            this->nextScene = "data/scenes/spot.scene.json";
        }

        void SpotPhysicsTestScene::setNextScene() {
            this->nextScene = "data/scenes/direction.scene.json";
        }

    }
}