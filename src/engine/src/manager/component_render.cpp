#include "../sys.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/geometric.hpp"

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
                /*
                ::render::shader::geometry::getMeshShader()->setProjection(glm::perspective(glm::radians(this->fov), app::getAspect(), this->znear, this->zfar));

                if(this->entity->hasParent()) {
                    ::render::shader::geometry::getMeshShader()->setView(toViewWithParent());
                } else {
                    ::render::shader::geometry::getMeshShader()->setView(this->toView());
                }
                */
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
                /*
                if(this->entity->hasParent()) {
                    ::render::shader::geometry::getMeshShader()->setModel(this->entity->transform.toParentMatrix(this->entity->parent) * this->entity->transform.toModel());
                } else {
                    ::render::shader::geometry::getMeshShader()->setModel(this->entity->transform.toModel());
                }

                assets::getMaterial(this->material)->bind();
                ::render::shader::geometry::getMeshShader()->drawMesh(assets::getMesh(this->mesh));
                assets::getMaterial(this->material)->unbind();
                */
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
            void SunComponent::init(Entity* entity) {
                this->entity = entity;
            }

            void SunComponent::handleEvent(SDL_Event* e) {

            }

            void SunComponent::update(float delta) {
                glm::vec3 rotation = this->entity->transform.getTransformedRotation();
                //std::cout << rotation.x << ", " << rotation.y << ", " << rotation.z << "\n";

                float angle = glm::length(rotation);
                glm::vec3 axis = glm::normalize(rotation);
                glm::vec3 down = glm::vec3(0.0f, 1.0f, 0.0f);

                if(angle < 0.001f) {
                    axis = glm::vec3(1.0f, 0.0f, 0.0f);
                }

                glm::vec3 direction = glm::angleAxis(angle, axis) * down;

                ::render::shader::lighting::getSunLight()->value.direction = direction;
                ::render::shader::lighting::getSunLight()->value.albedo = albedo;
                ::render::shader::lighting::getSunLight()->value.ambient = ambient;
                ::render::shader::lighting::getSunLight()->value.diffuse = diffuse;
                ::render::shader::lighting::getSunLight()->value.specular = specular;
                ::render::shader::lighting::getSunLight()->value.isOn = (isOn) ? 1 : 0;
                ::render::shader::lighting::getSunLight()->update();
            }

            void SunComponent::preRender() {

            }

            void SunComponent::render() {

            }

            void SunComponent::release() {
                this->entity = nullptr;
            }

            void SunComponent::load(Json::Value value) {
                this->albedo = glm::vec3(
                    value["albedo"]["x"].asFloat(),
                    value["albedo"]["y"].asFloat(),
                    value["albedo"]["z"].asFloat()
                );
                this->ambient = value["ambient"].asFloat();
                this->diffuse = value["diffuse"].asFloat();
                this->specular = value["specular"].asFloat();
                this->isOn = value["isOn"].asBool();
            }

        }
    }
}