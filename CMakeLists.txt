cmake_minimum_required(VERSION 3.9)

project(pigsnkings)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# if you want to see dang debug info drawn, enable this
#add_definitions(-DPNK_DEBUG_PRINT)
#add_definitions(-DDANG_DEBUG_DRAW)
#add_definitions(-DDANG_DEBUG_PRINT)
#add_definitions(-DPNK_DEBUG_MEM)
add_definitions(-DPNK_DEBUG_FPS)

# remove assert statements
#add_definitions(-DNDEBUG)

if (CMAKE_BUID_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
elseif (CMAKE_BUID_TYPE MATCHES Release)
    add_definitions(-DRELEASE=1)
elseif (CMAKE_BUID_TYPE MATCHES 32blit_HW)
    add_definitions(-DTARGET_32BLIT_HW=1)
endif()

#add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion")
add_compile_options("-Wextra" "-Wdouble-promotion" "-fexceptions" "-Wattributes")

# ---- 32blit stuff ----
#set(32BLIT_PATH "libs/32blit-beta" CACHE PATH "Path to 32blit.cmake")
#include (${32BLIT_PATH}/32blit.cmake)
find_package (32BLIT CONFIG REQUIRED PATHS libs/32blit-sdk)

# ---- DANG stuff ----
set(DANG_DIR "libs/DANG" CACHE PATH "Path to DANG.cmake")
include (${DANG_DIR}/DANG.cmake)

install (FILES ${PROJECT_DISTRIBS} DESTINATION .)

# ---- DANG library ----
add_subdirectory(libs/DANG)

# ---- Sources ----
aux_source_directory(src SRC)
aux_source_directory(src/levels SRC_LEVELS)
aux_source_directory(src/actors SRC_ACTORS)
aux_source_directory(src/actors/throwies SRC_ACTORS_THROWIES)
aux_source_directory(src/actors/npc SRC_ACTORS_NPC)
aux_source_directory(src/actors/others SRC_ACTORS_OTHERS)
aux_source_directory(src/actors/hero SRC_ACTORS_HERO)
aux_source_directory(rsrc RSRC)
set(SOURCES
        ${SRC}
        ${SRC_LEVELS}
        ${SRC_ACTORS}
        ${SRC_ACTORS_THROWIES}
        ${SRC_ACTORS_NPC}
        ${SRC_ACTORS_OTHERS}
        ${SRC_ACTORS_HERO}
        ${RSRC}
        tracks/kingsofdawn.h
        tracks/paperbird.h
        tracks/u4iasmok.h
        fonts/barcadebrawl.h
        fonts/hud_font_big.h
        fonts/hud_font_small.h
        sfx/bomb_explode_22050_mono.h
        sfx/bubble_pop_22050_mono.h
        sfx/cannon_fire_22050_mono.h
        sfx/cheat_22050_mono.h
        sfx/coin_22050_mono.h
        sfx/crate_explode_22050_mono.h
        sfx/defeat_22050_mono.h
        sfx/jump_22050_mono.h
        sfx/health_22050_mono.h
        sfx/king_damage_22050.h
        sfx/lifelost_22050_mono.h
        sfx/pig_squeal_22050_mono.h
        sfx/victory_22050_mono.h
        )
# in case there is a file which should not be added:
# list (REMOVE_ITEM SOURCES /path/to/folder/filetoberemoved.cpp /path/to/folder/anotherfiletoberemoved.txt)

# ---- Create binary ----
blit_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} DANG)
# build assets: perhaps later on

# this is not needed anymore when we pre-generate the assets (which we do)
#dang_asset_magic(gfx.yml ${CMAKE_CURRENT_SOURCE_DIR}/world/**/*.tsx)
#dang_generate_assetlist(gfx.yml ${CMAKE_CURRENT_SOURCE_DIR}/world/**/*.tsx)
#blit_assets_yaml (${PROJECT_NAME} gfx.yml)

blit_metadata (${PROJECT_NAME} metadata.yml)
set(PROJECT_DISTRIBS LICENSE README.md)

#add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${32BLIT_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/DANG/src
        )


