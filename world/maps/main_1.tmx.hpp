// This file is auto generated by the SwordLord DANG Tiled Exporter.
// It will get overwritten once you re-export your tile map!
// (c) 2019-2021 by SwordLord - the coding crew

// World ------------------------------------------------

static const dang::tmx_world main_1_world = {
    .width = 10,
    .height = 8,
    .tileWidth = 32,
    .tileHeight = 32
};

// Tilesets ------------------------------------------------

static const dang::tmx_tileset main_1_tilesets[] = {
    {"gfx_levels_castle_tiles", &gfx_levels_castle_tiles, 96,32,32,384,256,12,8},
    {"hud_ui", &hud_ui, 105,16,16,240,112,15,7},
    {"main_pnk_logo", &main_pnk_logo, 1,218,36,218,36,1,1},
    {"gfx_castle_decorations", &gfx_castle_decorations, 40,32,32,320,128,10,4},
    {"gfx_king", &gfx_king, 27,32,32,864,32,27,1},
    {"gfx_pig", &gfx_pig, 43,32,32,1376,32,43,1},
    {"common_menus", &common_menus, 3,58,32,58,96,1,3},
};

static const size_t main_1_tilesets_len = 7;

// Animations ------------------------------------------------

static const dang::tmx_tileanimation main_1_tileanimations[] = {
    {"gfx_king",0,"wait",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100},{5, 100},{6, 100},{0, 800}}},
    {"gfx_king",1,"blink",{{0, 100},{25, 100}}},
    {"gfx_king",7,"jump",{{9, 100},{7, 100}}},
    {"gfx_king",8,"on_air",{{8, 100}}},
    {"gfx_king",10,"walk",{{10, 100},{11, 100},{12, 100},{13, 100},{14, 100},{15, 100},{16, 100},{17, 100}}},
    {"gfx_king",18,"bubble",{{18, 100},{19, 100},{20, 100},{19, 100},{18, 100}}},
    {"gfx_king",21,"life_lost",{{21, 600},{22, 600},{23, 600}}},
    {"gfx_pig",0,"sleeping",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100},{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100}}},
    {"gfx_pig",11,"loitering",{{11, 100},{12, 100},{13, 100},{14, 100},{15, 100},{16, 100}}},
    {"gfx_pig",13,"bubbling",{{12, 100},{13, 100}}},
    {"gfx_pig",24,"poof",{{24, 100},{25, 100},{26, 100},{27, 100},{28, 100},{29, 100},{30, 100},{31, 100},{32, 100},{33, 100}}},
    {"gfx_pig",34,"lighting_match",{{34, 100},{35, 100},{36, 100}}},
    {"gfx_pig",37,"match_lit",{{37, 100},{38, 100},{39, 100}}},
    {"gfx_pig",40,"lighting_cannon",{{40, 100},{41, 100},{42, 100}}},
};

static const size_t main_1_tileanimations_len = 14;

// Layers ------------------------------------------------

// layer tilelayer: main_bg
static const dang::tmx_tile main_1_main_bg_tiles[] = {

    {4, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {5, 0},
    {26, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {50, 0}, {24, 0},
    {49, 0}, {65, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {64, 0}, {49, 0},
    {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0},
    {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0},
    {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0},
    {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0},
    {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}

};

static const size_t main_1_main_bg_tiles_len = 80;

static const dang::tmx_layer main_1_main_bg = {
    .name = "main_bg",
    .type = dang::tmx_layerType::ltTile,
    .opacity = 1,
    .visible = true,
    .z_order = 0,
    .tl_width = 10,
    .tl_height = 8,
    .tl_tileset = "gfx_levels_castle_tiles",
    .tl_tiles = main_1_main_bg_tiles,
    .tl_tiles_len = main_1_main_bg_tiles_len,
    .spriteobjects = nullptr,
    .spriteobejcts_len = 0
};

// Objects for layer: main_decoration
static const dang::tmx_spriteobject main_1_main_decoration_objects[] = {

    {15,"15","",16,96,32,32,true,"gfx_castle_decorations",0,""},
    {16,"16","",16,128,32,32,true,"gfx_castle_decorations",10,""},
    {17,"17","",16,160,32,32,true,"gfx_castle_decorations",20,""},
    {18,"18","",272,96,32,32,true,"gfx_castle_decorations",0,""},
    {19,"19","",272,128,32,32,true,"gfx_castle_decorations",10,""},
    {20,"20","",272,160,32,32,true,"gfx_castle_decorations",30,""},
    {35,"35","",51,49,218,36,true,"main_pnk_logo",0,""},
    {34,"rightcandle","candle",192,128,32,32,true,"gfx_castle_decorations",5,""},
    {33,"leftcandle","candle",96,128,32,32,true,"gfx_castle_decorations",5,""}

};

static const size_t main_1_main_decoration_objects_len = 9;

static const dang::tmx_layer main_1_main_decoration = {
    .name = "main_decoration",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 1,
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = main_1_main_decoration_objects,
    .spriteobejcts_len = main_1_main_decoration_objects_len
};

// Objects for layer: main_obj
static const dang::tmx_spriteobject main_1_main_obj_objects[] = {

    {31,"hero","",64,192,32,32,true,"gfx_king",10,""},
    {36,"about","button",131,91,58,32,true,"common_menus",0,""},
    {37,"play","button",131,123,58,32,true,"common_menus",1,""},
    {38,"prefs","button",131,155,58,32,true,"common_menus",2,""},
    {39,"piggie","",216,192,32,32,true,"gfx_pig",0,""}

};

static const size_t main_1_main_obj_objects_len = 5;

static const dang::tmx_layer main_1_main_obj = {
    .name = "main_obj",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 2,
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = main_1_main_obj_objects,
    .spriteobejcts_len = main_1_main_obj_objects_len
};

const static dang::tmx_layer main_1_layers[] = {
    main_1_main_bg,
    main_1_main_decoration,
    main_1_main_obj,
};

const static size_t main_1_layers_len = 3;

// Level ---------------------------------------------------

static const dang::tmx_level main_1_level {
    .w = &main_1_world,
    .tilesets = main_1_tilesets,
    .tilesets_len = main_1_tilesets_len,
    .tileanimations = main_1_tileanimations,
    .tileanimations_len = main_1_tileanimations_len,
    .layers = main_1_layers,
    .layers_len = main_1_layers_len
};


