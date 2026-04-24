#include "sound_hidden.hpp"


namespace sound {
    namespace alw {
        // Source
        void Source::init() {
            alGenSources(1, &this->id);
        }

        void Source::release() {
            alDeleteSources(1, &this->id);
        }


        void Source::setPosition(glm::vec3 position) {
            alSource3f(this->id, AL_POSITION, position.x, position.y, position.z);
        }

        glm::vec3 Source::getPosition() {
            float x, y, z;
            alGetSource3f(this->id, AL_POSITION, &x, &y, &z);
            return glm::vec3(x, y, z);
        }


        void Source::setVelocity(glm::vec3 velocity) {
            alSource3f(this->id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        }

        glm::vec3 Source::getVelocity() {
            float x, y, z;
            alGetSource3f(this->id, AL_VELOCITY, &x, &y, &z);
            return glm::vec3(x, y, z);
        }


        void Source::setVolume(float volume) {
            alSourcef(this->id, AL_GAIN, volume);
        }

        float Source::getVolume() {
            float x;
            alGetSourcef(this->id, AL_GAIN, &x);
            return x;
        }


        void Source::setRelative(bool value) {
            alSourcei(this->id, AL_SOURCE_RELATIVE, (value) ? AL_TRUE : AL_FALSE);
        }

        bool Source::isRelative() {
            int value;
            alGetSourcei(this->id, AL_SOURCE_RELATIVE, &value);
            return (value) ? true : false;
        }


        void Source::setType(ALint type) {
            alSourcei(this->id, AL_SOURCE_TYPE, type);
        }

        ALint Source::getType() {
            ALint type;
            alGetSourcei(this->id, AL_SOURCE_TYPE, &type);
            return type;
        }

        void Source::setLooping(bool value) {
            alSourcei(this->id, AL_LOOPING, (value) ? AL_TRUE : AL_FALSE);
        }

        bool Source::isLooping() {
            ALint looping;
            alGetSourcei(this->id, AL_LOOPING, &looping);
            return looping ? true : false;
        }


        void Source::setBuffer(Buffer* buffer) {
            alSourcei(this->id, AL_BUFFER, buffer->id);
        }


        int Source::getBufferedQueued() {
            int bufferQueued = 0;
            alGetSourcei(this->id, AL_BUFFERS_QUEUED, &bufferQueued);
            return bufferQueued;
        }


        int Source::getBufferedProcessed() {
            int bufferedProcessed = 0;
            alGetSourcei(this->id, AL_BUFFERS_PROCESSED, &bufferedProcessed);
            return bufferedProcessed;
        }


        void Source::setMinVolume(float value) {
            alSourcef(this->id, AL_MIN_GAIN, value);
        }

        float Source::getMinVolume() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_MIN_GAIN, &value);
            return value;
        }


        void Source::setMaxVolume(float value) {
            alSourcef(this->id, AL_MAX_GAIN, value);
        }

