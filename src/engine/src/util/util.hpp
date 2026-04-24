#ifndef UTIL_HPP
#define UTIL_HPP


#include <iostream>


namespace util {
    void init();
    void release();

    namespace random {
        void init();
        void release();
        bool randboolean();
        int32_t randrange(int32_t min, int32_t max);
        float randf(); // 0.0f -> 1.0f
    }
}

#endif