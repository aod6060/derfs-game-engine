#include "lua/lauxlib.hpp"
#include "lua/lua.hpp"
#include "lua/lualib.hpp"
#include "../sys.hpp"
#include "manager.hpp"
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

        this->componentIterator([&](component::IComponent* comp) {
            comp->init(this);
        });
        /*
        if(this->behavior) {
            this->behavior->init(this->script, this);
        }
        */
        if(this->behavior) {
            ((behavior::EntityBehavior*)this->behavior)->init(this);
        }
    }

    void Entity::postInit() {
        if(this->childeren.size() > 0) {
            for(int i = 0; i < childeren.size(); i++) {
                this->childeren.at(i)->postInit();
            }
        }

        if(this->behavior) {
            this->behavior->ready();
        }
    }

    void Entity::handleEvent(SDL_Event* e) {
        if(this->childeren.size() > 0) {
            for(int i = 0; i < childeren.size(); i++) {
                this->childeren.at(i)->handleEvent(e);
            }
        }

        this->componentIterator([&](component::IComponent* comp) {
            comp->handleEvent(e);
        });
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

        this->componentIterator([&](component::IComponent* comp) {
            comp->update(delta);
        });

        /*
        if(this->behavior) {
            this->behavior->update(delta);
        }
        */
        if(this->behavior) {
            std::cout << this->name << "\n";
            this->behavior->update(delta);
        }
    }

    void Entity::preRender() {
        if(this->childeren.size() > 0) {
            for(int i = 0; i < childeren.size(); i++) {
                this->childeren.at(i)->preRender();
            }
        }

        this->componentIterator([&](component::IComponent* comp) {
            comp->preRender();
        });
    }

    void Entity::render() {
        if(this->visible) {
            if(this->childeren.size() > 0) {
                for(int i = 0; i < childeren.size(); i++) {
                    this->childeren.at(i)->render();
                }
            }

            this->componentIterator([&](component::IComponent* comp) {
                comp->render();
            });
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

        this->componentIterator([&](component::IComponent* comp) {
            comp->release();
            delete comp;
            comp = nullptr;
        });

        this->components.clear();

        // if(this->behavior) {
        //     this->behavior->release();
        //     delete behavior;
        // }
        if(this->behavior) {
            this->behavior->release();
            delete this->behavior;
            this->behavior = nullptr;
        }
        this->transform.release();

        this->scene = nullptr;
    }

    void Entity::handleEntity(Json::Value value) {
        if(!value["name"].empty()) {
            this->name = value["name"].asString();
        }

        // if(!value["behavior"].empty() && !value["behavior"].isNull()) {
        //     this->script = value["behavior"].asString();
        //     this->behavior = new Behavior();
        // }

        if(!value["behavior"].empty() && !value["behavior"].isNull()) {
            this->behaviorName = value["behavior"].asString();
            this->behavior = manager::behavior::create(this->behaviorName);
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
                component::componentFactory(this, type, comp);
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


    void Entity::componentIterator(std::function<void(component::IComponent* comp)> callback) {
        for(std::map<std::string, component::IComponent*>::iterator it = this->components.begin(); it != this->components.end(); it++) {
            callback(it->second);
        }
    }
}