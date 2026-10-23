#include "Map2.h"

namespace AngryBomb3D
{
    Map2::Map2(std::shared_ptr<PlayStateGUILayer> gui) : BaseMap(std::move(gui))
    {
        loadPlayer();
        loadEnv();
        loadEnemies();
        loadPlayerPoints();
        loadBulletSet(BulletType::STANDARD_1_KG, 1, 3.0f);

        loadShaders();
        m_eyesLookAngleXZ = 5.0f;
        m_eyesLookAngleY = 1.247f;
        m_dirToSun = glm::normalize(glm::vec3(0.6f, 1.0f, -0.6f));
        m_sunLightDir = -m_dirToSun;
        m_skyBox = Beryll::Renderer::createSkyBox("skyboxes/whiteClouds");

        //BR_INFO(" X:%f Y:%f Z:%f", .x, .y, .z);
        //BR_INFO("%s", "");
        //BR_INFO(" %f", );
        //BR_INFO(" %d", );
    }

    Map2::~Map2()
    {

    }

    void Map2::loadEnv()
    {
        auto ground = std::make_shared<Beryll::SimpleCollidingObject>("models3D/maps/Ground_400.fbx",
                                                                      0.0f,
                                                                      false,
                                                                      Beryll::CollisionFlags::STATIC,
                                                                      EnumsAndVars::CollGr_STATIC_ENV,
                                                                      EnumsAndVars::CollGr_DYNAMIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET,
                                                                      EnumsAndVars::SceneGR_GROUND);
        ground->setFriction(1.0f);
        m_allEnv.push_back(ground);

        auto dynamicEnvWood = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M2_Dynamic1Wood_2kg.fbx",
                                                                                        2.0f,
                                                                                        true,
                                                                                        Beryll::CollisionFlags::DYNAMIC,
                                                                                        EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                        EnumsAndVars::CollGr_STATIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET |
                                                                                            EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                        EnumsAndVars::SceneGR_BRICK_WOOD);
        for(const auto& env : dynamicEnvWood)
        {
            env->setFriction(1.0f);
            env->setGravity(glm::vec3{0.0f, -55.0f, 0.0f});
            env->setDamping(0.1f, 0.2f);
            m_simpleObjForShadowMap.push_back(env);
            m_animatedOrDynamicObjects.push_back(env);
            m_allEnv.push_back(env);
        }
    }

    void Map2::loadEnemies()
    {
        auto enemies = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M2_Enemies.fbx",
                                                                                0.1f,
                                                                                true,
                                                                                Beryll::CollisionFlags::DYNAMIC,
                                                                                EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                EnumsAndVars::CollGr_STATIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET |
                                                                                    EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                EnumsAndVars::SceneGR_ENEMY);
        for(const auto& enemy : enemies)
        {
            enemy->setFriction(1.0f);
            enemy->setGravity(glm::vec3{0.0f, -55.0f, 0.0f});
            enemy->setDamping(0.1f, 0.2f);
            m_simpleObjForShadowMap.push_back(enemy);
            m_animatedOrDynamicObjects.push_back(enemy);
            m_allEnemies.emplace_back(enemy);
        }
    }

    void Map2::loadPlayerPoints()
    {
        auto points = Beryll::SimpleObject::loadManyModelsFromOneFile("models3D/maps/M2_PlayerPoints.fbx", EnumsAndVars::SceneGR_NONE);

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
