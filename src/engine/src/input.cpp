#include "SDL_events.h"
#include "SDL_mouse.h"
#include "glm/ext/scalar_common.hpp"
#include "sys.hpp"


namespace input {
    static std::vector<InputState> keys;
    static std::vector<InputState> mouseButtons;

    static glm::vec2 mousePosition;
    static glm::vec2 mouseVelocity;
    static glm::vec2 mouseWheel;

    void init() {
        keys.resize(Keyboard::KEYS_MAX_SIZE);
        mouseButtons.resize(MouseButtons::MBS_MAX_SIZE);

        // Default Keyboard
        std::for_each(keys.begin(), keys.end(), [](InputState& state) {
            state = InputState::IS_RELEASED;
        });
        // Default MouseButtons
        std::for_each(mouseButtons.begin(), mouseButtons.end(), [](InputState& state) {
            state = InputState::IS_RELEASED;
        });
        mousePosition = glm::vec2(0.0f);
        mouseVelocity = glm::vec2(0.0f);
        mouseWheel = glm::vec2(0.0f);
    }

    void release() {
        keys.clear();
        mouseButtons.clear();
    }

    void handleEvent(SDL_Event* e) {
        if(e->type == SDL_KEYDOWN) {
            if(keys[e->key.keysym.scancode] == InputState::IS_RELEASED) {
                keys[e->key.keysym.scancode] = InputState::IS_PRESSED_ONCE;
            }
        } else if(e->type == SDL_KEYUP) {
            if(keys[e->key.keysym.scancode] == InputState::IS_PRESSED) {
                keys[e->key.keysym.scancode] = InputState::IS_RELEASED_ONCE;
            }
        } else if(e->type == SDL_MOUSEBUTTONDOWN) {
            if(mouseButtons[e->button.button - 1] == InputState::IS_RELEASED) {
                mouseButtons[e->button.button - 1] = InputState::IS_PRESSED_ONCE;
            }
        } else if(e->type == SDL_MOUSEBUTTONUP) {
            if(mouseButtons[e->button.button - 1] == InputState::IS_PRESSED) {
                mouseButtons[e->button.button - 1] = InputState::IS_RELEASED_ONCE;
            }
        } else if(e->type == SDL_MOUSEMOTION) {
            mousePosition.x = e->motion.x;
            mousePosition.y = e->motion.y;
            mouseVelocity.x = e->motion.xrel;
            mouseVelocity.y = e->motion.yrel;

            // Clamp it between -1 to 1
            //mouseVelocity = glm::clamp(mouseVelocity, -1.0f, 1.0f);

        } else if(e->type == SDL_MOUSEWHEEL) {
            mouseWheel.x = e->wheel.x;
            mouseWheel.y = e->wheel.y;

            // Clamp it betwee -1 to 1
            mouseWheel = glm::clamp(mouseWheel, -1.0f, 1.0f);
        }
    }

    void update() {
        // Keyboard
        std::for_each(keys.begin(), keys.end(), [](InputState& state) {
            if(state == InputState::IS_PRESSED_ONCE) {
                state = InputState::IS_PRESSED;
            }

            if(state == InputState::IS_RELEASED_ONCE) {
                state = InputState::IS_RELEASED;
            }
        });
        // Mouse Buttons
        std::for_each(mouseButtons.begin(), mouseButtons.end(), [](InputState& state) {
            if(state == InputState::IS_PRESSED_ONCE) {
                state = InputState::IS_PRESSED;
            }

            if(state == InputState::IS_RELEASED_ONCE) {
                state = InputState::IS_RELEASED;
            }
        });

        mouseVelocity = glm::vec2(0.0f);

        mouseWheel = glm::vec2(0.0f);
    }

    // Keyboard
    bool isKeyReleased(Keyboard k) {
        return keys[k] == InputState::IS_RELEASED || keys[k] == InputState::IS_RELEASED_ONCE;
    }

    bool isKeyPressedOnce(Keyboard k) {
        return keys[k] == InputState::IS_PRESSED_ONCE;
    }

    bool isKeyPressed(Keyboard k) {
        return keys[k] == InputState::IS_PRESSED || keys[k] == InputState::IS_PRESSED_ONCE;
    }

