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
            m_statistics1 = Beryll::Renderer::createGUIText("", glm::vec3{0.06f, 0.06f, 0.06f}, glm::vec3{6.5f, 96.3f, 0.0f}, 3.0f);
            m_guiObjects.push_back(m_statistics1);
            m_statistics2 = Beryll::Renderer::createGUIText("", glm::vec3{0.06f, 0.06f, 0.06f}, glm::vec3{6.5f, 92.3f, 0.0f}, 3.0f);
            m_guiObjects.push_back(m_statistics2);
            m_statistics3 = Beryll::Renderer::createGUIText("", glm::vec3{0.06f, 0.06f, 0.06f}, glm::vec3{6.5f, 88.3f, 0.0f}, 3.0f);
            m_guiObjects.push_back(m_statistics3);
        }

//        shotButton = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/ShotButton.png", "",
//                                                                 0.07f, 0.4f, 0.1f, 0.1f * screenAR, true);
//        m_guiObjects.push_back(shotButton);
//
//        moveLeftButton = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/LeftArrow.png", "",
//                                                                     0.15f, 0.75f, 0.15f, 0.2f);
//        m_guiObjects.push_back(moveLeftButton);
//        moveLeftButton->disable();
//
//        moveRightButton = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/RightArrow.png", "",
//                                                                     0.7f, 0.75f, 0.15f, 0.2f);
//        m_guiObjects.push_back(moveRightButton);
//        moveRightButton->disable();
//
//        cameraBackToPlayerButton = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/CameraBackToPlayer.png", "",
//                                                                               0.8f, 0.3f, 0.068f, 0.12f);
//        m_guiObjects.push_back(cameraBackToPlayerButton);
//
//        cameraFollowPlayerButton = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/CameraFollowPlayer.png", "",
//                                                                               0.0f, 0.15f, 0.068f, 0.12f);
//        m_guiObjects.push_back(cameraFollowPlayerButton);
//
//        cameraFollowBulletButton = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/CameraFollowBullet.png", "",
//                                                                               0.0f, 0.15f, 0.068f, 0.12f);
//        m_guiObjects.push_back(cameraFollowBulletButton);
//        cameraFollowBulletButton->disable();

//        m_throwPowerDefaultTexture = Beryll::Renderer::createTexture("GUI/playState/ThrowPowerDefault.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
//        m_throwPowerChargedTexture = Beryll::Renderer::createTexture("GUI/playState/ThrowPowerCharged.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
//        m_cancelShotTexture = Beryll::Renderer::createTexture("GUI/playState/CancelShot.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
//
//        m_enemiesCounterFont = Beryll::MainImGUI::getInstance()->createFont(EnumsAndVars::FontsPath::roboto, 0.08f);
//        m_bulletsCounterFont = Beryll::MainImGUI::getInstance()->createFont(EnumsAndVars::FontsPath::roboto, 0.1f);
//        m_winLoseFont = Beryll::MainImGUI::getInstance()->createFont(EnumsAndVars::FontsPath::roboto, 0.15f);
//
//        m_enemyIconTexture = Beryll::Renderer::createTexture("GUI/playState/EnemyIcon.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
//
//        m_bulletStandard1kgTexture = Beryll::Renderer::createTexture("GUI/playState/bullets/Standard_1kg.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
//        m_bulletsCounterFont = Beryll::MainImGUI::getInstance()->createFont(EnumsAndVars::FontsPath::roboto, 0.07f);
//
//        m_replayMapTexture = Beryll::Renderer::createTexture("GUI/playState/ReplayMap.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
//        m_nextMapTexture = Beryll::Renderer::createTexture("GUI/playState/NextMap.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
//        m_exitTexture = Beryll::Renderer::createTexture("GUI/playState/Exit.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
//
//        m_tutorialCameraTexture = Beryll::Renderer::createTexture("GUI/playState/TutorialCamera.png", Beryll::TextureType::DIFFUSE_TEXTURE_MAT_1);
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

