#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map : public BaseMap
    {
    public:
        Map(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
