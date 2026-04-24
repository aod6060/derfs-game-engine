#include "render_hidden.hpp"


namespace render {
    const static std::string MATERIAL_NAME = "material";
    const static int MATERIAL_VERSION = 1;


    void Material::init(std::string path) {
        std::ifstream in(path);

        if(!in.is_open()) {
            std::cout << path << "doesn't exist!\n";
            return;
        }

        Json::Value root;
        in >> root;
        in.close();

        std::string name = root["name"].asString();
        int version = root["version"].asInt();

        if(name != MATERIAL_NAME) {
            std::cout << name << " isn't an material file. Will try to load.\n";
        }

        if(version != MATERIAL_VERSION) {
            std::cout << version << " isn't the current version which is 1. Will try to load.\n";
        }

        this->albedo = root["albedo"].asString();
        this->metal = root["metal"].asString();
        this->roughness = root["roughness"].asString();
        this->emissive = root["emissive"].asString();
        this->lit = root["lit"].asString();
    }

    void Material::bind() {
        assets::getTexture2D(this->albedo)->bind(GL_TEXTURE0);
        assets::getTexture2D(this->metal)->bind(GL_TEXTURE1);
        assets::getTexture2D(this->roughness)->bind(GL_TEXTURE2);
        assets::getTexture2D(this->emissive)->bind(GL_TEXTURE3);
        assets::getTexture2D(this->lit)->bind(GL_TEXTURE4);
    }
    
    void Material::unbind() {
        assets::getTexture2D(this->lit)->unbind(GL_TEXTURE4);
        assets::getTexture2D(this->emissive)->unbind(GL_TEXTURE3);
        assets::getTexture2D(this->roughness)->unbind(GL_TEXTURE2);
        assets::getTexture2D(this->metal)->unbind(GL_TEXTURE1);
        assets::getTexture2D(this->albedo)->unbind(GL_TEXTURE0);
    }
    
}