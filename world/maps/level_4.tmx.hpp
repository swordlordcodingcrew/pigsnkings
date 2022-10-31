// This file is auto generated by the SwordLord DANG Tiled Exporter.
// It will get overwritten once you re-export your tile map!
// (c) 2019-2021 by SwordLord - the coding crew

// World ------------------------------------------------

static const dang::tmx_world level_4_world = {
    .width = 54,
    .height = 8,
    .tileWidth = 32,
    .tileHeight = 32
};

// Tilesets ------------------------------------------------

static const dang::tmx_tileset level_4_tilesets[] = {
    {"gfx_levels_castle_tiles", &gfx_levels_castle_tiles, 96,32,32,384,256,12,8},
    {"gfx_castle_decorations", &gfx_castle_decorations, 32,32,32,256,128,8,4},
    {"pig_crate", &pig_crate, 40,32,32,1280,32,40,1},
    {"character_pigbomb", &character_pigbomb, 33,32,32,1056,32,33,1},
    {"character_cannonsnpigs", &character_cannonsnpigs, 9,32,32,288,32,9,1},
    {"gfx_pig", &gfx_pig, 43,32,32,1376,32,43,1},
    {"levels_door", &levels_door, 4,46,56,184,56,4,1},
    {"gfx_items", &gfx_items, 53,32,48,1696,48,53,1},
    {"gfx_king", &gfx_king, 28,32,32,896,32,28,1},
    {"gfx_bubbles", &gfx_bubbles, 11,32,32,352,32,11,1},
    {"common_items_bomb", &common_items_bomb, 11,64,64,704,64,11,1},
    {"hud_ui", &hud_ui, 105,16,16,240,112,15,7},
    {"character_pigking", &character_pigking, 33,32,32,1056,32,33,1},
    {"lives", &lives, 20,16,16,320,16,20,1},
};

static const size_t level_4_tilesets_len = 14;

// Animations ------------------------------------------------

