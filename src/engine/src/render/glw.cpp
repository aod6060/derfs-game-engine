#include "SDL_image.h"
#include "SDL_pixels.h"
#include "SDL_surface.h"
#include "../sys.hpp"
#include <algorithm>
#include <fstream>
#include <ios>


namespace render {
    namespace glw {
        // Shader
        void Shader::init(GLenum type, std::string path) {
            this->id = glCreateShader(type);

            std::ifstream in(path);
            in.seekg(0, std::ios::end);
            uint32_t size = in.tellg();
            in.seekg(0, std::ios::beg);
            std::string src;
            src.resize(size);
            in.read(src.data(), src.size());
            in.close();

            //std::cout << src << "\n";

            const char* c_src = src.c_str();

            glShaderSource(this->id, 1, &c_src, nullptr);

            glCompileShader(this->id);

            // Check for Errors
            int len = 0;
            glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &len);
            if(len > 0) {
                std::string err;
                err.resize(len);
                glGetShaderInfoLog(this->id, err.size(), nullptr, err.data());
                std::cout << err << "\n";
            }
        }

        void Shader::release() {
            glDeleteShader(this->id);
        }

        // Uniform
        void Uniform::init(Program* program) {
            this->program = program;
        }

        void Uniform::release() {
            this->program = nullptr;
        }
        
        void Uniform::createUniform(std::string name) {
            this->uniforms[name] = glGetUniformLocation(this->program->id, name.c_str());
        }

        // Integer
        void Uniform::uniform1i(std::string name, int32_t x) {
            glUniform1i(this->uniforms.at(name), x);
        }

        void Uniform::uniform2i(std::string name, int32_t x, int32_t y) {
            glUniform2i(this->uniforms.at(name), x, y);
        }

        void Uniform::uniform3i(std::string name, int32_t x, int32_t y, int32_t z) {
            glUniform3i(this->uniforms.at(name), x, y, z);
        }

        void Uniform::uniform4i(std::string name, int32_t x, int32_t y, int32_t z, int32_t w) {
            glUniform4i(this->uniforms.at(name), x, y, z, w);
        }

        // Float
        void Uniform::uniform1f(std::string name, float x) {
            glUniform1f(this->uniforms.at(name), x);
        }

        void Uniform::uniform2f(std::string name, float x, float y) {
            glUniform2f(this->uniforms.at(name), x, y);
        }

        void Uniform::uniform3f(std::string name, float x, float y, float z) {
            glUniform3f(this->uniforms.at(name), x, y, z);
        }

        void Uniform::uniform4f(std::string name, float x, float y, float z, float w) {
            glUniform4f(this->uniforms.at(name), x, y, z, w);
        }

        // Matricies
        void Uniform::uniformMat2(std::string name, const glm::mat2& m) {
            glUniformMatrix2fv(this->uniforms.at(name), 1, GL_FALSE, &m[0][0]);
        }

        void Uniform::uniformMat3(std::string name, const glm::mat3& m) {
            glUniformMatrix3fv(this->uniforms.at(name), 1, GL_FALSE, &m[0][0]);
        }

        void Uniform::uniformMat4(std::string name, const glm::mat4& m) {
            glUniformMatrix4fv(this->uniforms.at(name), 1, GL_FALSE, &m[0][0]);
        }


        // Attributes
        void Attribute::init(Program* program) {
            this->program = program;
            glGenVertexArrays(1, &this->id);
        }

        void Attribute::release() {
            glDeleteVertexArrays(1, &this->id);
            this->program = nullptr;
        }

        void Attribute::bind() {
            glBindVertexArray(this->id);
        }

        void Attribute::unbind() {
            glBindVertexArray(0);
        }

        void Attribute::createAttribute(std::string name, uint32_t id) {
            this->attributes[name] = id;
        }

        void Attribute::enableAttribute(std::string name) {
            //glEnableVertexAttribArray(this->attributes.at(name));
            this->enableAttribute(name, 0);
        }

        void Attribute::disableAttribute(std::string name) {
            //glDisableVertexAttribArray(this->attributes.at(name));
            this->disableAttribute(name, 0);
        }

        void Attribute::enableAttribute(std::string name, uint32_t index) {
            glEnableVertexAttribArray(this->attributes.at(name) + index);
        }

        void Attribute::disableAttribute(std::string name, uint32_t index) {
            glDisableVertexAttribArray(this->attributes.at(name) + index);
        }


