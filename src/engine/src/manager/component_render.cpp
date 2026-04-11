#include "../sys.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/geometric.hpp"
#include "json/value.h"

namespace manager {
    namespace component {
        namespace render {
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
                ::render::shader::geometry::getCameraUBO()->value.proj = glm::perspective(glm::radians(this->fov), app::getAspect(), this->znear, this->zfar);
                if(this->entity->hasParent()) {
                    ::render::shader::geometry::getCameraUBO()->value.view = this->toViewWithParent();
                } else {
                    ::render::shader::geometry::getCameraUBO()->value.view = this->toView();
                }
                ::render::shader::geometry::getCameraUBO()->update();

                ::render::shader::lighting::getLightingShader()->bind();
                ::render::shader::lighting::getLightingShader()->setCameraPosition(entity->transform.getGlobalPosition());
                ::render::shader::lighting::getLightingShader()->unbind();
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
                glm::mat4 m = this->entity->transform.toModel();
                if(this->entity->hasParent()) {
                    m = this->entity->transform.toParentMatrix(this->entity->parent) * this->entity->transform.toModel();
                }
                ::render::submitMeshDraw(this->mesh, this->material, m);
            }

            void MeshComponent::release() {
                this->entity = nullptr;
            }

            void MeshComponent::load(Json::Value value) {
                this->mesh = value["mesh"].asString();
                this->material = value["material"].asString();
            }



            // SunComponent
            void LightComponent::init(Entity* entity) {
                this->entity = entity;
            }

            void LightComponent::handleEvent(SDL_Event* e) {

            }

            void LightComponent::update(float delta) {
                if(this->light.type == ::render::shader::lighting::LightType::LT_DIRECTION) {
                    glm::vec3 rotation = this->entity->transform.getTransformedRotation();
                    //std::cout << rotation.x << ", " << rotation.y << ", " << rotation.z << "\n";

                    float angle = glm::length(rotation);
                    glm::vec3 axis = glm::normalize(rotation);
                    glm::vec3 down = glm::vec3(0.0f, 1.0f, 0.0f);

                    if(angle < 0.001f) {
                        axis = glm::vec3(1.0f, 0.0f, 0.0f);
                    }

                    glm::vec3 direction = glm::angleAxis(angle, axis) * down;

                    this->light.position = direction;
            
                } else if(this->light.type == ::render::shader::lighting::LightType::LT_POINT) {
                    this->light.position = this->entity->transform.getTransformedPosition();
                } else if(this->light.type == ::render::shader::lighting::LightType::LT_SPOT) {
                    this->light.position = this->entity->transform.getTransformedPosition();
                    // spotDirection
                    glm::vec3 rotation = this->entity->transform.getTransformedRotation();
                    //std::cout << rotation.x << ", " << rotation.y << ", " << rotation.z << "\n";

                    float angle = glm::length(rotation);
                    glm::vec3 axis = glm::normalize(rotation);
                    glm::vec3 forward = glm::vec3(0.0f, 1.0f, 0.0f);

                    if(angle < 0.001f) {
                        axis = glm::vec3(1.0f, 0.0f, 0.0f);
                    }

                    glm::vec3 direction = glm::angleAxis(angle, axis) * forward;

                    this->light.spotDirection = direction;
                }

                ::render::shader::lighting::addLight(this->light);
            }

            void LightComponent::preRender() {

            }

            void LightComponent::render() {

            }

            void LightComponent::release() {
                this->entity = nullptr;
            }

            void LightComponent::load(Json::Value value) {
                Json::Value light = value["light"];

                std::string type = light["type"].asString();
                // type
                if(type == "direction") {
                    this->light.type = ::render::shader::lighting::LightType::LT_DIRECTION;
                } else if(type == "point") {
                    this->light.type = ::render::shader::lighting::LightType::LT_POINT;
                } else if(type == "spot") {
                    this->light.type = ::render::shader::lighting::LightType::LT_SPOT;
                }
                // albedo
                this->light.albedo = glm::vec3(
                    light["albedo"]["x"].asFloat(),
                    light["albedo"]["y"].asFloat(),
                    light["albedo"]["z"].asFloat()
                );
                // ambient
                this->light.ambient = light["ambient"].asFloat();
                // diffuse
                this->light.diffuse = light["diffuse"].asFloat();
                // specular
                this->light.specular = light["specular"].asFloat();
                // attenuation
                /*
                Json::Value attenuation = light["attenuation"];
                this->light.constant = attenuation["constant"].asFloat();
                std::cout << "constant: " << this->light.constant << "\n";
                this->light.linear = attenuation["linear"].asFloat();
                std::cout << "linear: " << this->light.linear << "\n";
                this->light.quadratic = attenuation["quadratic"].asFloat();
                std::cout << "quadratic: " << this->light.quadratic << "\n";
                */
                this->light.radius = light["radius"].asFloat();
                // spot-cut-off
                this->light.spotCutOff = light["spotCutOff"].asFloat();
            }

        }
    }
}