#ifndef SOUND_HPP
#define SOUND_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <glm/glm.hpp>
#include <vorbis/vorbisfile.h>
#include <AL/al.h>
#include <AL/alc.h>

namespace manager {
    struct Transform;
}

namespace sound {

    enum AudioDataMetaData {
        ADMD_BUFFER_COUNT = 2,
        ADMD_CHUNK_SIZE = 4096,
        ADMD_BUFFER_SIZE = 4096 * 32
    };

    enum AudioDataSeek {
        ADS_BEGIN = 0,
        ADS_END,
        ADS_MAX_SIZE
    };

    struct IAudioData {
        virtual bool init(std::string path) = 0;
        virtual void release() = 0;
        virtual long read(std::vector<char>& buffer) = 0;
        virtual int64_t maxSize() = 0;
        virtual int64_t tell() = 0;
        virtual void seek(int64_t position) = 0;
        virtual long getFrequence() = 0;
        virtual long getChannelCount() = 0;
        virtual long getBitPerSample() = 0;

        virtual ~IAudioData() {}
    };

    void init();
    void release();

    IAudioData* initAudioData(std::string path);

    // Volume Section
    void setMasterVolume(float volume);
    float getMasterVolume();

    void addVolumeGroup(std::string name, float value);
    void setGroupVolume(std::string name, float value);
    float getGroupVolume(std::string name);

    void setListenerPosition(const glm::vec3& position);
    void setListenerVelocity(const glm::vec3& velocity);
    void setListenerOrientation(const glm::mat4& rotationMatrix);

    void transformListener(manager::Transform& tran);

    namespace codec {

        struct WavAudioData : public IAudioData {
            struct WAVHeader {
                uint32_t type;
                uint32_t fileSize;
                uint32_t format;
            };

            struct WAVChunkDescription {
                uint32_t formatBlockID;
                uint32_t blockSize;
                uint16_t audioFormat;
                uint16_t channels;
                uint32_t frequency;
                uint32_t bytePerSecond;
                uint16_t bytePerBlock;
                uint16_t bitsPerSample;
            };

            struct WAVSampledData {
                uint32_t dataBlockID;
                uint32_t dataSize;
            };

            struct WAVString {
                unsigned char v1;
                unsigned char v2;
                unsigned char v3;
                unsigned char v4;

                std::string toString() {
                    std::stringstream ss;
                    ss << v1 << v2 << v3 << v4;
                    return ss.str();
                }
            };

            struct WAVFile {
                WAVHeader header;
                WAVChunkDescription chunkDescription;
                WAVSampledData sampleData;
            };


            WAVFile waveFile;

            size_t start = 0;
            size_t end = 0;

            //std::ifstream stream;
            FILE* stream;

            virtual bool init(std::string path);
            virtual void release();
            virtual long read(std::vector<char>& buffer);
            virtual int64_t maxSize();
            virtual int64_t tell();
            virtual void seek(int64_t position);
            virtual long getFrequence();
            virtual long getChannelCount();
            virtual long getBitPerSample();
        };


        struct OggAudioData : public IAudioData {
            FILE* fp = nullptr;
            OggVorbis_File file;
            vorbis_info* info;

            virtual bool init(std::string path);
            virtual void release();
            virtual long read(std::vector<char>& buffer);
            virtual int64_t maxSize();
            virtual int64_t tell();
            virtual void seek(int64_t position);
            virtual long getFrequence();
            virtual long getChannelCount();
            virtual long getBitPerSample();
        };


    }

    namespace alw {
        struct Buffer;

        struct Source {
            ALuint id = 0;

            void init();
            void release();

            void setPosition(glm::vec3 position);
            glm::vec3 getPosition();

            void setVelocity(glm::vec3 velocity);
            glm::vec3 getVelocity();

            void setVolume(float volume);
            float getVolume();

            void setRelative(bool value);
            bool isRelative();

            void setType(ALint type);
            ALint getType();

            void setLooping(bool value);
            bool isLooping();

            void setBuffer(Buffer* buffer);

            int getBufferedQueued();

            int getBufferedProcessed();

            void setMinVolume(float value);
            float getMinVolume();

            void setMaxVolume(float value);
            float getMaxVolume();

            void setReferenceDistance(float value);
            float getReferenceDistance();

            void setRolloffFactor(float value);
            float getRolloffFactor();

            void setMaxDistance(float value);
            float getMaxDistance();

            void setPitch(float value);
            float getPitch();

            void setDirection(const glm::vec3& direction);
            glm::vec3 getDirection();

            void setConeInnerAngle(float angle); // Value Between 0.0f, 360.0f
            float getConeInnerAngle();

            void setConeOuterAngle(float angle);
            float getConeOuterAngle();

            void setConeOuterVolume(float volume);
            float getConeOuterVolume();

            void setSecOffset(float sec);
            float getSecOffset();

            void setSampleOffset(float sample);
            float getSampleOffset();

            void setByteOffset(ALint offset);
            ALint getByteOffset();

            void sourceQueueBuffers(std::vector<ALuint>& buffers);
            void sourceUnqueueBuffers(std::vector<ALuint>& buffers);

            ALenum getState();

            void play();
            void pause();
            void stop();
            void rewind();
        };

        struct Buffer {
            ALuint id = 0;

            void init();
            void release();

            void setFrequency(int freq);
            int getFrequency();

            void setSize(int size);
            int getSize();

            void setBits(int bits); // Either 8 or 16
            int getBits(); // Either 8 or 16

            void setChannels(int channel); // 1 mono or 2 sterio
            int getChannels(); // 1 mono or 2 sterio

            void bufferData(ALenum format, void* data, size_t size, ALsizei frequency);

        };
    }
}

#endif