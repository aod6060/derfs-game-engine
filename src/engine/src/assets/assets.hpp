#ifndef ASSETS_HPP
#define ASSETS_HPP


#include <iostream>

namespace render {
    namespace glw {
        struct Texture2D;
    }

    namespace mesh {
        struct Mesh;
    }

    struct Material;
}

namespace sound {
    struct IAudioData;
}

namespace assets {

    template<typename T>
    struct Asset {
        uint64_t id = 0;
        T value;
    };

    void init();
    void release();

    uint64_t getMeshID(std::string name);
    render::mesh::Mesh* getMesh(std::string name);

    uint64_t getTexture2DID(std::string name);
    render::glw::Texture2D* getTexture2D(std::string name);

    uint64_t getSoundID(std::string name);
    sound::IAudioData* getSound(std::string name);

    uint64_t getMaterialID(std::string name);
    render::Material* getMaterial(std::string name);
}

#endif