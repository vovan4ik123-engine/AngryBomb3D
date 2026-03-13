#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map5 : public BaseMap
    {
    public:
        Map5(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map5() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
