#include "BaseMap.h"
#include "SendStatisticsHelper.h"
#include "DataBaseHelper.h"

namespace AngryBomb3D
{
    BaseMap::BaseMap(std::shared_ptr<PlayStateGUILayer> gui) : m_gui(std::move(gui))
    {
        m_simpleObjDiffuse.reserve(800);
        m_simpleObjNormalMap.reserve(20);
        m_simpleObjForShadowMap.reserve(800);

        m_allSimpleEnv.reserve(800);
        m_allBullets.reserve(20);
        m_allEnemies.reserve(20);

        //BR_INFO(" X:%f Y:%f Z:%f", .x, .y, .z);
        //BR_INFO("%s", "");
        //BR_INFO(" %f", );
        //BR_INFO(" %d", );
    }

    BaseMap::~BaseMap()
    {

    }

    void BaseMap::updateBeforePhysics()
    {
        //BR_INFO("%s", "BaseMap::updateBeforePhysics()");
        EnumsAndVars::mapPlayTimeSec += Beryll::TimeStep::getTimeStepSec();

        for(int i = 0; i < EnumsAndVars::notEmptyBulletsSets.size(); ++i)
        {
            EnumsAndVars::notEmptyBulletsSets[i]->buttonBullet->updateBeforePhysics();

            if(EnumsAndVars::notEmptyBulletsSets[i]->buttonBullet->getIsPressed())
                EnumsAndVars::currentBulletSetIndex = i;
        }

        handleControls();
        m_player->update();
    }

    void BaseMap::updateAfterPhysics()
    {
        //BR_INFO("%s", "BaseMap::updateAfterPhysics()");
        for(const std::shared_ptr<Beryll::BaseSimpleObject>& so : m_allSimpleEnv)
        {
            if(so->getIsEnabledUpdate())
            {
                so->updateAfterPhysics();

                if(so->getCollisionFlag() == Beryll::CollisionFlags::DYNAMIC && so->getOrigin().y < -50.0f)
                {
                    so->disableForEver();
                    BR_INFO("%s", "so->disableForEver();");
                }
            }
        }

        for(auto& bullet : m_allBullets)
            if(bullet->getIsEnabledUpdate())
                bullet->updateAfterPhysics();

        for(auto& enemy : m_allEnemies)
            enemy.update();

        handleBulletsSets();
        handleCollisions();
        //handleWinLose();
        handleCamera();

        if(m_player->getIsMoving())
            EnumsAndVars::showGUIControls = false;
        else
            EnumsAndVars::showGUIControls = true;
    }

