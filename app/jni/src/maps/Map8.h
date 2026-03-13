#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map8 : public BaseMap
    {
    public:
        Map8(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map8() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
