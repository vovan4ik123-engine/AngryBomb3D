#include "PlayStateSceneLayer.h"
#include "EnumsAndVariables.h"
#include "SendStatisticsHelper.h"
#include "enemies/BaseEnemy.h"
#include "maps/Map0.h"
#include "maps/Map1.h"
#include "maps/Map2.h"
#include "maps/Map3.h"
#include "maps/Map4.h"
#include "maps/Map5.h"
#include "maps/Map6.h"
#include "maps/Map7.h"
#include "maps/Map8.h"
#include "maps/Map9.h"
#include "maps/Map10.h"

namespace AngryBomb3D
{
    PlayStateSceneLayer::PlayStateSceneLayer(std::shared_ptr<PlayStateGUILayer> gui)
    {
        m_ID = Beryll::LayerID::PLAY_SCENE;

        Beryll::Physics::hardRemoveAllObjects();
        EnumsAndVars::reset();
        BaseEnemy::resetCounts();
        SendStatisticsHelper::reset();

        SendStatisticsHelper::sendMapStart();

        if(EnumsAndVars::MapsProgress::currentMapIndex == 0)
            m_map = std::make_shared<Map0>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 1)
            m_map = std::make_shared<Map1>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 2)
            m_map = std::make_shared<Map2>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 3)
            m_map = std::make_shared<Map2>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 4)
            m_map = std::make_shared<Map3>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 5)
            m_map = std::make_shared<Map4>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 6)
            m_map = std::make_shared<Map5>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 7)
            m_map = std::make_shared<Map6>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 8)
            m_map = std::make_shared<Map7>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 9)
            m_map = std::make_shared<Map8>(gui);
        else if(EnumsAndVars::MapsProgress::currentMapIndex == 10)
            m_map = std::make_shared<Map9>(gui);
        else
        {
            BR_ASSERT(false, "Map index does not handled: %d", EnumsAndVars::MapsProgress::currentMapIndex);
        }

        EnumsAndVars::MapsProgress::mapIndexWhenMapStart = EnumsAndVars::MapsProgress::currentMapIndex;
        Beryll::Physics::enableSimulation();
        Beryll::TimeStep::fixateTime();

        //BR_INFO(" X:%f Y:%f Z:%f", .x, .y, .z);
        //BR_INFO("%s", "");
        //BR_INFO(" %f", );
        //BR_INFO(" %d", );
    }

    PlayStateSceneLayer::~PlayStateSceneLayer()
    {

    }

    void PlayStateSceneLayer::updateBeforePhysics()
    {
        m_map->updateBeforePhysics();
    }

    void PlayStateSceneLayer::updateAfterPhysics()
    {
        m_map->updateAfterPhysics();
    }

    void PlayStateSceneLayer::draw()
    {
        m_map->draw();
    }
}
