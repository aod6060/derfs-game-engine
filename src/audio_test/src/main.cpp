#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <deque>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>


#define BUFFER_COUNT 2
#define BUFFER_SIZE 4096 * 32
#define CHUNK_SIZE 4096

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

struct OggStreamPlayer {
    OggAudioData* data;
    //ALuint buffer;
    std::vector<ALuint> buffers;
    ALuint source;
    bool eof = false;
    bool eoq = false;

    ALint state = AL_STOPPED;

    enum ChunkType {
        CT_START = 0,
        CT_DATA,
        CT_END,
        CT_MAX_SIZE
    };

    struct Chunk {
        ChunkType type;
        int len;
        std::vector<char> data;
    };

    struct Buffer {
        std::vector<char> data;
        int len;
    };

    std::deque<Chunk> chunks;
    std::vector<char> buffer;

    int length = 0;
    
    bool start = true;
    int processingPointer = 0;
    int playingPointer = 0;
    int processingIncrementor = 0;
    int playingIncrementor = 0;

    bool playlast = false;
    bool playing = true;

    void printQueue();

    void init(OggAudioData* data);

    void release();

    void update();

    bool isPlaying();

    void stream_from_ogg();
    void process_current_buffer();
    void play_current_buffer();
    void check_current_buffer_state();
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

    OggAudioData data;
    data.init("data/sound/fx/fart-with-reverb.ogg");

    OggStreamPlayer player;
    player.init(&data);

    /*
    while(player.isPlaying()) {
        player.update();
    }
    */
    
    player.release();

    data.release();
    
    std::cout << "Here 1\n";  
    alcMakeContextCurrent(nullptr);
    std::cout << "Here 2\n";
    alcDestroyContext(context);
    std::cout << "Here 3\n";
    alcCloseDevice(device);
    std::cout << "Here 1\n";
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
    //this->fp = fopen(path.c_str(), "rb");

    
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

long OggAudioData::read(std::vector<char>& buffer, int* bitstream) {
    int len = ov_read(&this->file, buffer.data(), buffer.size(), SDL_BYTEORDER == SDL_BIG_ENDIAN, int(sizeof(int16_t)), 1, bitstream);
    if(len == 0) {
        std::cout << buffer.data() << "\n";
    }
    return len;
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

// OggStreamPlayer
void OggStreamPlayer::init(OggAudioData* data) {
    this->data = data;
    buffers.resize(BUFFER_COUNT);
    alGenBuffers(buffers.size(), buffers.data());
    // Setup OpenAL Source
    alGenSources(1, &source);

    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, 0.5f);
    alSourcei(source, AL_LOOPING, AL_FALSE);

    Chunk startChunk;
    startChunk.type = ChunkType::CT_START;
    this->chunks.push_back(startChunk);

    buffer.resize(BUFFER_SIZE);
}

void OggStreamPlayer::release() {
    //buffer.clear();
    alDeleteSources(1, &this->source);
    alDeleteBuffers(buffers.size(), buffers.data());
    this->data = nullptr;
    this->data = nullptr;
}

void OggStreamPlayer::stream_from_ogg() {
    if(!eof) {
        int bitstream;
        Chunk chunk;
        chunk.type = ChunkType::CT_DATA;
        chunk.data.resize(CHUNK_SIZE);
        chunk.len = this->data->read(chunk.data, &bitstream);

        //std::cout << chunk.len << "\n";

        if(chunk.len == 0) {
            eof = true;
            chunk.data.clear();
            Chunk endChunk;
            endChunk.type = ChunkType::CT_END;
            this->chunks.push_back(endChunk);
            //this->chunks.emplace(endChunk);
        } else if(chunk.len < 0) {
            chunk.data.clear();
            std::cout << "There was an error\n";
        } else {
            //this->chunks.emplace(chunk);
            this->chunks.push_back(chunk);
        }
    }
}

void OggStreamPlayer::process_current_buffer() {
    if(this->chunks.front().type != CT_END) {
        while(!this->chunks.empty()) {
            if(this->chunks.front().type == CT_START) {
                this->chunks.pop_front();
                continue;
            } else if(this->chunks.front().type == CT_END) {
                break;
            } else {
                if(this->chunks.front().len + length < BUFFER_SIZE) {
                    memcpy(buffer.data() + length, this->chunks.front().data.data(), this->chunks.front().len);
                    length += this->chunks.front().len;
                    this->chunks.front().data.clear();
                    this->chunks.pop_front();
                } else {

                    if(start || processingPointer != playingPointer) {
                        int format = 0;

                        if(data->getChannelCount() == 1 && data->getBitPerSample() == 16) {
                            format = AL_FORMAT_MONO16;
                        } else if(data->getChannelCount() == 2 && data->getBitPerSample() == 16) {
                            format = AL_FORMAT_STEREO16;
                        }

                        alBufferData(buffers[processingPointer], format, buffer.data(), length, data->getFrequence());
                        
                        length = 0;
                        processingPointer += 1;
                        processingPointer %= BUFFER_COUNT;
                        processingIncrementor += 1;

                        if(start) {
                            start = false;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    } else {
        if(!eoq) {
            if(start || processingPointer != playingPointer) {
                eoq = true;
                int format = 0;

                if(data->getChannelCount() == 1 && data->getBitPerSample() == 16) {
                    format = AL_FORMAT_MONO16;
                } else if(data->getChannelCount() == 2 && data->getBitPerSample() == 16) {
                    format = AL_FORMAT_STEREO16;
                }

                alBufferData(buffers[processingPointer], format, buffer.data(), length, data->getFrequence());

                length = 0;
                processingPointer += 1;
                processingPointer %= BUFFER_COUNT;
                processingIncrementor += 1;

                if(start) {
                    start = false;
                }
            }
        }
    }
}

void OggStreamPlayer::play_current_buffer() {
    if(state != AL_PLAYING){
        alSourceQueueBuffers(source, 1, &buffers[playingPointer]);
        alSourcePlay(source);
        state = AL_PLAYING; 
    }
}

void OggStreamPlayer::check_current_buffer_state() {
    if(state == AL_PLAYING) {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        if(state != AL_PLAYING) {
            alSourceStop(source);
            alSourceUnqueueBuffers(source, 1, &buffers[playingPointer]);

            playingPointer += 1;
            playingPointer %= BUFFER_COUNT;
            playingIncrementor += 1;
        }

        if(processingIncrementor <= playingIncrementor) {
            playing = false;
        }
    }
}

void OggStreamPlayer::update() {
    //printQueue();
    stream_from_ogg();

    if(start) {
        this->process_current_buffer();
    } else {
        this->process_current_buffer();

        this->check_current_buffer_state();

        this->play_current_buffer();
    }
}

bool OggStreamPlayer::isPlaying() {
    return playing;
}

void OggStreamPlayer::printQueue() {
    if(!chunks.empty()) {
        std::string f = "data";
        if(chunks.front().type == CT_START) {
            f = "start";
        } else if(chunks.front().type == CT_END) {
            f = "end";
        }

        std::string b = "data";


        if(chunks.back().type == CT_START) {
            b = "start";
        } else if(chunks.back().type == CT_END) {
            b = "end";
        }


        std::cout << "front: " << f << ", back: " << b << " size: " << chunks.size() << "\n";
    }
}