    void BaseMap::draw()
    {
        //BR_INFO("%s", "BaseMap::draw()");

        // 1. Draw into shadow map.
        m_sunPos = m_cameraFront + (Beryll::Camera::getCameraFrontDirectionXZ() * 100.0f) + (m_dirToSun * 300.0f);
        updateSunPosition(m_sunPos);

        Beryll::Renderer::disableFaceCulling();
        m_shadowMap->drawIntoShadowMap(m_simpleObjForShadowMap, {}, m_sunLightVPMatrix);
        Beryll::Renderer::enableFaceCulling();

        glm::mat4 modelMatrix{1.0f};
        const glm::vec3& camPos = Beryll::Camera::getCameraPos();

        m_simpleObjSunLightShadows->bind();
        m_simpleObjSunLightShadows->set3Float("sunLightDir", m_sunLightDir);
        m_simpleObjSunLightShadows->set3Float("cameraPos", camPos);
        m_simpleObjSunLightShadows->set1Float("ambientLight", m_ambientLight);
        m_simpleObjSunLightShadows->set1Float("sunLightStrength", 0.75f);
        m_simpleObjSunLightShadows->set1Float("specularLightStrength", 0.2f);
        m_simpleObjSunLightShadows->set1Float("alphaTransparency", 1.0f);

        modelMatrix = m_player->getObj()->getModelMatrix();
        m_simpleObjSunLightShadows->setMatrix4x4Float("MVPLightMatrix", m_sunLightVPMatrix * modelMatrix);
        m_simpleObjSunLightShadows->setMatrix4x4Float("modelMatrix", modelMatrix);
        m_simpleObjSunLightShadows->setMatrix3x3Float("normalMatrix", glm::mat3(modelMatrix));
        Beryll::Renderer::drawObject(m_player->getObj(), modelMatrix, m_simpleObjSunLightShadows);

        for(const auto& staticObj : m_simpleObjDiffuse)
        {
            if(staticObj->getIsEnabledDraw())
            {
                modelMatrix = staticObj->getModelMatrix();
                m_simpleObjSunLightShadows->setMatrix4x4Float("MVPLightMatrix", m_sunLightVPMatrix * modelMatrix);
                m_simpleObjSunLightShadows->setMatrix4x4Float("modelMatrix", modelMatrix);
                m_simpleObjSunLightShadows->setMatrix3x3Float("normalMatrix", glm::mat3(modelMatrix));
                Beryll::Renderer::drawObject(staticObj, modelMatrix, m_simpleObjSunLightShadows);
            }
        }

        for(const auto& enemy : m_allEnemies)
        {
            if(enemy.getObj()->getIsEnabledDraw())
            {
                modelMatrix = enemy.getObj()->getModelMatrix();
                m_simpleObjSunLightShadows->setMatrix4x4Float("MVPLightMatrix", m_sunLightVPMatrix * modelMatrix);
                m_simpleObjSunLightShadows->setMatrix4x4Float("modelMatrix", modelMatrix);
                m_simpleObjSunLightShadows->setMatrix3x3Float("normalMatrix", glm::mat3(modelMatrix));
                Beryll::Renderer::drawObject(enemy.getObj(), modelMatrix, m_simpleObjSunLightShadows);
            }
        }

        m_simpleObjSunLightShadows->set1Float("sunLightStrength", 1.0f);
        m_simpleObjSunLightShadows->set1Float("specularLightStrength", 1.0f);

        for(const auto& bullet : m_allBullets)
        {
            if(bullet->getIsEnabledDraw())
            {
                modelMatrix = bullet->getModelMatrix();
                m_simpleObjSunLightShadows->setMatrix4x4Float("MVPLightMatrix", m_sunLightVPMatrix * modelMatrix);
                m_simpleObjSunLightShadows->setMatrix4x4Float("modelMatrix", modelMatrix);
                m_simpleObjSunLightShadows->setMatrix3x3Float("normalMatrix", glm::mat3(modelMatrix));
                Beryll::Renderer::drawObject(bullet, modelMatrix, m_simpleObjSunLightShadows);
            }
        }

        m_simpleObjSunLightShadowsNormals->bind();
        m_simpleObjSunLightShadowsNormals->set3Float("sunLightDir", m_sunLightDir);
        m_simpleObjSunLightShadowsNormals->set3Float("cameraPos", camPos);
        m_simpleObjSunLightShadowsNormals->set1Float("ambientLight", m_ambientLight);
        m_simpleObjSunLightShadowsNormals->set1Float("specularLightStrength", 0.6f);

        for(const auto& normalMapObj : m_simpleObjNormalMap)
        {
            modelMatrix = normalMapObj->getModelMatrix();
            m_simpleObjSunLightShadowsNormals->setMatrix4x4Float("MVPLightMatrix", m_sunLightVPMatrix * modelMatrix);
            m_simpleObjSunLightShadowsNormals->setMatrix4x4Float("modelMatrix", modelMatrix);
            m_simpleObjSunLightShadowsNormals->setMatrix3x3Float("normalMatrix", glm::mat3(modelMatrix));
            Beryll::Renderer::drawObject(normalMapObj, modelMatrix, m_simpleObjSunLightShadowsNormals);
        }

        if(m_gui->sliderPower->getValue() > m_gui->sliderPower->getMin() + 0.14f && !EnumsAndVars::notEmptyBulletsSets.empty())
        {
            m_bulletTrajectory.calculateAndDraw(EnumsAndVars::notEmptyBulletsSets[EnumsAndVars::currentBulletSetIndex]->getMass(),
                                                EnumsAndVars::bulletGravity,
                                                m_bulletStartPosition,
                                                EnumsAndVars::throwAngleRadians,
                                                m_bulletImpulseVector,
                                                glm::vec3(1.0f),
                                                m_sunLightDir);
        }

        m_skyBox->draw();
        Beryll::ParticleSystem::draw();
        Beryll::TextOnScene::draw();

//        m_simpleObjSemiTransparent->bind();
//        m_simpleObjSemiTransparent->set3Float("sunLightDir", m_sunLightDir);
//        m_simpleObjSemiTransparent->set3Float("cameraPos", camPos);
//        m_simpleObjSemiTransparent->set1Float("ambientLight", m_ambientLight);
//        m_simpleObjSemiTransparent->set1Float("sunLightStrength", 0.75f);
//        m_simpleObjSemiTransparent->set1Float("specularLightStrength", 0.2f);
//        if(m_bulletTrajectory.getIsHit())
//        {
//            const glm::vec3 hitDir = glm::normalize(m_bulletTrajectory.getHitPoint() - camPos);
//            float hitDistance = glm::distance(m_bulletTrajectory.getHitPoint(), camPos);
//            hitDistance = glm::max(10.0f, hitDistance - 10.0f);
//            m_simpleObjSemiTransparent->set1Int("trajectoryHasHit", 1);
//            m_simpleObjSemiTransparent->set3Float("trajectoryHitPoint", camPos + (hitDir * hitDistance));
//        }
//        else
//        {
//            m_simpleObjSemiTransparent->set1Int("trajectoryHasHit", 0);
//        }
//
//        std::sort(m_simpleObjDiffuse.begin(), m_simpleObjDiffuse.end(), [&](std::shared_ptr<Beryll::BaseSimpleObject>& o1, std::shared_ptr<Beryll::BaseSimpleObject>& o2)
//        {
//            return (glm::distance(camPos, o1->getOrigin()) > glm::distance(camPos, o2->getOrigin()));
//        });
//
//        for(const auto& obj : m_simpleObjDiffuse)
//        {
//            if(obj->getIsEnabledDraw())
//            {
//                modelMatrix = obj->getModelMatrix();
//                m_simpleObjSemiTransparent->setMatrix4x4Float("MVPLightMatrix", m_sunLightVPMatrix * modelMatrix);
//                m_simpleObjSemiTransparent->setMatrix4x4Float("modelMatrix", modelMatrix);
//                m_simpleObjSemiTransparent->setMatrix3x3Float("normalMatrix", glm::mat3(modelMatrix));
//                Beryll::Renderer::drawObject(obj, modelMatrix, m_simpleObjSemiTransparent);
//            }
//        }
    }