        void Attribute::attributePointer(std::string name, int size, GLenum type) {
            glVertexAttribPointer(this->attributes.at(name), size, type, GL_FALSE, 0, nullptr);
        }

        void Attribute::attributePointer(std::string name, uint32_t index, int size, GLenum type, uint32_t offset, const void* pointer) {
            glVertexAttribPointer(
                attributes.at(name) + index,
                size,
                type,
                GL_FALSE,
                offset,
                pointer
            );

        }
        
        void Attribute::attributeDivisor(std::string name, uint32_t index, uint32_t divisor) {
            glVertexAttribDivisor(
                attributes.at(name) + index,
                divisor
            );
        }


        // UniformBlock
        void UniformBlock::init(Program* program) {
            this->program = program;
        }

        void UniformBlock::release() {
            this->program = nullptr;
        }

        void UniformBlock::createUniformBlock(std::string name, uint32_t index) {
            this->uniformBlocks[name] = glGetUniformBlockIndex(program->id, name.c_str());
            std::cout << "Uniform Block: " << this->uniformBlocks.at(name) << " index: " << index << "\n";
            glUniformBlockBinding(this->program->id, this->uniformBlocks.at(name), index);
        }

        // Program
        void Program::init(std::vector<Shader*> shaders) {
            this->id = glCreateProgram();

            std::for_each(shaders.begin(), shaders.end(), [&](Shader* shader) {
                glAttachShader(this->id, shader->id);
                this->shaders.push_back(shader);
            });

            glLinkProgram(this->id);

            int len = 0;
            glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &len);

            if(len > 0) {
                std::string err;
                err.resize(len);
                glGetProgramInfoLog(this->id, err.size(), nullptr, err.data());
                std::cout << err << "\n";
            }

