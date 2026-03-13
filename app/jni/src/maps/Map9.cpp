#include "Map9.h"

namespace AngryBomb3D
{
    Map9::Map9(std::shared_ptr<PlayStateGUILayer> gui) : BaseMap(std::move(gui))
    {
        loadPlayer();
        loadEnv();
        loadEnemies();
        loadPlayerPoints();
        loadBulletSet(BulletType::STANDARD_1_KG, 1, 1.0f);

        loadShaders();
        m_dirToSun = glm::normalize(glm::vec3(0.5f, 1.5f, 1.0f));
        m_sunLightDir = -m_dirToSun;
        m_skyBox = Beryll::Renderer::createSkyBox("skyboxes/whiteClouds");

        //BR_INFO(" X:%f Y:%f Z:%f", .x, .y, .z);
        //BR_INFO("%s", "");
        //BR_INFO(" %f", );
        //BR_INFO(" %d", );
    }

    Map9::~Map9()
    {

    }

    void Map9::loadEnv()
    {
        auto ground = std::make_shared<Beryll::SimpleCollidingObject>("models3D/mapCommon/Ground1.fbx",
                                                                                     0.0f,
                                                                                     false,
                                                                                     Beryll::CollisionFlags::STATIC,
                                                                                     Beryll::CollisionGroups::STATIC_ENVIRONMENT,
                                                                                     Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER_BULLET,
                                                                                     Beryll::SceneObjectGroups::GROUND);
        ground->setFriction(0.5f);
        m_allEnv.push_back(ground);

        auto dynamicEnv1 = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/map9/Dynamic_1kg_0_5friction.fbx",
                                                                                    1.0f,
                                                                                    false,
                                                                                    Beryll::CollisionFlags::DYNAMIC,
                                                                                    Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                    Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER_BULLET |
                                                                                    Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                    Beryll::SceneObjectGroups::DYNAMIC_ENVIRONMENT);
        for(const auto& env : dynamicEnv1)
        {
            env->setFriction(0.5f);
            m_simpleObjForShadowMap.push_back(env);
            m_animatedOrDynamicObjects.push_back(env);
            m_allEnv.push_back(env);
        }

        auto dynamicEnv2 = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/map9/Dynamic_10kg_0_5friction.fbx",
                                                                                    10.0f,
                                                                                    false,
                                                                                    Beryll::CollisionFlags::DYNAMIC,
                                                                                    Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                    Beryll::CollisionGroups::STATIC_ENVIRONMENT | Beryll::CollisionGroups::PLAYER_BULLET |
                                                                                    Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                    Beryll::SceneObjectGroups::DYNAMIC_ENVIRONMENT);
        for(const auto& env : dynamicEnv2)
        {
            env->setFriction(0.5f);
            m_simpleObjForShadowMap.push_back(env);
            m_animatedOrDynamicObjects.push_back(env);
            m_allEnv.push_back(env);
        }
    }

    void Map9::loadEnemies()
    {
        auto enemies = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/map9/Enemies.fbx",
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
            m_simpleObjForShadowMap.push_back(enemy);
            m_animatedOrDynamicObjects.push_back(enemy);
            m_allEnemies.emplace_back(enemy);
        }
    }

    void Map9::loadPlayerPoints()
    {
        auto points = Beryll::SimpleObject::loadManyModelsFromOneFile("models3D/map9/PlayerPoints.fbx", Beryll::SceneObjectGroups::GROUND);

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
