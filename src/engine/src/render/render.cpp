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

    uint32_t tex = GL_LINEAR;

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

        glw::FrameBuffer frameBuffer;
        glw::Texture2D output;

        virtual void init(IStage* previousStage);
        virtual void render();
        virtual void release();
    } lightingStage;

    struct PostProcessingStage : public IStage {
        IStage* previousStage = nullptr;

        glw::Texture2D a, b, output, bloom, lineArt;
        glw::FrameBuffer frameBuffer;

        virtual void init(IStage* previousStage);
        virtual void render();
        virtual void release();
        
        void copy(glw::Texture2D* output, glw::Texture2D* in);
        
        void gaussianBlur(glw::Texture2D* output, glw::Texture2D* in, float widthDistance, float heightDistance);
        
        void combine(
            glw::Texture2D* output, 
            glw::Texture2D* a, 
            glw::Texture2D* b, 
            shader::postprocess::CombinePostProcessShader::CombineOP op, 
            float mixValue = 0.0f);

        void combineCamera(
            glw::Texture2D* output,
            glw::Texture2D* a,
            shader::postprocess::CombinePostProcessShader::CombineOP op
        );

        void threshold(glw::Texture2D* output, glw::Texture2D* in, float minValue, float maxValue);

        void edgeDetection(glw::Texture2D* output, glw::Texture2D* in, float widthDistance, float heightDistance);

        void invert(glw::Texture2D* output, glw::Texture2D* in);

        void desaturate(glw::Texture2D* output, glw::Texture2D* in, float value);

        void axis(glw::Texture2D* output, glw::Texture2D* in, shader::postprocess::AxisPostProcessShader::Axis axis);

        void outputScreenFrameBuffer(glw::Texture2D* in);

    } postProcessingStage;

    void init() {
        SDL_GL_SetSwapInterval(1);
        //mainShader.init();
        shader::prepass::init();
        shader::geometry::init();
        stand2DUniformBuffer.init();
        stand2DUniformBuffer.value.proj = glm::ortho(0.0f, app::getWidthFloat(), app::getHeightFloat(), 0.0f);
        stand2DUniformBuffer.value.model = 
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(app::getWidthFloat(), app::getHeightFloat(), 0.0f));
        stand2DUniformBuffer.update();
        stand2DUniformBuffer.bind();
        stand2DUniformBuffer.bufferRange(1);
        stand2DUniformBuffer.unbind();
        
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

        geometryBufferStage.init(nullptr);
        lightingStage.init(&geometryBufferStage);
        postProcessingStage.init(&lightingStage);
    }

    void release() {
        postProcessingStage.release();
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
        shader::lighting::uploadLights();
        lightingStage.render();
        shader::lighting::clearLights();
        postProcessingStage.render();
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
        depthBuffer.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        depthBuffer.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        depthBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        depthBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        depthBuffer.unbind(GL_TEXTURE0);

        // Init PositionBuffer
        positionBuffer.init();
        positionBuffer.bind(GL_TEXTURE0);
        positionBuffer.texImage2D(0, GL_RGBA32F, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_FLOAT, nullptr);
        positionBuffer.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        positionBuffer.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        positionBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        positionBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        positionBuffer.unbind(GL_TEXTURE0);

        // Init NormalBuffer
        normalBuffer.init();
        normalBuffer.bind(GL_TEXTURE0);
        normalBuffer.texImage2D(0, GL_RGBA32F, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_FLOAT, nullptr);
        normalBuffer.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        normalBuffer.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        normalBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        normalBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        normalBuffer.unbind(GL_TEXTURE0);

        // Init AlbedoBuffer
        albedoBuffer.init();
        albedoBuffer.bind(GL_TEXTURE0);
        albedoBuffer.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        albedoBuffer.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        albedoBuffer.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        albedoBuffer.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        albedoBuffer.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        albedoBuffer.unbind(GL_TEXTURE0);

        // Init MRELBuffer
        mrelBuffer.init();
        mrelBuffer.bind(GL_TEXTURE0);
        mrelBuffer.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        mrelBuffer.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        mrelBuffer.texParameter(GL_TEXTURE_MIN_FILTER, tex);
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

        this->output.init();
        this->output.bind(GL_TEXTURE0);
        this->output.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        this->output.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        this->output.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        this->output.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        this->output.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        this->output.unbind(GL_TEXTURE0);

        frameBuffer.init();

        frameBuffer.bind();
        frameBuffer.attachColorBuffer(&output, GL_COLOR_ATTACHMENT0);
        frameBuffer.drawBuffers({
            GL_COLOR_ATTACHMENT0,
        });

        if(!frameBuffer.wasCreated()) {
            std::cout << "Lighting pass framebuffer wasn't create :(!\n";
        } else {
            std::cout << "Lighting pass framebuffer was create :D!!!\n";
        }
        frameBuffer.unbind();
    }

    void LightingStage::render() {
        this->frameBuffer.bind();
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
        this->frameBuffer.unbind();

    }

    void LightingStage::release() {
        this->frameBuffer.release();
        this->output.release();
        this->previousStage = nullptr;
    }

    // Post Processing Stage
    void PostProcessingStage::init(IStage* previousStage) {
        this->previousStage = previousStage;

        a.init();
        a.bind(GL_TEXTURE0);
        a.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        a.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        a.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        a.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        a.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        a.unbind(GL_TEXTURE0);

        b.init();
        b.bind(GL_TEXTURE0);
        b.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        b.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        b.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        b.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        b.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        b.unbind(GL_TEXTURE0);

        output.init();
        output.bind(GL_TEXTURE0);
        output.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        output.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        output.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        output.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        output.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        output.unbind(GL_TEXTURE0);

        bloom.init();
        bloom.bind(GL_TEXTURE0);
        bloom.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        bloom.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        bloom.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        bloom.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        bloom.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        bloom.unbind(GL_TEXTURE0);

        lineArt.init();
        lineArt.bind(GL_TEXTURE0);
        lineArt.texImage2D(0, GL_RGBA, app::getWidthInteger(), app::getHeightInteger(), GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        lineArt.texParameter(GL_TEXTURE_MAG_FILTER, tex);
        lineArt.texParameter(GL_TEXTURE_MIN_FILTER, tex);
        lineArt.texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        lineArt.texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        lineArt.unbind(GL_TEXTURE0);

        frameBuffer.init();
    }

    void PostProcessingStage::render() {
        LightingStage* lightingStage = (LightingStage*)this->previousStage;
        GeometryBufferStage* geomStage = (GeometryBufferStage*)lightingStage->previousStage;

        // Bloom
        this->copy(&this->a, &lightingStage->output);
        this->threshold(&this->output, &this->a, 0.9, 1.0);

        float size_limit = 0.3f;

        for(int i = 0; i < 128; i++) {
            this->copy(&a, &output);
            this->gaussianBlur(&output, &a, app::getWidthFloat() * size_limit, app::getHeightFloat() * size_limit);
        }

        this->copy(&a, &output);
        this->combine(&bloom, &a, &lightingStage->output, shader::postprocess::CombinePostProcessShader::CombineOP::COMBINE_OP_ADD);

        // Line Art
        float size_limit_2 = 0.9f;

        this->copy(&a, &geomStage->normalBuffer);
        this->edgeDetection(&output, &a, app::getWidthFloat() * size_limit_2, app::getHeightFloat() * size_limit_2);
        this->copy(&a, &output);
        this->invert(&output, &a);
        this->copy(&this->lineArt, &output);

        this->combine(&output, &bloom, &lineArt, shader::postprocess::CombinePostProcessShader::COMBINE_OP_MUL);

        this->outputScreenFrameBuffer(&bloom);
    }

    void PostProcessingStage::release() {
        frameBuffer.release();
        lineArt.release();
        bloom.release();
        output.release();
        b.release();
        a.release();
        this->previousStage = nullptr;
    }

    void PostProcessingStage::copy(glw::Texture2D* output, glw::Texture2D* in) {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});


        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getCopyShader()->bind();

        //lightingStage->output.bind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getCopyShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getCopyShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getCopyShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getCopyShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getCopyShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::gaussianBlur(glw::Texture2D* output, glw::Texture2D* in, float widthDistance, float heightDistance) {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});

        
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getGaussianBlurShader()->bind();

        //shader::postprocess::getGaussianBlurShader()->setSampleDistance(sampleDistance);
        shader::postprocess::getGaussianBlurShader()->setWidthDistance(widthDistance);
        shader::postprocess::getGaussianBlurShader()->setHeightDistance(heightDistance);

        //lightingStage->output.bind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getGaussianBlurShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getGaussianBlurShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getGaussianBlurShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getGaussianBlurShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getGaussianBlurShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::combine(
        glw::Texture2D* output, 
        glw::Texture2D* a, 
        glw::Texture2D* b, 
        shader::postprocess::CombinePostProcessShader::CombineOP op, 
        float mixValue) 
    {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});

        
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getCombineShader()->bind();

        shader::postprocess::getCombineShader()->setCombineOp(op);
        shader::postprocess::getCombineShader()->setMixValue(mixValue);
        //lightingStage->output.bind(GL_TEXTURE0);
        a->bind(GL_TEXTURE0);
        b->bind(GL_TEXTURE1);
        shader::postprocess::getCombineShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getCombineShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getCombineShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getCombineShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        b->bind(GL_TEXTURE1);
        a->bind(GL_TEXTURE0);

        shader::postprocess::getCombineShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::combineCamera(
        glw::Texture2D* output,
        glw::Texture2D* a,
        shader::postprocess::CombinePostProcessShader::CombineOP op
    )
    {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});

        
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getCombineShader()->bind();

        shader::postprocess::getCombineShader()->setCombineOp(op);
        shader::postprocess::getCombineShader()->setCameraPosition(shader::lighting::getLightingShader()->getCameraPosition());

        //lightingStage->output.bind(GL_TEXTURE0);
        a->bind(GL_TEXTURE0);
        shader::postprocess::getCombineShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getCombineShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getCombineShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getCombineShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        a->bind(GL_TEXTURE0);

        shader::postprocess::getCombineShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::threshold(glw::Texture2D* output, glw::Texture2D* in, float minValue, float maxValue) {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});

        
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getThresholdShader()->bind();

        //lightingStage->output.bind(GL_TEXTURE0);
        shader::postprocess::getThresholdShader()->setMinValue(minValue);
        shader::postprocess::getThresholdShader()->setMaxValue(maxValue);

        in->bind(GL_TEXTURE0);

        shader::postprocess::getThresholdShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getThresholdShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getThresholdShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getThresholdShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getThresholdShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::edgeDetection(glw::Texture2D* output, glw::Texture2D* in, float widthDistance, float heightDistance) {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});

        
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getModifiedEdgeDetectionShader()->bind();

        //shader::postprocess::getModifiedEdgeDetectionShader()->setSampleDistance(sampleDistance);
        shader::postprocess::getModifiedEdgeDetectionShader()->setWidthDistance(widthDistance);
        shader::postprocess::getModifiedEdgeDetectionShader()->setHeightDistance(heightDistance);

        //lightingStage->output.bind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getModifiedEdgeDetectionShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getModifiedEdgeDetectionShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getModifiedEdgeDetectionShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getModifiedEdgeDetectionShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getModifiedEdgeDetectionShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::invert(glw::Texture2D* output, glw::Texture2D* in) {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});

        
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getInvertShader()->bind();

        //lightingStage->output.bind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getInvertShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getInvertShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getInvertShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getInvertShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getInvertShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::desaturate(glw::Texture2D* output, glw::Texture2D* in, float value) {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});

        
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getDesaturateShader()->bind();

        shader::postprocess::getDesaturateShader()->setValue(value);

        //lightingStage->output.bind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getDesaturateShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getDesaturateShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getDesaturateShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getDesaturateShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getDesaturateShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::axis(glw::Texture2D* output, glw::Texture2D* in, shader::postprocess::AxisPostProcessShader::Axis axis) {
        this->frameBuffer.bind();
        this->frameBuffer.attachColorBuffer(output, GL_COLOR_ATTACHMENT0);
        this->frameBuffer.drawBuffers({GL_COLOR_ATTACHMENT0});

        
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getAxisShader()->bind();

        shader::postprocess::getAxisShader()->setAxis(axis);

        //lightingStage->output.bind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getAxisShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getAxisShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getAxisShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getAxisShader()->unbindVertexArray();

        //lightingStage->output.unbind(GL_TEXTURE0);
        in->bind(GL_TEXTURE0);

        shader::postprocess::getAxisShader()->unbind();

        this->frameBuffer.unbind();
    }

    void PostProcessingStage::outputScreenFrameBuffer(glw::Texture2D* in) {
        render::clear2D(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shader::postprocess::getCopyShader()->bind();

        in->bind(GL_TEXTURE0);

        shader::postprocess::getCopyShader()->bindVertexArray();
        screenVertices.bind();
        shader::postprocess::getCopyShader()->verticesPointer();
        screenVertices.unbind();

        screenTexCoords.bind();
        shader::postprocess::getCopyShader()->texCoordPointer();
        screenTexCoords.unbind();

        screenIndencies.bind();
        drawElements(GL_TRIANGLES, screenIndencies.count());
        screenIndencies.unbind();

        shader::postprocess::getCopyShader()->unbindVertexArray();

        in->unbind(GL_TEXTURE0);

        shader::postprocess::getCopyShader()->unbind();
    }
}

