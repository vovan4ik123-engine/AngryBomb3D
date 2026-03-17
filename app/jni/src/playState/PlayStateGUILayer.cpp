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

        sliderPower = std::make_shared<Beryll::SliderVertical>("GUI/SliderVerticalTrack.png", "GUI/SliderVerticalThumb.png",
                                                               glm::vec3{8.0f, 10.0f, 0.5f}, glm::vec2{2.5f, 50.0f}, 0.0f, 300.0f);
        m_guiObjects.push_back(sliderPower);

        buttonShot = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/ShotButton.png", "",
                                                                 glm::vec3{16.0f, 15.0f, 0.5f}, glm::vec2{20.0f / screenAR, 20.0f});
        m_guiObjects.push_back(buttonShot);

        buttonMoveLeft = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/LeftArrow.png", "",
                                                                     glm::vec3{30.0f, 5.0f, 0.5f}, glm::vec2{10.0f, 15.0f});
        m_guiObjects.push_back(buttonMoveLeft);
        buttonMoveLeft->disable();

        buttonMoveRight = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/RightArrow.png", "",
                                                                      glm::vec3{60.0f, 5.0f, 0.5f}, glm::vec2{10.0f, 15.0f});
        m_guiObjects.push_back(buttonMoveRight);
        buttonMoveRight->disable();

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

//        if(m_nextMapButtonPressed)
//        {
//            AngryBomb3D::GameStateHelper::popState();
//            ++EnumsAndVars::MapsProgress::currentMapIndex;
//            DataBaseHelper::storeMapsProgressCurrentMapIndex(EnumsAndVars::MapsProgress::currentMapIndex);
//            AngryBomb3D::GameStateHelper::pushPlayState();
//        }
//        else if(m_replayMapButtonPressed)
//        {
//            AngryBomb3D::GameStateHelper::popState();
//            AngryBomb3D::GameStateHelper::pushPlayState();
//        }
//        else if(m_exitButtonPressed)
//        {
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
    }
}
