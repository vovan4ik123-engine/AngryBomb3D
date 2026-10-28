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
        static constexpr inline float cameraDistance = 20.0f;
        static constexpr inline float cameraHorizontalSpeed = 350.0f * 0.5f;
        static constexpr inline float cameraVerticalSpeed = 125.0f * 0.5f;
    };

    struct MapsProgress
    {
        // Stored in DB.
        static inline int currentMapIndex = 1; // Index of selected map on start screen.
        static inline int lastOpenedMapIndex = 15; // Last index of map available for select and play.
        // Not stored in DB.
        static constexpr inline int maxMapIndex = 15; // Last possible map index (opened or not). Must be hardcoded before release.
        static inline int mapIndexWhenMapStart = 1;
    };

    constexpr inline Beryll::CollisionGroups CollGr_DYNAMIC_ENV = Beryll::CollisionGroups::GROUP_1;
    constexpr inline Beryll::CollisionGroups CollGr_STATIC_ENV = Beryll::CollisionGroups::GROUP_2;
    constexpr inline Beryll::CollisionGroups CollGr_PLAYER = Beryll::CollisionGroups::GROUP_3;
    constexpr inline Beryll::CollisionGroups CollGr_PLAYER_BULLET = Beryll::CollisionGroups::GROUP_4;

    constexpr inline Beryll::SceneObjectGroups SceneGR_NONE = Beryll::SceneObjectGroups::NONE;
    constexpr inline Beryll::SceneObjectGroups SceneGR_GROUND = Beryll::SceneObjectGroups::GROUP_1;
    constexpr inline Beryll::SceneObjectGroups SceneGR_PLAYER = Beryll::SceneObjectGroups::GROUP_2;
    constexpr inline Beryll::SceneObjectGroups SceneGR_PLAYER_BULLET = Beryll::SceneObjectGroups::GROUP_3;
    constexpr inline Beryll::SceneObjectGroups SceneGR_ENEMY = Beryll::SceneObjectGroups::GROUP_4;
    constexpr inline Beryll::SceneObjectGroups SceneGR_BRICK_GLASS = Beryll::SceneObjectGroups::GROUP_5;
    constexpr inline Beryll::SceneObjectGroups SceneGR_BRICK_WOOD = Beryll::SceneObjectGroups::GROUP_6;
    constexpr inline Beryll::SceneObjectGroups SceneGR_BRICK_STONE = Beryll::SceneObjectGroups::GROUP_7;

    constexpr inline glm::vec3 bulletGravity{0.0f, -20.0f, 0.0f};

    inline float throwAngleRadians = 0.0f;

    inline bool showPlayerMoveArrows = false;

    inline std::vector<std::shared_ptr<AngryBomb3D::BulletSet>> notEmptyBulletsSets;
    inline std::vector<std::shared_ptr<AngryBomb3D::BulletSet>> emptyBulletsSetsWithActiveBullets;
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

        notEmptyBulletsSets.clear();
        currentBulletSetIndex = 0;

        // Map.
        mapPlayTimeSec = 0.0f;
        mapPlayerWin = false;
        mapPlayerLose = false;

        // Pause.
        gameOnPause = false;
    }
}
