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

        // Load.
        void loadShaders();
        void loadPlayer();
        void loadBulletSet(const BulletType type, const int count, const float disableTimeAfterFirstCollision);

        void handleControls();
        void handleBulletsSets();
        void handleCollisions();
        void handleWinLose();
        void handleCamera();

        bool getIsDynamicObjectsMoves();

        void updateSunPosition(const glm::vec3& pos);

    protected:
        const std::shared_ptr<PlayStateGUILayer> m_gui;
        std::shared_ptr<Player> m_player;
        std::unique_ptr<Beryll::SkyBox> m_skyBox;

        std::vector<std::shared_ptr<Beryll::SceneObject>> m_animatedOrDynamicObjects;
        std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_allEnv;
        std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_allBullets;
        std::vector<BaseEnemy> m_allEnemies;
        std::vector<std::shared_ptr<Beryll::BaseSimpleObject>> m_simpleObjForShadowMap;

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
        const float m_shadowsCubeWidthHeight = 400.0f;

        // Camera.
        float m_lastFingerMovePosX = 0.0f;
        float m_lastFingerMovePosY = 0.0f;
        float m_eyesLookAngleXZ = 0.0f; // Degrees.
        float m_eyesLookAngleY = -10.0f; // Degrees.
        glm::vec3 m_cameraOffset{0.0f};
        glm::vec3 m_cameraFront{0.0f};
        const float m_cameraXZAngleThreshold = 45.0f;

        const glm::vec3 m_worldDefaultDir{1.0f, 0.0f, 0.0f};
        glm::vec3 m_playerCurrentDir{0.0f};
        glm::vec3 m_playerPrevDir{1.0f, 0.0f, 0.0f};
        float m_playerRotatedTotalAngle = 0.0f; // Player rotated around scene.

        // Player bullet + trajectory.
        PlayerBulletAirTrajectory m_bulletTrajectory;
        glm::vec3 m_bulletImpulseVector{0.0f};
        glm::vec3 m_bulletStartPosition{0.0f};
    };
}
