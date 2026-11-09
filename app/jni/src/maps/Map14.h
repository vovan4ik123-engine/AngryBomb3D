#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map14 : public BaseMap
    {
    public:
        Map14(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map14() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