//        m_anyBulletActive = false;
//        for(auto& bulletSet : EnumsAndVars::allBulletsSets)
//        {
//            if(bulletSet.getIsAnyBulletActive())
//            {
//                m_anyBulletActive = true;
//                break;
//            }
//        }
//
//        if(!EnumsAndVars::showGUIControls)
//        {
//            shotButton->disable();
//            moveLeftButton->disable();
//            moveRightButton->disable();
//            cameraFollowBulletButton->disable();
//            cameraFollowPlayerButton->disable();
//        }
//        else
//        {
//            if(!EnumsAndVars::allBulletsSets.empty() && !m_anyBulletActive)
//                shotButton->enable();
//            else
//                shotButton->disable();
//
//            if(EnumsAndVars::showPlayerMoveArrows)
//            {
//                moveLeftButton->enable();
//                moveRightButton->enable();
//            }
//
//            if(EnumsAndVars::cameraShouldFollowBullet)
//            {
//                cameraFollowPlayerButton->disable();
//                cameraFollowBulletButton->enable();
//            }
//            else
//            {
//                cameraFollowPlayerButton->enable();
//                cameraFollowBulletButton->disable();
//            }
//        }
//
//        if(cameraFollowPlayerButton->getIsPressed())
//            EnumsAndVars::cameraShouldFollowBullet = true;
//        else if(cameraFollowBulletButton->getIsPressed())
//            EnumsAndVars::cameraShouldFollowBullet = false;
//
//        if(m_nextMapButtonPressed)
//        {
//            m_nextMapButtonPressed = true;
//
//            AngryBomb3D::GameStateHelper::popState();
//            ++EnumsAndVars::MapsProgress::currentMapIndex;
//            DataBaseHelper::storeMapsProgressCurrentMapIndex(EnumsAndVars::MapsProgress::currentMapIndex);
//            AngryBomb3D::GameStateHelper::pushPlayState();
//        }
//        else if(m_replayMapButtonPressed)
//        {
//            m_replayMapButtonPressed = false;
//
//            AngryBomb3D::GameStateHelper::popState();
//            AngryBomb3D::GameStateHelper::pushPlayState();
//        }
//        else if(m_exitButtonPressed)
//        {
//            m_exitButtonPressed = false;
//
//            AngryBomb3D::GameStateHelper::popState();
//            AngryBomb3D::GameStateHelper::pushStartMenuState();
//        }
    }

    void PlayStateGUILayer::updateAfterPhysics()
    {
        // Dont need update.
    }

    void PlayStateGUILayer::draw()
    {
        for(auto it = m_guiObjects.rbegin(); it != m_guiObjects.rend(); ++it)
        {
            if((*it)->getIsEnabled())
            {
                (*it)->draw();
            }
        }

//        if(EnumsAndVars::mapPlayerWin)
//        {
//            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0.15f, 0.15f, 0.15f, 1.0f});
//            ImGui::SetNextWindowPos(ImVec2(0.28f * GUIWidth, 0.3f * GUIHeight));
//            ImGui::SetNextWindowSize(ImVec2(0.44f * GUIWidth, 0.4f * GUIHeight));
//            ImGui::Begin("playerWinMenu", nullptr, m_noFrame);
//
//            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.9f, 0.9f, 0.9f, 1.0f});
//            ImGui::PushFont(m_winLoseFont);
//            ImGui::SetCursorPos(ImVec2(0.165f * GUIWidth, 0.0f * GUIHeight));
//            ImGui::Text("WIN");
//            ImGui::PopFont();
//            ImGui::PopStyleColor(1);
//
//            if(EnumsAndVars::MapsProgress::currentMapIndex < EnumsAndVars::MapsProgress::maxMapIndex)
//            {
//                ImGui::SetCursorPos(ImVec2(0.0f * GUIWidth, 0.247f * GUIHeight));
//                if(ImGui::ImageButton("nextMapButton", reinterpret_cast<ImTextureID>(ImU64(m_nextMapTexture->getID())),
//                                      ImVec2(0.14f * GUIWidth, 0.153f * GUIHeight)))
//                    m_nextMapButtonPressed = true;
//            }
//
//            ImGui::SetCursorPos(ImVec2(0.15f * GUIWidth, 0.247f * GUIHeight));
//            if(ImGui::ImageButton("replayMapButton", reinterpret_cast<ImTextureID>(ImU64(m_replayMapTexture->getID())),
//                                  ImVec2(0.14f * GUIWidth, 0.153f * GUIHeight)))
//                m_replayMapButtonPressed = true;
//
//            ImGui::SetCursorPos(ImVec2(0.3f * GUIWidth, 0.247f * GUIHeight));
//            if(ImGui::ImageButton("exitButton", reinterpret_cast<ImTextureID>(ImU64(m_exitTexture->getID())),
//                                  ImVec2(0.14f * GUIWidth, 0.153f * GUIHeight)))
//                m_exitButtonPressed = true;
//
//            ImGui::End();
//            ImGui::PopStyleColor(1);
//
//            return;
//        }
//        else if(EnumsAndVars::mapPlayerLose) // EnumsAndVars::mapPlayerLose
//        {
//            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0.15f, 0.15f, 0.15f, 1.0f});
//            ImGui::SetNextWindowPos(ImVec2(0.28f * GUIWidth, 0.3f * GUIHeight));
//            ImGui::SetNextWindowSize(ImVec2(0.44f * GUIWidth, 0.4f * GUIHeight));
//            ImGui::Begin("playerLoseMenu", nullptr, m_noFrame);
//
//            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.9f, 0.9f, 0.9f, 1.0f});
//            ImGui::PushFont(m_winLoseFont);
//            ImGui::SetCursorPos(ImVec2(0.15f * GUIWidth, 0.0f * GUIHeight));
//            ImGui::Text("LOSE");
//            ImGui::PopFont();
//            ImGui::PushFont(m_enemiesCounterFont);
//            ImGui::SetCursorPos(ImVec2(0.225f * GUIWidth, 0.16f * GUIHeight));
//            ImGui::Text("%d/%d", BaseEnemy::getActiveCount(), BaseEnemy::getAllCount());
//            ImGui::PopFont();
//            ImGui::PopStyleColor(1);
//
//            ImGui::SetCursorPos(ImVec2(0.175f * GUIWidth, 0.16f * GUIHeight));
//            ImGui::Image(reinterpret_cast<ImTextureID>(ImU64(m_enemyIconTexture->getID())),
//                         ImVec2(0.04f * GUIWidth, 0.04f * m_screenAR * GUIHeight));
//
//            ImGui::SetCursorPos(ImVec2(0.0f * GUIWidth, 0.247f * GUIHeight));
//            if(ImGui::ImageButton("replayMapButton", reinterpret_cast<ImTextureID>(ImU64(m_replayMapTexture->getID())),
//                                  ImVec2(0.14f * GUIWidth, 0.153f * GUIHeight)))
//                m_replayMapButtonPressed = true;
//
//            ImGui::SetCursorPos(ImVec2(0.3f * GUIWidth, 0.247f * GUIHeight));
//            if(ImGui::ImageButton("exitButton", reinterpret_cast<ImTextureID>(ImU64(m_exitTexture->getID())),
//                                  ImVec2(0.14f * GUIWidth, 0.153f * GUIHeight)))
//                m_exitButtonPressed = true;
//
//            ImGui::End();
//            ImGui::PopStyleColor(1);
//
//            return;
//        }
//
//        ImGui::SetNextWindowPos(ImVec2(0.895f * GUIWidth, 0.0f * GUIHeight));
//        ImGui::SetNextWindowSize(ImVec2(0.0f * GUIWidth, 0.0f * GUIHeight));
//        ImGui::Begin("enemiesCounter", nullptr, m_noBackgroundNoFrame);
//
//        ImGui::SetCursorPos(ImVec2(0.0f * GUIWidth, 0.0f * GUIHeight));
//        ImGui::Image(reinterpret_cast<ImTextureID>(ImU64(m_enemyIconTexture->getID())),
//                     ImVec2(0.04f * GUIWidth, 0.04f * m_screenAR * GUIHeight));
//
//        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.15f, 0.15f, 0.15f, 1.0f});
//        ImGui::PushFont(m_enemiesCounterFont);
//        ImGui::SetCursorPos(ImVec2(0.043f * GUIWidth, 0.0f * GUIHeight));
//        ImGui::Text("%d/%d", BaseEnemy::getActiveCount(), BaseEnemy::getAllCount());
//        ImGui::PopFont();
//        ImGui::PopStyleColor(1);
//
//        ImGui::End();
//
//        if(EnumsAndVars::MapsProgress::currentMapIndex == 0 && showTutorialCameraTip)
//        {
//            ImGui::SetNextWindowPos(ImVec2(0.72f * GUIWidth, 0.52f * GUIHeight));
//            ImGui::SetNextWindowSize(ImVec2(0.0f * GUIWidth, 0.0f * GUIHeight));
//            ImGui::Begin("tutorialCameraTip", nullptr, m_noBackgroundNoFrame);
//
//            ImGui::Image(reinterpret_cast<ImTextureID>(ImU64(m_tutorialCameraTexture->getID())),
//                         ImVec2(0.15f * GUIWidth, 0.25f * GUIHeight));
//
//            ImGui::End();
//        }
//
//        if(!EnumsAndVars::showGUIControls || EnumsAndVars::allBulletsSets.empty() || m_anyBulletActive)
//            return;
//
//        ImGui::SetNextWindowPos(ImVec2(0.2f * GUIWidth, 0.3f * GUIHeight));
//        ImGui::SetNextWindowSize(ImVec2(0.0f * GUIWidth, 0.0f * GUIHeight));
//        ImGui::Begin("throwPowerBar", nullptr, m_noBackgroundNoFrame);
//
//        // Throw power bar.
//        float elemTop = 0.39f;
//        float elemWidth = 0.01f;
//        const float chargedFraction = EnumsAndVars::throwPowerCharged / EnumsAndVars::throwPowerMaxCharged;
//        const int chargedGUIThreshold = int(25.0f * chargedFraction);
//        for(int i = 0; i < 25; ++i)
//        {
//            ImGui::SetCursorPos(ImVec2((0.026f - (elemWidth * 0.5f)) * GUIWidth, elemTop * GUIHeight));
//            if(i < chargedGUIThreshold)
//                ImGui::Image(reinterpret_cast<ImTextureID>(ImU64(m_throwPowerChargedTexture->getID())),
//                             ImVec2(elemWidth * GUIWidth, 0.015f * GUIHeight));
//            else
//                ImGui::Image(reinterpret_cast<ImTextureID>(ImU64(m_throwPowerDefaultTexture->getID())),
//                             ImVec2(elemWidth * GUIWidth, 0.015f * GUIHeight));
//
//            elemTop -= 0.015f;
//            elemWidth += 0.0015f;
//        }
//
//        ImGui::End();
//
//        if(shotButton->getIsPressed())
//        {
//            ImGui::SetNextWindowPos(ImVec2(0.08f * GUIWidth, 0.62f * GUIHeight));
//            ImGui::SetNextWindowSize(ImVec2(0.0f * GUIWidth, 0.0f * GUIHeight));
//            ImGui::Begin("cancelShotButton", nullptr, m_noBackgroundNoFrame);
//
//            ImGui::Image(reinterpret_cast<ImTextureID>(ImU64(m_cancelShotTexture->getID())),
//                         ImVec2(0.08f * GUIWidth, 0.08f * m_screenAR * GUIHeight));
//
//            ImGui::End();
//        }
//
//        // Bullets.
//        const float bulletImageWidth = 0.13f / m_screenAR * GUIWidth;
//        const float bulletImageHeight = 0.13f * GUIHeight;
//        const float spaceBetween = bulletImageWidth * 0.1f;
//        const int bulletsSetsCount = EnumsAndVars::allBulletsSets.size();
//        const float totalWidth = (bulletImageWidth * bulletsSetsCount) + (spaceBetween * (bulletsSetsCount - 1));
//
//        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0.0f, 0.0f, 0.0f, 0.9f});
//        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.15f, 0.15f, 0.15f, 1.0f});
//        ImGui::SetNextWindowPos(ImVec2(0.5f * GUIWidth - totalWidth * 0.5f, 0.75f * GUIHeight));
//        ImGui::SetNextWindowSize(ImVec2(0.0f * GUIWidth, 0.0f * GUIHeight));
//        ImGui::Begin("bullets", nullptr, m_noBackgroundNoFrame); // m_noBackgroundNoFrame
//        ImGui::PushFont(m_bulletsCounterFont);
//
//        for(int i = 0; i < EnumsAndVars::allBulletsSets.size(); ++i)
//        {
//            const float currentLeftPos = (bulletImageWidth + spaceBetween) * i;
//
//            uint32_t textureId = 0;
//            if(EnumsAndVars::allBulletsSets[i].getType() == BulletType::STANDARD_1_KG)
//            {
//                textureId = m_bulletStandard1kgTexture->getID();
//            }
//            else
//            {
//                BR_ASSERT(false, "%s", "Unknown Bullet Type.")
//            }
//
//            m_bulletButtonID = "bulletButton" + std::to_string(i);
//            if(i == EnumsAndVars::currentBulletSetIndex) // Selected bullet.
//            {
//                // Count text.
//                ImGui::SetCursorPos(ImVec2(currentLeftPos + bulletImageWidth * 0.4f, 0.0f * GUIHeight));
//                ImGui::Text("%d", EnumsAndVars::allBulletsSets[i].getAvailableCount());
//
//                // Bullet image.
//                ImGui::SetCursorPos(ImVec2(currentLeftPos, 0.1f * GUIHeight - bulletImageHeight * 0.2f));
//                if(ImGui::ImageButton(m_bulletButtonID.c_str(), reinterpret_cast<ImTextureID>(ImU64(textureId)),ImVec2(bulletImageWidth, bulletImageHeight)))
//                    EnumsAndVars::currentBulletSetIndex = i;
//            }
//            else
//            {
//                // Count text.
//                ImGui::SetCursorPos(ImVec2(currentLeftPos + bulletImageWidth * 0.4f, 0.0f * GUIHeight + bulletImageHeight * 0.2f));
//                ImGui::Text("%d", EnumsAndVars::allBulletsSets[i].getAvailableCount());
//
//                // Bullet image.
//                ImGui::SetCursorPos(ImVec2(currentLeftPos, 0.1f * GUIHeight));
//                if(ImGui::ImageButton(m_bulletButtonID.c_str(), reinterpret_cast<ImTextureID>(ImU64(textureId)),ImVec2(bulletImageWidth, bulletImageHeight)))
//                    EnumsAndVars::currentBulletSetIndex = i;
//            }
//        }
    }
}
