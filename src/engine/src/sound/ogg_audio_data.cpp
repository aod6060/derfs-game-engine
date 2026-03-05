#include "../sys.hpp"

namespace sound {
    namespace codec {
        bool OggAudioData::init(std::string path) {
            return false;
        }

        void OggAudioData::release() {

        }

        long OggAudioData::read(std::vector<char>& buffer) {
            return 0;
        }

        int64_t OggAudioData::maxSize() {
            return 0;
        }

        int64_t OggAudioData::tell() {
            return 0;
        }

        void OggAudioData::seek(int64_t position) {

        }

        long OggAudioData::getFrequence() {
            return 0;
        }

        long OggAudioData::getChannelCount() {
            return 0;
        }

        long OggAudioData::getBitPerSample() {
            return 0;            
        }

    }
}