#include "game.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"


int main(int argc, char** argv) {
    game::GameApp app;
    app::Config config;
    game::setup(&config, &app);
    app::init(&config);
    app::update();
    app::release();
    return 0;
    /*
    glm::vec3 test = glm::vec3(45.0f, 34.0f, 10.0f);
    std::cout << "test: " << test.x << ", " << test.y << ", " << test.z << "\n";

    glm::vec3 test2 = glm::vec3(24.0f, 48.0f, 80.0f);
    std::cout << "test2: " << test2.x << ", " << test2.y << ", " << test2.z << "\n";

    glm::vec3 test2_radian = glm::radians(test2);
    std::cout << "test2_radian: " << test2_radian.x << ", " << test2_radian.y << ", " << test2_radian.z << "\n";

    float test2_angle = glm::length(test2_radian);
    std::cout << "test2_angle: " << test2_angle << "\n";

    glm::vec3 test2_axis = glm::normalize(test2_radian);
    std::cout << "test2_axis: " << test2_axis.x << ", " << test2_axis.y << ", " << test2_axis.z << "\n";

    glm::mat4 test2_parent = glm::rotate(glm::mat4(1.0f), test2_angle, test2_axis);
    glm::mat4 test2_parent_invert = glm::inverse(test2_parent);

    glm::vec3 test_rad = glm::radians(test);
    std::cout << "test_rad: " << test_rad.x << ", " << test_rad.y << ", " << test_rad.z << "\n";

    glm::vec4 test_rad_transformed = test2_parent * glm::vec4(test_rad, 0.0f);
    std::cout << "test_rad_transformed: " << test_rad_transformed.x << ", " << test_rad_transformed.y << ", " << test_rad_transformed.z << "\n";

    glm::vec3 test_transform = glm::degrees(glm::vec3(test_rad_transformed.x, test_rad_transformed.y, test_rad_transformed.z));
    std::cout << "test_rad_transformed: " << test_transform.x << ", " << test_transform.y << ", " << test_transform.z << "\n";

    glm::vec4 true_test_rad = test2_parent_invert * test_rad_transformed;
    std::cout << "true_test_rad: " << true_test_rad.x << ", " << true_test_rad.y << ", " << true_test_rad.z << "\n";


    glm::vec3 true_test = glm::degrees(glm::vec3(true_test_rad.x, true_test_rad.y, true_test_rad.z));
    std::cout << "true_test: " << true_test.x << ", " << true_test.y << ", " << true_test.z << "\n";


    glm::vec3 test_tran3 = glm::vec3(test_rad_transformed.x, test_rad_transformed.y, test_rad_transformed.z);
    float test_tran3_angle = glm::length(test_tran3);
    glm::vec3 test_tran3_axis = glm::normalize(test_tran3);
    std::cout << "axis: " << test_tran3_axis.x << ", " << test_tran3_axis.y << ", " << test_tran3_axis.z << "; angle: " << test_tran3_angle << "\n";

    return 0;
    */
}