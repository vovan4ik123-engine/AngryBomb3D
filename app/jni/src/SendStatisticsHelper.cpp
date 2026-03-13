#include "SendStatisticsHelper.h"
#include "EnumsAndVariables.h"

namespace AngryBomb3D
{
    // Maps progress.
    bool SendStatisticsHelper::m_canSendMapStartEvent = true;
    bool SendStatisticsHelper::m_canSendMapWinEvent = true;

    void SendStatisticsHelper::reset()
    {
        // Maps progress.
        m_canSendMapStartEvent = true;
        m_canSendMapWinEvent = true;
    }

    void SendStatisticsHelper::sendMapStart()
    {
        if(m_canSendMapStartEvent)
        {
            m_canSendMapStartEvent = false;

            std::string eventType = "map_" + std::to_string(EnumsAndVars::MapsProgress::currentMapIndex) + "_start";

            Beryll::GoogleAnalytics::getInstance()->sendEventEmpty(eventType);
        }
    }

    void SendStatisticsHelper::sendMapWin()
    {
        if(m_canSendMapWinEvent)
        {
            m_canSendMapWinEvent = false;

            std::string eventType = "map_" + std::to_string(EnumsAndVars::MapsProgress::currentMapIndex) + "_win";

            Beryll::GoogleAnalytics::getInstance()->sendEventEmpty(eventType);
        }
    }
}
