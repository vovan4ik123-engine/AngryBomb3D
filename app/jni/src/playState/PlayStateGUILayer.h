#pragma once

#include "EngineHeaders.h"

namespace AngryBomb3D
{
    class PlayStateGUILayer : public Beryll::Layer
    {
    public:
        PlayStateGUILayer();
        ~PlayStateGUILayer() override;

        void updateBeforePhysics() override;
        void updateAfterPhysics() override;
        void draw() override;

        std::shared_ptr<Beryll::ButtonWithTexture> shotButton;
        std::shared_ptr<Beryll::ButtonWithTexture> moveLeftButton;
        std::shared_ptr<Beryll::ButtonWithTexture> moveRightButton;
        std::shared_ptr<Beryll::ButtonWithTexture> cameraBackToPlayerButton;
        std::shared_ptr<Beryll::ButtonWithTexture> cameraFollowPlayerButton;
        std::shared_ptr<Beryll::ButtonWithTexture> cameraFollowBulletButton;

        bool showTutorialCameraTip = false;

    private:
        std::vector<std::shared_ptr<Beryll::GUIObject>> m_guiObjects;

#if defined(BR_DEBUG)
        bool m_statisticsShow = true;
#else
        bool m_statisticsShow = false;
#endif
        std::shared_ptr<Beryll::GUIText> m_statistics1;
        std::shared_ptr<Beryll::GUIText> m_statistics2;
        std::shared_ptr<Beryll::GUIText> m_statistics3;
        uint64_t m_statisticsUpdateTime = 0;
    };
}
