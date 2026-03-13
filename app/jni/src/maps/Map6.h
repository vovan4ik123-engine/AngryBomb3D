#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map6 : public BaseMap
    {
    public:
        Map6(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map6() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
