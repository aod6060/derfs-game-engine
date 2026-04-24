#include "sound_hidden.hpp"

namespace sound {
    namespace codec {
        bool WavAudioData::init(std::string path) {
            // Open File Stream
            #if _MSC_VER
                errno_t err = fopen_s(&this->stream, path.c_str(), "rb");

                if(err == 0) {
                    std::cout << path << " was opened\n";
                } else {
                    std::cout << path << " wasn't opened\n";
                }
            #else
                this->stream = fopen(path.c_str(), "rb");

                if(this->stream == nullptr) {
                    std::cout << path << " wasn't opened\n";
                } else {
                    std::cout << path << " was opened\n";
                }
            #endif

            // Load WavFileHeader
            fread(&this->waveFile, sizeof(WAVFile), 1, this->stream);

            this->start = sizeof(WAVFile);
            this->end = this->waveFile.sampleData.dataSize;

            return false;
        }

        void WavAudioData::release() {
            fclose(this->stream);
        }

        long WavAudioData::read(std::vector<char>& buffer) {
            int len = 0;
            int offset = this->end - this->tell();

            if(offset >= buffer.size()) {
                len = buffer.size();
            } else {
                len = offset;
            }

            int status = fread(buffer.data(), len, 1, this->stream);

            if(status > 0) {
                return len;
            } else {
                return status;
            }
        }

        int64_t WavAudioData::maxSize() {
            return this->end;
        }

        int64_t WavAudioData::tell() {
            return ftell(this->stream) - this->start;
        }

        void WavAudioData::seek(int64_t position) {
            if(position == sound::AudioDataSeek::ADS_BEGIN) {
                fseek(stream, this->start, 0);
            } else if(position == sound::AudioDataSeek::ADS_END) {
                fseek(stream, start, end);
            } else {
                fseek(stream, start, position);
            }
        }

        long WavAudioData::getFrequence() {
            return waveFile.chunkDescription.frequency;
        }

        long WavAudioData::getChannelCount() {
            return waveFile.chunkDescription.channels;
        }

        long WavAudioData::getBitPerSample() {
            return waveFile.chunkDescription.bitsPerSample;
        }

    }
}