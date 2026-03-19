#include "lua/lauxlib.hpp"
#include "lua/lua.hpp"
#include "lua/lualib.hpp"
#include "../sys.hpp"
#include "json/value.h"
#include <vector>


namespace manager {
    void Scene::init(Global* global) {
        this->global = global;

        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->init(this);
        }

        if(this->behavior) {
            this->behavior->init(this->script, this);
        }
    }

    void Scene::handleEvent(SDL_Event* e) {
        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->handleEvent(e);
        }
    }

    void Scene::update(float delta) {
        std::vector<Entity*>::iterator it = this->entities.begin();

        while(it != this->entities.end()) {
            if((*it)->needRemoval) {
                (*it)->release();
                delete (*it);
                entities.erase(it);
                continue;
            } else {
                (*it)->update(delta);
                it++;
            }
        }

        //camera.update(delta);
        if(this->behavior) {
            this->behavior->update(delta);
        }
    }

    void Scene::render() {
        /*
        render::clear(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

        render::shader::geometry::getMeshShader()->bind();

        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->preRender();
        }

        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->render();
        }

        render::shader::geometry::getMeshShader()->unbind();
        */

        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->preRender();
        }

        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->render();
        }

        render::present();
    }

    void Scene::release() {
        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->release();
        }
        this->entities.clear();

        if(this->behavior) {
            this->behavior->release();
            delete this->behavior;
            this->behavior = nullptr;
        }

        this->global = nullptr;
    }

    void Scene::load(Json::Value value) {
        // Camera
        //this->camera.load(value["camera"]);
        if(!value["behavior"].isNull()) {
            this->script = value["behavior"].asString();
            this->behavior = new Behavior();
        }

        // Entity
        Json::Value entities = value["entities"];
        for(int i = 0; i < entities.size(); i++) {
            Json::Value obj = entities[i];

            Entity* temp = new Entity();

            temp->load(obj);

            this->entities.push_back(temp);
        }
    }

    void Scene::addEntity(Entity* entity) {
        entity->init(this);
        this->entities.push_back(entity);
    }

    void Scene::removeEntity(Entity* entity) {
        entity->needRemoval = true;
    }
}