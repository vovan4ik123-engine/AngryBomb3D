#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map9 : public BaseMap
    {
    public:
        Map9(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map9() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
