#pragma once

#include "BaseMap.h"

namespace AngryBomb3D
{
    class Map13 : public BaseMap
    {
    public:
        Map13(std::shared_ptr<PlayStateGUILayer> gui);
        ~Map13() override;

    private:
        void loadEnv();
        void loadEnemies();
        void loadPlayerPoints();
    };
}