static const dang::tmx_tileanimation level_4_tileanimations[] = {
    {"gfx_castle_decorations",5,"flicker",{{5, 100},{6, 100},{7, 100},{13, 100},{14, 100},{15, 100}}},
    {"pig_crate",0,"picking_up",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100}}},
    {"pig_crate",5,"sleeping",{{13, 100},{6, 100},{5, 100},{7, 100},{8, 100},{9, 100},{10, 100},{11, 100},{12, 100},{13, 100}}},
    {"pig_crate",14,"loitering",{{14, 100},{15, 100},{16, 100},{17, 100},{18, 100},{19, 100}}},
    {"pig_crate",20,"throwing",{{20, 100},{21, 100},{22, 100},{23, 100},{24, 100}}},
    {"pig_crate",25,"looking_out",{{25, 100},{26, 100},{27, 100}}},
    {"pig_crate",28,"jumping",{{28, 100},{29, 100},{30, 100},{31, 100},{32, 100},{33, 100}}},
    {"pig_crate",33,"hit",{{33, 80},{34, 80},{35, 80},{36, 80},{37, 80}}},
    {"pig_crate",34,"crate",{{34, 100}}},
    {"pig_crate",38,"bubbling",{{38, 100},{39, 100}}},
    {"character_pigbomb",0,"picking_up",{{2, 100},{1, 100},{0, 100},{3, 100}}},
    {"character_pigbomb",4,"sleeping",{{13, 100},{4, 100},{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100},{11, 100},{12, 100},{13, 100}}},
    {"character_pigbomb",14,"loitering",{{14, 100},{15, 100},{16, 100},{17, 100},{18, 100},{19, 100}}},
    {"character_pigbomb",20,"throwing",{{20, 100},{21, 100},{22, 100},{23, 100},{24, 100}}},
    {"character_pigbomb",25,"bomb_off",{{25, 100}}},
    {"character_pigbomb",26,"bomb_on",{{26, 100},{27, 100},{28, 100},{29, 100}}},
    {"character_pigbomb",31,"bubbling",{{31, 100},{32, 100}}},
    {"character_cannonsnpigs",0,"idling",{{0, 100}}},
    {"character_cannonsnpigs",1,"shooting",{{1, 100},{2, 100},{3, 100},{4, 100}}},
    {"character_cannonsnpigs",5,"muzzle_flash",{{5, 100},{6, 100},{7, 100}}},
    {"character_cannonsnpigs",8,"cannonball",{{8, 100}}},
    {"gfx_pig",0,"sleeping",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100},{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100}}},
    {"gfx_pig",11,"loitering",{{11, 100},{12, 100},{13, 100},{14, 100},{15, 100},{16, 100}}},
    {"gfx_pig",13,"bubbling",{{12, 100},{13, 100}}},
    {"gfx_pig",24,"poof",{{24, 100},{25, 100},{26, 100},{27, 100},{28, 100},{29, 100},{30, 100},{31, 100},{32, 100},{33, 100}}},
    {"gfx_pig",34,"lighting_match",{{34, 100},{35, 100},{36, 100}}},
    {"gfx_pig",37,"match_lit",{{37, 100},{38, 100},{39, 100}}},
    {"gfx_pig",40,"lighting_cannon",{{40, 100},{41, 100},{42, 100}}},
    {"gfx_items",0,"coin_silver",{{0, 100},{1, 100},{2, 100},{3, 100}}},
    {"gfx_items",4,"coin_gold",{{4, 100},{5, 100},{6, 100},{7, 100}}},
    {"gfx_items",8,"coin_poof",{{8, 100},{9, 100},{10, 100},{11, 100}}},
    {"gfx_items",12,"gem_blue",{{12, 100},{13, 100},{14, 100},{15, 100}}},
    {"gfx_items",16,"gem_green",{{16, 100},{17, 100},{18, 100},{19, 100}}},
    {"gfx_items",20,"gem_red",{{20, 100},{21, 100},{22, 100},{23, 100}}},
    {"gfx_items",24,"gem_poof",{{24, 100},{25, 100},{26, 100},{27, 100}}},
    {"gfx_items",28,"potion_blue",{{28, 100},{29, 100},{30, 100},{31, 100},{32, 100},{33, 100},{34, 100}}},
    {"gfx_items",35,"potion_red",{{35, 100},{36, 100},{37, 100},{38, 100},{39, 100},{40, 100},{41, 100}}},
    {"gfx_items",42,"potion_green",{{42, 100},{43, 100},{44, 100},{45, 100},{46, 100},{47, 100},{48, 100}}},
    {"gfx_items",49,"potion_poof",{{49, 100},{50, 100},{51, 100},{52, 100}}},
    {"gfx_king",0,"wait",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100},{5, 100},{6, 100},{0, 800}}},
    {"gfx_king",1,"blink",{{0, 100},{25, 100}}},
    {"gfx_king",7,"jump",{{9, 100},{7, 100}}},
    {"gfx_king",8,"on_air",{{8, 100}}},
    {"gfx_king",10,"walk",{{10, 100},{11, 100},{12, 100},{13, 100},{14, 100},{15, 100},{16, 100},{17, 100}}},
    {"gfx_king",18,"bubble",{{18, 100},{19, 100},{20, 100},{19, 100},{18, 100}}},
    {"gfx_king",21,"life_lost",{{21, 600},{22, 600},{23, 600}}},
    {"gfx_bubbles",0,"bubble_blow",{{0, 120},{1, 120},{2, 120},{3, 120},{4, 100}}},
    {"gfx_bubbles",4,"bubble_bobble",{{3, 120},{5, 120},{4, 120},{3, 120},{6, 120}}},
    {"gfx_bubbles",7,"bubble_poof",{{7, 100},{8, 100},{9, 100}}},
    {"common_items_bomb",0,"bomb_idle",{{0, 100}}},
    {"common_items_bomb",1,"bomb_lit",{{1, 100},{2, 100},{3, 100},{4, 100}}},
    {"common_items_bomb",5,"boom",{{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100}}},
    {"character_pigking",0,"sleeping",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100},{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100},{11, 100}}},
    {"character_pigking",12,"running",{{12, 100},{13, 100},{14, 100},{15, 100},{16, 100},{17, 100}}},
    {"character_pigking",17,"landing",{{18, 100},{19, 100},{20, 100}}},
    {"character_pigking",21,"jumping",{{21, 100},{22, 100},{23, 100},{24, 100},{25, 100}}},
    {"character_pigking",26,"hit",{{26, 100},{27, 100}}},
    {"character_pigking",28,"die",{{28, 100},{29, 100},{30, 100},{31, 100}}},
    {"lives",0,"heart_in_hud",{{0, 200},{1, 200},{2, 200},{3, 200},{4, 200},{5, 200},{6, 200},{7, 200}}},
    {"lives",8,"heart_in_hud_lost",{{8, 200},{9, 200}}},
};

