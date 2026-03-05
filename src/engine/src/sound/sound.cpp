#include "../sys.hpp"
#include "AL/al.h"
#include "AL/alc.h"


namespace sound {
    static ALCdevice* device = nullptr;
    static ALCcontext* context = nullptr;
    
    void init() {
        ALCdevice* device = alcOpenDevice(nullptr);

        if(!device) {
            std::cout << "Device wasn't created.\n";
            return;
        }
        
        ALCcontext* context = alcCreateContext(device, nullptr);

        alcMakeContextCurrent(context);

        if(!context) {
            std::cout << "Context wasn't created.\n";
            return;
        }
    }

    void release() {
        alcMakeContextCurrent(nullptr);

        alcDestroyContext(context);
        alcCloseDevice(device);
    }

    IAudioData* loadAudioData(std::string path) {
        return nullptr;
    }

    void setListenerPosition(const glm::vec3& pos) {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    void setListenerVelocity(const glm::vec3& velocity) {
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }

    void setListenerOrientation(const glm::mat4& transform) {
        glm::vec4 forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

        forward = transform * forward;
        up = transform * up;

        float ori[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};

        alListenerfv(AL_ORIENTATION, ori);
    }
    
    void transformListener(manager::Transform& tran) {
        setListenerPosition(tran.getTransformedPosition());
        setListenerOrientation(tran.toGlobalRotaionMatrix());
    }

}