        float Source::getMaxVolume() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_MAX_GAIN, &value);
            return value;
        }


        void Source::setReferenceDistance(float value) {
            alSourcef(this->id, AL_REFERENCE_DISTANCE, value);
        }

        float Source::getReferenceDistance() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_REFERENCE_DISTANCE, &value);
            return value;
        }


        void Source::setRolloffFactor(float value) {
            alSourcef(this->id, AL_ROLLOFF_FACTOR, value);
        }

        float Source::getRolloffFactor() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_ROLLOFF_FACTOR, &value);
            return value;
        }


        void Source::setMaxDistance(float value) {
            alSourcef(this->id, AL_MAX_DISTANCE, value);
        }

        float Source::getMaxDistance() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_MAX_DISTANCE, &value);
            return value;
        }


        void Source::setPitch(float value) {
            alSourcef(this->id, AL_PITCH, value);
        }

        float Source::getPitch() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_PITCH, &value);
            return value;
        }


        void Source::setDirection(const glm::vec3& direction) {
            alSource3f(this->id, AL_DIRECTION, direction.x, direction.y, direction.z);
        }

        glm::vec3 Source::getDirection() {
            float x, y, z;
            alGetSource3f(this->id, AL_DIRECTION, &x, &y, &z);
            return glm::vec3(x, y, z);
        }

        // Value Between 0.0f, 360.0
        void Source::setConeInnerAngle(float angle) {
            alSourcef(this->id, AL_CONE_INNER_ANGLE, angle);
        }

        float Source::getConeInnerAngle() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_CONE_INNER_ANGLE, &value);
            return value;
        }


        void Source::setConeOuterAngle(float angle) {
            alSourcef(this->id, AL_CONE_OUTER_ANGLE, angle);
        }

        float Source::getConeOuterAngle() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_CONE_OUTER_ANGLE, &value);
            return value;
        }


        void Source::setConeOuterVolume(float volume) {
            alSourcef(this->id, AL_CONE_OUTER_GAIN, volume);
        }

        float Source::getConeOuterVolume() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_CONE_OUTER_GAIN, &value);
            return value;
        }


        void Source::setSecOffset(float sec) {
            alSourcef(this->id, AL_SEC_OFFSET, sec);
        }

        float Source::getSecOffset() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_SEC_OFFSET, &value);
            return value;
        }


        void Source::setSampleOffset(float sample) {
            alSourcef(this->id, AL_SAMPLE_OFFSET, sample);
        }

        float Source::getSampleOffset() {
            float value = 0.0f;
            alGetSourcef(this->id, AL_SAMPLE_OFFSET, &value);
            return value;
        }


        void Source::setByteOffset(ALint offset) {
            alSourcei(this->id, AL_BYTE_OFFSET, offset);
        }

        ALint Source::getByteOffset() {
            ALint offset = 0;
            alGetSourcei(this->id, AL_BYTE_OFFSET, &offset);
            return offset;
        }

        void Source::sourceQueueBuffers(std::vector<ALuint>& buffers) {
            alSourceQueueBuffers(this->id, buffers.size(), buffers.data());
        }

        void Source::sourceUnqueueBuffers(std::vector<ALuint>& buffers) {
            alSourceUnqueueBuffers(this->id, buffers.size(), buffers.data());
        }


        ALenum Source::getState() {
            int state = 0;
            alGetSourcei(this->id, AL_SOURCE_STATE, &state);
            return state;
        }


        void Source::play() {
            alSourcePlay(this->id);
        }

        void Source::pause() {
            alSourcePause(this->id);
        }

        void Source::stop() {
            alSourceStop(this->id);
        }

        void Source::rewind() {
            alSourceRewind(this->id);
        }


        // Buffer
        void Buffer::init() {
            alGenBuffers(1, &this->id);
        }

        void Buffer::release() {
            alDeleteBuffers(1, &this->id);
        }

        void Buffer::setFrequency(int freq) {
            alBufferi(this->id, AL_FREQUENCY, freq);
        }

        int Buffer::getFrequency() {
            int value;
            alGetBufferi(this->id, AL_FREQUENCY, &value);
            return value;
        }

        void Buffer::setSize(int size) {
            alBufferi(this->id, AL_SIZE, size);
        }

        int Buffer::getSize() {
            int value;
            alGetBufferi(this->id, AL_SIZE, &value);
            return value;
        }

        void Buffer::setBits(int bits) {
            alBufferi(this->id, AL_BITS, bits);
        }

        int Buffer::getBits() {
            int value;
            alGetBufferi(this->id, AL_BITS, &value);
            return value;
        }

        void Buffer::setChannels(int channel) {
            alBufferi(this->id, AL_CHANNELS, channel);
        }

        int Buffer::getChannels() {
            int value;
            alGetBufferi(this->id, AL_CHANNELS, &value);
            return value;
        }

        void Buffer::bufferData(ALenum format, void* data, size_t size, ALsizei frequency) {
            alBufferData(this->id, format, data, size, frequency);
        }

    }
}