#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "sys.hpp"


namespace manager {
    namespace component {

        // CameraComponent
        void CameraComponent::init(Entity* entity) {
            this->entity = entity;
        }

        void CameraComponent::handleEvent(SDL_Event* e) {

        }

        void CameraComponent::update(float delta) {

        }

        glm::mat4 CameraComponent::toView() {
            return 
                glm::rotate(glm::mat4(1.0f), glm::radians(this->entity->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(this->entity->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(this->entity->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::translate(glm::mat4(1.0f), -this->entity->transform.position);
        }

        glm::mat4 CameraComponent::toViewWithParent() {
            return  toView() * glm::inverse(this->entity->toParentMatrix(this->entity->parent));
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
                render::getMainShader()->setModel(this->entity->toParentMatrix(this->entity->parent) * this->entity->transform.toModel());
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

    }
}