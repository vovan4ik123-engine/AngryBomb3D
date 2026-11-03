#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map10 : public BaseMap
    {
    public:
        Map10(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map10() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
