#include "sys.hpp"
#include "json/value.h"
#include <fstream>


namespace assets {
    std::map<std::string, render::mesh::Mesh> meshes;
    std::map<std::string, render::glw::Texture2D> texture2Ds;
    std::map<std::string, sound::IAudioData*> sounds;

    void init() {
        std::ifstream in("data/assets.json");

        Json::Value root;

        in >> root;

        in.close();

        std::string name = root["name"].asString();
        int version = root["version"].asInt();

        if(name != "assets") {
            std::cout << name << " isn't an assets file. Will try to load.\n";
        }

        if(version != 2) {
            std::cout << version << " isn't the current version which is 1. Will try to load.\n";
        }


        // Meshes
        Json::Value meshes = root["meshes"];

        for(int i = 0; i < meshes.size(); i++) {
            Json::Value obj = meshes[i];

            std::string name = obj["name"].asString();
            std::string path = obj["path"].asString();

            assets::meshes[name].init(path);
        }

        // Textures
        Json::Value textures = root["textures"];

        for(int i = 0; i < textures.size(); i++) {
            Json::Value obj = textures[i];

            std::string name = obj["name"].asString();
            std::string path = obj["path"].asString();

            assets::texture2Ds[name].init();
            render::glw::Texture2D::createTextureFromFile(&assets::texture2Ds[name], path);
        }

        // Sounds
        Json::Value sounds = root["sounds"];

        for(int i = 0; i < sounds.size(); i++) {
            Json::Value obj = sounds[i];

            std::string name = obj["name"].asString();
            std::string path = obj["path"].asString();

            assets::sounds[name] = ::sound::initAudioData(path);
        }
    }

    void release() {
        for(std::map<std::string, render::mesh::Mesh>::iterator it = meshes.begin(); it != meshes.end(); it++) {
            it->second.release();
        }

        meshes.clear();

        for(std::map<std::string, render::glw::Texture2D>::iterator it = texture2Ds.begin(); it != texture2Ds.end(); it++) {
            it->second.release();
        }

        texture2Ds.clear();

        for(std::map<std::string, sound::IAudioData*>::iterator it = sounds.begin(); it != sounds.end(); it++) {
            it->second->release();
            delete it->second;
            it->second = nullptr;
        }

        sounds.clear();
    }

    render::mesh::Mesh* getMesh(std::string name) {
        return &meshes.at(name);
    }

    render::glw::Texture2D* getTexture2D(std::string name) {
        return &texture2Ds.at(name);
    }

    sound::IAudioData* getSound(std::string name) {
        return sounds.at(name);
    }
}