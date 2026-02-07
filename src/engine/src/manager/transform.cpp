#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/trigonometric.hpp"
#include "../sys.hpp"


namespace manager {
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