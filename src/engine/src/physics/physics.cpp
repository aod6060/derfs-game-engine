#include "../sys.hpp"

namespace physics {

    static uint32_t maxBodies = 0;
    static uint32_t numBodyMutexes = 0; // Just using default for now
    static uint32_t maxBodyPairs = 0;
    static uint32_t maxContactConstraints = 0;

    void init() {
        std::cout << "MaxBodies: " << getMaxBodies() << "\n";
        std::cout << "NumBodyMutexes: " << numBodyMutexes << "\n";
        std::cout << "MaxBodyPairs: " << getMaxBodyPairs() << "\n";
        std::cout << "MaxContactConstraints: " << getMaxContactConstraints() << "\n";
    }

    void release() {

    }

    uint32_t getMaxBodies() {
        return maxBodies;
    }

    void setMaxBodies(uint32_t num) {
        maxBodies = num;
    }

    uint32_t getMaxBodyPairs() {
        return maxBodyPairs;
    }

    void setMaxBodyPairs(uint32_t num) {
        maxBodyPairs = num;
    }

    uint32_t getMaxContactConstraints() {
        return maxContactConstraints;
    }

    void setMaxContactContraints(uint32_t num) {
        maxContactConstraints = num;
    }

}