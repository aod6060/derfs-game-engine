#include <cstdlib>
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


#define BUFFER_COUNT 2
#define BUFFER_SIZE 4096 * 32
#define CHUNK_SIZE 4096

struct AudioData {
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

struct StreamPlayer {
    AudioData* data;
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

    void init(AudioData* data);

    void release();

    void update();

    bool isPlaying();

    void stream_from_ogg();
    void process_current_buffer();
    void play_current_buffer();
    void check_current_buffer_state();
};

int main(int argc, char** argv) {
    /*
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

    AudioData data;
    data.init("data/sound/music/menu.dsaf");

    StreamPlayer player;
    player.init(&data);

    while(player.isPlaying()) {
        player.update();
    }

    
    player.release();

    data.release();

    alcMakeContextCurrent(nullptr);

    alcDestroyContext(context);
    alcCloseDevice(device);
    */


    return 0;
}

bool AudioData::init(std::string path) {
    return false;
}

long AudioData::read(std::vector<char>& buffer, int* bitstream) {
    return 0;
}

int64_t AudioData::tell() {
    return 0;
}

void AudioData::seek(int64_t position) {
}

int64_t AudioData::maxSize() {
    return 0;
}

void AudioData::release() {
}

long AudioData::getFrequence() {
    return 0;
}

long AudioData::getChannelCount() {
    return 0;
}

long AudioData::getBitPerSample() {
    return 0;
}

// OggStreamPlayer
void StreamPlayer::init(AudioData* data) {
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

void StreamPlayer::release() {
    //buffer.clear();
    alDeleteSources(1, &this->source);
    alDeleteBuffers(buffers.size(), buffers.data());
    this->data = nullptr;
    this->data = nullptr;
}

void StreamPlayer::stream_from_ogg() {
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

void StreamPlayer::process_current_buffer() {
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

void StreamPlayer::play_current_buffer() {
    if(state != AL_PLAYING){
        alSourceQueueBuffers(source, 1, &buffers[playingPointer]);
        alSourcePlay(source);
        state = AL_PLAYING; 
    }
}

void StreamPlayer::check_current_buffer_state() {
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


void StreamPlayer::update() {
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

bool StreamPlayer::isPlaying() {
    return playing;
}

void StreamPlayer::printQueue() {
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