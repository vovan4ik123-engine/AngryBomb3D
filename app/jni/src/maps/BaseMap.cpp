#include "BaseMap.h"
#include "SendStatisticsHelper.h"
#include "DataBaseHelper.h"

namespace AngryBomb3D
{
    BaseMap::BaseMap(std::shared_ptr<PlayStateGUILayer> gui) : m_gui(std::move(gui))
    {
        m_animatedOrDynamicObjects.reserve(1500);
        m_allEnv.reserve(1000);
        m_simpleObjForShadowMap.reserve(1000);

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
        if(EnumsAndVars::mapPlayerWin || EnumsAndVars::mapPlayerLose)
            return;

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
        if(EnumsAndVars::mapPlayerWin || EnumsAndVars::mapPlayerLose)
            return;

        for(const std::shared_ptr<Beryll::SceneObject>& so : m_animatedOrDynamicObjects)
        {
            if(so->getIsEnabledUpdate())
            {
                so->updateAfterPhysics();

                if(so->getSceneObjectGroup() == Beryll::SceneObjectGroups::DYNAMIC_ENVIRONMENT && so->getOrigin().y < -5.0f)
                {
                    so->disableForEver();
                    BR_INFO("%s", "so->disableForEver();");
                }
            }
        }

        for(auto& enemy : m_allEnemies)
            enemy.update();

        for(auto& bulletSet : EnumsAndVars::notEmptyBulletsSets)
            bulletSet->update();

        handleCollisions();
        //handleWinLose();

        const auto emptyBulletSetIter = std::find_if(EnumsAndVars::notEmptyBulletsSets.begin(), EnumsAndVars::notEmptyBulletsSets.end(),
                                                                      [](const std::shared_ptr<AngryBomb3D::BulletSet>& set){ return set->getAvailableCount() <= 0; });
        if (emptyBulletSetIter != EnumsAndVars::notEmptyBulletsSets.end())
        {
            BR_INFO("%s", "NOT empty bullets sets.erase()");
            EnumsAndVars::emptyBulletsSetsWithActiveBullets.push_back(*emptyBulletSetIter);
            EnumsAndVars::notEmptyBulletsSets.erase(emptyBulletSetIter);
            EnumsAndVars::currentBulletSetIndex = 0;
        }

        for(auto& emptyBulletSet : EnumsAndVars::emptyBulletsSetsWithActiveBullets)
            emptyBulletSet->update();

        const auto emptyNotActiveBulletSetIter = std::find_if(EnumsAndVars::emptyBulletsSetsWithActiveBullets.begin(), EnumsAndVars::emptyBulletsSetsWithActiveBullets.end(),
                                                                               [](const std::shared_ptr<AngryBomb3D::BulletSet>& set){ return !set->getIsAnyBulletActive(); });
        if (emptyNotActiveBulletSetIter != EnumsAndVars::emptyBulletsSetsWithActiveBullets.end())
        {
            BR_INFO("%s", "empty bullets sets.erase()");
            EnumsAndVars::emptyBulletsSetsWithActiveBullets.erase(emptyNotActiveBulletSetIter);
        }

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
        m_sunPos = m_cameraFront + (Beryll::Camera::getCameraFrontDirectionXZ() * 150.0f) + (m_dirToSun * 400.0f);
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

        for(const auto& staticObj : m_allEnv)
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

        m_bulletTrajectory.calculateAndDraw(EnumsAndVars::bulletMass,
                                            EnumsAndVars::bulletGravity,
                                            m_bulletStartPosition,
                                            EnumsAndVars::throwAngleRadians,
                                            m_bulletImpulseVector,
                                            glm::vec3(1.0f),
                                            m_sunLightDir);

        m_skyBox->draw();

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
//        std::sort(m_allEnv.begin(), m_allEnv.end(), [&](std::shared_ptr<Beryll::BaseSimpleObject>& o1, std::shared_ptr<Beryll::BaseSimpleObject>& o2)
//        {
//            return (glm::distance(camPos, o1->getOrigin()) > glm::distance(camPos, o2->getOrigin()));
//        });
//
//        for(const auto& staticObj : m_allEnv)
//        {
//            if(staticObj->getIsEnabledDraw())
//            {
//                modelMatrix = staticObj->getModelMatrix();
//                m_simpleObjSemiTransparent->setMatrix4x4Float("MVPLightMatrix", m_sunLightVPMatrix * modelMatrix);
//                m_simpleObjSemiTransparent->setMatrix4x4Float("modelMatrix", modelMatrix);
//                m_simpleObjSemiTransparent->setMatrix3x3Float("normalMatrix", glm::mat3(modelMatrix));
//                Beryll::Renderer::drawObject(staticObj, modelMatrix, m_simpleObjSemiTransparent);
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

        m_simpleObjSemiTransparent = Beryll::Renderer::createShader("shaders/GLES/gameSpecific/SimpleObjectSemiTransparent.vert",
                                                                    "shaders/GLES/gameSpecific/SimpleObjectSemiTransparent.frag");
        m_simpleObjSemiTransparent->bind();
        m_simpleObjSemiTransparent->activateDiffuseTextureMat1();
        m_simpleObjSemiTransparent->activateShadowMapTexture();

        m_simpleObjSunLightShadowsNormals = Beryll::Renderer::createShader("shaders/GLES/SimpleObjectSunLightShadowsNormals.vert",
                                                                           "shaders/GLES/SimpleObjectSunLightShadowsNormals.frag");
        m_simpleObjSunLightShadowsNormals->bind();
        m_simpleObjSunLightShadowsNormals->activateDiffuseTextureMat1();
        m_simpleObjSunLightShadowsNormals->activateNormalMapTextureMat1();
        m_simpleObjSunLightShadowsNormals->activateShadowMapTexture();

        m_animatedObjSunLightShadows = Beryll::Renderer::createShader("shaders/GLES/AnimatedObjectSunLightShadows.vert",
                                                                      "shaders/GLES/AnimatedObjectSunLightShadows.frag");
        m_animatedObjSunLightShadows->bind();
        m_animatedObjSunLightShadows->activateDiffuseTextureMat1();
        m_animatedObjSunLightShadows->activateShadowMapTexture();

        m_shadowMap = Beryll::Renderer::createShadowMap(4000, 4000);
    }

