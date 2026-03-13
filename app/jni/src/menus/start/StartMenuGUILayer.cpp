#include "StartMenuGUILayer.h"
#include "EnumsAndVariables.h"
#include "GameStateHelper.h"
#include "DataBaseHelper.h"

namespace AngryBomb3D
{
    StartMenuGUILayer::StartMenuGUILayer()
    {
        m_ID = Beryll::LayerID::START_SCREEN_GUI;

        auto background = std::make_shared<Beryll::GUITexture>("GUI/menus/start/StartBackground.jpg",
                                                               glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec2{100.0f, 100.0f});
        m_guiObjects.push_back(background);

        m_buttonMapSwipeLeft = std::make_shared<Beryll::ButtonWithTexture>("GUI/menus/start/MapSwipeLeft.png", "",
                                                                           glm::vec3{20.0f, 45.0f, 0.1f}, glm::vec2{10.0f, 14.0f});
        m_guiObjects.push_back(m_buttonMapSwipeLeft);
        m_buttonMapSwipeRight = std::make_shared<Beryll::ButtonWithTexture>("GUI/menus/start/MapSwipeRight.png", "",
                                                                            glm::vec3{70.0f, 45.0f, 0.1f}, glm::vec2{10.0f, 14.0f});
        m_guiObjects.push_back(m_buttonMapSwipeRight);
        m_buttonPlay = std::make_shared<Beryll::ButtonWithTexture>("GUI/menus/start/Play.png", "",
                                                                   glm::vec3{40.0f, 7.0f, 0.1f}, glm::vec2{20.0f, 20.0f});
        m_guiObjects.push_back(m_buttonPlay);

        m_levelIndex = Beryll::Renderer::createGUIText("", glm::vec3{0.06f, 0.06f, 0.06f}, glm::vec3{34.0f, 45.0f, 0.1f}, 20.0f);
        m_guiObjects.push_back(m_levelIndex);

        enableDisableButtons();

        // Sort to update nearest objects first. But draw should starts from farest object(in reverse order).
        std::sort(m_guiObjects.begin(), m_guiObjects.end(), [](std::shared_ptr<Beryll::GUIObject> o1, std::shared_ptr<Beryll::GUIObject> o2)
        {
            return (o1->getPositionNormalized().z > o2->getPositionNormalized().z);
        });
    }

    StartMenuGUILayer::~StartMenuGUILayer()
    {

    }

    void StartMenuGUILayer::updateBeforePhysics()
    {
        for(const std::shared_ptr<Beryll::GUIObject>& go : m_guiObjects)
        {
            if(go->getIsEnabled())
            {
                go->updateBeforePhysics();
            }
        }

        m_levelIndex->text = "Level: ";
        m_levelIndex->text += std::to_string(EnumsAndVars::MapsProgress::currentMapIndex);

        if(m_buttonPlay->getIsPressed())
        {
            GameStateHelper::popState();
            GameStateHelper::pushPlayState();
        }
        else if(m_buttonMapSwipeLeft->getIsPressed() && EnumsAndVars::MapsProgress::currentMapIndex > 0)
        {
            --EnumsAndVars::MapsProgress::currentMapIndex;
            enableDisableButtons();
            BR_INFO("currentMapIndex: %d", EnumsAndVars::MapsProgress::currentMapIndex);
            DataBaseHelper::storeMapsProgressCurrentMapIndex(EnumsAndVars::MapsProgress::currentMapIndex);
        }
        else if(m_buttonMapSwipeRight->getIsPressed() &&
                EnumsAndVars::MapsProgress::currentMapIndex < EnumsAndVars::MapsProgress::lastOpenedMapIndex &&
                EnumsAndVars::MapsProgress::currentMapIndex < EnumsAndVars::MapsProgress::maxMapIndex)
        {
            ++EnumsAndVars::MapsProgress::currentMapIndex;
            enableDisableButtons();
            BR_INFO("currentMapIndex: %d", EnumsAndVars::MapsProgress::currentMapIndex);
            DataBaseHelper::storeMapsProgressCurrentMapIndex(EnumsAndVars::MapsProgress::currentMapIndex);
        }
    }

    void StartMenuGUILayer::updateAfterPhysics()
    {

    }

    void StartMenuGUILayer::draw()
    {
        for(auto it = m_guiObjects.rbegin(); it != m_guiObjects.rend(); ++it)
        {
            if((*it)->getIsEnabled())
            {
                (*it)->draw();
            }
        }
    }

    void StartMenuGUILayer::enableDisableButtons()
    {
        // Map swipe left enable.
        if(EnumsAndVars::MapsProgress::currentMapIndex > 0)
        {
            m_buttonMapSwipeLeft->enable();
        }
        else
        {
            m_buttonMapSwipeLeft->disable();
        }

        // Map swipe right enable.
        if(EnumsAndVars::MapsProgress::currentMapIndex < EnumsAndVars::MapsProgress::lastOpenedMapIndex &&
           EnumsAndVars::MapsProgress::currentMapIndex < EnumsAndVars::MapsProgress::maxMapIndex)
        {
            m_buttonMapSwipeRight->enable();
        }
        else
        {
            m_buttonMapSwipeRight->disable();
        }
    }
}
