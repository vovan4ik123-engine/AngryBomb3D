#include "Map1.h"

namespace AngryBomb3D
{
    Map1::Map1(std::shared_ptr<PlayStateGUILayer> gui) : BaseMap(std::move(gui))
    {
        loadPlayer();
        loadEnv();
        loadEnemies();
        loadPlayerPoints();
        loadBulletSet(BulletType::STANDARD_1_KG, 1, 3.0f);
        loadBulletSet(BulletType::STANDARD_1_KG, 13, 3.0f);
        loadBulletSet(BulletType::STANDARD_1_KG, 4, 3.0f);
        loadBulletSet(BulletType::STANDARD_1_KG, 2, 3.0f);
        loadBulletSet(BulletType::STANDARD_1_KG, 10, 3.0f);

        loadShaders();
        m_eyesLookAngleXZ = -9.0f;
        m_dirToSun = glm::normalize(glm::vec3(-0.5f, 1.0f, -0.6f));
        m_sunLightDir = -m_dirToSun;
        m_skyBox = Beryll::Renderer::createSkyBox("skyboxes/whiteClouds");

        //BR_INFO(" X:%f Y:%f Z:%f", .x, .y, .z);
        //BR_INFO("%s", "");
        //BR_INFO(" %f", );
        //BR_INFO(" %d", );
    }

    Map1::~Map1()
    {

    }

    void Map1::loadEnv()
    {
        auto ground = std::make_shared<Beryll::SimpleCollidingObject>("models3D/mapCommon/Ground_500.fbx",
                                                                                     0.0f,
                                                                                     false,
                                                                                     Beryll::CollisionFlags::STATIC,
                                                                                     Beryll::CollisionGroups::STATIC_ENVIRONMENT,
                                                                                     Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER_BULLET,
                                                                                     Beryll::SceneObjectGroups::GROUND);
        ground->setFriction(1.0f);
        m_allEnv.push_back(ground);

        auto dynamicEnv1 = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/map1/Dynamic_1kg_1friction.fbx",
                                                                                    1.0f,
                                                                                    false,
                                                                                    Beryll::CollisionFlags::DYNAMIC,
                                                                                    Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                    Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER_BULLET |
                                                                                    Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                    Beryll::SceneObjectGroups::DYNAMIC_ENVIRONMENT);
        for(const auto& env : dynamicEnv1)
        {
            env->setFriction(1.0f);
            env->setGravity(glm::vec3{0.0f, -45.0f, 0.0f});
            m_simpleObjForShadowMap.push_back(env);
            m_animatedOrDynamicObjects.push_back(env);
            m_allEnv.push_back(env);
        }
    }

    void Map1::loadEnemies()
    {
        auto enemies = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/map1/Enemies.fbx",
                                                                                1.0f,
                                                                                true,
                                                                                Beryll::CollisionFlags::DYNAMIC,
                                                                                Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER_BULLET |
                                                                                Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                Beryll::SceneObjectGroups::DYNAMIC_ENVIRONMENT);
        for(const auto& enemy : enemies)
        {
            enemy->setFriction(0.5f);
            enemy->setGravity(glm::vec3{0.0f, -45.0f, 0.0f});
            m_simpleObjForShadowMap.push_back(enemy);
            m_animatedOrDynamicObjects.push_back(enemy);
            m_allEnemies.emplace_back(enemy);
        }
    }

    void Map1::loadPlayerPoints()
    {
        auto points = Beryll::SimpleObject::loadManyModelsFromOneFile("models3D/map1/PlayerPoints.fbx", Beryll::SceneObjectGroups::GROUND);

        std::vector<glm::vec3> origins;
        origins.reserve(points.size());
        for(const auto& point : points)
        {
            origins.push_back(point->getOrigin());
            m_simpleObjForShadowMap.push_back(point);
            m_allEnv.push_back(point);
        }

        m_player->setPlayerPoints(std::move(origins));
    }
}