    void BaseMap::updateSunPosition(const glm::vec3& pos)
    {
        glm::mat4 lightProjection = glm::ortho(-m_shadowsCubeWidthHeight * 0.5f, m_shadowsCubeWidthHeight * 0.5f, -m_shadowsCubeWidthHeight * 0.5f, m_shadowsCubeWidthHeight * 0.5f, 15.0f, 800.0f);
        glm::mat4 lightView = glm::lookAt(pos, pos + m_sunLightDir, BeryllConstants::worldUp);

        m_sunLightVPMatrix = lightProjection * lightView;
    }

    void BaseMap::loadPlayer()
    {
        auto playerModel = std::make_shared<Beryll::SimpleCollidingObject>("models3D/player/Player1.fbx",
                                                                                          0,
                                                                                          true,
                                                                                          Beryll::CollisionFlags::STATIC,
                                                                                          Beryll::CollisionGroups::PLAYER,
                                                                                          Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT,
                                                                                          Beryll::SceneObjectGroups::PLAYER);

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
                                                                          Beryll::CollisionGroups::PLAYER_BULLET,
                                                                          Beryll::CollisionGroups::DYNAMIC_ENVIRONMENT | Beryll::CollisionGroups::STATIC_ENVIRONMENT,
                                                                          Beryll::SceneObjectGroups::BULLET);

            m_animatedOrDynamicObjects.push_back(bullet);
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