static const size_t level_4_tileanimations_len = 60;

// Layers ------------------------------------------------

// layer tilelayer: lvl_4_bg
static const dang::tmx_tile level_4_lvl_4_bg_tiles[] = {

    {4, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {5, 0}, {13, 0}, {4, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {5, 0}, {4, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {5, 0},
    {14, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {50, 0}, {12, 0}, {13, 0}, {14, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {85, 0}, {85, 0}, {85, 0}, {77, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {76, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {77, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {50, 0}, {12, 0}, {14, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {50, 0}, {12, 0},
    {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {0, 0}, {1, 0}, {2, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {0, 0}, {1, 0}, {1, 0}, {2, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0},
    {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {0, 0}, {2, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {24, 0}, {26, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0},
    {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {24, 0}, {26, 0}, {61, 0}, {61, 0}, {61, 0}, {67, 0}, {85, 0}, {85, 0}, {54, 0}, {61, 0}, {36, 0}, {38, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0},
    {26, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {24, 0}, {25, 0}, {26, 0}, {60, 0}, {61, 0}, {61, 0}, {39, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {13, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {36, 0}, {38, 0}, {61, 0}, {62, 0}, {0, 0}, {2, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0},
    {85, 0}, {89, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {88, 0}, {85, 0}, {85, 0}, {85, 0}, {89, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {74, 0}, {12, 0}, {13, 0}, {14, 0}, {72, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {74, 0}, {12, 0}, {13, 0}, {13, 0}, {14, 0}, {72, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {74, 0}, {12, 0}, {14, 0}, {72, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {74, 0}, {12, 0},
    {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}, {13, 0}, {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}, {13, 0}, {13, 0}, {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}, {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}

};

static const size_t level_4_lvl_4_bg_tiles_len = 432;

static const dang::tmx_layer level_4_lvl_4_bg = {
    .name = "lvl_4_bg",
    .type = dang::tmx_layerType::ltTile,
    .opacity = 1,
    .visible = true,
    .z_order = 0,
    .position = {0,0},
    .tl_width = 54,
    .tl_height = 8,
    .tl_tileset = "gfx_levels_castle_tiles",
    .tl_tiles = level_4_lvl_4_bg_tiles,
    .tl_tiles_len = level_4_lvl_4_bg_tiles_len,
    .spriteobjects = nullptr,
    .spriteobejcts_len = 0
};

// Objects for layer: lvl_4_mood
static const dang::tmx_spriteobject level_4_lvl_4_mood_objects[] = {

    {1,"1","",32,72,32,32,true,"gfx_castle_decorations",9,"",0,0b0000},
    {3,"3","",64,72,32,32,true,"gfx_castle_decorations",11,"",0,0b0000},
    {4,"4","",256,72,32,32,true,"gfx_castle_decorations",11,"",0,0b0000},
    {7,"7","",224,72,32,32,true,"gfx_castle_decorations",9,"",0,0b0000},
    {32,"32","",608,192,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {33,"33","",608,160,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {34,"34","",608,128,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {35,"35","",608,96,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {37,"37","",512,128,32,32,true,"gfx_castle_decorations",1,"",0,0b0000},
    {38,"38","",544,128,32,32,true,"gfx_castle_decorations",3,"",0,0b0000},
    {39,"39","",384,64,32,32,true,"gfx_castle_decorations",1,"",0,0b0000},
    {40,"40","",416,64,32,32,true,"gfx_castle_decorations",3,"",0,0b0000},
    {59,"59","",1664,96,32,32,true,"gfx_castle_decorations",11,"",0,0b0000},
    {60,"60","",1632,96,32,32,true,"gfx_castle_decorations",10,"",0,0b0000},
    {61,"61","",1600,96,32,32,true,"gfx_castle_decorations",9,"",0,0b0000},
    {65,"65","",848,136,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {67,"67","",800,160,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {66,"66","",736,192,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {135,"135","",928,104,32,32,true,"gfx_castle_decorations",12,"",0,0b0000},
    {152,"152","",1640,40,46,56,true,"levels_door",3,"",0,0b0000},
    {153,"153","",1568,160,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {154,"154","",1600,192,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {155,"155","",1344,192,32,32,true,"gfx_castle_decorations",4,"",0,0b0000},
    {156,"156","",1248,120,32,32,true,"gfx_castle_decorations",9,"",0,0b0000},
    {157,"157","",1280,120,32,32,true,"gfx_castle_decorations",11,"",0,0b0000}

};

static const size_t level_4_lvl_4_mood_objects_len = 25;

static const dang::tmx_layer level_4_lvl_4_mood = {
    .name = "lvl_4_mood",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 1,
    .position = {0,0},
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = level_4_lvl_4_mood_objects,
    .spriteobejcts_len = level_4_lvl_4_mood_objects_len
};

// Objects for layer: lvl_4_obj
static const dang::tmx_spriteobject level_4_lvl_4_obj_objects[] = {

    {50,"50","pig_box",512,96,32,32,true,"pig_crate",5,"",20,0b0000},
    {51,"51","pig_bomb",416,32,32,32,true,"character_pigbomb",4,"",20,0b0001},
    {52,"52","cannon",1280,128,32,32,true,"character_cannonsnpigs",0,"",20,0b0001},
    {53,"53","pig_cannon",1248,128,32,32,true,"gfx_pig",0,"",20,0b0001},
    {54,"54","pig_normal",1376,128,32,32,true,"gfx_pig",0,"",20,0b0000},
    {55,"55","pig_bomb",1184,64,32,32,true,"character_pigbomb",4,"",20,0b0000},
    {58,"58","pig_normal",1592,192,32,32,true,"gfx_pig",0,"",20,0b0000},
    {64,"64","potion_red",1664,184,32,48,true,"gfx_items",35,"",0,0b0000},
    {73,"73","hotrect",0,224,1728,32,true,"",0,"",0,0b0000},
    {75,"75","hotrect",0,32,32,160,true,"",0,"",0,0b0000},
    {76,"76","hotrect",0,0,1728,32,true,"",0,"",0,0b0000},
    {77,"77","hotrect",1696,32,32,192,true,"",0,"",0,0b0000},
    {78,"78","hotrect",1504,128,64,32,true,"",0,"",0,0b0000},
    {79,"79","hotrect",1376,32,64,96,true,"",0,"",0,0b0000},
    {80,"80","hotrect",1376,160,64,64,true,"",0,"",0,0b0000},
    {81,"81","hotrect",1600,96,96,12,true,"",0,"",0,0b0000},
    {82,"82","hotrect",1248,160,64,32,true,"",0,"",0,0b0000},
    {83,"83","hotrect",1184,96,64,64,true,"",0,"",0,0b0000},
    {84,"84","hotrect",992,64,128,160,true,"",0,"",0,0b0000},
    {85,"85","hotrect",640,64,96,160,true,"",0,"",0,0b0000},
    {86,"86","hotrect",288,32,96,160,true,"",0,"",0,0b0000},
    {21,"21","pig_box",224,40,32,32,true,"pig_crate",5,"",20,0b0000},
    {22,"22","pig_box",64,40,32,32,true,"pig_crate",5,"",20,0b0001},
    {23,"23","pig_box",192,192,32,32,true,"pig_crate",5,"",20,0b0000},
    {24,"24","pig_box",96,192,32,32,true,"pig_crate",5,"",20,0b0000},
    {68,"68","potion_blue",848,184,32,48,true,"gfx_items",28,"",0,0b0000},
    {70,"70","gem_green",848,48,32,48,true,"gfx_items",16,"",0,0b0000},
    {71,"71","gem_blue",960,184,32,48,true,"gfx_items",12,"",0,0b0000},
    {69,"69","gem_red",848,16,32,48,true,"gfx_items",20,"",0,0b0000},
    {72,"72","coin_silver",736,184,32,48,true,"gfx_items",0,"",0,0b0000},
    {62,"62","pig_bomb",1600,64,32,32,true,"character_pigbomb",4,"",20,0b0000},
    {87,"5","level_trigger",1656,40,16,56,true,"",0,"",0,0b0000},
    {88,"88","hotrect",32,72,64,12,true,"",0,"",0,0b0000},
    {90,"90","hotrect",224,72,64,12,true,"",0,"",0,0b0000},
    {91,"91","hotrect_platform",608,96,32,4,true,"",0,"",0,0b0000},
    {92,"92","hotrect_platform",608,128,32,4,true,"",0,"",0,0b0000},
    {93,"93","hotrect_platform",608,160,32,4,true,"",0,"",0,0b0000},
    {94,"94","hotrect_platform",608,192,32,4,true,"",0,"",0,0b0000},
    {95,"95","hotrect_platform",384,64,64,4,true,"",0,"",0,0b0000},
    {96,"96","hotrect_platform",512,128,64,4,true,"",0,"",0,0b0000},
    {97,"97","hotrect_platform",736,192,32,4,true,"",0,"",0,0b0000},
    {98,"98","hotrect_platform",800,160,32,4,true,"",0,"",0,0b0000},
    {99,"99","hotrect_platform",848,136,32,4,true,"",0,"",0,0b0000},
    {100,"100","hotrect",480,160,32,32,true,"",0,"",0,0b0000},
    {111,"111","king",0,192,32,32,true,"gfx_king",0,"",50,0b0000},
    {112,"112","pig_box",240,192,32,32,true,"pig_crate",5,"",20,0b0001},
    {113,"113","bubble_proto",240,-40,32,32,true,"gfx_bubbles",0,"",50,0b0000},
    {114,"114","crate_proto",472,-40,32,32,true,"pig_crate",34,"",20,0b0000},
    {115,"115","bomb_proto",392,-48,32,32,true,"character_pigbomb",26,"",20,0b0000},
    {116,"116","pigpoof_proto",272,-40,32,32,true,"gfx_pig",24,"",70,0b0000},
    {117,"117","cannonmuzzle_proto",208,-40,32,32,true,"character_cannonsnpigs",5,"",20,0b0000},
    {118,"118","cannonball_proto",424,-40,32,32,true,"character_cannonsnpigs",8,"",20,0b0000},
    {119,"119","boom_proto",304,0,64,64,true,"common_items_bomb",5,"",70,0b0000},
    {128,"1","room_trigger",349,192,2,32,true,"",0,"",0,0b0000},
    {129,"0","room_trigger",320,192,2,32,true,"",0,"",0,0b0000},
    {130,"2","room_trigger",701,32,2,32,true,"",0,"",0,0b0000},
    {131,"1","room_trigger",672,32,2,32,true,"",0,"",0,0b0000},
    {132,"3","room_trigger",1085,32,2,32,true,"",0,"",0,0b0000},
    {133,"2","room_trigger",1056,32,2,32,true,"",0,"",0,0b0000},
    {136,"136","hotrect",928,104,32,12,true,"",0,"",0,0b0000},
    {158,"158","hotrect_platform",1344,192,32,4,true,"",0,"",0,0b0000},
    {159,"159","hotrect_platform",1568,160,32,4,true,"",0,"",0,0b0000},
    {160,"160","hotrect_platform",1600,192,32,4,true,"",0,"",0,0b0000},
    {161,"161","hotrect",1248,120,64,8,true,"",0,"",0,0b0000}

};

static const size_t level_4_lvl_4_obj_objects_len = 64;

static const dang::tmx_layer level_4_lvl_4_obj = {
    .name = "lvl_4_obj",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 2,
    .position = {0,0},
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = level_4_lvl_4_obj_objects,
    .spriteobejcts_len = level_4_lvl_4_obj_objects_len
};

// Objects for layer: lvl_4_fg
static const dang::tmx_spriteobject level_4_lvl_4_fg_objects[] = {

    {9,"9","",32,32,32,32,true,"gfx_castle_decorations",0,"",0,0b0000},
    {10,"10","",32,64,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {11,"11","",32,96,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {12,"12","",32,128,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {13,"13","",32,160,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {14,"14","",32,192,32,32,true,"gfx_castle_decorations",24,"",0,0b0000},
    {15,"15","",256,32,32,32,true,"gfx_castle_decorations",0,"",0,0b0000},
    {16,"16","",256,64,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {17,"17","",256,96,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {18,"18","",256,128,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {20,"20","",256,160,32,32,true,"gfx_castle_decorations",24,"",0,0b0000},
    {41,"41","",384,32,32,32,true,"gfx_castle_decorations",0,"",0,0b0000},
    {42,"42","",384,64,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {43,"43","",384,96,32,32,true,"gfx_castle_decorations",16,"",0,0b0000},
    {44,"44","",544,32,32,32,true,"gfx_castle_decorations",0,"",0,0b0000},
    {45,"45","",544,64,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {46,"46","",544,96,32,32,true,"gfx_castle_decorations",8,"",0,0b0000},
    {47,"47","",544,128,32,32,true,"gfx_castle_decorations",24,"",0,0b0000}

};

static const size_t level_4_lvl_4_fg_objects_len = 18;

static const dang::tmx_layer level_4_lvl_4_fg = {
    .name = "lvl_4_fg",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 3,
    .position = {0,0},
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = level_4_lvl_4_fg_objects,
    .spriteobejcts_len = level_4_lvl_4_fg_objects_len
};

// Objects for layer: lvl_4_hud
static const dang::tmx_spriteobject level_4_lvl_4_hud_objects[] = {

    {137,"137","hud_hero",0,0,32,32,true,"gfx_king",26,"",0,0b0000},
    {138,"138","",31,16,16,16,true,"hud_ui",61,"",0,0b0000},
    {139,"139","",47,16,16,16,true,"hud_ui",62,"",0,0b0000},
    {140,"140","",63,16,16,16,true,"hud_ui",63,"",0,0b0000},
    {141,"141","hud_boss",288,0,32,32,true,"character_pigking",32,"",0,0b0000},
    {142,"142","hud_boss_health_p1",240,16,16,16,true,"hud_ui",61,"",0,0b0000},
    {143,"143","hud_boss_health_p2",256,16,16,16,true,"hud_ui",62,"",0,0b0000},
    {144,"144","hud_boss_health_p3",272,16,16,16,true,"hud_ui",63,"",0,0b0000},
    {149,"149","hud_l1",32,4,16,16,true,"lives",0,"",0,0b0000},
    {150,"150","hud_l2",48,4,16,16,true,"lives",0,"",0,0b0000},
    {151,"151","hud_l3",64,4,16,16,true,"lives",0,"",0,0b0000}

};

static const size_t level_4_lvl_4_hud_objects_len = 11;

static const dang::tmx_layer level_4_lvl_4_hud = {
    .name = "lvl_4_hud",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 4,
    .position = {0,0},
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = level_4_lvl_4_hud_objects,
    .spriteobejcts_len = level_4_lvl_4_hud_objects_len
};

// path layer
static const dang::tmx_waypoint level_4_waypoints[] = {
};

static const size_t level_4_waypoints_len = 0;

static const dang::tmx_waypoint_connection level_4_connections[] = {
};

static const size_t level_4_connections_len = 0;

// zones
static const dang::tmx_zone level_4_zones[] = {
    {0,0,0,352,256},
    {1,320,0,384,256},
    {2,672,0,416,256},
    {3,1056,0,672,256},
};

static const size_t level_4_zones_len = 4;

// zone passages
static const dang::tmx_zone_passage level_4_zone_passages[] = {
    {0,-1,0,192},
    {1,-1,352,192},
    {2,-1,704,32},
    {3,-1,1088,32}
};

static const size_t level_4_zone_passages_len = 4;

const static dang::tmx_layer level_4_layers[] = {
    level_4_lvl_4_bg,
    level_4_lvl_4_mood,
    level_4_lvl_4_obj,
    level_4_lvl_4_fg,
    level_4_lvl_4_hud,
};

const static size_t level_4_layers_len = 7;

// Level ---------------------------------------------------

static const dang::tmx_level level_4_level {
    .w = &level_4_world,
    .tilesets = level_4_tilesets,
    .tilesets_len = level_4_tilesets_len,
    .tileanimations = level_4_tileanimations,
    .tileanimations_len = level_4_tileanimations_len,
    .layers = level_4_layers,
    .layers_len = level_4_layers_len,
    .waypoints = level_4_waypoints,
    .waypoints_len = level_4_waypoints_len,
    .waypoint_connections = level_4_connections,
    .waypoint_connections_len = level_4_connections_len,
    .wavepoints = nullptr,
    .wavepoints_len = 0,
    .zones = level_4_zones,
    .zones_len = level_4_zones_len,
    .zone_passages = level_4_zone_passages,
    .zone_passages_len = level_4_zone_passages_len
};


