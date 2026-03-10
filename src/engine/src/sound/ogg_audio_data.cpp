#include "../sys.hpp"
#include "vorbis/vorbisfile.h"

namespace sound {
    namespace codec {
        bool OggAudioData::init(std::string path) {
            #if _MSC_VER
                errno_t err = fopen_s(&this->fp, path.c_str(), "rb");

                if(err == 0) {
                    std::cout << path << " was opened\n";
                } else {
                    std::cout << path << " wasn't opened\n";
                }
            #else
                this->fp = fopen(path.c_str(), "rb");
            #endif

            if(!this->fp) {
                std::cout << path << " doesn't exist\n";
                return true;
            }

            if(ov_open_callbacks(this->fp, &this->file, nullptr, 0, OV_CALLBACKS_NOCLOSE)) {
                std::cout << "Input doesn't not appear to be an Ogg Bitstream\n";
                return true;
            }

            this->info = ov_info(&this->file, -1);

            return false;
        }

        void OggAudioData::release() {
            this->info = nullptr;
            ov_clear(&this->file);
            fclose(this->fp);
        }

        long OggAudioData::read(std::vector<char>& buffer) {
            int bitstream = 0;
            return ov_read(&this->file, buffer.data(), buffer.size(), SDL_BYTEORDER == SDL_BIG_ENDIAN, int(sizeof(int16_t)), 1, &bitstream);
        }

        int64_t OggAudioData::maxSize() {
            return ov_raw_total(&this->file, -1);
        }

        int64_t OggAudioData::tell() {
            return ov_pcm_tell(&this->file);
        }

        void OggAudioData::seek(int64_t position) {
            ov_pcm_seek(&this->file, position);
        }

        long OggAudioData::getFrequence() {
            return this->info->rate;
        }

        long OggAudioData::getChannelCount() {
            return this->info->channels;
        }

        long OggAudioData::getBitPerSample() {
            return 16;            
        }

    }
}