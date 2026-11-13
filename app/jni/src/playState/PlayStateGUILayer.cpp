#include "PlayStateGUILayer.h"
#include "EnumsAndVariables.h"
#include "GameStateHelper.h"
#include "DataBaseHelper.h"
#include "enemies/BaseEnemy.h"

namespace AngryBomb3D
{
    PlayStateGUILayer::PlayStateGUILayer()
    {
        m_ID = Beryll::LayerID::PLAY_GUI;

        const float screenAR = Beryll::Window::getInstance()->getScreenAspectRatio();

        if(m_statisticsShow)
        {
            m_statistics1 = Beryll::Renderer::createGUIText("", glm::vec3{0.06f, 0.06f, 0.06f}, glm::vec3{0.5f, 96.3f, 0.0f}, 3.0f);
            m_guiObjects.push_back(m_statistics1);
            m_statistics2 = Beryll::Renderer::createGUIText("", glm::vec3{0.06f, 0.06f, 0.06f}, glm::vec3{0.5f, 92.3f, 0.0f}, 3.0f);
            m_guiObjects.push_back(m_statistics2);
            m_statistics3 = Beryll::Renderer::createGUIText("", glm::vec3{0.06f, 0.06f, 0.06f}, glm::vec3{0.5f, 88.3f, 0.0f}, 3.0f);
            m_guiObjects.push_back(m_statistics3);
        }

        m_buttonExitToMenu = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/Cancel.png", "",
                                                                   glm::vec3{93.5f, 100.0f - 6.5f * screenAR, 0.5f},
                                                                   glm::vec2{6.0f, 6.0f * screenAR}, false, false, false);
        m_guiObjects.push_back(m_buttonExitToMenu);

        sliderPower = std::make_shared<Beryll::SliderVertical>("GUI/slvtrack.png", "GUI/slvthumb.png",
                                                               glm::vec3{1.0f, 5.0f, 0.5f}, glm::vec2{11.0f, 60.0f}, 0.0f, 1.0f);
        m_guiObjects.push_back(sliderPower);