    void BaseMap::loadShaders()
    {
        m_simpleObjSunLightShadows = Beryll::Renderer::createShader("shaders/GLES/SimpleObjectSunLightShadows.vert",
                                                                    "shaders/GLES/SimpleObjectSunLightShadows.frag");
        m_simpleObjSunLightShadows->bind();
        m_simpleObjSunLightShadows->activateDiffuseTextureMat1();
        m_simpleObjSunLightShadows->activateShadowMapTexture();
        m_simpleObjSunLightShadows->unBind();

        m_simpleObjSemiTransparent = Beryll::Renderer::createShader("shaders/GLES/gameSpecific/SimpleObjectSemiTransparent.vert",
                                                                    "shaders/GLES/gameSpecific/SimpleObjectSemiTransparent.frag");
        m_simpleObjSemiTransparent->bind();
        m_simpleObjSemiTransparent->activateDiffuseTextureMat1();
        m_simpleObjSemiTransparent->activateShadowMapTexture();
        m_simpleObjSemiTransparent->unBind();

        m_simpleObjSunLightShadowsNormals = Beryll::Renderer::createShader("shaders/GLES/SimpleObjectSunLightShadowsNormals.vert",
                                                                           "shaders/GLES/SimpleObjectSunLightShadowsNormals.frag");
        m_simpleObjSunLightShadowsNormals->bind();
        m_simpleObjSunLightShadowsNormals->activateDiffuseTextureMat1();
        m_simpleObjSunLightShadowsNormals->activateNormalMapTextureMat1();
        m_simpleObjSunLightShadowsNormals->activateShadowMapTexture();
        m_simpleObjSunLightShadowsNormals->unBind();

        m_animatedObjSunLightShadows = Beryll::Renderer::createShader("shaders/GLES/AnimatedObjectSunLightShadows.vert",
                                                                      "shaders/GLES/AnimatedObjectSunLightShadows.frag");
        m_animatedObjSunLightShadows->bind();
        m_animatedObjSunLightShadows->activateDiffuseTextureMat1();
        m_animatedObjSunLightShadows->activateShadowMapTexture();
        m_animatedObjSunLightShadows->unBind();

        m_shadowMap = Beryll::Renderer::createShadowMap(4000, 4000);
    }

    void BaseMap::updateSunPosition(const glm::vec3& pos)
    {
        glm::mat4 lightProjection = glm::ortho(-m_shadowsCubeWidthHeight * 0.5f, m_shadowsCubeWidthHeight * 0.5f, -m_shadowsCubeWidthHeight * 0.5f, m_shadowsCubeWidthHeight * 0.5f, 5.0f, 800.0f);
        glm::mat4 lightView = glm::lookAt(pos, pos + m_sunLightDir, BeryllConstants::worldUp);

        m_sunLightVPMatrix = lightProjection * lightView;
    }

