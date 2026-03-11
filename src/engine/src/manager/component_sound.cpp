#include "../sys.hpp"
#include "AL/al.h"
#include <cstring>
#include <deque>
#include <limits>
#include <vector>


namespace manager {
    namespace component {
        namespace sound {
            // SoundListenerComponent
            void SoundListenerComponent::init(Entity* entity) {
                this->entity = entity;
            }

            void SoundListenerComponent::handleEvent(SDL_Event* e) {

            }

            void SoundListenerComponent::update(float delta) {
                ::sound::transformListener(entity->transform);
            }

            void SoundListenerComponent::preRender() {

            }

            void SoundListenerComponent::render() {

            }

            void SoundListenerComponent::release() {
                entity = nullptr;
            }

            void SoundListenerComponent::load(Json::Value value) {
                // Listener are really simple
                // It doesn't really need any other thing hooked on them.
            }

            // SoundStreamPlayerComponent
            void MusicStreamPlayerComponent::init(Entity* entity) {
                this->entity = entity;
                buffers.resize(BUFFER_COUNT);
                for(int i = 0; i < buffers.size(); i++) {
                    buffers[i].init();
                }
                this->sounce.init();
                this->reset();
                if(this->autoPlay) {
                    this->play();
                }
            }

            void MusicStreamPlayerComponent::handleEvent(SDL_Event* e) {

            }

            void MusicStreamPlayerComponent::update(float delta) {
                if(this->ssState == StreamState::SS_PLAY) {
                    /*
                    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
                    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
                    alSourcef(source, AL_PITCH, 1.0f);
                    alSourcef(source, AL_GAIN, 0.5f);
                    alSourcei(source, AL_LOOPING, AL_FALSE);
                    */
                    // Update Gain
                    this->sounce.setVolume(::sound::getGroupVolume(this->volumeGroup));

                    stream_data();
                    if(start) {
                        this->process_current_buffer();
                    } else {
                        this->process_current_buffer();
                        this->check_current_buffer_state();
                        this->play_current_buffer();
                    }
                } else {
                    if(this->ssState == StreamState::SS_STOP) {
                        if(looping) {
                            this->play();
                        }
                    }
                }
            }

            void MusicStreamPlayerComponent::preRender() {

            }

            void MusicStreamPlayerComponent::render() {

            }

            void MusicStreamPlayerComponent::release() {
                this->entity = nullptr;
            }

            void MusicStreamPlayerComponent::load(Json::Value value) {
                this->audioDataName = value["audio-data-name"].asString();
                this->volumeGroup = value["volume-group"].asString();
                this->looping = value["looping"].asBool();
                this->autoPlay = value["auto-play"].asBool();
                this->relative = value["relative"].asBool();
            }

            void MusicStreamPlayerComponent::play() {
                if(this->ssState == StreamState::SS_STOP) {
                    this->reset();
                }
                this->ssState = StreamState::SS_PLAY;
            }

            void MusicStreamPlayerComponent::pause() {
                this->ssState = StreamState::SS_PAUSE;
            }

            void MusicStreamPlayerComponent::stop() {
                this->ssState = StreamState::SS_STOP;
            }

            bool MusicStreamPlayerComponent::isLooping() {
                return this->looping;
            }

            void MusicStreamPlayerComponent::setLooping(bool value) {
                this->looping = value;
            }

            bool MusicStreamPlayerComponent::isAutoPlay() {
                return this->autoPlay;
            }

            void MusicStreamPlayerComponent::setAutoPlay(bool value) {
                this->autoPlay = value;
            }

            bool MusicStreamPlayerComponent::isRelative() {
                return this->relative;
            }

            void MusicStreamPlayerComponent::setRelative(bool value) {
                this->relative = value;
                this->sounce.setRelative(this->relative);
            }

            void MusicStreamPlayerComponent::stream_data() {
                if(!endOfFile) {
                    assets::getSound(this->audioDataName)->seek(this->currentPosition);

                    Chunk chunk;
                    chunk.type = ChunkType::CT_DATA;
                    chunk.data.resize(CHUNK_SIZE);
                    chunk.len = assets::getSound(this->audioDataName)->read(chunk.data);

                    if(chunk.len == 0) {
                        endOfFile = true;
                        chunk.data.clear();
                        Chunk endChunk;
                        endChunk.type = ChunkType::CT_END;
                        chunks.push_back(endChunk);
                    } else if(chunk.len < 0) {
                        chunk.data.clear();
                        std::cout << "There was an error\n";
                    } else {
                        this->chunks.push_back(chunk);
                    }

                    this->currentPosition = assets::getSound(this->audioDataName)->tell();
                }
            }

