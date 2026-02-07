#include "sys.hpp"


namespace util {
    void init() {
        random::init();
    }

    void release() {
        random::release();
    }

}