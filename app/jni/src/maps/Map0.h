#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map0 : public BaseMap
    {
    public:
        Map0(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map0() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