    void BaseMap::loadPlayer()
    {
        auto playerModel = std::make_shared<Beryll::SimpleCollidingObject>("models3D/player/Player1.fbx",
                                                                           0,
                                                                           true,
                                                                           Beryll::CollisionFlags::STATIC,
                                                                           EnumsAndVars::CollGr_PLAYER,
                                                                           EnumsAndVars::CollGr_DYNAMIC_ENV,
                                                                           EnumsAndVars::SceneGR_PLAYER);

        m_simpleObjForShadowMap.push_back(playerModel);
        m_player = std::make_shared<Player>(playerModel, 100);
    }

    void BaseMap::loadBulletSet(const BulletType type, const int count, const float disableTimeAfterFirstCollision)
    {
        float bulletMass = 0.0f;
        std::string modelPath;

        if(type == BulletType::STANDARD_1_KG)
        {
            bulletMass = 1.0f;
            modelPath = "models3D/bullets/Standard_1kg.fbx";
        }
        else
        {
            BR_ASSERT(false, "%s", "Unknown bullet type.");
        }

        std::vector<std::shared_ptr<Beryll::SimpleCollidingObject>> bullets;
        bullets.reserve(count);
        for(int i = 0; i < count; ++i)
        {
            auto bullet = std::make_shared<Beryll::SimpleCollidingObject>(modelPath.c_str(),
                                                                          bulletMass,
                                                                          true,
                                                                          Beryll::CollisionFlags::DYNAMIC,
                                                                          EnumsAndVars::CollGr_PLAYER_BULLET,
                                                                          EnumsAndVars::CollGr_DYNAMIC_ENV | EnumsAndVars::CollGr_STATIC_ENV,
                                                                          EnumsAndVars::SceneGR_PLAYER_BULLET);

            m_simpleObjForShadowMap.push_back(bullet);
            m_allBullets.push_back(bullet);

            bullets.push_back(bullet);
        }

        EnumsAndVars::notEmptyBulletsSets.push_back(std::make_shared<AngryBomb3D::BulletSet>(type, std::move(bullets), disableTimeAfterFirstCollision));

        BR_INFO("EnumsAndVars::notEmptyBulletsSets.size() %d", EnumsAndVars::notEmptyBulletsSets.size());
    }

    void BaseMap::handleControls()
    {
        if(m_gui->buttonMoveLeft->getIsPressed())
            m_player->moveLeft();

        if(m_gui->buttonMoveRight->getIsPressed())
            m_player->moveRight();

        if(m_gui->buttonShot->getIsPressed() &&
           !EnumsAndVars::notEmptyBulletsSets.empty() &&
           EnumsAndVars::mapLastShootSec + 0.5f < EnumsAndVars::mapPlayTimeSec) // Protect from double click.
        {
            m_gui->sliderPower->setValue(0.12f);
            EnumsAndVars::notEmptyBulletsSets[EnumsAndVars::currentBulletSetIndex]->shoot(m_bulletStartPosition, m_bulletImpulseVector);
            EnumsAndVars::mapLastShootSec = EnumsAndVars::mapPlayTimeSec;
        }
    }

    void BaseMap::handleBulletsSets()
    {
        for(auto& bulletSet : EnumsAndVars::notEmptyBulletsSets)
            bulletSet->update();

        for(auto& emptyBulletSet : EnumsAndVars::emptyBulletsSetsWithActiveBullets)
            emptyBulletSet->update();

        const auto findEmptySet = std::find_if(EnumsAndVars::notEmptyBulletsSets.begin(), EnumsAndVars::notEmptyBulletsSets.end(),
                                                     [](const std::shared_ptr<AngryBomb3D::BulletSet>& set){ return set->getAvailableCount() <= 0; });
        if (findEmptySet != EnumsAndVars::notEmptyBulletsSets.end())
        {
            BR_INFO("%s", "Not empty bullets sets.erase()");
            EnumsAndVars::emptyBulletsSetsWithActiveBullets.push_back(*findEmptySet);
            EnumsAndVars::notEmptyBulletsSets.erase(findEmptySet);
            EnumsAndVars::currentBulletSetIndex = 0;
        }

        const auto findEmptySetNotActiveBullets = std::find_if(EnumsAndVars::emptyBulletsSetsWithActiveBullets.begin(), EnumsAndVars::emptyBulletsSetsWithActiveBullets.end(),
                                                              [](const std::shared_ptr<AngryBomb3D::BulletSet>& set){ return !set->getIsAnyBulletActive(); });
        if (findEmptySetNotActiveBullets != EnumsAndVars::emptyBulletsSetsWithActiveBullets.end())
        {
            BR_INFO("%s", "Empty bullets sets.erase()");
            EnumsAndVars::emptyBulletsSetsWithActiveBullets.erase(findEmptySetNotActiveBullets);
        }
    }