            uniforms.init(this);
            attributes.init(this);
            this->uniformBlock.init(this);
        }

        void Program::release() {
            this->uniformBlock.release();
            attributes.release();
            uniforms.release();

            std::for_each(this->shaders.begin(), this->shaders.end(), [&](Shader* shader) {
                glDetachShader(this->id, shader->id);
            });

            shaders.clear();

            glDeleteProgram(this->id);
        }
        
        void Program::bind() {
            glUseProgram(this->id);
        }

        void Program::unbind() {
            glUseProgram(0);
        }

        // VertexBuffer
        void VertexBuffer::clear() {
            list.clear();
        }

        void VertexBuffer::add1f(float x) {
            list.push_back(x);
        }

        void VertexBuffer::add2f(float x, float y) {
            list.push_back(x);
            list.push_back(y);
        }

        void VertexBuffer::add3f(float x, float y, float z) {
            list.push_back(x);
            list.push_back(y);
            list.push_back(z);
        }

        void VertexBuffer::add4f(float x, float y, float z, float w) {
            list.push_back(x);
            list.push_back(y);
            list.push_back(z);
            list.push_back(w);
        }
        
        void VertexBuffer::addVec4(const glm::vec4& v) {
            add4f(v.x, v.y, v.z, v.w);
        }

        void VertexBuffer::addMat4(const glm::mat4& m) {
            this->addVec4(m[0]);
            this->addVec4(m[1]);
            this->addVec4(m[2]);
            this->addVec4(m[3]);
        }

        void VertexBuffer::init() {
            glGenBuffers(1, &this->id);
        }

        void VertexBuffer::release() {
            this->clear();
            glDeleteBuffers(1, &this->id);
        }

        void VertexBuffer::update() {
            this->bind();
            glBufferData(GL_ARRAY_BUFFER, this->dataSize(), list.data(), GL_DYNAMIC_DRAW);
            this->unbind();
        }

        void VertexBuffer::bind() {
            glBindBuffer(GL_ARRAY_BUFFER, this->id);
        }

        void VertexBuffer::unbind() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        size_t VertexBuffer::typeSize() {
            return sizeof(float);
        }

        size_t VertexBuffer::count() {
            return list.size();
        }

        size_t VertexBuffer::dataSize() {
            return typeSize() * count();
        }

        // IndexBuffer
        void IndexBuffer::clear() {
            this->list.clear();
        }

        void IndexBuffer::add1ui(uint32_t x) {
            list.push_back(x);
        }

        void IndexBuffer::add2ui(uint32_t x, uint32_t y) {
            list.push_back(x);
            list.push_back(y);
        }

        void IndexBuffer::add3ui(uint32_t x, uint32_t y, uint32_t z) {
            list.push_back(x);
            list.push_back(y);
            list.push_back(z);
        }

        void IndexBuffer::add4ui(uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
            list.push_back(x);
            list.push_back(y);
            list.push_back(z);
            list.push_back(w);
        }

        void IndexBuffer::init() {
            glGenBuffers(1, &this->id);
        }

        void IndexBuffer::release() {
            this->clear();
            glDeleteBuffers(1, &this->id);
        }

        void IndexBuffer::update() {
            this->bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->dataSize(), this->list.data(), GL_DYNAMIC_DRAW);
            this->unbind();
        }

        void IndexBuffer::bind() {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
        }

        void IndexBuffer::unbind() {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        size_t IndexBuffer::typeSize() {
            return sizeof(uint32_t);
        }

        size_t IndexBuffer::count() {
            return this->list.size();
        }

        size_t IndexBuffer::dataSize() {
            return this->typeSize() * this->count();
        }

        // Texture2D
        void Texture2D::init() {
            glGenTextures(1, &this->id);
        }

        void Texture2D::release() {
            glDeleteTextures(1, &this->id);
        }

        void Texture2D::bind(GLenum active) {
            glActiveTexture(active);
            glBindTexture(GL_TEXTURE_2D, this->id);
        }

        void Texture2D::unbind(GLenum active) {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Texture2D::texImage2D(int32_t level, int32_t internalFormat, size_t width, size_t height, GLenum format, GLenum type, const void* pixels) {
            glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, type, pixels);
        }

        void Texture2D::texParameter(GLenum type, int32_t value) {
            glTexParameteri(GL_TEXTURE_2D, type, value);
        }

        void Texture2D::genMipmaps() {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        void Texture2D::createTextureFromFile(Texture2D* tex, std::string path) {
            SDL_Surface* temp = IMG_Load(path.c_str());

            if(temp == nullptr) {
                std::cout << path << " doesn't exist!";
            }

            if(temp->format->format != SDL_PIXELFORMAT_ABGR8888) {
                SDL_Surface* convert = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_ABGR8888, 0);
                SDL_FreeSurface(temp);
                temp = convert;
                convert = nullptr;
            }

            tex->width = temp->w;
            tex->height = temp->h;
            
            tex->bind(GL_TEXTURE0);

            tex->texImage2D(0, GL_RGBA, temp->w, temp->h, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);

            tex->texParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            tex->texParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            tex->genMipmaps();
            
            tex->unbind(GL_TEXTURE0);

            SDL_FreeSurface(temp);
        }

        void Cubemap::init() {
            glGenTextures(1, &this->id);
        }

        void Cubemap::release() {
            glDeleteTextures(1, &this->id);
        }

        void Cubemap::bind(GLenum active) {
            glActiveTexture(active);
            glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        }

        void Cubemap::unbind(GLenum active) {
            glActiveTexture(active);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }

        void Cubemap::texParameter(GLenum type, int32_t value) {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, type, value);
        }

        void Cubemap::texImage(
            Face face,
            int32_t level,
            int32_t internalFormat,
            size_t width,
            size_t height,
            GLenum format,
            GLenum type,
            const void* pixels
        ) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 
                level, 
                internalFormat, 
                width, 
                height, 
                0, 
                format, 
                type, 
                pixels
            );
        }

        void Cubemap::createTextureFromFile(Cubemap* map, std::vector<std::string> paths) {
            for(int i = 0; i < paths.size(); i++) {
                SDL_Surface* temp = IMG_Load(paths[i].c_str());

                if(temp == nullptr) {
                    std::cout << paths[i] << " doesn't exist\n";
                }

                if(temp->format->format != SDL_PIXELFORMAT_ABGR8888) {
                    SDL_Surface* convert = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_ABGR8888, 0);
                    SDL_FreeSurface(temp);
                    temp = convert;
                    convert = nullptr;
                }

                map->bind(GL_TEXTURE0);

                map->texImage((Face)i, 0, GL_RGBA, temp->w, temp->h, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
                map->unbind(GL_TEXTURE0);

                SDL_FreeSurface(temp);
            }

            map->bind(GL_TEXTURE0);
            map->texParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            map->texParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            map->texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            map->texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            map->texParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            map->unbind(GL_TEXTURE0);
        }

        // Texture2DArray

        void Texture2DArray::init() {
            glGenTextures(1, &this->id);
        }

        void Texture2DArray::release() {
            glDeleteTextures(1, &this->id);
        }

        void Texture2DArray::bind(GLenum tex) {
            glActiveTexture(tex);
            glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
        }

        void Texture2DArray::unbind(GLenum tex) {
            glActiveTexture(tex);
            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        }

        void Texture2DArray::texParameter(GLenum type, int32_t value) {
            glTexParameteri(GL_TEXTURE_2D_ARRAY, type, value);
        }

        void Texture2DArray::genMipmaps() {
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        }

        void Texture2DArray::texStorage3D(
            size_t levels,
            GLenum internalFormat,
            size_t width,
            size_t height,
            size_t depth
        ) 
        {
            glTexStorage3D(
                GL_TEXTURE_2D_ARRAY,
                levels,
                internalFormat,
                width,
                height,
                depth
            );
        }

        void Texture2DArray::texSubImage3D(
            int32_t level,
            int32_t xoffset,
            int32_t yoffset,
            int32_t zoffset,
            size_t width,
            size_t height,
            size_t depth,
            GLenum format,
            GLenum type,
            const void* pixels   
        ) 
        {
            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                level,
                xoffset,
                yoffset,
                zoffset,
                width,
                height,
                depth,
                format,
                type,
                pixels
            );
        }

        void Texture2DArray::createTextureArrayFromFiles(
            Texture2DArray* tex,
            const std::vector<std::string>& paths
        ) 
        {
            // All Textures need to be the same width and height for a texture array
            // Handle Texture Storage
            SDL_Surface* temp = IMG_Load(paths.at(0).c_str());
            tex->bind(GL_TEXTURE0);
            //tex->texStorage3D(8, GL_RGBA, size_t width, size_t height, size_t depth)
            tex->texStorage3D(8, GL_RGBA, temp->w, temp->h, paths.size());
            tex->width = temp->w;
            tex->height = temp->h;
            tex->count = paths.size();

            // Load Texture Data
            for(int i = 0; i < paths.size(); i++) {
                SDL_Surface* s = IMG_Load(paths.at(i).c_str());
                if(s == nullptr) {
                    std::cout << i << "> " << paths.at(i) << " doesn't exist\n";
                }

                if(s->format->format != SDL_PIXELFORMAT_ABGR8888) {
                    SDL_Surface* convert = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
                    SDL_FreeSurface(s);
                    s = convert;
                    convert = nullptr;
                }

                tex->texSubImage3D(
                    0, 
                    0, 
                    0, 
                    i, 
                    tex->width, 
                    tex->height, 
                    1, 
                    GL_RGBA, 
                    GL_UNSIGNED_BYTE,
                    s->pixels);

                SDL_FreeSurface(s);
            }

            tex->texParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            tex->texParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            tex->genMipmaps();

            tex->unbind(GL_TEXTURE0);
            SDL_FreeSurface(temp);
        }

        // RenderBuffer
        void RenderBuffer::init() {
            glGenRenderbuffers(1, &this->id);
        }

        void RenderBuffer::release() {
            glDeleteRenderbuffers(1, &this->id);
        }

        void RenderBuffer::bind() {
            glBindRenderbuffer(GL_RENDERBUFFER, this->id);
        }

        void RenderBuffer::unbind() {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        void RenderBuffer::update(GLenum type, uint32_t width, uint32_t height) {
            this->width = width;
            this->height = height;
            glRenderbufferStorage(GL_RENDERBUFFER, type, this->width, this->height);
        }

        // FrameBuffer
        void FrameBuffer::init() {
            glGenFramebuffers(1, &this->id);
        }

        void FrameBuffer::release() {
            glDeleteFramebuffers(1, &this->id);
        }

        void FrameBuffer::bind() {
            glBindFramebuffer(GL_FRAMEBUFFER, this->id);
        }

        void FrameBuffer::unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void FrameBuffer::attachDepthBuffer(Texture2D* tex) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->id, 0);
        }

        void FrameBuffer::attachDepthBuffer(RenderBuffer* renderBuffer) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer->id);
        }

        void FrameBuffer::attachColorBuffer(Texture2D* tex, GLenum attachment) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex->id, 0);
        }

        void FrameBuffer::drawBuffers(const std::vector<GLenum>& attachment) {
            glDrawBuffers(attachment.size(), attachment.data());
        }

        bool FrameBuffer::wasCreated() {
            return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        }

    }
}