        if(m_gui->buttonShot->getIsPressed() && !EnumsAndVars::notEmptyBulletsSets.empty())
        {
            //m_gui->sliderPower->setValue(0.0f);
            EnumsAndVars::notEmptyBulletsSets[EnumsAndVars::currentBulletSetIndex]->shoot(m_bulletStartPosition, m_bulletImpulseVector);
        }
    }

    void BaseMap::handleCollisions()
    {
        float anyBulletHasCollision = false;

        for(auto& bulletSet : EnumsAndVars::notEmptyBulletsSets)
        {
            if(bulletSet->getIsAnyBulletHasCollision())
                anyBulletHasCollision = true;
        }

        for(auto& emptyBulletSet : EnumsAndVars::emptyBulletsSetsWithActiveBullets)
        {
            if(emptyBulletSet->getIsAnyBulletHasCollision())
                anyBulletHasCollision = true;
        }

        if(anyBulletHasCollision)
        {
            BR_INFO("%s", "Make active all dynamic objects.")

            for(const std::shared_ptr<Beryll::SceneObject>& so : m_animatedOrDynamicObjects)
            {
                if(so->getIsEnabledUpdate() && so->getSceneObjectGroup() == Beryll::SceneObjectGroups::DYNAMIC_ENVIRONMENT)
                {
                    so->activate();
                }
            }
        }
    }

    void BaseMap::handleWinLose()
    {
        if(BaseEnemy::getActiveCount() == 0 && !getIsDynamicObjectsMoves())
        {
            BR_INFO("%s", "Player win.");
            EnumsAndVars::mapPlayerWin = true;

            SendStatisticsHelper::sendMapWin();

            if(EnumsAndVars::MapsProgress::lastOpenedMapIndex == EnumsAndVars::MapsProgress::currentMapIndex)
            {
                ++EnumsAndVars::MapsProgress::lastOpenedMapIndex;
                BR_INFO("lastOpenedMapIndex: %d", EnumsAndVars::MapsProgress::lastOpenedMapIndex);
                DataBaseHelper::storeMapsProgressLastOpenedMapIndex(EnumsAndVars::MapsProgress::lastOpenedMapIndex);
            }
        }
        else if(EnumsAndVars::notEmptyBulletsSets.empty() && !getIsDynamicObjectsMoves())
        {
            BR_INFO("%s", "Player lose.");
            EnumsAndVars::mapPlayerLose = true;
        }
    }

    bool BaseMap::getIsDynamicObjectsMoves()
    {
        for(const std::shared_ptr<Beryll::SceneObject>& so : m_animatedOrDynamicObjects)
        {
            if(so->getSceneObjectGroup() != Beryll::SceneObjectGroups::BULLET &&
               glm::length(so->getOrigin()) < m_player->getFarthestPointDistance() * 1.2f &&
               (glm::length(so->getLinearVelocity()) > 1.0f || glm::length(so->getAngularVelocity()) > 1.0f))
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
            if(f.handled || f.normalizedPos.x < 0.5f)
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

        if(m_eyesLookAngleY > 40.0f) m_eyesLookAngleY = 40.0f; // Eye down.
        if(m_eyesLookAngleY < -16.0f) m_eyesLookAngleY = -16.0f; // Eye up.

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

                if(BeryllUtils::Common::getAngleInDegrees(m_playerCurrentDir, m_worldDefaultDir) > 90.0f)
                    m_playerRotatedTotalAngle += currentToPrevAngle;
                else
                    m_playerRotatedTotalAngle += -currentToPrevAngle;
            }
            else if(BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerCurrentDir) == BeryllUtils::Common::VectorSide::ON_RIGHT_SIDE &&
                    BeryllUtils::Common::getVectorSide(m_worldDefaultDir, m_playerPrevDir) == BeryllUtils::Common::VectorSide::ON_LEFT_SIDE)
            {
                const float currentToPrevAngle = BeryllUtils::Common::getAngleInDegrees(m_playerCurrentDir, m_playerPrevDir);

                if(BeryllUtils::Common::getAngleInDegrees(m_playerCurrentDir, m_worldDefaultDir) > 90.0f)
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
        m_cameraFront.y += 10.0f;

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
        Beryll::Camera::setCameraPos(Beryll::Camera::getCameraPos() + Beryll::Camera::getCameraLeftXYZ() * 20.0f);
        Beryll::Camera::setCameraFrontPos(Beryll::Camera::getCameraFrontPos() + Beryll::Camera::getCameraLeftXYZ() * 12.0f);
        // Move camera pos up to get view more from top if player is on high point.
        //Beryll::Camera::setCameraPos(Beryll::Camera::getCameraPos() + glm::vec3(0.0f, m_player->getObj()->getOrigin().y * 0.7f, 0.0f));

        // Update shoot dir after camera.
        const float factorToReduceAngle = std::pow(m_gui->sliderPower->getValue() / m_gui->sliderPower->getMax(), 1.0f/2.4f); // sqrt_2.4 (x)
        const float addToThrowAngleRadians = glm::radians(60.0f - (factorToReduceAngle * 46.0f));
        const float angleBetweenWorldUpAndCameraBack = BeryllUtils::Common::getAngleInRadians(BeryllConstants::worldUp, Beryll::Camera::getCameraBackDirectionXYZ());
        EnumsAndVars::throwAngleRadians = angleBetweenWorldUpAndCameraBack - glm::half_pi<float>();
        EnumsAndVars::throwAngleRadians += addToThrowAngleRadians;

        m_bulletImpulseVector = m_player->getObj()->getFaceDirXZ();
        m_bulletImpulseVector.y = glm::tan(EnumsAndVars::throwAngleRadians);
        m_bulletImpulseVector = glm::normalize(m_bulletImpulseVector);
        m_bulletImpulseVector *= EnumsAndVars::throwPowerDefault + m_gui->sliderPower->getValue();

        m_bulletStartPosition = m_player->getObj()->getOrigin() + m_player->getObj()->getFaceDirXZ() * 2.0f;
        m_bulletStartPosition.y += 3.0f;
    }
}