    void BaseMap::handleCollisions()
    {
        bool wakeUpObjects = false;
        std::vector<std::shared_ptr<Beryll::SimpleCollidingObject>> activeBullets;

        for(const auto& bulletSet : EnumsAndVars::notEmptyBulletsSets)
        {
            if(bulletSet->getIsAnyBulletHasCollision())
                wakeUpObjects = true;

            for(const auto& bullet : bulletSet->getBullets())
            {
                if(bullet->getIsEnabledUpdate())
                    activeBullets.push_back(bullet);
            }
        }

        for(const auto& emptyBulletSet : EnumsAndVars::emptyBulletsSetsWithActiveBullets)
        {
            if(emptyBulletSet->getIsAnyBulletHasCollision())
                wakeUpObjects = true;

            for(const auto& bullet : emptyBulletSet->getBullets())
            {
                if(bullet->getIsEnabledUpdate())
                    activeBullets.push_back(bullet);
            }
        }

        for(const auto& bullet : activeBullets)
        {
            const std::vector<int> bulletCollisions = Beryll::Physics::getAllUniqueCollisionsForID(bullet->getID());
            if(!bulletCollisions.empty())
            {
                for(int IDCollidingWith : bulletCollisions)
                {
                    for(const auto& objEnv : m_allSimpleEnv)
                    {
                        if(!objEnv->getIsEnabledUpdate())
                            continue;

                        if(objEnv->getID() == IDCollidingWith && objEnv->getSceneObjectGroup() == EnumsAndVars::SceneGR_BRICK_GLASS)
                        {
                            breakGlassBrick(objEnv);
                            wakeUpObjects = true;
                            bullet->disableForEver(); // Disable bullet if it broke glass to avoid ricochet.
                            goto nextTopLoopIteration;
                        }
                        else if(objEnv->getID() == IDCollidingWith && objEnv->getSceneObjectGroup() == EnumsAndVars::SceneGR_DYNAMITE)
                        {
                            // Dynamite has logic based on DynamiteInfo. Find it.
                            auto it = std::find_if(m_dynamiteInfo.begin(), m_dynamiteInfo.end(), [&objEnv](const DynamiteInfo& info)
                            {
                                return objEnv->getID() == info.obj->getID();
                            });

                            if (it != m_dynamiteInfo.end())
                            {
                                explodeDynamite(*it);
                                wakeUpObjects = true;
                            }
                        }
                    }
                }
            }
            nextTopLoopIteration: ;
        }

        // Check if any dynamite moved somehow more than 5 meters from initial position and explode it.
        auto it = std::find_if(m_dynamiteInfo.begin(), m_dynamiteInfo.end(), [](const DynamiteInfo& info)
        {
            return (info.obj->getIsEnabledUpdate() &&
                    glm::distance2(info.obj->getOrigin(), info.initialPos) > 25.0f &&
                    Beryll::Physics::getAnyCollisionForID(info.obj->getID()) > 0);
        });

        if (it != m_dynamiteInfo.end())
        {
            explodeDynamite(*it);
            wakeUpObjects = true;
        }

        if(wakeUpObjects)
        {
            BR_INFO("%s", "Make active all dynamic objects.")
            for(const std::shared_ptr<Beryll::BaseSimpleObject>& so : m_allSimpleEnv)
            {
                if(so->getIsEnabledUpdate() && so->getCollisionFlag() == Beryll::CollisionFlags::DYNAMIC)
                {
                    so->activate();
                }
            }

            for(auto& enemy : m_allEnemies)
                enemy.getObj()->activate();
        }
    }

    void BaseMap::breakGlassBrick(std::shared_ptr<Beryll::BaseSimpleObject> glass)
    {
        const float partSize = (glass->getXZRadius() + glass->getObjectHeight()) * 0.18f;
        Beryll::ParticleSystem::EmitCubesFromCenter(10, 0.45f, partSize, partSize * 0.6f,
                                                    glm::vec4{0.0f, 0.5f, 1.0f, 1.0f}, glm::vec4{0.0f, 0.5f, 1.0f, 1.0f},
                                                    glass->getOrigin(),
                                                    glm::vec3{Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f},
                                                    10.0f);
        glass->disableForEver();
    }

