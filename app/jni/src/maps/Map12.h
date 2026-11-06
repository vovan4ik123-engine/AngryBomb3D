#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map12 : public BaseMap
    {
    public:
        Map12(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map12() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
