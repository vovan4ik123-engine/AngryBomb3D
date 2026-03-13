#include "GameStateHelper.h"
#include "playState/PlayStateGUILayer.h"
#include "playState/PlayStateSceneLayer.h"
#include "menus/start/StartMenuGUILayer.h"

namespace AngryBomb3D
{
    void GameStateHelper::pushStartMenuState()
    {
        std::shared_ptr<StartMenuGUILayer> GUILayer = std::make_shared<StartMenuGUILayer>();

        std::shared_ptr<Beryll::GameState> startMenuState = std::make_shared<Beryll::GameState>();
        startMenuState->ID = Beryll::GameStateID::START_SCREEN;
        startMenuState->layerStack.pushOverlay(GUILayer);

        Beryll::GameStateMachine::pushState(startMenuState);
    }

    void GameStateHelper::pushPlayState()
    {
        std::shared_ptr<AngryBomb3D::PlayStateGUILayer> GUILayer = std::make_shared<AngryBomb3D::PlayStateGUILayer>();
        std::shared_ptr<AngryBomb3D::PlayStateSceneLayer> sceneLayer = std::make_shared<AngryBomb3D::PlayStateSceneLayer>(GUILayer);

        std::shared_ptr<Beryll::GameState> playState = std::make_shared<Beryll::GameState>();
        playState->ID = Beryll::GameStateID::PLAY;
        playState->layerStack.pushLayer(sceneLayer);
        playState->layerStack.pushOverlay(GUILayer);

        Beryll::GameStateMachine::pushState(playState);
    }

    void GameStateHelper::popState()
    {
        Beryll::ParticleSystem::disableAll(); // Before state exited.

        Beryll::GameStateMachine::popState();
    }
}
