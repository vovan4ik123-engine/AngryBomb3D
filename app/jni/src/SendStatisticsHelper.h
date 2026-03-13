#pragma once

#include "EngineHeaders.h"

namespace AngryBomb3D
{
    class SendStatisticsHelper
    {
    public:
        static void reset();

        // Maps progress.
        static void sendMapStart();
        static void sendMapWin();

        // Custom message.
        static void sendCustomMessage(const std::string text);

    private:
        static bool m_canSendMapStartEvent;
        static bool m_canSendMapWinEvent;
    };
}