    void BaseMap::explodeDynamite(const DynamiteInfo& dynamite)
    {
        const float partSize = (dynamite.obj->getXZRadius() + dynamite.obj->getObjectHeight()) * 0.4f;
        Beryll::ParticleSystem::EmitCubesFromCenter(6, 0.4f, partSize, partSize * 2.0f,
                                                    glm::vec4{1.0f, 0.98f, 0.2578f, 1.0f}, glm::vec4{1.0f, 0.98f, 0.2578f, 1.0f},
                                                    dynamite.obj->getOrigin(),
                                                    glm::vec3{Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 2.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f},
                                                    25.0f);
        Beryll::ParticleSystem::EmitCubesFromCenter(6, 0.4f, partSize, partSize * 2.0f,
                                                    glm::vec4{0.98f, 0.332f, 0.078f, 1.0f}, glm::vec4{0.98f, 0.332f, 0.078f, 1.0f},
                                                    dynamite.obj->getOrigin(),
                                                    glm::vec3{Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 2.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f},
                                                    25.0f);
        Beryll::ParticleSystem::EmitCubesFromCenter(6, 0.45f, partSize, partSize * 1.4f,
                                                    glm::vec4{0.5f, 0.5f, 0.5f, 1.0f}, glm::vec4{0.5f, 0.5f, 0.5f, 1.0f},
                                                    dynamite.obj->getOrigin(),
                                                    glm::vec3{Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 2.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f},
                                                    25.0f);

        const float explosionRadiusSquared = dynamite.radius * dynamite.radius;

        for(const std::shared_ptr<Beryll::BaseSimpleObject>& so : m_allSimpleEnv)
        {
            if(!so->getIsEnabledUpdate())
                continue;

            if(so->getSceneObjectGroup() == EnumsAndVars::SceneGR_BRICK_GLASS &&
               glm::distance2(so->getOrigin(), dynamite.obj->getOrigin()) < explosionRadiusSquared)
            {
                breakGlassBrick(so);
            }
            else if(so->getCollisionFlag() == Beryll::CollisionFlags::DYNAMIC &&
                    glm::distance2(so->getOrigin(), dynamite.obj->getOrigin()) < explosionRadiusSquared)
            {
                so->applyCentralImpulse(glm::fastNormalize(so->getOrigin() - dynamite.obj->getOrigin()) * dynamite.power * so->getCollisionMass());

                so->applyTorqueImpulse(glm::vec3{Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                 Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                 Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f} *
                                       (Beryll::RandomGenerator::getFloat() + 1.0f) * so->getCollisionMass());
            }
        }

        dynamite.obj->disableForEver();
    }

    void BaseMap::handleWinLose()
    {
        if(BaseEnemy::getActiveCount() <= 0 && !getIsEnvObjectsMoves())
        {
            EnumsAndVars::mapPlayerWin = true;
            SendStatisticsHelper::sendMapWin();

            if(EnumsAndVars::MapsProgress::lastOpenedMapIndex == EnumsAndVars::MapsProgress::currentMapIndex)
            {
                ++EnumsAndVars::MapsProgress::lastOpenedMapIndex;
                BR_INFO("lastOpenedMapIndex: %d", EnumsAndVars::MapsProgress::lastOpenedMapIndex);
                DataBaseHelper::storeMapsProgressLastOpenedMapIndex(EnumsAndVars::MapsProgress::lastOpenedMapIndex);
            }
        }
    }

    bool BaseMap::getIsEnvObjectsMoves()
    {
        const float farthestDistanceSquared = (m_player->getFarthestPointDistance() * 1.5f) * (m_player->getFarthestPointDistance() * 1.5f);
        for(const std::shared_ptr<Beryll::BaseSimpleObject>& so : m_allSimpleEnv)
        {
            if(so->getIsEnabledUpdate() &&
               so->getCollisionFlag() == Beryll::CollisionFlags::DYNAMIC &&
               so->getSceneObjectGroup() != EnumsAndVars::SceneGR_PLAYER_BULLET &&
               glm::distance2(glm::vec3{0.0f}, so->getOrigin()) < farthestDistanceSquared &&
               (glm::distance2(glm::vec3{0.0f}, so->getLinearVelocity()) > 1.0f || glm::distance2(glm::vec3{0.0f}, so->getAngularVelocity()) > 1.0f))
            {
                return true;
            }
        }

        return false;
    }

