cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(pigsnkings)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(32BLIT_PATH "libs/32blit-beta" CACHE PATH "Path to 32blit.cmake")
include (${32BLIT_PATH}/32blit.cmake)

add_subdirectory(libs/DANG)

# ---- Sources ----

set(SOURCES
        src/pnk_globals.h
        src/pigsnkings.cpp
        src/Hero.cpp
        src/Hero.h
        src/MotionState.cpp
        src/MotionState.h
        src/SomaticState.cpp
        src/SomaticState.h
        src/ActionState.cpp
        src/ActionState.h
        src/Enemy.cpp
        src/Enemy.h
        src/PnkEvent.cpp
        src/PnkEvent.h
        src/Bubble.cpp
        src/Bubble.h
        src/GameState.cpp
        src/GameState.h
        src/GSIntro.cpp
        src/GSIntro.h
        src/GSHome.cpp
        src/GSHome.h
        src/GSPlay.cpp
        src/GSPlay.h
        src/LevelFlow.cpp
        src/LevelFlow.h
        rsrc/tiles_bg_png.h
        rsrc/players_png.h
        rsrc/pnk_32_lvl1.tmx.hpp
        rsrc/pnk_32_menu.tmx.hpp
        rsrc/menus_png.h
        rsrc/sl_shield_32blit_png.h

        )

# ---- Create binary ----

blit_executable (pigsnkings ${SOURCES})
target_link_libraries(pigsnkings DANG)
#add_executable(pigsnkings ${SOURCES})
#target_link_libraries(pigsnkings dang)
#set_target_properties(CPMExampleCereal PROPERTIES CXX_STANDARD 17)

target_include_directories(pigsnkings SYSTEM
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${32BLIT_PATH}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/DANG/src
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/DANG/src/tween
        )

