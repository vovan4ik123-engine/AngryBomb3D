#pragma once

#include "EngineHeaders.h"
#include "EnumsAndVariables.h"
#include "playState/PlayStateGUILayer.h"
#include "player/Player.h"
#include "player/PlayerBulletAirTrajectory.h"
#include "enemies/BaseEnemy.h"

namespace AngryBomb3D
{
    class BaseMap
    {
    public:
        BaseMap(std::shared_ptr<PlayStateGUILayer> gui);
        virtual ~BaseMap();

        virtual void updateBeforePhysics();
        virtual void updateAfterPhysics();
        virtual void draw();

    protected:
        const std::shared_ptr<PlayStateGUILayer> m_gui;
        std::shared_ptr<Player> m_player;
        std::unique_ptr<Beryll::SkyBox> m_skyBox;

        // Only for drawing.
        std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_simpleObjDiffuse;
        std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_simpleObjNormalMap;
        std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_simpleObjForShadowMap;
        // Only for logic.
        std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_allSimpleEnv;
        // Drawing and logic.
        std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_allBullets;
        std::vector<BaseEnemy> m_allEnemies;

        struct DynamiteInfo
        {
            const std::shared_ptr<Beryll::BaseSimpleObject> obj; // Current data.
            const glm::vec3 initialPos{0.0f}; // Objects origin initial position.
            const float radius = 10.0f;
            const float power = 10.0f;
        };
        std::vector<DynamiteInfo> m_dynamiteInfo; // Dynamite have logic based on distance from initial position.

        // Shaders and light.
        std::shared_ptr<Beryll::Shader> m_simpleObjSunLightShadows;
        std::shared_ptr<Beryll::Shader> m_simpleObjSemiTransparent;
        std::shared_ptr<Beryll::Shader> m_simpleObjSunLightShadowsNormals;
        std::shared_ptr<Beryll::Shader> m_animatedObjSunLightShadows;
        std::unique_ptr<Beryll::ShadowMap> m_shadowMap;
        glm::mat4 m_sunLightVPMatrix{1.0f};
        // Assign in subclass constructor.
        glm::vec3 m_dirToSun{0.0f};
        glm::vec3 m_sunLightDir{0.0f};
        glm::vec3 m_sunPos{0.0f};
        float m_ambientLight = 0.75f;
        const float m_shadowsCubeWidthHeight = 300.0f;

        // Camera.
        float m_lastFingerMovePosX = 0.0f;
        float m_lastFingerMovePosY = 0.0f;
        float m_eyesLookAngleXZ = 0.0f; // Degrees.
        float m_eyesLookAngleY = 0.0f; // Degrees.
        glm::vec3 m_cameraOffset{0.0f};
        glm::vec3 m_cameraFront{0.0f};
        const float m_cameraXZAngleThreshold = 60.0f;

        const glm::vec3 m_worldDefaultDir{1.0f, 0.0f, 0.0f};
        glm::vec3 m_playerCurrentDir{0.0f};
        glm::vec3 m_playerPrevDir{1.0f, 0.0f, 0.0f};
        float m_playerRotatedTotalAngle = 0.0f; // Player rotated around scene.

        // Player bullet + trajectory.
        PlayerBulletAirTrajectory m_bulletTrajectory;
        glm::vec3 m_bulletImpulseVector{0.0f};
        glm::vec3 m_bulletStartPosition{0.0f};
        float m_bulletMaxThrowPower = 100.0f;

        // Load.
        void loadShaders();
        void loadPlayer();
        void loadBulletSet(const BulletType type, const int count, const float disableTimeAfterFirstCollision);
        // Logic
        void handleControls();
        void handleBulletsSets();
        virtual void handleCollisions();
        virtual void breakGlassBrick(std::shared_ptr<Beryll::BaseSimpleObject> glass);
        virtual void explodeDynamite(const DynamiteInfo& dynamite);
        void handleWinLose();
        void handleCamera();
        bool getIsDynamicObjectsActive();
        void updateSunPosition(const glm::vec3& pos);
    };
}
