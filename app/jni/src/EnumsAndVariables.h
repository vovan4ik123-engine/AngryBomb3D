#pragma once

#include "bullets/BulletSet.h"

namespace EnumsAndVars
{
    struct FontsPath
    {
        static const inline std::string roboto = "fonts/roboto.ttf";
        static const inline std::string cousine = "fonts/cousine.ttf";
    };

    struct AnimationIndexes
    {
        static constexpr inline int run = 0;    // 1_Run    - names in blender.
        static constexpr inline int stand = 1;  // 2_Stand
        static constexpr inline int attack = 2; // 3_Attack
    };

    enum class CameraState
    {
        FOLLOW_PLAYER = 0,
        FOLLOW_BULLET = 1,
        FROZEN_IN_AIR = 2
    };
    inline bool cameraShouldFollowBullet = false;

    inline bool showGUIControls = true;

    // Database tables.
    struct SettingsMenu
    {
        // Stored in DB.
        static inline int FPSLimit = 60;
        // Not stored in DB.
        static constexpr inline float cameraHorizontalSpeed = 350.0f * 0.8f;
        static constexpr inline float cameraVerticalSpeed = 125.0f * 0.8f;
    };

    struct MapsProgress
    {
        // Stored in DB.
        static inline int currentMapIndex = 0; // Index of selected map on start screen.
        static inline int lastOpenedMapIndex = 10; // Last index of map available for select and play.
        // Not stored in DB.
        static constexpr inline int maxMapIndex = 10; // Last possible map index (opened or not). Must be hardcoded before release.
        static inline int mapIndexWhenMapStart = 0;
    };

    constexpr inline float bulletMass = 1.0f;
    constexpr inline glm::vec3 bulletGravity{0.0f, -60.0f, 0.0f};

    // Throw power.
    constexpr inline float throwPowerMin = 50.0f;
    inline float throwPowerCharged = 0.0f;
    constexpr inline float throwPowerMaxCharged = 110.0f;
    constexpr inline float throwPowerChargeSpeed = 60.0f; // In one sec.
    inline bool throwPowerCharging = false;

    inline bool showPlayerMoveArrows = false;

    inline std::vector<AngryBomb3D::BulletSet> allBulletsSets;
    inline int currentBulletSetIndex = 0;

    // Map.
    inline float mapPlayTimeSec = 0.0f;
    inline bool mapPlayerWin = false;
    inline bool mapPlayerLose = false;

    // Pause.
    inline bool gameOnPause = false;

    inline void reset()
    {
        BR_INFO("%s", "EnumsAndVars::reset()");
        cameraShouldFollowBullet = false;

        showGUIControls = true;

        // Throw power.
        throwPowerCharged = 0.0f;
        throwPowerCharging = false;

        showPlayerMoveArrows = false;

        allBulletsSets.clear();
        currentBulletSetIndex = 0;

        // Map.
        mapPlayTimeSec = 0.0f;
        mapPlayerWin = false;
        mapPlayerLose = false;

        // Pause.
        gameOnPause = false;
    }
}
