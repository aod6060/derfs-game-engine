#include "sys.hpp"


namespace input {
    namespace mapping {
        void createKeyboardMapping(Mapping& mapping, Keyboard key) {
            mapping.isMouse = false;
            mapping.key = key;
        }

        void createMouseButtonMapping(Mapping& mapping, MouseButtons mb) {
            mapping.isMouse = true;
            mapping.mb = mb;
        }

        bool isMappingReleased(Mapping& k) {
            if(k.isMouse) {
                return input::isMouseButtonReleased(k.mb);
            } else {
                return input::isKeyReleased(k.key);
            }
        }

        bool isMappingPressedOnce(Mapping& k) {
            if(k.isMouse) {
                return input::isMouseButtonPressedOnce(k.mb);
            } else {
                return input::isKeyPressedOnce(k.key);
            }
        }

        bool isMappingPressed(Mapping& k) {
            if(k.isMouse) {
                return input::isMouseButtonPressed(k.mb);
            } else {
                return input::isKeyPressed(k.key);
            }
        }

        bool isMappingReleasedOnce(Mapping& k) {
            if(k.isMouse) {
                return input::isMouseButtonReleasedOnce(k.mb);
            } else {
                return input::isKeyReleasedOnce(k.key);
            }
        }

        float getMappingReleasedValue(Mapping& k) {
            return isMappingReleased(k) ? 1.0f : 0.0f;
        }

        float getMappingPressedOnceValue(Mapping& k) {
            return isMappingPressedOnce(k) ? 1.0f : 0.0f;
        }

        float getMappingPressedValue(Mapping& k) {
            return isMappingPressed(k) ? 1.0f : 0.0f;
        }

        float getMappingReleasedOnceValue(Mapping& k) {
            return isMappingReleasedOnce(k) ? 1.0f : 0.0f;
        }

        float getMappingReleasedAxis(Mapping& negative, Mapping& positive) {
            return getMappingReleasedValue(positive) - getMappingReleasedValue(negative);
        }

        float getMappingPressedOnceAxis(Mapping& negative, Mapping& positive) {
            return getMappingPressedOnceValue(positive) - getMappingPressedOnceValue(negative);
        }

        float getMappingPressedAxis(Mapping& negative, Mapping& positive) {
            return getMappingPressedValue(positive) - getMappingPressedValue(negative);
        }

        float getMappingReleasedOnceAxis(Mapping& negative, Mapping& positive) {
            return getMappingReleasedOnceValue(positive) - getMappingReleasedOnceValue(negative);
        }

    }
}