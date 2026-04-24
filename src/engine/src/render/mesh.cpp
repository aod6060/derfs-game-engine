#include "render_hidden.hpp"


namespace render {
    namespace mesh {
        void Mesh::init(std::string path) {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
            const aiMesh* mesh = scene->mMeshes[0];

            for(int i = 0; i < mesh->mNumVertices; i++) {
                Vertex v;
                // Postion
                v.position = glm::vec3(
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
                );

                // Normals
                v.normal = glm::vec3(
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                );
                // AlbedoTexCoord
                v.albedoTexCoord = glm::vec2(
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                );
                // MetalTexCoords
                v.metalTexCoord = glm::vec2(
                    mesh->mTextureCoords[1][i].x,
                    mesh->mTextureCoords[1][i].y
                );
                // RoughnessTexCoords
                v.roughnessTexCoord = glm::vec2(
                    mesh->mTextureCoords[2][i].x,
                    mesh->mTextureCoords[2][i].y
                );
                // EmissiveTexCoords
                v.emissiveTexCoord = glm::vec2(
                    mesh->mTextureCoords[3][i].x,
                    mesh->mTextureCoords[3][i].y
                );
                // LitTexCoords
                v.litTexCoord = glm::vec2(
                    mesh->mTextureCoords[4][i].x,
                    mesh->mTextureCoords[4][i].y
                );

                this->data.vertices.push_back(v);
            }

            for(int i = 0; i < mesh->mNumFaces; i++) {
                Triangle t;
                t.v1 = mesh->mFaces[i].mIndices[0];
                t.v2 = mesh->mFaces[i].mIndices[1];
                t.v3 = mesh->mFaces[i].mIndices[2];
                this->data.triangles.push_back(t);
            }

            vertices.init();
            normals.init();
            //texCoords.init();
            albedoTexCoords.init();
            metalTexCoords.init();
            roughnessTexCoords.init();
            emissiveTexCoords.init();
            litTexCoords.init();

            for(int i = 0; i < this->data.vertices.size(); i++) {
                vertices.add3f(data.vertices[i].position.x, data.vertices[i].position.y, data.vertices[i].position.z);
                normals.add3f(data.vertices[i].normal.x, data.vertices[i].normal.y, data.vertices[i].normal.z);
                //texCoords.add2f(data.vertices[i].texCoord.x, data.vertices[i].texCoord.y);
                albedoTexCoords.add2f(data.vertices[i].albedoTexCoord.x, data.vertices[i].albedoTexCoord.y);
                metalTexCoords.add2f(data.vertices[i].metalTexCoord.x, data.vertices[i].metalTexCoord.y);
                roughnessTexCoords.add2f(data.vertices[i].roughnessTexCoord.x, data.vertices[i].roughnessTexCoord.y);
                emissiveTexCoords.add2f(data.vertices[i].emissiveTexCoord.x, data.vertices[i].emissiveTexCoord.y);
                litTexCoords.add2f(data.vertices[i].litTexCoord.x, data.vertices[i].litTexCoord.y);
            }

            vertices.update();
            normals.update();
            //texCoords.update();
            albedoTexCoords.update();
            metalTexCoords.update();
            roughnessTexCoords.update();
            emissiveTexCoords.update();
            litTexCoords.update();

            this->indencies.init();

            for(int i = 0; i < this->data.triangles.size(); i++) {
                this->indencies.add3ui(
                    data.triangles[i].v1,
                    data.triangles[i].v2,
                    data.triangles[i].v3
                );
            }

            this->indencies.update();
        }

        void Mesh::release() {
            indencies.release();
            //texCoords.release();
            albedoTexCoords.release();
            metalTexCoords.release();
            roughnessTexCoords.release();
            emissiveTexCoords.release();
            litTexCoords.release();

            normals.release();
            vertices.release();
        }

    }
}