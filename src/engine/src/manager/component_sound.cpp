#include "../sys.hpp"


namespace manager {
    namespace component {
        namespace sound {
            // SoundListenerComponent
            void SoundListenerComponent::init(Entity* entity) {
                this->entity = entity;
            }

            void SoundListenerComponent::handleEvent(SDL_Event* e) {

            }

            void SoundListenerComponent::update(float delta) {
                ::sound::transformListener(entity->transform);
            }

            void SoundListenerComponent::preRender() {

            }

            void SoundListenerComponent::render() {

            }

            void SoundListenerComponent::release() {
                entity = nullptr;
            }

            void SoundListenerComponent::load(Json::Value value) {
                // Listener are really simple
                // It doesn't really need any other thing hooked on them.
            }

            // SoundStreamPlayerComponent
            void SoundStreamPlayerComponent::init(Entity* entity) {
                this->entity = entity;
            }

            void SoundStreamPlayerComponent::handleEvent(SDL_Event* e) {

            }

            void SoundStreamPlayerComponent::update(float delta) {

            }

            void SoundStreamPlayerComponent::preRender() {

            }

            void SoundStreamPlayerComponent::render() {

            }

            void SoundStreamPlayerComponent::release() {
                this->entity = nullptr;
            }

            void SoundStreamPlayerComponent::load(Json::Value value) {

            }

        }
    }
}