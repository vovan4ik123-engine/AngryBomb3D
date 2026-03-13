#pragma once

namespace AngryBomb3D
{
    class GameStateHelper
    {
    public:
        static void pushStartMenuState();
        static void pushPlayState();
        static void popState();
    };
}
