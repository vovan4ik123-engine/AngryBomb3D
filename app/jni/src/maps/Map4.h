#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map4 : public BaseMap
    {
    public:
        Map4(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map4() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