            void MusicStreamPlayerComponent::process_current_buffer() {
                if(this->chunks.front().type != CT_END) {
                    while(!this->chunks.empty()) {
                        if(this->chunks.front().type == CT_START) {
                            this->chunks.pop_front();
                        } else if(this->chunks.front().type == CT_END) {
                            break;
                        } else {
                            if(this->chunks.front().len + length < BUFFER_SIZE) {
                                memcpy(bufferData.data() + length, this->chunks.front().data.data(), this->chunks.front().len);
                                length += this->chunks.front().len;
                                this->chunks.front().data.clear();
                                this->chunks.pop_front();
                            } else {
                                if(start || processingPointer != playingPointer) {
                                    int format = 0;

                                    if(assets::getSound(this->audioDataName)->getChannelCount() == 1 && assets::getSound(this->audioDataName)->getBitPerSample() == 8) {
                                        format = AL_FORMAT_MONO8;
                                    } else if(assets::getSound(this->audioDataName)->getChannelCount() == 2 && assets::getSound(this->audioDataName)->getBitPerSample() == 8) {
                                        format = AL_FORMAT_STEREO8;
                                    } else if(assets::getSound(this->audioDataName)->getChannelCount() == 1 && assets::getSound(this->audioDataName)->getBitPerSample() == 16) {
                                        format = AL_FORMAT_MONO16;
                                    } else if(assets::getSound(this->audioDataName)->getChannelCount() == 2 && assets::getSound(this->audioDataName)->getBitPerSample() == 16) {
                                        format = AL_FORMAT_STEREO16;
                                    }

                                    buffers[processingPointer].bufferData(format, bufferData.data(), length, assets::getSound(this->audioDataName)->getFrequence());

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
                    if(!this->endOfQueue) {
                        if(start || processingPointer != playingPointer) {
                            endOfQueue = true;
                            int format = 0;

                            if(assets::getSound(this->audioDataName)->getChannelCount() == 1 && assets::getSound(this->audioDataName)->getBitPerSample() == 8) {
                                format = AL_FORMAT_MONO8;
                            } else if(assets::getSound(this->audioDataName)->getChannelCount() == 2 && assets::getSound(this->audioDataName)->getBitPerSample() == 8) {
                                format = AL_FORMAT_STEREO8;
                            } else if(assets::getSound(this->audioDataName)->getChannelCount() == 1 && assets::getSound(this->audioDataName)->getBitPerSample() == 16) {
                                format = AL_FORMAT_MONO16;
                            } else if(assets::getSound(this->audioDataName)->getChannelCount() == 2 && assets::getSound(this->audioDataName)->getBitPerSample() == 16) {
                                format = AL_FORMAT_STEREO16;
                            }

                            buffers[processingPointer].bufferData(format, bufferData.data(), length, assets::getSound(this->audioDataName)->getFrequence());

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

            void MusicStreamPlayerComponent::check_current_buffer_state() {
                if(state != AL_PLAYING) {
                    std::vector<ALuint> bid = {this->buffers[playingPointer].id};
                    this->sounce.sourceQueueBuffers(bid);
                    this->sounce.play();
                    state = AL_PLAYING;
                }
            }

            void MusicStreamPlayerComponent::play_current_buffer() {
                if(state == AL_PLAYING) {
                    state = this->sounce.getState();
                    if(state != AL_PLAYING) {
                        std::vector<ALuint> bid = {this->buffers[playingPointer].id};
                        this->sounce.stop();
                        this->sounce.sourceUnqueueBuffers(bid);
                        playingPointer += 1;
                        playingPointer %= BUFFER_COUNT;
                        playingIncrementor += 1;
                    }

                    if(processingIncrementor <= playingIncrementor && endOfQueue) {
                        playing = false;
                        this->stop();
                    }
                }
            }

            void MusicStreamPlayerComponent::reset() {
                std::cout << "Here???\n";
                while(!this->chunks.empty()) {
                    this->chunks.front().data.clear();
                    this->chunks.pop_front();
                }

                Chunk startChunk;
                startChunk.type = ChunkType::CT_START;
                this->chunks.push_back(startChunk);

                bufferData.clear();
                bufferData.resize(BUFFER_SIZE);

                currentPosition = 0;
                state = AL_STOPPED;
                endOfFile = false;
                endOfQueue = false;
                length = 0;
                start = true;
                processingPointer = 0;
                playingPointer = 0;
                processingIncrementor = 0;
                playingIncrementor = 0;
                playlast = false;
                playing = true;
            }


            // SoundPlayerComponent
            void SoundPlayerComponent::init(Entity* entity) {
                this->entity = entity;

                // Create Sound
                this->source.init();
                
                // Set Sound Values
                this->source.setLooping(this->looping);
                this->source.setRelative(this->relative);
                this->source.setReferenceDistance(this->referenceDistance);
                this->source.setRolloffFactor(this->rolloffFactor);
                this->source.setMaxDistance(this->maxDistance);
                this->source.setPitch(this->pitch);

                // Create Buffer
                this->buffer.init();

                // Fill Buffer
                std::vector<char> bufferData;
                std::deque<Chunk> chunks;

                int len = 1;
                int length = 0;
            
                std::cout << "Here 1\n";

                while(len != 0 || len < 0) {
                    Chunk chunk;
                    std::cout << "Here 2\n";

                    chunk.data.resize(4096);
                    len = assets::getSound(this->audioDataName)->read(chunk.data);
                    if(len == 0) {
                        continue;
                    } else if(len < 0) {
                        continue;
                    } else {
                        length += len;
                        chunk.len = len;
                        chunks.push_back(chunk);
                    }
                }

                std::cout << "Here 2\n";

                assets::getSound(this->audioDataName)->seek(::sound::AudioDataSeek::ADS_BEGIN);
                bufferData.resize(length);

                std::cout << bufferData.size() << "\n";

                int offset = 0;

                while(!chunks.empty()) {
                    memcpy(bufferData.data() + offset, chunks.front().data.data(), chunks.front().len);
                    offset += chunks.front().len;
                    chunks.pop_front();
                }

                std::cout << "Seeking the the begining.\n";

                int format = 0;

                if(assets::getSound(this->audioDataName)->getChannelCount() == 1 && assets::getSound(this->audioDataName)->getBitPerSample() == 8) {
                    format = AL_FORMAT_MONO8;
                } else if(assets::getSound(this->audioDataName)->getChannelCount() == 2 && assets::getSound(this->audioDataName)->getBitPerSample() == 8) {
                    format = AL_FORMAT_STEREO8;
                } else if(assets::getSound(this->audioDataName)->getChannelCount() == 1 && assets::getSound(this->audioDataName)->getBitPerSample() == 16) {
                    format = AL_FORMAT_MONO16;
                } else if(assets::getSound(this->audioDataName)->getChannelCount() == 2 && assets::getSound(this->audioDataName)->getBitPerSample() == 16) {
                    format = AL_FORMAT_STEREO16;
                }

                buffer.bufferData(format, bufferData.data(), bufferData.size(), assets::getSound(this->audioDataName)->getFrequence());

                // Set buffer to source
                source.setBuffer(&buffer);

                if(this->autoPlay) {
                    this->play();
                }

                int type = source.getType();

                if(type == AL_STATIC) {
                    std::cout << "Static Source\n";
                } else if(type == AL_STREAMING) {
                    std::cout << "Streaming Source\n";
                } else {
                    std::cout << "Undetermined Source\n";
                }
            }

            void SoundPlayerComponent::handleEvent(SDL_Event* e) {

            }

            void SoundPlayerComponent::update(float delta) {

                this->source.setPosition(entity->transform.getGlobalPosition());

                this->source.setVolume(::sound::getGroupVolume(this->volumeGroup));


            }

            void SoundPlayerComponent::preRender() {

            }

            void SoundPlayerComponent::render() {

            }

            void SoundPlayerComponent::release() {
                this->buffer.release();
                this->source.release();
            }

            void SoundPlayerComponent::load(Json::Value value) {
                this->audioDataName = value["audio-data-name"].asString();
                this->volumeGroup = value["volume-group"].asString();
                this->autoPlay = value["auto-play"].asBool();
                this->looping = value["looping"].asBool();
                this->relative = value["relative"].asBool();
                this->referenceDistance = value["reference-distance"].asFloat();
                this->rolloffFactor = value["rolloff-factor"].asFloat();
                this->maxDistance = value["max-distance"].asFloat();
                this->pitch = value["pitch"].asFloat();
            }

            void SoundPlayerComponent::play() {
                this->source.play();
            }

            void SoundPlayerComponent::pause() {
                this->source.pause();
            }

            void SoundPlayerComponent::stop() {
                this->source.stop();
            }

            
            
        }
    }
}