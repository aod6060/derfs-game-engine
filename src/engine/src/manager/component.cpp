#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "../sys.hpp"
#include "json/value.h"
#include <functional>
#include <iostream>


namespace manager {
    namespace component {
        static std::map<std::string, std::function<IComponent*()>> _componentSetup = 
        {
            {
                "mesh-component",
                []() {
                    return new render::MeshComponent();
                }
            },
            {
                "camera-component",
                []() {
                    return new render::CameraComponent();
                }
            },
            {
                "static-body-component",
                []() {
                    return new physics::StaticBodyComponent();
                }
            },
            {
                "dynamic-body-component",
                []() {
                    return new physics::DynamicBodyComponent();
                }
            },
            {
                "kinematic-body-component",
                []() {
                    return new physics::KinematicBodyComponent();
                }
            },
            {
                "trigger-component",
                []() {
                    return new physics::TriggerComponent();
                }
            },
            {
                "push-arm-component",
                []() {
                    return new physics::PushArmComponent();
                }
            },
            {
                "ray-cast-component",
                []() {
                    return new physics::RayCastComponent();
                }
            },
            {
                "sound-listener-component",
                []() {
                    return new sound::SoundListenerComponent();
                }
            },
            {
                "music-stream-player-component",
                []() {
                    return new sound::MusicStreamPlayerComponent();
                }
            },
            {
                "sound-player-component",
                []() {
                    return new sound::SoundPlayerComponent();
                }
            }
        };

        void componentFactory(Entity* entity, std::string type, Json::Value value) {
            if(_componentSetup.find(type) != _componentSetup.end()) {
                //_componentSetup.at(type)(entity, type, value);
                entity->components[type] = _componentSetup.at(type)();
                entity->components.at(type)->load(value);
                return;
            } else {
                std::cout << "\"" << type << "\" isn't a component type!\n";
                return;
            }
        }
    }
}