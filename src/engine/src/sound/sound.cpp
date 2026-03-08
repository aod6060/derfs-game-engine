#include "../sys.hpp"
#include "AL/al.h"
#include "AL/alc.h"
#include <filesystem>
#include <functional>


namespace sound {
    std::map<std::string, std::function<IAudioData*()>> audioDataFactory = {
        {
            ".wav",
            []() {
                return new codec::WavAudioData();
            }
        },
        {
            ".ogg",
            []() {
                return new codec::OggAudioData();
            }
        }
    };

    static ALCdevice* device = nullptr;
    static ALCcontext* context = nullptr;

    static bool isInited = false;
    static float masterVolume;
    static std::map<std::string, float> volumeGroups;

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

        isInited = true;
        setMasterVolume(masterVolume);
    }

    void release() {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }

    IAudioData* initAudioData(std::string path) {
        std::filesystem::path p(path);
        std::string ext = p.extension().string();

        if(audioDataFactory.find(ext) != audioDataFactory.end()) {
            IAudioData* audioData = audioDataFactory.at(ext)();
            audioData->init(path);
            return audioData;
        } else {
            std::cout << path << " isn't a supported format. The engine only uses .wav and .ogg files.\n";
            return nullptr;
        }
    }

    void setMasterVolume(float volume) {
        masterVolume = volume;
        if(isInited) {
            alListenerf(AL_GAIN, volume);
        }
    }

    float getMasterVolume() {
        float volume;
        alGetListenerf(AL_GAIN, &volume);
        return volume;
    }

    void addVolumeGroup(std::string name, float value) {
        volumeGroups[name] = value;
    }

    void setGroupVolume(std::string name, float value) {
        volumeGroups.at(name) = value;
    }

    float getGroupVolume(std::string name) {
        return volumeGroups.at(name);
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