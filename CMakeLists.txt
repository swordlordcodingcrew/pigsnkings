cmake_minimum_required(VERSION 3.8)

project(pigsnkings)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion")
add_compile_options("-Wextra" "-Wdouble-promotion" "-fexceptions")

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
aux_source_directory(rsrc RSRC)
set(SOURCES
        ${SRC}
        ${RSRC}
        src/actors/npc/Throwies.cpp
        src/actors/npc/Throwies.h
        src/actors/npc/HenchPig.cpp
        src/actors/npc/HenchPig.h
        src/actors/npc/PigCrate.cpp
        src/actors/npc/PigCrate.h
        src/actors/npc/PigBomb.cpp
        src/actors/npc/PigBomb.h
        tracks/gocryogo.h
        fonts/barcadebrawl.h
        fonts/hud_font_big.h
        fonts/hud_font_small.h
        sfx/bubble_pop_22050_mono.h
        sfx/coin_22050_mono.h
        sfx/pig_squeal_22050_mono.h
        sfx/king_damage_22050.h
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


