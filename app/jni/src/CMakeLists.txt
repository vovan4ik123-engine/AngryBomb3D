cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-fexceptions -frtti)

# for info/warning/assert messages
    add_definitions(-DBR_DEBUG)

add_library(main SHARED main.cpp
                        GameStateHelper.cpp
                        DataBaseHelper.cpp
                        SendStatisticsHelper.cpp
                        # Play state.
                        playState/PlayStateGUILayer.cpp
                        playState/PlayStateSceneLayer.cpp
                        # Menus.
                        menus/start/StartMenuGUILayer.cpp
                        # Player.
                        player/Player.cpp
                        player/PlayerBulletAirTrajectory.cpp
                        # Enemies.
                        enemies/BaseEnemy.cpp
                        # Bullets.
                        bullets/BulletSet.cpp
                        # Maps.
                        maps/BaseMap.cpp
                        maps/Map0.cpp maps/Map1.cpp maps/Map2.cpp maps/Map3.cpp
            )

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#all other desired libs should be included in Beryll-static lib
target_link_libraries(main Beryll-static GLESv3)
