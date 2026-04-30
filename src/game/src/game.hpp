#ifndef GAME_HPP
#define GAME_HPP

#include "../../engine/src/sys.hpp"

namespace game {

    struct GameApp : app::IApp {
        manager::Global global;

        //float time = 0.0f;
        //float maxTime = 1.0f;

        virtual void init();
        virtual void handleEvent(SDL_Event* e);
        virtual void update(float delta);
        virtual void render();
        virtual void release();
    };

    void setup(app::Config* config, GameApp* app);

    namespace behavior {
        struct AbstractPhysicsTestScene;

        // Entity
        struct CrateTestEntity : public manager::behavior::EntityBehavior {
            manager::component::physics::DynamicBodyComponent* bodyComponent = nullptr;

            float time = 0.0f;
            float maxTime = 1.0f;

            glm::vec3 min = glm::vec3(-20.0f, 20.0f, -2.0f);
            glm::vec3 max = glm::vec3(20.0f, 60.0f, 20.0f);

            virtual void ready();
            virtual void update(float delta);
            virtual void release();

            void reset();
        };

        struct PlayerFPSEntity : public manager::behavior::EntityBehavior {
            float moveSpeed = 8.0f;
            float jumpSpeed = 10.0f;

            glm::vec3 min = glm::vec3(-20.0f, 20.0f, -20.0f);
            glm::vec3 max = glm::vec3(20.0f, 60.0f, 20.0f);
            
            manager::Scene* scene = nullptr;
            //manager::behavior::SceneBehavior* sceneBehavior = nullptr;
            AbstractPhysicsTestScene* sceneBehavior = nullptr;
            manager::Global* global = nullptr;
            manager::component::physics::RayCastComponent* rayCast = nullptr;
            manager::Entity* yPivotEntity = nullptr;
            manager::Entity* pivotEntity = nullptr;
            manager::component::physics::PushArmComponent* pivotEntityPushArm = nullptr;
            manager::Entity* cameraEntity = nullptr;
            manager::Entity* meshEntity = nullptr;
            manager::component::physics::DynamicBodyComponent* bodyComponent = nullptr;

            float time = 0.0f;
            
            float tdistance = 20.0f;
            float fdistance = 0.0f;
            float cdistance = tdistance;

            bool toggleFPS = false;
            bool animatedCamera = false;
            float animateTime = 0.0f;
            float maxAnimateTime = 1.0f;
            
            virtual void ready();
            virtual void update(float delta);
            virtual void release();
            void reset();
            float lerp(float v0, float v1, float t);
            float clamp(float min, float max, float value);
        };


        // Scene
        struct AbstractPhysicsTestScene : public manager::behavior::SceneBehavior {
            std::string prefab = "data/prefabs/crate.prefab.json";

            glm::vec3 min = glm::vec3(-20.0f, 20.0f, -20.0f);
            glm::vec3 max = glm::vec3(20.0f, 60.0f, 20.0f);

            int amount = 100;

            manager::Entity* cratesEntity = nullptr;

            std::string nextScene;

            virtual void ready();
            virtual void update(float delta);
            virtual void release();

            virtual void setNextScene() = 0;
        };

        struct DirectionPhysicsTestScene : public AbstractPhysicsTestScene {
            virtual void setNextScene();
        };

        struct PointPhysicsTestScene : public AbstractPhysicsTestScene {
            virtual void setNextScene();
        };

        struct SpotPhysicsTestScene : public AbstractPhysicsTestScene {
            virtual void setNextScene();
        };
    }

}

#endif