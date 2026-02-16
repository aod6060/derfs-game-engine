#include "LinearMath/btQuaternion.h"
#include "LinearMath/btScalar.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "glm/exponential.hpp"
#include "glm/ext/matrix_transform.hpp"
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

    /*
    // Arbutrary rotations in degress
    glm::vec3 rot = glm::vec3(32.0f, 24.0f, 128.0f);

    std::cout << "Original: " << rot.x << ", " << rot.y << ", " << rot.z << "\n";

    // Arbutrary rotation in radians
    glm::vec3 rrot = glm::radians(rot);

    std::cout << "Radians: " << rrot.x << ", " << rrot.y << ", " << rrot.z << "\n";

    float angle = glm::length(rrot);

    std::cout << "Angle: Radian > " << angle << " Degrees > " << glm::degrees(angle) << "\n";

    glm::vec3 nrrot = glm::normalize(rrot);

    std::cout << "Normalize Radians: " << nrrot.x << ", " << nrrot.y << ", " << nrrot.z << "\n";

    // Test: 32, 24, 128
    glm::vec3 test = glm::degrees(nrrot * angle);

    std::cout << "Test: " << test.x << ", " << test.y << ", " << test.z << "\n";
    */

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
        btTransform temp;
        temp.setOrigin(btVector3(this->position.x, this->position.y, this->position.z));

        glm::vec3 r = glm::radians(this->rotation);
        float angle = glm::length(r);
        glm::vec3 nr = glm::normalize(r);
        if(angle < 0.001f) {
            nr = glm::vec3(1.0f, 0.0f, 0.0f);
        }

        btQuaternion q = btQuaternion(btVector3(nr.x, nr.y, nr.z), angle);
        temp.setRotation(q);
        
        return temp;
    }

    void Transform::interpretBulletTransform(const btTransform& transform) {
        this->position = glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
        btVector3 axis = transform.getRotation().getAxis();
        float angle = transform.getRotation().getAngle();
        btVector3 btrot = axis * angle;
        this->rotation = glm::degrees(glm::vec3(btrot.x(), btrot.y(), btrot.z()));

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

}