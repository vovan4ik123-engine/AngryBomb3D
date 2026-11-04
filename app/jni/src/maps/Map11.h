#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map11 : public BaseMap
    {
    public:
        Map11(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map11() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
