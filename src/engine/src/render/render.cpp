#include "SDL_video.h"
#include "../sys.hpp"
#include <functional>
#include <vector>




namespace render {

    //static MainShader mainShader;
    struct MeshName {
        uint64_t meshID;
        uint64_t materialID;
        uint64_t toHash() const;
    };

    struct MeshBatch {
        std::string mesh;
        std::string material;
        std::vector<glm::mat4> model;
    };
}

namespace std {
    template<> struct less<render::MeshName> {
        bool operator() (const render::MeshName& l, const render::MeshName& r) const {
            //return l.meshID < r.meshID || l.materialID < r.materialID;
            return l.toHash() < r.toHash();
        }
    };
}

namespace render {

    std::map<MeshName, MeshBatch> meshBaches;
    glw::VertexBuffer modelBuffer;

    void init() {
        SDL_GL_SetSwapInterval(1);
    
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        //mainShader.init();
        shader::prepass::init();
        shader::geometry::init();
        shader::lighting::init();
        shader::postprocess::init();
        modelBuffer.init();

    }

    void release() {
        modelBuffer.release();
        shader::postprocess::release();
        shader::lighting::release();
        shader::geometry::release();
        shader::prepass::release();
        //mainShader.release();
    }

    void clear(glm::vec4 clearColor) {
        glViewport(0, 0, app::getWidthInteger(), app::getHeightInteger());
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void drawArrays(GLenum type, uint32_t vertexCount) {
        glDrawArrays(type, 0, vertexCount);
    }

    void drawElements(GLenum type, uint32_t count) {
        glDrawElements(type, count, GL_UNSIGNED_INT, nullptr);
    }

    void drawElementsInstance(GLenum type, uint32_t indexCount, uint32_t instanceCount) {
        glDrawElementsInstanced(
            type,
            indexCount,
            GL_UNSIGNED_INT,
            0,
            instanceCount
        );
    }

    void submitMeshDraw(std::string mesh, std::string material, const glm::mat4& model) {
        MeshName name;
        name.meshID = assets::getMeshID(mesh);
        name.materialID = assets::getMaterialID(material);

        if(meshBaches.find(name) != meshBaches.end()) {
            // Insert matrix into batch
            meshBaches.at(name).model.push_back(model);
        } else {
            // Create a new Mesh Batch
            meshBaches[name].mesh = mesh;
            meshBaches.at(name).material = material;
            meshBaches.at(name).model.push_back(model);
        }
    }

    void _render_mesh_gb() {
        render::shader::geometry::getMeshShader()->bind();
        // Testing batching...
        for(std::map<MeshName, MeshBatch>::iterator it = meshBaches.begin(); it != meshBaches.end(); it++) {
            assets::getMaterial(it->second.material)->bind();
            modelBuffer.clear();
            for(int i = 0; i < it->second.model.size(); i++) {
                modelBuffer.addMat4(it->second.model.at(i));
            }
            // Make sure this is clear for next frame
            it->second.model.clear();
            modelBuffer.update();
            // Draw Meshes
            render::shader::geometry::getMeshShader()->drawMesh(assets::getMesh(it->second.mesh), &modelBuffer);
            // Loop through model matrices and render them
            assets::getMaterial(it->second.material)->unbind();
            // Clear Models
            it->second.model.clear(); // To make sure the models are clear.
        }
        render::shader::geometry::getMeshShader()->unbind();
        meshBaches.clear(); // This has to be cleared out every frame because 
        // if not it will over draw.
    }
    
    void present() {
        render::clear(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

        _render_mesh_gb();
    }

    uint64_t MeshName::toHash() const {
        uint64_t a = this->meshID;
        uint64_t b = this->materialID;
        // Cantor pairing Function 
        // https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
        return (a + b) * (a + b + 1) / 2 + a;
        // Szudzik's Function
        //https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
        //return a >= b ? a * a + a + b : a + b * b;
    }
}