    bool isKeyReleasedOnce(Keyboard k) {
        return keys[k] == InputState::IS_RELEASED_ONCE;
    }

    float getKeyReleasedValue(Keyboard k) {
        return isKeyReleased(k) ? 1.0f : 0.0f;
    }

    float getKeyPressedOnceValue(Keyboard k) {
        return isKeyPressedOnce(k) ? 1.0f : 0.0f;
    }

    float getKeyPressedValue(Keyboard k) {
        return isKeyPressed(k) ? 1.0f : 0.0f;
    }

    float getKeyReleasedOnceValue(Keyboard k) {
        return isKeyReleasedOnce(k) ? 1.0f : 0.0f;
    }

    float getKeyReleasedAxis(Keyboard negative, Keyboard positive) {
        return getKeyReleasedValue(positive) - getKeyReleasedValue(negative);
    }

    float getKeyPressedOnceAxis(Keyboard negative, Keyboard positive) {
        return getKeyPressedOnceValue(positive) - getKeyPressedOnceValue(negative);
    }

    float getKeyPressedAxis(Keyboard negative, Keyboard positive) {
        return getKeyPressedValue(positive) - getKeyPressedValue(negative);
    }

    float getKeyReleasedOnceAxis(Keyboard negative, Keyboard positive) {
        return getKeyReleasedOnceValue(positive) - getKeyReleasedOnceValue(negative);
    }


    // Mouse
    glm::vec2 toPosition() {
        return mousePosition;
    }

    glm::vec2 toVelocity() {
        return mouseVelocity;
    }

    glm::vec2 toMouseWheel() {
        return mouseWheel;
    }

    bool isMouseButtonReleased(MouseButtons mb) {
        return mouseButtons[mb] == InputState::IS_RELEASED || mouseButtons[mb] == InputState::IS_RELEASED_ONCE;
    }

    bool isMouseButtonPressedOnce(MouseButtons mb) {
        return mouseButtons[mb] == InputState::IS_PRESSED_ONCE;
    }

    bool isMouseButtonPressed(MouseButtons mb) {
        return mouseButtons[mb] == InputState::IS_PRESSED || mouseButtons[mb] == InputState::IS_PRESSED_ONCE;
    }

    bool isMouseButtonReleasedOnce(MouseButtons mb) {
        return mouseButtons[mb] == InputState::IS_RELEASED_ONCE;
    }

    float getMouseButtonReleasedValue(MouseButtons k) {
        return isMouseButtonReleased(k) ? 1.0f : 0.0f;
    }

    float getMouseButtonPressedOnceValue(MouseButtons k) {
        return isMouseButtonPressedOnce(k) ? 1.0f : 0.0f;
    }

    float getMouseButtonPressedValue(MouseButtons k) {
        return isMouseButtonPressed(k) ? 1.0f : 0.0f;
    }

    float getMouseButtonReleasedOnceValue(MouseButtons k) {
        return isMouseButtonReleasedOnce(k) ? 1.0f : 0.0f;
    }

    float getMouseButtonReleasedAxis(MouseButtons negative, MouseButtons positive) {
        return getMouseButtonReleasedValue(positive) - getMouseButtonReleasedValue(negative);
    }

    float getMouseButtonPressedOnceAxis(MouseButtons negative, MouseButtons positive) {
        return getMouseButtonPressedOnceValue(positive) - getMouseButtonPressedOnceValue(negative);
    }

    float getMouseButtonPressedAxis(MouseButtons negative, MouseButtons positive) {
        return getMouseButtonPressedValue(positive) - getMouseButtonPressedValue(negative);
    }

    float getMouseButtonReleasedOnceAxis(MouseButtons negative, MouseButtons positive) {
        return getMouseButtonReleasedOnceValue(positive) - getMouseButtonReleasedOnceValue(negative);
    }

    // Input Grab
    bool isGrab() {
        return SDL_GetRelativeMouseMode() ? true : false;
    }

    void setGrab(bool grab) {
        SDL_SetRelativeMouseMode((grab) ? SDL_TRUE : SDL_FALSE);
    }

    void toggleGrab() {
        setGrab(!isGrab());
    }

}
