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

    inline bool showGUIControls = true;

    // Database tables.
    struct SettingsMenu
    {
        // Stored in DB.
        static inline int FPSLimit = 60;
        // Not stored in DB.
        static constexpr inline float cameraDistance = 47.0f;
        static constexpr inline float cameraHorizontalSpeed = 350.0f * 0.5f;
        static constexpr inline float cameraVerticalSpeed = 125.0f * 0.5f;
    };

    struct MapsProgress
    {
        // Stored in DB.
        static inline int currentMapIndex = 0; // Index of selected map on start screen.
        static inline int lastOpenedMapIndex = 2; // Last index of map available for select and play.
        // Not stored in DB.
        static constexpr inline int maxMapIndex = 10; // Last possible map index (opened or not). Must be hardcoded before release.
        static inline int mapIndexWhenMapStart = 0;
    };

    constexpr inline float bulletMass = 1.0f;
    constexpr inline glm::vec3 bulletGravity{0.0f, -60.0f, 0.0f};

    constexpr inline float throwPowerDefault = 50.0f;
    inline float throwAngleRadians = 0.0f;

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
        showGUIControls = true;

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
