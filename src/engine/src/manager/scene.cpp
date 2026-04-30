#include "manager_hidden.hpp"


namespace manager {
    void Scene::init(Global* global) {
        this->global = global;
        
        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->init(this);
        }
        
        if(this->behavior) {
            ((behavior::SceneBehavior*)this->behavior)->init(this);
        }
    }

    void Scene::postInit() {
        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->postInit();
        }

        if(this->behavior) {
            this->behavior->ready();
        }
    }

    void Scene::handleEvent(SDL_Event* e) {
        for(int i = 0; i < this->entities.size(); i++) {
            this->entities[i]->handleEvent(e);
        }
    }

    void Scene::update(float delta) {
        std::vector<Entity*>::iterator it = this->entities.begin();

        int i = 0;

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
        // if(this->behavior) {
        //     this->behavior->update(delta);
        // }
        if(this->behavior) {
            this->behavior->update(delta);
        }
    }

    void Scene::render() {
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

        // if(this->behavior) {
        //     this->behavior->release();
        //     delete this->behavior;
        //     this->behavior = nullptr;
        // }

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
        // if(!value["behavior"].isNull()) {
        //     this->script = value["behavior"].asString();
        //     this->behavior = new Behavior();
        // }

        if(!value["behavior"].isNull()) {
            this->behaviorName = value["behavior"].asString();
            this->behavior = behavior::create(this->behaviorName);
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