#include "glm/ext/vector_float2.hpp"
#include "glm/trigonometric.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <SDL.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

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

void loadWaveHeader(std::string path, WAVFile* file, std::vector<char>& data);


struct OggAudioData {
    FILE* fp = nullptr;
    OggVorbis_File file;
    vorbis_info* info;

    bool init(std::string path);
    long read(std::vector<char>& buffer, int* bitstream);
    int64_t maxSize();
    int64_t tell();
    void seek(int64_t position);
    void release();

    long getFrequence();
    long getChannelCount();
    long getBitPerSample();
};

int main(int argc, char** argv) {
    ALCdevice* device = alcOpenDevice(nullptr);

    if(!device) {
        std::cout << "Device wasn't created.\n";
        return -1;
    }
    
    ALCcontext* context = alcCreateContext(device, nullptr);

    alcMakeContextCurrent(context);

    if(!context) {
        std::cout << "Context wasn't created.\n";
        return -1;
    }

    // They don't have the OpenGL id issue :D
    //id_test();

    WAVFile file;
    std::vector<char> d;

    loadWaveHeader("data/sound/fx/explosion.wav", &file, d);

    std::cout << "Header\n";
    std::cout << "type: " << ((WAVString*)(&file.header.type))->toString() << "\n";
    std::cout << "file-size: " << file.header.fileSize << "\n";
    std::cout << "format: " << ((WAVString*)(&file.header.format))->toString() << "\n";
    std::cout << "\n";
    std::cout << "ChunkDescripter\n";
    std::cout << "format-block-id: " << ((WAVString*)(&file.chunkDescription))->toString() << "\n";
    std::cout << "block-size: " << file.chunkDescription.blockSize << "\n";
    std::cout << "audio-format: " << file.chunkDescription.audioFormat << "\n";
    std::cout << "channels: " << file.chunkDescription.channels << "\n";
    std::cout << "freq: " << file.chunkDescription.frequency << "\n";
    std::cout << "bytePerSec: " << file.chunkDescription.bytePerSecond << "\n";
    std::cout << "bytePerBloc: " << file.chunkDescription.bytePerBlock << "\n";
    std::cout << "bitsPerSample: " << file.chunkDescription.bitsPerSample << "\n";
    std::cout << "\n";
    std::cout << "SampledData\n";
    std::cout << "data-block_id: " << ((WAVString*)(&file.sampleData.dataBlockID))->toString() << "\n";
    std::cout << "data-size: " << file.sampleData.dataSize << "\n";

    OggAudioData ogg;
    ogg.init("data/sound/fx/explosion.ogg");

    std::cout << "frequency: " << ogg.getFrequence() << "\n";
    std::cout << "channel: " << ogg.getChannelCount() << "\n";
    std::cout << ogg.info->bitrate_lower << "\n";
    std::cout << ogg.info->bitrate_upper << "\n";
    std::cout << ogg.info->bitrate_nominal << "\n";
    std::cout << ogg.info->bitrate_window << "\n";
    
    ALuint bufferID = 0;
    alGenBuffers(1, &bufferID);
    std::cout << "BufferID: " << bufferID << "\n";
    ALenum format;
    

    if(ogg.getChannelCount() == 1 && ogg.getBitPerSample() == 8) {
        format = AL_FORMAT_MONO8;
    } else if(ogg.getChannelCount() == 1 && ogg.getBitPerSample() == 16) {
        format = AL_FORMAT_MONO16;
    } else if(ogg.getChannelCount() == 2 && ogg.getBitPerSample() == 8) {
        format = AL_FORMAT_STEREO8;
    } else if(ogg.getChannelCount() == 2 && ogg.getBitPerSample() == 16) {
        format = AL_FORMAT_STEREO16;
    }
    
    // ogg are ment for streaming.
    std::vector<char> data;
    data.resize(ogg.maxSize());
    int bitstream = 0;

    int ret = ogg.read(data, &bitstream);

    std::cout << "ret: " << ret << "\n";

    std::cout << bitstream << "\n";
    std::cout << ogg.maxSize() << "\n";

    alBufferData(bufferID, format, data.data(), ret, ogg.getFrequence());

    ALuint sourceID = 0;
    alGenSources(1, &sourceID);
    std::cout << "sourceID" << sourceID << "\n";
    alSourcef(sourceID, AL_PITCH, 1.0f);
    alSourcef(sourceID, AL_GAIN, 1.0f);
    alSource3f(sourceID, AL_VELOCITY, 0, 0, 0);
    alSourcei(sourceID, AL_LOOPING, AL_FALSE);
    alSourcei(sourceID, AL_BUFFER, bufferID);

    alSourcePlay(sourceID);

    ALint state = AL_PLAYING;

    float rotate = 0.0f;

    while(state == AL_PLAYING) {
        rotate += (1.0 / 1024.0f);
        float rad = glm::radians(rotate);

        glm::vec2 circle = glm::vec2(glm::cos(rad), glm::sin(rad)) * 5.0f;

        alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
        alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    }

    alDeleteSources(1, &sourceID);
    alDeleteBuffers(1, &bufferID);

    ogg.release();

    alcMakeContextCurrent(nullptr);

    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}

void loadWaveHeader(std::string path, WAVFile* file, std::vector<char>& data) {
    std::ifstream in(path, std::ios::binary);

    if(!in.is_open()) {
        std::cout << path << " doesn't exit\n";
        return;
    }

    in.read((char*)file, sizeof(WAVFile));

    data.resize(file->sampleData.dataSize);

    in.read(data.data(), data.size());

    in.close();
}


bool OggAudioData::init(std::string path) {
    this->fp = fopen(path.c_str(), "rb");

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

long OggAudioData::read(std::vector<char>& buffer, int* bitstream) {
    std::cout << "buffer-size: " << buffer.size() << "\n";
    return ov_read(&this->file, buffer.data(), buffer.size(), SDL_BYTEORDER == SDL_BIG_ENDIAN, int(sizeof(int16_t)), 1, bitstream);
}

int64_t OggAudioData::tell() {
    return ov_pcm_tell(&this->file);
}

void OggAudioData::seek(int64_t position) {
    ov_pcm_seek(&this->file, position);
}

int64_t OggAudioData::maxSize() {
    return ov_pcm_total(&this->file, -1);
}

void OggAudioData::release() {
    this->info = nullptr;
    ov_clear(&this->file);
    fclose(this->fp);
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