#pragma once

#include "EngineHeaders.h"

namespace AngryBomb3D
{
    class StartMenuGUILayer : public Beryll::Layer
    {
    public:
        StartMenuGUILayer();
        ~StartMenuGUILayer() override;

        void updateBeforePhysics() override;
        void updateAfterPhysics() override;
        void draw() override;

    private:
        std::vector<std::shared_ptr<Beryll::GUIObject>> m_guiObjects;

        std::shared_ptr<Beryll::ButtonWithTexture> m_buttonMapSwipeLeft;
        std::shared_ptr<Beryll::ButtonWithTexture> m_buttonMapSwipeRight;
        std::shared_ptr<Beryll::ButtonWithTexture> m_buttonPlay;

        void enableDisableButtons();

        std::shared_ptr<Beryll::GUIText> m_levelIndex;
    };
}
