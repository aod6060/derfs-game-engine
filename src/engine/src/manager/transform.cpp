#include "LinearMath/btQuaternion.h"
#include "LinearMath/btScalar.h"
#include "LinearMath/btTransform.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/trigonometric.hpp"
#include "../sys.hpp"


namespace manager {

    void Transform::init(Entity* entity) {
        this->entity = entity;
    }

    void Transform::release() {
        this->entity = nullptr;
    }

    glm::mat4 Transform::toParentMatrix(Entity* entity) {
        if(entity == nullptr) {
            return glm::mat4(1.0f);
        }

        glm::mat4 m = entity->transform.toModel();
        if(entity->hasParent()) {
            m = this->toParentMatrix(entity->parent) * m;
        }
        return m;
    }

    glm::vec3 Transform::getGlobalPosition() {
        glm::vec4 position = glm::vec4(this->position, 1.0f);
        position = this->toParentMatrix(this->entity->parent) * position;
        return glm::vec3(position.x, position.y, position.z);
    }


    btTransform Transform::convertToBulletTransform() {
        btVector3 position = btVector3(this->position.x, this->position.y, this->position.z);

        btQuaternion rotation = btQuaternion(
            btRadians(this->rotation.z),
            btRadians(this->rotation.y),
            btRadians(this->rotation.x)
        );

        std::cout << rotation.x() << ", " << rotation.y() << ", " << rotation.z() << ", " << rotation.w() << "\n";

        return btTransform(rotation, position);
    }

    void Transform::interpretBulletTransform(const btTransform& transform) {
        this->position = glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
        float x, y, z;
        transform.getRotation().getEulerZYX(z, y, x);
        this->rotation.x = btDegrees(x);
        this->rotation.y = btDegrees(y);
        this->rotation.z = btDegrees(z);
    }


    glm::mat4 Transform::toModel() {
        return
            glm::translate(glm::mat4(1.0f), this->position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), this->scale);
    }

    void Transform::load(Json::Value v) {
        this->position = glm::vec3(
            v["position"]["x"].asFloat(),
            v["position"]["y"].asFloat(),
            v["position"]["z"].asFloat()
        );

        this->rotation = glm::vec3(
            v["rotation"]["x"].asFloat(),
            v["rotation"]["y"].asFloat(),
            v["rotation"]["z"].asFloat()
        );

        this->scale = glm::vec3(
            v["scale"]["x"].asFloat(),
            v["scale"]["y"].asFloat(),
            v["scale"]["z"].asFloat()
        );

    }

}