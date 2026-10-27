#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map7 : public BaseMap
    {
    public:
        Map7(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map7() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
