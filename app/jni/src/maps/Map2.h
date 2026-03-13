#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map2 : public BaseMap
    {
    public:
        Map2(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map2() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
