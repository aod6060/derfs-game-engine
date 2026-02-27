#include "LinearMath/btQuaternion.h"
#include "LinearMath/btScalar.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "glm/exponential.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/geometric.hpp"
#include <glm/gtx/matrix_decompose.hpp>

#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"
#include "../sys.hpp"
#include <functional>


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

    glm::vec3 Transform::getTransformedPosition() {
        if(this->entity->hasParent()) {
            glm::vec4 position = glm::vec4(this->position, 1.0f);
            glm::mat4 m = this->toParentTranslateMatrix(entity->parent);
            position = m * position;
            return glm::vec3(position.x, position.y, position.z);
        } else {
            return this->position;
        }
    }

    glm::vec3 Transform::getTransformedRotation() {
        if(this->entity->hasParent()) {
            glm::vec4 rotation = glm::vec4(glm::radians(this->rotation), 0.0f);
            glm::mat4 m = this->toParentRotationMatrix(entity->parent);
            rotation = m * rotation;
            return glm::vec3(rotation.x, rotation.y, rotation.y);
        } else {
            return glm::radians(this->rotation);
        }
    }

    void Transform::setTransformedPosition(glm::vec3 position) {
        if(this->entity->hasParent()) {
            glm::vec4 _pos = glm::vec4(position, 1.0f);
            glm::mat4 m = glm::inverse(this->toParentTranslateMatrix(entity->parent));
            _pos = m * _pos;
            this->position = glm::vec3(_pos.x, _pos.y, _pos.z);
        } else {
            this->position = position;
        }
    }

    void Transform::setTransformedRotation(glm::vec3 rotation) {
        if(this->entity->hasParent()) {
            glm::vec4 _rot = glm::vec4(rotation, 0.0f);
            glm::mat4 m = glm::inverse(this->toParentRotationMatrix(this->entity->parent));
            _rot = m * _rot;
            this->rotation = glm::degrees(glm::vec3(_rot.x, _rot.y, _rot.z));
        } else {
            this->rotation = glm::degrees(rotation);
        }
    }

    glm::mat4 Transform::toParentTranslateMatrix(Entity* entity) {
        if(entity == nullptr) {
            return glm::mat4(1.0f);
        }
        glm::mat4 m = glm::translate(glm::mat4(1.0f), entity->transform.position);
        if(entity->hasParent()) {
            m = this->toParentTranslateMatrix(entity->parent) * m;
        }
        return m;
    }

    glm::mat4 Transform::toParentRotationMatrix(Entity* entity) {
        if(entity == nullptr) {
            return glm::mat4(1.0f);
        }
        Transform::Axis axis = this->toAxis(glm::radians(entity->transform.rotation));
        glm::mat4 m = glm::rotate(glm::mat4(1.0f), axis.angle, axis.axis);
        if(entity->hasParent()) {
            m = this->toParentRotationMatrix(entity->parent) * m;
        }
        return m;
    }


    btVector3 Transform::toBulletVector3(glm::vec3 v) {
        return btVector3(v.x, v.y, v.z);
    }

    glm::vec3 Transform::toGLMVector3(const btVector3& v) {
        return glm::vec3(v.x(), v.y(), v.z());
    }
    
    Transform::Axis Transform::toAxis(glm::vec3 rotation) {
        Axis axis;
        axis.setup(rotation);
        return axis;
    }

    btTransform Transform::convertToBulletTransform() {
        btTransform temp;
        //temp.setOrigin(btVector3(this->position.x, this->position.y, this->position.z));
        temp.setOrigin(this->toBulletVector3(this->getTransformedPosition()));
        Axis a;
        a.setup(this->getTransformedRotation());
        btQuaternion q = btQuaternion(this->toBulletVector3(a.axis), a.angle);
        temp.setRotation(q);
        return temp;
    }

    void Transform::interpretBulletTransform(const btTransform& transform) {
        this->setTransformedPosition(this->toGLMVector3(transform.getOrigin()));
        Axis a;
        a.axis = this->toGLMVector3(transform.getRotation().getAxis());
        a.angle = transform.getRotation().getAngle();
        this->setTransformedRotation(a.convert());
    }

    glm::mat4 Transform::toModel() {
        glm::vec3 r = glm::radians(this->rotation);
        float angle = glm::length(r);
        glm::vec3 nr = glm::normalize(r);
        if(angle < 0.001f) {
            nr = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        return 
            glm::translate(glm::mat4(1.0f), this->position) *
            glm::rotate(glm::mat4(1.0f), angle, nr) * 
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

    void Transform::Axis::setup(glm::vec3 rotations) {
        glm::vec3 r = rotations;
        this->angle = glm::length(r);
        this->axis = glm::normalize(r);
        if(this->angle < 0.001f) {
            // These are arbutrary. 
            this->axis = glm::vec3(1.0f, 0.0f, 0.0f);
        }
    }

    glm::vec3 Transform::Axis::convert() {
        glm::vec3 r = this->axis * this->angle;
        return r;
    }

}