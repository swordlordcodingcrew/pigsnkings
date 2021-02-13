cmake_minimum_required(VERSION 3.8)

project(pigsnkings)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion")
add_compile_options("-Wextra" "-Wdouble-promotion" "-fexceptions")

# ---- 32blit stuff ----
set(32BLIT_PATH "libs/32blit-beta" CACHE PATH "Path to 32blit.cmake")
include (${32BLIT_PATH}/32blit.cmake)

# ---- DANG stuff ----
set(DANG_PATH "libs/DANG" CACHE PATH "Path to DANG.cmake")
include (${DANG_PATH}/DANG.cmake)

install (FILES ${PROJECT_DISTRIBS} DESTINATION .)

# ---- DANG library ----
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
        rsrc/pnk_sprites_png.h
        tracks/gocryogo.h
        sfx/bubble_pop_22050_mono.h
        sfx/coin_22050_mono.h
        )

# ---- Create binary ----
blit_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} DANG)
# build assets: perhaps later on

dang_asset_magic(gfx.yml ${CMAKE_CURRENT_SOURCE_DIR}/world/**/*.tsx)

#dang_generate_assetlist(gfx.yml ${CMAKE_CURRENT_SOURCE_DIR}/world/**/*.tsx)
#blit_assets_yaml (${PROJECT_NAME} gfx.yml)

blit_metadata (${PROJECT_NAME} metadata.yml)

#add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${32BLIT_PATH}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/DANG/src
        )


