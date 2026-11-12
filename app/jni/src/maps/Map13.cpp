#include "Map13.h"

namespace AngryBomb3D
{
    Map13::Map13(std::shared_ptr<PlayStateGUILayer> gui) : BaseMap(std::move(gui))
    {
        loadPlayer();
        loadEnv();
        loadEnemies();
        loadPlayerPoints();
        loadBulletSet(BulletType::STANDARD_1_KG, 2, 0.5f);
        m_bulletMaxThrowPower = 72.0f;

        loadShaders();
        m_eyesLookAngleXZ = 1.5659f;
        m_eyesLookAngleY = -6.49f;
        m_dirToSun = glm::normalize(glm::vec3(0.6f, 1.0f, -0.6f));
        m_sunLightDir = -m_dirToSun;
        m_skyBox = Beryll::Renderer::createSkyBox("skyboxes/whiteClouds");

        //BR_INFO(" X:%f Y:%f Z:%f", .x, .y, .z);
        //BR_INFO("%s", "");
        //BR_INFO(" %f", );
        //BR_INFO(" %d", );
    }

    Map13::~Map13()
    {

    }

    void Map13::loadEnv()
    {
        auto ground = std::make_shared<Beryll::SimpleCollidingObject>("models3D/maps/Ground_400.fbx",
                                                                      0.0f,
                                                                      false,
                                                                      Beryll::CollisionFlags::STATIC,
                                                                      EnumsAndVars::CollGr_STATIC_ENV,
                                                                      EnumsAndVars::CollGr_DYNAMIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET,
                                                                      EnumsAndVars::SceneGR_GROUND);
        ground->setFriction(1.0f);
        m_simpleObjNormalMap.push_back(ground);
        m_allSimpleEnv.push_back(ground);

        auto dynamicEnvWood = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M13_Dynamic1Wood_2kg.fbx",
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
            env->setGravity(glm::vec3{0.0f, -50.0f, 0.0f});
            env->setDamping(0.1f, 0.2f);
            m_simpleObjDiffuse.push_back(env);
            m_simpleObjForShadowMap.push_back(env);
            m_allSimpleEnv.push_back(env);
        }

        auto dynamicEnvGlass = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M13_Dynamic1Glass_2kg.fbx",
                                                                                        2.0f,
                                                                                        true,
                                                                                        Beryll::CollisionFlags::DYNAMIC,
                                                                                        EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                        EnumsAndVars::CollGr_STATIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET |
                                                                                            EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                        EnumsAndVars::SceneGR_BRICK_GLASS);
        for(const auto& env : dynamicEnvGlass)
        {
            env->setFriction(0.05f);
            env->setGravity(glm::vec3{0.0f, -50.0f, 0.0f});
            env->setDamping(0.1f, 0.2f);
            m_simpleObjDiffuse.push_back(env);
            m_simpleObjForShadowMap.push_back(env);
            m_allSimpleEnv.push_back(env);
        }

        auto dynamicEnvStone = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M13_Dynamic1Stone_2kg.fbx",
                                                                                        2.0f,
                                                                                        true,
                                                                                        Beryll::CollisionFlags::DYNAMIC,
                                                                                        EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                        EnumsAndVars::CollGr_STATIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET |
                                                                                            EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                        EnumsAndVars::SceneGR_BRICK_STONE);
        for(const auto& env : dynamicEnvStone)
        {
            env->setFriction(1.0f);
            env->setGravity(glm::vec3{0.0f, -50.0f, 0.0f});
            env->setDamping(0.1f, 0.2f);
            m_simpleObjDiffuse.push_back(env);
            m_simpleObjForShadowMap.push_back(env);
            m_allSimpleEnv.push_back(env);
        }

        auto dynamite = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M13_Dynamite1.fbx",
                                                                                 2.0f,
                                                                                 true,
                                                                                 Beryll::CollisionFlags::DYNAMIC,
                                                                                 EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                 EnumsAndVars::CollGr_STATIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET |
                                                                                    EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                                 EnumsAndVars::SceneGR_DYNAMITE);
        for(const auto& env : dynamite)
        {
            env->setFriction(1.0f);
            env->setGravity(glm::vec3{0.0f, -50.0f, 0.0f});
            env->setDamping(0.1f, 0.2f);
            m_simpleObjDiffuse.push_back(env);
            m_simpleObjForShadowMap.push_back(env);
            m_allSimpleEnv.push_back(env);

            m_dynamiteInfo.emplace_back(DynamiteInfo{env, env->getOrigin(), 6.0f, 24.0f});
        }
    }

    void Map13::loadEnemies()
    {
        auto enemies = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M13_Enemies.fbx",
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
            enemy->setGravity(glm::vec3{0.0f, -50.0f, 0.0f});
            enemy->setDamping(0.1f, 0.2f);
            m_simpleObjForShadowMap.push_back(enemy);
            m_allEnemies.emplace_back(enemy);
        }
    }

    void Map13::loadPlayerPoints()
    {
        auto points = Beryll::SimpleObject::loadManyModelsFromOneFile("models3D/maps/M13_PlayerPoints.fbx", EnumsAndVars::SceneGR_NONE);

        std::vector<glm::vec3> origins;
        origins.reserve(points.size());
        for(const auto& point : points)
        {
            origins.push_back(point->getOrigin());
            m_simpleObjDiffuse.push_back(point);
            m_simpleObjForShadowMap.push_back(point);
            m_allSimpleEnv.push_back(point);
        }

        m_player->setPlayerPoints(std::move(origins));
    }
}
