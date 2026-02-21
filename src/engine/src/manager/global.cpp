#include "LinearMath/btVector3.h"
#include "../sys.hpp"
#include "json/value.h"
#include <fstream>

#define GLOBAL_VERSION 4
#define SCENE_VERSION 4

namespace manager {

    void Global::init() {
        if(scene) {
            scene->init(this);
        }

        if(this->behavior) {
            behavior->init(this->script, this);
        }
    }

    void Global::handleEvent(SDL_Event* e) {
        if(scene) {
            scene->handleEvent(e);
        }
    }

    void Global::update(float delta) {

        if(isSceneChange) {
            this->changeScene(scenePath);
            isSceneChange = false;
        }

        if(scene) {
            scene->update(delta);
        }

        if(behavior) {
            behavior->update(delta);
        }
    }

    void Global::render() {
        if(scene) {
            scene->render();
        }
    }

    void Global::release() {
        if(behavior) {
            behavior->release();
            delete behavior;
            behavior = nullptr;
        }
        if(scene) {
            scene->release();
        }
    }

    Json::Value Global::open_json(std::string path) {
        std::ifstream in(path);

        Json::Value root;

        in >> root;

        std::string name = root["name"].asString();
        int version = root["version"].asInt();

        if(name != "scene") {
            std::cout << path << "> Name of the file isn't scene. Will try to load\n";
        }

        if(version != SCENE_VERSION) {
            std::cout << path << "> The file is the wrong version will try to load\n";
        }

        return root;
    }

    void Global::startGame() {
        this->load(this->defaultScenePath);
    }

    void Global::load(std::string path) {
        Json::Value root = this->open_json(path);
        this->scene = new Scene();
        this->scene->load(root);
    }

    void Global::changeScene(std::string path) {
        if(this->scene) {
            this->scene->release();
            delete scene;
            scene = nullptr;
        }
        Json::Value root = this->open_json(path);
        this->scene = new Scene();
        this->scene->load(root);

        if(this->scene) {
            this->scene->init(this);
        }
    }

    void Global::global_load() {
        std::ifstream in("data/global.json");
        Json::Value root;
        in >> root;
        in.close();

        std::string name = root["name"].asString();
        int version = root["version"].asInt();

        if(!root["behavior"].isNull()) {
            this->script = root["behavior"].asString();
            this->behavior = new Behavior();
        }

        if(!root["default-scene"].isNull()) {
            this->defaultScenePath = root["default-scene"].asString();
        }
        // Input Section
        if(!root["input"].empty() || !root["input"].isNull()) {
            Json::Value _input = root["input"];

            Json::Value _mapping = _input["mapping"];

            for(int i = 0; i < _mapping.size(); i++) {
                Json::Value obj = _mapping[i];

                std::string name = obj["name"].asString();
                std::string input = obj["input"].asString();
                bool isMouse = obj["is-mouse"].asBool();

                input::mapping::Mapping mapping;

                if(isMouse) {
                    input::mapping::createMouseButtonMapping(mapping, this->mouseBouttons.at(input));
                } else {
                    input::mapping::createKeyboardMapping(mapping, this->keyboard.at(input));
                }

                this->mappings[name] = mapping;
            }
        }

        // Physics Section
        if(!root["physics"].empty() || !root["physics"].isNull()) {
            Json::Value _physics = root["physics"];

            // Gravity
            Json::Value _gravity = _physics["gravity"];
            physics::setGravity(
                btVector3(
                    _gravity["x"].asFloat(),
                    _gravity["y"].asFloat(),
                    _gravity["z"].asFloat()
                )
            );

            // Groups
            Json::Value _groups = _physics["groups"];
            if(!_groups.empty()) {
                for(int i = 0; i < _groups.size(); i++) {
                    this->groups[_groups[i].asString()] = groupIndex;
                    groupIndex += 1;
                }
            }
        }
    }

    int Global::getPhysicsGroups(std::string name) {
        return this->groups.at(name);
    }
}