        buttonShot = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/ShotButton2.png", "",
                                                                 glm::vec3{14.2f, 10.0f, 0.5f}, glm::vec2{25.0f / screenAR, 25.0f});
        m_guiObjects.push_back(buttonShot);

        buttonMoveLeft = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/LeftArrow.png", "",
                                                                     glm::vec3{30.0f, 5.0f, 0.5f}, glm::vec2{10.0f, 15.0f}, false, false, false);
        m_guiObjects.push_back(buttonMoveLeft);
        buttonMoveLeft->disable();

        buttonMoveRight = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/RightArrow.png", "",
                                                                      glm::vec3{60.0f, 5.0f, 0.5f}, glm::vec2{10.0f, 15.0f}, false, false, false);
        m_guiObjects.push_back(buttonMoveRight);
        buttonMoveRight->disable();

        // Win/Lose menu.
        m_backgroundWin = std::make_shared<Beryll::GUITexture>("GUI/playState/BackgroundWin.png",
                                                               glm::vec3{86.5f, 27.0f, 0.5f}, glm::vec2{13.0f, 46.0f}, false);
        m_guiObjects.push_back(m_backgroundWin);
        m_backgroundLose = std::make_shared<Beryll::GUITexture>("GUI/playState/BackgroundLose.png",
                                                                glm::vec3{86.5f, 42.0f, 0.5f}, glm::vec2{13.0f, 31.0f}, false);
        m_guiObjects.push_back(m_backgroundLose);

        m_buttonExitToMenu2 = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/ButtonMenu.png", "",
                                                                          glm::vec3{87.5f, 59.0f, 0.6f}, glm::vec2{11.0f, 12.0f}, false, false, false);
        m_guiObjects.push_back(m_buttonExitToMenu2);
        m_buttonReplayMap = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/ButtonReplay.png", "",
                                                                        glm::vec3{87.5f, 44.0f, 0.6f}, glm::vec2{11.0f, 12.0f}, false, false, false);
        m_guiObjects.push_back(m_buttonReplayMap);
        m_buttonNextMap = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/ButtonNextMap.png", "",
                                                                      glm::vec3{87.5f, 29.0f, 0.6f}, glm::vec2{11.0f, 12.0f}, false, false, false);
        m_guiObjects.push_back(m_buttonNextMap);

        // Tutorial.
        if(EnumsAndVars::MapsProgress::currentMapIndex == 1)
        {
            m_tutorialSliderPower = std::make_shared<Beryll::GUITexture>("GUI/playState/tutorial/TutorialSliderPower.png",
                                                                         glm::vec3{11.0f, 45.0f, 0.1f}, glm::vec2{6.0f, 35.0f}, false);
            m_tutorialCamera = std::make_shared<Beryll::GUITexture>("GUI/playState/tutorial/TutorialCamera.png",
                                                                    glm::vec3{66.0f, 30.0f, 0.1f}, glm::vec2{26.0f, 40.0f}, false);
            m_tutorialShot = std::make_shared<Beryll::GUITexture>("GUI/playState/tutorial/TutorialShot.png",
                                                                  glm::vec3{18.0f, 45.0f, 0.1f}, glm::vec2{4.0f, 25.0f}, false);
            m_tutorialWinLoseMenu = std::make_shared<Beryll::GUITexture>("GUI/playState/tutorial/TutorialWinLoseMenu.png",
                                                                         glm::vec3{71.0f, 70.0f, 0.1f}, glm::vec2{16.0f, 30.0f}, false);
        }

        // Sort to update nearest objects first. But draw should starts from farest object(in reverse order).
        std::sort(m_guiObjects.begin(), m_guiObjects.end(), [](std::shared_ptr<Beryll::GUIObject> o1, std::shared_ptr<Beryll::GUIObject> o2)
        {
            return (o1->getPositionNormalized().z > o2->getPositionNormalized().z);
        });
    }

    PlayStateGUILayer::~PlayStateGUILayer()
    {

    }

    void PlayStateGUILayer::updateBeforePhysics()
    {
        for(const std::shared_ptr<Beryll::GUIObject>& go : m_guiObjects)
        {
            if(go->getIsEnabled())
            {
                go->updateBeforePhysics();
            }
        }

        if(sliderPower->getValue() < 0.12f)
            sliderPower->setValue(0.12f);

        if(buttonShot->getIsPressed())
            m_tutorialShotPressed = true;

        if(m_statisticsShow && Beryll::TimeStep::getMilliSecFromStart() > m_statisticsUpdateTime + 200) // Update every 200 ms.
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(1);
            stream << "Frame: " << Beryll::GameLoop::getFrameTime() << "  FPS: " << Beryll::GameLoop::getFPS();
            m_statistics1->text = stream.str();

            stream.str(""); // Way to clear std::stringstream.
            stream << std::fixed << std::setprecision(1);
            stream << "Phys: " << Beryll::Physics::getSimulationTime();
            stream << "  Logic: " << (Beryll::GameLoop::getCPUTime() - Beryll::Physics::getSimulationTime());
            stream << "  GPU: " << Beryll::GameLoop::getGPUTime();
            m_statistics2->text = stream.str();

            stream.str("");
            stream << "Time: " << int(EnumsAndVars::mapPlayTimeSec / 60.0f) << ":" << int(std::fmod(EnumsAndVars::mapPlayTimeSec, 60.0f));
            m_statistics3->text = stream.str();

            m_statisticsUpdateTime = Beryll::TimeStep::getMilliSecFromStart();
        }

        if(!EnumsAndVars::showGUIControls)
        {
            sliderPower->disable();
            buttonShot->disable();
            buttonMoveLeft->disable();
            buttonMoveRight->disable();
        }
        else
        {
            sliderPower->enable();
            buttonShot->enable();

            if(EnumsAndVars::showPlayerMoveArrows)
            {
                buttonMoveLeft->enable();
                buttonMoveRight->enable();
            }
        }

        if(!EnumsAndVars::mapPlayerWin && !EnumsAndVars::mapPlayerLose)
        {
            m_backgroundWin->disable();
            m_backgroundLose->disable();
            m_buttonExitToMenu2->disable();
            m_buttonReplayMap->disable();
            m_buttonNextMap->disable();
        }
        else if(EnumsAndVars::mapPlayerWin)
        {
            m_backgroundWin->enable();
            m_backgroundLose->disable();
            m_buttonExitToMenu2->enable();
            m_buttonReplayMap->enable();
            if(EnumsAndVars::MapsProgress::currentMapIndex < EnumsAndVars::MapsProgress::maxMapIndex)
                m_buttonNextMap->enable();
        }
        else if(EnumsAndVars::mapPlayerLose)
        {
            m_backgroundWin->disable();
            m_backgroundLose->enable();
            m_buttonExitToMenu2->enable();
            m_buttonReplayMap->enable();
            m_buttonNextMap->disable();
        }

        if(m_buttonExitToMenu->getIsPressed() || m_buttonExitToMenu2->getIsPressed())
        {
            GameStateHelper::popState();
            GameStateHelper::pushStartMenuState();
            return;
        }
        else if(m_buttonReplayMap->getIsPressed())
        {
            GameStateHelper::popState();
            GameStateHelper::pushPlayState();
        }
        else if(m_buttonNextMap->getIsPressed())
        {
            GameStateHelper::popState();
            ++EnumsAndVars::MapsProgress::currentMapIndex;
            DataBaseHelper::storeMapsProgressCurrentMapIndex(EnumsAndVars::MapsProgress::currentMapIndex);
            GameStateHelper::pushPlayState();
        }
    }

    void PlayStateGUILayer::updateAfterPhysics()
    {
        // Dont need update.
    }

    void PlayStateGUILayer::draw()
    {
        // Tutorial.
        if(EnumsAndVars::MapsProgress::currentMapIndex == 1)
        {
            if(!m_tutorialSliderChangedValue)
            {
                glm::vec3 currPos = m_tutorialSliderPower->getPositionInPercents();
                m_tutorialSliderPowerPosY = currPos.y;
                m_tutorialSliderPowerPosY += 20.0f * Beryll::TimeStep::getTimeStepSec();
                if(m_tutorialSliderPowerPosY > 63.0f)
                    m_tutorialSliderPowerPosY = 43.0f;

                currPos.y = m_tutorialSliderPowerPosY;
                m_tutorialSliderPower->updatePositionInPercents(currPos);

                m_tutorialSliderPower->draw();

                if(sliderPower->getValue() > 0.7f)
                    m_tutorialSliderChangedValue = true;
            }
            else if(!m_tutorialCameraRotated)
            {
                m_tutorialCamera->draw();

                for(const Beryll::Finger& f : Beryll::EventHandler::getFingers())
                {
                    if(f.normalizedPos.x > 0.55f)
                    {
                        m_tutorialCameraRotated = true;
                        m_tutorialCameraRotatedTime = EnumsAndVars::mapPlayTimeSec;
                    }
                }
            }
            else if(!m_tutorialShotPressed && m_tutorialCameraRotatedTime + 1.5f < EnumsAndVars::mapPlayTimeSec)
            {
                glm::vec3 currPos = m_tutorialShot->getPositionInPercents();
                m_tutorialShotPosY = currPos.y;
                m_tutorialShotPosY -= 10.0f * Beryll::TimeStep::getTimeStepSec();
                if(m_tutorialShotPosY < 35.0f)
                    m_tutorialShotPosY = 45.0f;

                currPos.y = m_tutorialShotPosY;
                m_tutorialShot->updatePositionInPercents(currPos);

                m_tutorialShot->draw();
            }

            if(EnumsAndVars::mapPlayerWin || EnumsAndVars::mapPlayerLose)
            {
                glm::vec3 currPos = m_tutorialWinLoseMenu->getPositionInPercents();
                m_tutorialWinLoseMenuPosY = currPos.y;
                m_tutorialWinLoseMenuPosY -= 30.0f * Beryll::TimeStep::getTimeStepSec();
                if(EnumsAndVars::mapPlayerLose && m_tutorialWinLoseMenuPosY < 44.0f)
                    m_tutorialWinLoseMenuPosY = 71.0f;
                if(EnumsAndVars::mapPlayerWin && m_tutorialWinLoseMenuPosY < 29.0f)
                    m_tutorialWinLoseMenuPosY = 71.0f;

                currPos.y = m_tutorialWinLoseMenuPosY;
                m_tutorialWinLoseMenu->updatePositionInPercents(currPos);

                m_tutorialWinLoseMenu->draw();
            }
        }

        for(auto it = m_guiObjects.rbegin(); it != m_guiObjects.rend(); ++it)
        {
            if((*it)->getIsEnabled())
            {
                (*it)->draw();
            }
        }

        if(!EnumsAndVars::notEmptyBulletsSets.empty())
        {
            const float width = EnumsAndVars::notEmptyBulletsSets[0]->buttonBullet->getWidthHeightInPercents().x;
            const float height = EnumsAndVars::notEmptyBulletsSets[0]->buttonBullet->getWidthHeightInPercents().y;
            const float space = 1.0f;
            const float totalWidth = EnumsAndVars::notEmptyBulletsSets.size() * width + ((EnumsAndVars::notEmptyBulletsSets.size() - 1) * space);
            const float firstElementLeftPos = 50.0f - totalWidth * 0.5f;
            const float bottomPos = 1.0f;
            float currentElementLeftPos = firstElementLeftPos;
            glm::vec3 newPos{0.0f};

            for(int i = 0; i < EnumsAndVars::notEmptyBulletsSets.size(); ++i)
            {
                newPos = EnumsAndVars::notEmptyBulletsSets[i]->buttonBullet->getPositionInPercents();
                newPos.x = currentElementLeftPos;
                newPos.y = bottomPos;
                if(EnumsAndVars::currentBulletSetIndex == i)
                    newPos.y += 1.0f;
                if(newPos != EnumsAndVars::notEmptyBulletsSets[i]->buttonBullet->getPositionInPercents())
                {
                    BR_INFO("%s", "Update button pos.");
                    EnumsAndVars::notEmptyBulletsSets[i]->buttonBullet->updatePositionInPercents(newPos);
                }
                EnumsAndVars::notEmptyBulletsSets[i]->buttonBullet->draw();

                newPos = EnumsAndVars::notEmptyBulletsSets[i]->textBulletsCount->getPositionInPercents();
                newPos.x = currentElementLeftPos + width * 0.35f;
                newPos.y = bottomPos + 3.3f;
                if(EnumsAndVars::currentBulletSetIndex == i)
                    newPos.y += 1.0f;
                if(newPos != EnumsAndVars::notEmptyBulletsSets[i]->textBulletsCount->getPositionInPercents())
                {
                    BR_INFO("%s", "Update text count pos.");
                    EnumsAndVars::notEmptyBulletsSets[i]->textBulletsCount->updatePositionInPercents(newPos);
                }
                EnumsAndVars::notEmptyBulletsSets[i]->textBulletsCount->draw();

                currentElementLeftPos += (width + space);
            }
        }
    }
}
