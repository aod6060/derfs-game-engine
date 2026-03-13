#include "../sys.hpp"


namespace assets {

    std::map<std::string, uint64_t> counters;

    //std::map<std::string, render::mesh::Mesh> meshes;
    std::map<std::string, Asset<render::mesh::Mesh>> meshes;
    //std::map<std::string, render::glw::Texture2D> texture2Ds;
    std::map<std::string, Asset<render::glw::Texture2D>> texture2Ds;
    //std::map<std::string, sound::IAudioData*> sounds;
    std::map<std::string, Asset<sound::IAudioData*>> sounds;

    uint64_t _createID(std::string name);

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

            assets::meshes[name].value.init(path);
            assets::meshes.at(name).id = _createID("meshes");

            std::cout << "MeshID: " << assets::meshes.at(name).id << "\n";
        }

        // Textures
        Json::Value textures = root["textures"];

        for(int i = 0; i < textures.size(); i++) {
            Json::Value obj = textures[i];

            std::string name = obj["name"].asString();
            std::string path = obj["path"].asString();

            assets::texture2Ds[name].value.init();
            render::glw::Texture2D::createTextureFromFile(&assets::texture2Ds.at(name).value, path);
            assets::texture2Ds.at(name).id = _createID("texture2Ds");

            std::cout << "Texture2DID: " << assets::texture2Ds.at(name).id << "\n";
        }

        // Sounds
        Json::Value sounds = root["sounds"];

        for(int i = 0; i < sounds.size(); i++) {
            Json::Value obj = sounds[i];

            std::string name = obj["name"].asString();
            std::string path = obj["path"].asString();

            assets::sounds[name].value = ::sound::initAudioData(path);
            assets::sounds.at(name).id = _createID("sounds");

            std::cout << "SoundID: " << assets::sounds.at(name).id << "\n";
        }
    }

    void release() {
        for(std::map<std::string, Asset<render::mesh::Mesh>>::iterator it = meshes.begin(); it != meshes.end(); it++) {
            it->second.value.release();
        }

        meshes.clear();

        for(std::map<std::string, Asset<render::glw::Texture2D>>::iterator it = texture2Ds.begin(); it != texture2Ds.end(); it++) {
            it->second.value.release();
        }

        texture2Ds.clear();

        for(std::map<std::string, Asset<sound::IAudioData*>>::iterator it = sounds.begin(); it != sounds.end(); it++) {
            it->second.value->release();
            delete it->second.value;
            it->second.value = nullptr;
        }

        sounds.clear();
    }

    render::mesh::Mesh* getMesh(std::string name) {
        return &meshes.at(name).value;
    }

    render::glw::Texture2D* getTexture2D(std::string name) {
        return &texture2Ds.at(name).value;
    }

    sound::IAudioData* getSound(std::string name) {
        return sounds.at(name).value;
    }

    uint64_t getMeshID(std::string name) {
        return assets::meshes.at(name).id;
    }

    uint64_t getTexture2DID(std::string name) {
        return assets::texture2Ds.at(name).id;
    }

    uint64_t getSoundID(std::string name) {
        return assets::sounds.at(name).id;
    }

    uint64_t _createID(std::string name) {
        uint64_t id;
        if(counters.find(name) == counters.end()) {
            counters[name] = id = 1;
        } else {
            counters.at(name) += 1;
            id = counters.at(name);
        }
        return id;
    }
}