    void BaseMap::handleCamera()
    {
        m_gui->showTutorialCameraTip = true;
        const std::vector<Beryll::Finger>& fingers = Beryll::EventHandler::getFingers();
        for(const Beryll::Finger& f : fingers)
        {
            if(f.normalizedPos.x < 0.5f)
                continue;

            m_gui->showTutorialCameraTip = false;

            if(f.downEvent)
            {
                m_lastFingerMovePosX = f.normalizedPos.x;
                m_lastFingerMovePosY = f.normalizedPos.y;
                break;
            }
            else
            {
                float deltaX = (f.normalizedPos.x - m_lastFingerMovePosX) * EnumsAndVars::SettingsMenu::cameraHorizontalSpeed;
                float deltaY = (f.normalizedPos.y - m_lastFingerMovePosY) * EnumsAndVars::SettingsMenu::cameraVerticalSpeed;

                m_lastFingerMovePosX = f.normalizedPos.x;
                m_lastFingerMovePosY = f.normalizedPos.y;

                m_eyesLookAngleXZ += deltaX;
                m_eyesLookAngleY -= deltaY;
                //if(m_eyesLookAngleY > 30.0f) m_eyesLookAngleY = 30.0f; // Eye down.
                //if(m_eyesLookAngleY < -15.0f) m_eyesLookAngleY = -15.0f; // Eye up.
                //BR_INFO("m_eyesLookAngleXZ %f", m_eyesLookAngleXZ);
                break;
            }
        }

        if(m_eyesLookAngleY > 50.0f) m_eyesLookAngleY = 50.0f; // Eye down.
        if(m_eyesLookAngleY < -30.0f) m_eyesLookAngleY = -30.0f; // Eye up.
        //BR_INFO("m_eyesLookAngleXZ %f", m_eyesLookAngleXZ);
        //BR_INFO("m_eyesLookAngleY %f", m_eyesLookAngleY);

        m_playerCurrentDir = m_player->getObj()->getOrigin();
        m_playerCurrentDir.y = 0.0f;
        m_playerCurrentDir = glm::normalize(m_playerCurrentDir);

        if(m_playerCurrentDir != m_playerPrevDir)
        {
            const float playerRotatedCurrentAngle = BeryllUtils::Common::getAngleInDegrees(m_playerCurrentDir, m_worldDefaultDir);
            const float playerRotatedPrevAngle = BeryllUtils::Common::getAngleInDegrees(m_playerPrevDir, m_worldDefaultDir);
            const float angleDif = playerRotatedCurrentAngle - playerRotatedPrevAngle;

            if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::ON_LEFT_SIDE &&
               BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerPrevDir) == BeryllUtils::Common::VectorSide::ON_RIGHT_SIDE)
            {
                const float currentToPrevAngle = BeryllUtils::Common::getAngleInDegrees(m_playerCurrentDir, m_playerPrevDir);

                if(BeryllUtils::Common::getAngleInDegrees(m_playerCurrentDir, m_worldDefaultDir) > m_cameraXZAngleThreshold * 2.0f)
                    m_playerRotatedTotalAngle += currentToPrevAngle;
                else
                    m_playerRotatedTotalAngle += -currentToPrevAngle;
            }
            else if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::ON_RIGHT_SIDE &&
                    BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerPrevDir) == BeryllUtils::Common::VectorSide::ON_LEFT_SIDE)
            {
                const float currentToPrevAngle = BeryllUtils::Common::getAngleInDegrees(m_playerCurrentDir, m_playerPrevDir);

                if(BeryllUtils::Common::getAngleInDegrees(m_playerCurrentDir, m_worldDefaultDir) > m_cameraXZAngleThreshold * 2.0f)
                    m_playerRotatedTotalAngle += -currentToPrevAngle;
                else
                    m_playerRotatedTotalAngle += currentToPrevAngle;
            }
            else if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::PARALLEL_SAME_DIR &&
                    BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerPrevDir) == BeryllUtils::Common::VectorSide::ON_LEFT_SIDE)
            {
                m_playerRotatedTotalAngle += -angleDif;
            }
            else if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::PARALLEL_SAME_DIR &&
                    BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerPrevDir) == BeryllUtils::Common::VectorSide::ON_RIGHT_SIDE)
            {
                m_playerRotatedTotalAngle += angleDif;
            }
            else if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::PARALLEL_OPPOSITE_DIR &&
                    BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerPrevDir) == BeryllUtils::Common::VectorSide::ON_LEFT_SIDE)
            {
                m_playerRotatedTotalAngle += -angleDif;
            }
            else if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::PARALLEL_OPPOSITE_DIR &&
                    BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerPrevDir) == BeryllUtils::Common::VectorSide::ON_RIGHT_SIDE)
            {
                m_playerRotatedTotalAngle += angleDif;
            }
            else if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::ON_RIGHT_SIDE)
            {
                m_playerRotatedTotalAngle += angleDif;
            }
            else if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::ON_LEFT_SIDE)
            {
                m_playerRotatedTotalAngle += -angleDif;
            }

            m_playerPrevDir = m_playerCurrentDir;
        }

        //BR_INFO("m_eyesLookAngleXZ %f", m_eyesLookAngleXZ);
        if(m_eyesLookAngleXZ > m_playerRotatedTotalAngle + m_cameraXZAngleThreshold)
            m_eyesLookAngleXZ = m_playerRotatedTotalAngle + m_cameraXZAngleThreshold;
        else if(m_eyesLookAngleXZ < m_playerRotatedTotalAngle - m_cameraXZAngleThreshold)
            m_eyesLookAngleXZ = m_playerRotatedTotalAngle - m_cameraXZAngleThreshold;

        m_cameraFront = m_player->getObj()->getOrigin();
        m_cameraFront.y += 6.5f;

        // Euler angles.
        float m_eyesLookAngleXZRadians = glm::radians(m_eyesLookAngleXZ);
        float m_eyesLookAngleYRadians = glm::radians(m_eyesLookAngleY);
        m_cameraOffset.x = glm::cos(m_eyesLookAngleXZRadians) * glm::cos(m_eyesLookAngleYRadians);
        m_cameraOffset.y = glm::sin(m_eyesLookAngleYRadians);
        m_cameraOffset.z = glm::sin(m_eyesLookAngleXZRadians) * glm::cos(m_eyesLookAngleYRadians);
        m_cameraOffset = glm::normalize(m_cameraOffset);

        Beryll::Camera::setCameraPos(m_cameraFront + m_cameraOffset * EnumsAndVars::SettingsMenu::cameraDistance);
        Beryll::Camera::setCameraFrontPos(m_cameraFront);
        Beryll::Camera::updateCameraVectors();

        m_player->getObj()->rotateToDirection(Beryll::Camera::getCameraFrontDirectionXZ(), true);

        // Move camera a bit to left side(player's character will not directly on the middle of the screen).
        // That allow player see all trajectory including part going down.
        Beryll::Camera::setCameraPos(Beryll::Camera::getCameraPos() + Beryll::Camera::getCameraLeftXYZ() * 2.2f);
        Beryll::Camera::setCameraFrontPos(Beryll::Camera::getCameraFrontPos() + Beryll::Camera::getCameraLeftXYZ() * 2.0f);
        // Move camera pos up to get view more from top if player is on high point.
        //Beryll::Camera::setCameraPos(Beryll::Camera::getCameraPos() + glm::vec3(0.0f, m_player->getObj()->getOrigin().y * 0.7f, 0.0f));

        // Update shoot dir after camera.
        const float sliderRange_0_1 = m_gui->sliderPower->getValue();
        const float baseAddedAngle =              27.5f;
        const float substrAngleWhenPowerGrowing = 24.0f * std::pow(sliderRange_0_1, 0.185f);
        const float addAngleAtMaxPower =          2.0f * std::pow(sliderRange_0_1, 1.85f);
        const float angleBetweenWorldUpAndCameraBack = BeryllUtils::Common::getAngleInRadians(BeryllConstants::worldUp, Beryll::Camera::getCameraBackDirectionXYZ());
        EnumsAndVars::throwAngleRadians = angleBetweenWorldUpAndCameraBack - glm::half_pi<float>();
        EnumsAndVars::throwAngleRadians += glm::radians(baseAddedAngle - substrAngleWhenPowerGrowing + addAngleAtMaxPower);

        m_bulletImpulseVector = m_player->getObj()->getFaceDirXZ();
        m_bulletImpulseVector.y = glm::tan(EnumsAndVars::throwAngleRadians);
        m_bulletImpulseVector = glm::normalize(m_bulletImpulseVector);
        m_bulletImpulseVector *= sliderRange_0_1 *
                                 m_bulletMaxThrowPower *
                                 EnumsAndVars::notEmptyBulletsSets[EnumsAndVars::currentBulletSetIndex]->getMass();


        m_bulletStartPosition = m_player->getObj()->getOrigin() + m_player->getObj()->getFaceDirXZ() * 1.5f;
        m_bulletStartPosition.y += 2.0f;
    }
}
