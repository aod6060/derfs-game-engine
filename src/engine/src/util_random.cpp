#include "sys.hpp"
#include <chrono>
#include <climits>
#include <random>

namespace util {

    namespace random {

        std::mt19937 mrand;

        void init() {
            mrand = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());
        }

        void release() {

        }

        bool randboolean() {
            return mrand() % 2; // Returns True or False
        }
    
        int32_t randrange(int32_t min, int32_t max) {
            return mrand() % (max - min) + min;
        }

        float randf() {
            return (mrand() % INT_MAX) / (float)INT_MAX;
        }

         // 0.0f -> 1.0f
    }
}