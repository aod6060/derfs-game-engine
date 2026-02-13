#include "lua/lauxlib.hpp"
#include "lua/lua.hpp"
#include "lua/lualib.hpp"
#include "../sys.hpp"
#include "json/value.h"
#include <fstream>
#include <vector>

#define PREFAB_VERSION 1

namespace manager {
    void Entity::init(Scene* scene) {
        this->scene = scene;

        this->transform.init(this);

        for(int i = 0; i < this->childeren.size(); i++) {
            this->childeren.at(i)->init(scene);
        }

        if(this->cameraComponent) {
            this->cameraComponent->init(this);
        }

        if(this->meshComponent) {
            this->meshComponent->init(this);
        }

        if(this->behavior) {
            this->behavior->init(this->script, this);
        }
    }

    void Entity::handleEvent(SDL_Event* e) {
        if(this->childeren.size() > 0) {
            for(int i = 0; i < childeren.size(); i++) {
                this->childeren.at(i)->handleEvent(e);
            }
        }

        if(this->cameraComponent) {
            this->cameraComponent->handleEvent(e);
        }

        if(this->meshComponent) {
            this->meshComponent->handleEvent(e);
        }


    }

    void Entity::update(float delta) {
        std::vector<Entity*>::iterator it = this->childeren.begin();

        while(it != this->childeren.end()) {
            if((*it)->needRemoval) {
                (*it)->release();
                delete (*it);
                childeren.erase(it);
                continue;
            } else {
                (*it)->update(delta);
                it++;
            }
        }
        
        if(this->cameraComponent) {
            this->cameraComponent->update(delta);
        }

        if(this->meshComponent) {
            this->meshComponent->update(delta);
        }

        if(this->behavior) {
            this->behavior->update(delta);
        }
    }

    void Entity::preRender() {
        if(this->childeren.size() > 0) {
            for(int i = 0; i < childeren.size(); i++) {
                this->childeren.at(i)->preRender();
            }
        }

        if(this->cameraComponent) {
            this->cameraComponent->preRender();
        }

        if(this->meshComponent) {
            this->meshComponent->preRender();
        }
    }

    void Entity::render() {
        if(this->childeren.size() > 0) {
            for(int i = 0; i < childeren.size(); i++) {
                this->childeren.at(i)->render();
            }
        }

        if(this->cameraComponent) {
            this->cameraComponent->render();
        }

        if(this->meshComponent) {
            this->meshComponent->render();
        }
    }

    void Entity::release() {
        if(this->childeren.size() > 0) {
            for(int i = 0; i < childeren.size(); i++) {
                this->childeren.at(i)->release();
                delete this->childeren[i];
                this->childeren[i] = nullptr;
            }
            this->childeren.clear();
        }

        if(this->cameraComponent) {
            this->cameraComponent->release();
            delete this->cameraComponent;
        }

        if(this->meshComponent) {
            this->meshComponent->release();
            delete this->meshComponent;
        }

        if(this->behavior) {
            this->behavior->release();
            delete behavior;
        }

        this->transform.release();

        this->scene = nullptr;
    }

    void Entity::handleEntity(Json::Value value) {
        if(!value["name"].empty()) {
            this->name = value["name"].asString();
        }

        if(!value["behavior"].empty() && !value["behavior"].isNull()) {
            this->script = value["behavior"].asString();
            this->behavior = new Behavior();
        }

        // Transform

        if(!value["transform"].empty()) {
            transform.load(value["transform"]);
        }


        if(!value["components"].empty()) {
            Json::Value components = value["components"];

            for(int i = 0; i < components.size(); i++) {
                Json::Value comp = components[i];
                std::string type = comp["type"].asString();

                if(type == "mesh-component") {
                    this->meshComponent = new component::MeshComponent();
                    this->meshComponent->load(comp);
                } else if(type == "camera-component") {
                    this->cameraComponent = new component::CameraComponent();
                    this->cameraComponent->load(comp);
                }
            }
        }

        if(!value["entities"].empty()) {
            Json::Value entities = value["entities"];

            for(int i = 0; i < entities.size(); i++) {
                Json::Value entity = entities[i];
                Entity* temp = new Entity();
                temp->load(entity);
                temp->parent = this;
                this->childeren.push_back(temp);
            }
        }
    }

    void Entity::loadPrefab(std::string path) {
        std::ifstream in = std::ifstream(path);
        Json::Value root;
        in >> root;
        in.close();

        std::string name = root["name"].asString();
        int version = root["version"].asInt();

        if(name != "prefab") {
            std::cout << path << "> Name of the file isn't prefab. Will try to load\n";
        }

        if(version != PREFAB_VERSION) {
            std::cout << path << "> The file is the wrong version will try to load\n";
        }

        this->handleEntity(root["entity"]);
    }

    void Entity::load(Json::Value value) {
        this->type = value["type"].asString();

        if(this->type == "instance") {
            this->handleEntity(value);
        } else if(this->type == "prefab") {
            std::string path = value["path"].asString();
            loadPrefab(path);
            // This is for overides and additions
            handleEntity(value);
        }
    }

    bool Entity::hasParent() {
        return this->parent != nullptr;
    }

    glm::mat4 Entity::toParentMatrix(Entity* entity) {
        if(entity == nullptr) {
            return glm::mat4(1.0f);
        }

        glm::mat4 m = entity->transform.toModel();
        if(entity->hasParent()) {
            m = this->toParentMatrix(entity->parent) * m;
        }
        return m;
    }

    glm::vec3 Entity::getGlobalPosition() {
        glm::vec4 position = glm::vec4(this->transform.position, 1.0f);
        position = this->toParentMatrix(this->parent) * position;
        return glm::vec3(position.x, position.y, position.z);
    }

    void Entity::addChildEntity(Entity* entity) {
        entity->parent = this;
        entity->init(this->scene);
        this->childeren.push_back(entity);
    }

    Entity* Entity::getChildEntity(size_t index) {
        return this->childeren.at(index);
    }

    size_t Entity::getChilderenAmount() {
        return this->childeren.size();
    }

    
    void Entity::removeEntity(Entity* entity) {
        entity->needRemoval = true;
    }
}