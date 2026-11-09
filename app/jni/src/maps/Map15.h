#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map15 : public BaseMap
    {
    public:
        Map15(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map15() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
