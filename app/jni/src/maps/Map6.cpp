#include "Map6.h"

namespace AngryBomb3D
{
    Map6::Map6(std::shared_ptr<PlayStateGUILayer> gui) : BaseMap(std::move(gui))
    {
        loadPlayer();
        loadEnv();
        loadEnemies();
        loadPlayerPoints();
        loadBulletSet(BulletType::STANDARD_1_KG, 1, 3.0f);
        m_bulletMaxThrowPower = 150.0f;

        loadShaders();
        m_eyesLookAngleXZ = -9.4952f;
        m_eyesLookAngleY = 0.4635f;
        m_dirToSun = glm::normalize(glm::vec3(0.6f, 1.0f, -0.6f));
        m_sunLightDir = -m_dirToSun;
        m_skyBox = Beryll::Renderer::createSkyBox("skyboxes/whiteClouds");

        //BR_INFO(" X:%f Y:%f Z:%f", .x, .y, .z);
        //BR_INFO("%s", "");
        //BR_INFO(" %f", );
        //BR_INFO(" %d", );
    }

    Map6::~Map6()
    {

    }

    void Map6::loadEnv()
    {
        auto ground = std::make_shared<Beryll::SimpleCollidingObject>("models3D/maps/Ground_400.fbx",
                                                                      0.0f,
                                                                      false,
                                                                      Beryll::CollisionFlags::STATIC,
                                                                      EnumsAndVars::CollGr_STATIC_ENV,
                                                                      EnumsAndVars::CollGr_DYNAMIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET,
                                                                      EnumsAndVars::SceneGR_GROUND);
        ground->setFriction(1.0f);
        m_simpleObjWithNormalMap.push_back(ground);

        auto staticEnvGlass = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M6_Static1Glass.fbx",
                                                                                       0.0f,
                                                                                       false,
                                                                                       Beryll::CollisionFlags::STATIC,
                                                                                       EnumsAndVars::CollGr_STATIC_ENV,
                                                                                       EnumsAndVars::CollGr_DYNAMIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET,
                                                                                       EnumsAndVars::SceneGR_BRICK_GLASS);
        for(const auto& env : staticEnvGlass)
        {
            env->setFriction(0.05f);
            m_simpleObjForShadowMap.push_back(env);
            m_allEnv.push_back(env);
        }

        auto staticEnvStone = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M6_Static1Stone.fbx",
                                                                                       0.0f,
                                                                                       false,
                                                                                       Beryll::CollisionFlags::STATIC,
                                                                                       EnumsAndVars::CollGr_STATIC_ENV,
                                                                                       EnumsAndVars::CollGr_DYNAMIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET,
                                                                                       EnumsAndVars::SceneGR_BRICK_STONE);
        for(const auto& env : staticEnvStone)
        {
            env->setFriction(1.0f);
            m_simpleObjForShadowMap.push_back(env);
            m_allEnv.push_back(env);
        }

        auto dynamicEnvWood = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M6_Dynamic1Wood_1kg.fbx",
                                                                                       0.2f,
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

        auto dynamicEnvStone = std::make_shared<Beryll::SimpleCollidingObject>("models3D/maps/M6_Dynamic1Stone_100kg.fbx",
                                                                               100.0f,
                                                                               false,
                                                                               Beryll::CollisionFlags::DYNAMIC,
                                                                               EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                               EnumsAndVars::CollGr_STATIC_ENV | EnumsAndVars::CollGr_PLAYER_BULLET |
                                                                                    EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                               EnumsAndVars::SceneGR_BRICK_STONE);

        dynamicEnvStone->setFriction(1.0f);
        dynamicEnvStone->setGravity(glm::vec3{0.0f, -55.0f, 0.0f});
        dynamicEnvStone->setDamping(0.0f, 0.0f);
        m_simpleObjForShadowMap.push_back(dynamicEnvStone);
        m_animatedOrDynamicObjects.push_back(dynamicEnvStone);
        m_allEnv.push_back(dynamicEnvStone);

        m_dynamicStoneID = dynamicEnvStone->getID();
    }

    void Map6::loadEnemies()
    {
        auto enemies = Beryll::SimpleCollidingObject::loadManyModelsFromOneFile("models3D/maps/M6_Enemies.fbx",
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

    void Map6::loadPlayerPoints()
    {
        auto points = Beryll::SimpleObject::loadManyModelsFromOneFile("models3D/maps/M6_PlayerPoints.fbx", EnumsAndVars::SceneGR_NONE);

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

    void Map6::handleCollisions()
    {
        BaseMap::handleCollisions();

        const std::vector<int> stoneCollisions = Beryll::Physics::getAllCollisionsForID(m_dynamicStoneID);
        if(!stoneCollisions.empty())
        {
            for(int objCollidingWithID : stoneCollisions)
            {
                for(const std::shared_ptr<Beryll::SceneObject>& so : m_animatedOrDynamicObjects)
                {
                    if(so->getID() == objCollidingWithID && so->getOrigin().y < 5.0f && ((so->getSceneObjectGroup() == EnumsAndVars::SceneGR_BRICK_GLASS ||
                                                                                           so->getSceneObjectGroup() == EnumsAndVars::SceneGR_BRICK_WOOD ||
                                                                                           so->getSceneObjectGroup() == EnumsAndVars::SceneGR_BRICK_STONE)))
                    {
                        so->disableForEver();
                    }
                }
            }

        }
    }
}
