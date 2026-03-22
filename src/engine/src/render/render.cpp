#include "SDL_video.h"
#include "../sys.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
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

    struct Standard2DTransform {
        glm::mat4 proj;
        glm::mat4 model;
    };

    // Basically staging during a render pass
    struct IStage {
        virtual void init(IStage* previousStage) = 0;
        virtual void render() = 0;
        virtual void release() = 0;
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

    glw::VertexBuffer screenVertices;
    glw::VertexBuffer screenTexCoords;
    glw::IndexBuffer screenIndencies;

    glw::UniformBuffer<Standard2DTransform> stand2DUniformBuffer;

    struct GeometryBufferStage : public IStage {
        IStage* previousStage = nullptr;

        std::map<MeshName, MeshBatch> meshBaches;
        glw::VertexBuffer modelBuffer;

        glw::FrameBuffer frameBuffer;

        // Textures
        glw::Texture2D depthBuffer;
        glw::Texture2D positionBuffer;
        glw::Texture2D normalBuffer;
        glw::Texture2D albedoBuffer;
        // Basically Metal, Rounghess, Emissive, Lit Buffer
        // There is no point on separating these.
        glw::Texture2D mrelBuffer;

        virtual void init(IStage* previousStage);
        virtual void render();
        virtual void release();

        void renderMeshComponents();

        void submitMeshComponent(std::string mesh, std::string material, const glm::mat4& model);
    } geometryBufferStage;

    struct LightingStage : public IStage {
        IStage* previousStage = nullptr;

        virtual void init(IStage* previousStage);
        virtual void render();
        virtual void release();
    } lightingStage;

    void init() {
        SDL_GL_SetSwapInterval(1);
        //mainShader.init();
        shader::prepass::init();
        shader::geometry::init();
        shader::lighting::init();
        shader::postprocess::init();
        //modelBuffer.init();

        // Screen Vertices
        screenVertices.init();
        screenVertices.add3f(0.0f, 0.0f, 0.0f);
        screenVertices.add3f(1.0f, 0.0f, 0.0f);
        screenVertices.add3f(0.0f, 1.0f, 0.0f);
        screenVertices.add3f(1.0f, 1.0f, 0.0f);
        screenVertices.update();

        // Screen TexCoords
        screenTexCoords.init();
        screenTexCoords.add2f(0.0f, 1.0f);
        screenTexCoords.add2f(1.0f, 1.0f);
        screenTexCoords.add2f(0.0f, 0.0f);
        screenTexCoords.add2f(1.0f, 0.0f);
        screenTexCoords.update();

        // Screen Indencies
        screenIndencies.init();
        screenIndencies.add3ui(0, 1, 2);
        screenIndencies.add3ui(2, 1, 3);
        screenIndencies.update();

        stand2DUniformBuffer.init();
        stand2DUniformBuffer.value.proj = glm::ortho(0.0f, app::getWidthFloat(), app::getHeightFloat(), 0.0f);
        stand2DUniformBuffer.value.model = 
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(app::getWidthFloat(), app::getHeightFloat(), 0.0f));
        stand2DUniformBuffer.update();
        stand2DUniformBuffer.bind();
        stand2DUniformBuffer.bufferRange(1);
        stand2DUniformBuffer.unbind();

        geometryBufferStage.init(nullptr);
        lightingStage.init(&geometryBufferStage);
    }

    void release() {
        lightingStage.release();
        geometryBufferStage.release();

        stand2DUniformBuffer.release();
        screenIndencies.release();
        screenTexCoords.release();
        screenVertices.release();
        //modelBuffer.release();
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

    void clear2D(glm::vec4 clearColor) {
        glViewport(0, 0, app::getWidthInteger(), app::getHeightInteger());
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
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
        geometryBufferStage.submitMeshComponent(mesh, material, model);
    }

    void present() {
        geometryBufferStage.render();
        lightingStage.render();
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



    // Stages

    // Geometry Buffer Stage
    void GeometryBufferStage::init(IStage* previousStage) {
        this->previousStage = previousStage;
        this->modelBuffer.init();

        // Init Textures
        // Init DepthBuffer
        depthBuffer.init();
        depthBuffer.bind(GL_TEXTURE0);
        depthBuffer.texImage2D(0, GL_DEPTH_COMPONENT32F, app::getWidthInteger(), app::getHeightInteger(), GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        depthBuffer.texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        depthBuffer.texParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        depthBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        depthBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        depthBuffer.unbind(GL_TEXTURE0);

        // Init PositionBuffer
        positionBuffer.init();
        positionBuffer.bind(GL_TEXTURE0);
        positionBuffer.texImage2D(0, GL_RGBA32F, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_FLOAT, nullptr);
        positionBuffer.texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        positionBuffer.texParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        positionBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        positionBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        positionBuffer.unbind(GL_TEXTURE0);

        // Init NormalBuffer
        normalBuffer.init();
        normalBuffer.bind(GL_TEXTURE0);
        normalBuffer.texImage2D(0, GL_RGBA32F, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_FLOAT, nullptr);
        normalBuffer.texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        normalBuffer.texParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        normalBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        normalBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        normalBuffer.unbind(GL_TEXTURE0);

        // Init AlbedoBuffer
        albedoBuffer.init();
        albedoBuffer.bind(GL_TEXTURE0);
        albedoBuffer.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        albedoBuffer.texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        albedoBuffer.texParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        albedoBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        albedoBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        albedoBuffer.unbind(GL_TEXTURE0);

        // Init MRELBuffer
        mrelBuffer.init();
        mrelBuffer.bind(GL_TEXTURE0);
        mrelBuffer.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        mrelBuffer.texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        mrelBuffer.texParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        mrelBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        mrelBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        mrelBuffer.unbind(GL_TEXTURE0);


        frameBuffer.init();

        frameBuffer.bind();
        frameBuffer.attachDepthBuffer(&this->depthBuffer);
        frameBuffer.attachColorBuffer(&positionBuffer, GL_COLOR_ATTACHMENT0);
        frameBuffer.attachColorBuffer(&normalBuffer, GL_COLOR_ATTACHMENT1);
        frameBuffer.attachColorBuffer(&albedoBuffer, GL_COLOR_ATTACHMENT2);
        frameBuffer.attachColorBuffer(&mrelBuffer, GL_COLOR_ATTACHMENT3);
        frameBuffer.drawBuffers({
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3
        });
        if(!frameBuffer.wasCreated()) {
            std::cout << "The framebuffer wasn't create :(!\n";
        } else {
            std::cout << "The framebuffer was create :D!!!\n";
        }
        frameBuffer.unbind();
    }

    void GeometryBufferStage::render() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        frameBuffer.bind();
        render::clear(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        this->renderMeshComponents();
        frameBuffer.unbind();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    void GeometryBufferStage::release() {
        frameBuffer.release();

        mrelBuffer.release();
        albedoBuffer.release();
        normalBuffer.release();
        positionBuffer.release();
        depthBuffer.release();

        this->modelBuffer.release();
        this->previousStage = nullptr;
    }

    void GeometryBufferStage::renderMeshComponents() {
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

    void GeometryBufferStage::submitMeshComponent(std::string mesh, std::string material, const glm::mat4& model) {
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


    // Lighting Stage
    void LightingStage::init(IStage* previousStage) {
        this->previousStage = previousStage;
    }

    void LightingStage::render() {
        GeometryBufferStage* geom = (GeometryBufferStage*)this->previousStage;

        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::lighting::getLightingShader()->bind();
        
        geom->depthBuffer.bind(GL_TEXTURE0);
        geom->positionBuffer.bind(GL_TEXTURE1);
        geom->normalBuffer.bind(GL_TEXTURE2);
        geom->albedoBuffer.bind(GL_TEXTURE3);
        geom->mrelBuffer.bind(GL_TEXTURE4);

        shader::lighting::getLightingShader()->bindVertexArray();
        screenVertices.bind();
        shader::lighting::getLightingShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::lighting::getLightingShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::lighting::getLightingShader()->unbindVertexArray();

        geom->mrelBuffer.bind(GL_TEXTURE4);
        geom->albedoBuffer.bind(GL_TEXTURE3);
        geom->normalBuffer.bind(GL_TEXTURE2);
        geom->positionBuffer.bind(GL_TEXTURE1);
        geom->depthBuffer.bind(GL_TEXTURE0);
        shader::lighting::getLightingShader()->unbind();
    }

    void LightingStage::release() {
        this->previousStage = nullptr;
    }

}

