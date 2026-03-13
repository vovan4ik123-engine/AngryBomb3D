#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map3 : public BaseMap
    {
    public:
        Map3(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map3() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
