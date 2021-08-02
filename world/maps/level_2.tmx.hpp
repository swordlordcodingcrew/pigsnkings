// This file is auto generated by the SwordLord DANG Tiled Exporter.
// It will get overwritten once you re-export your tile map!
// (c) 2019-2021 by SwordLord - the coding crew

// World ------------------------------------------------

static const dang::tmx_world level_2_world = {
    .width = 60,
    .height = 16,
    .tileWidth = 32,
    .tileHeight = 32
};

// Tilesets ------------------------------------------------

static const dang::tmx_tileset level_2_tilesets[] = {
    {"gfx_levels_castle_tiles", &gfx_levels_castle_tiles, 96,32,32,384,256,12,8},
    {"gfx_castle_decorations", &gfx_castle_decorations, 40,32,32,320,128,10,4},
    {"gfx_items", &gfx_items, 53,32,48,1696,48,53,1},
    {"gfx_bubbles", &gfx_bubbles, 11,32,32,352,32,11,1},
    {"hud_ui", &hud_ui, 105,16,16,240,112,15,7},
    {"gfx_king", &gfx_king, 27,32,32,864,32,27,1},
    {"gfx_pig", &gfx_pig, 43,32,32,1376,32,43,1},
    {"character_pigbomb", &character_pigbomb, 33,32,32,1056,32,33,1},
    {"character_pigcrate", &character_pigcrate, 39,32,32,1248,32,39,1},
    {"character_cannonsnpigs", &character_cannonsnpigs, 9,32,32,288,32,9,1},
    {"common_items_bomb", &common_items_bomb, 11,64,64,704,64,11,1},
};

static const size_t level_2_tilesets_len = 11;

// Animations ------------------------------------------------

static const dang::tmx_tileanimation level_2_tileanimations[] = {
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
    {"gfx_bubbles",0,"bubble_blow",{{0, 120},{1, 120},{2, 120},{3, 120},{4, 100}}},
    {"gfx_bubbles",4,"bubble_bobble",{{3, 120},{5, 120},{4, 120},{3, 120},{6, 120}}},
    {"gfx_bubbles",7,"bubble_poof",{{7, 100},{8, 100},{9, 100}}},
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
    {"character_pigbomb",0,"picking_up",{{0, 100},{1, 100},{2, 100},{3, 100}}},
    {"character_pigbomb",4,"sleeping",{{4, 100},{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100},{11, 100},{12, 100},{13, 100}}},
    {"character_pigbomb",14,"loitering",{{14, 100},{15, 100},{16, 100},{17, 100},{18, 100},{19, 100}}},
    {"character_pigbomb",20,"throwing",{{20, 100},{21, 100},{22, 100},{23, 100},{24, 100}}},
    {"character_pigbomb",23,"bubbling",{{23, 100},{24, 100}}},
    {"character_pigbomb",25,"bomb_off",{{25, 100}}},
    {"character_pigbomb",26,"bomb_on",{{26, 100},{27, 100},{28, 100},{29, 100}}},
    {"character_pigbomb",31,"bubbling",{{31, 100},{32, 100}}},
    {"character_pigcrate",0,"picking_up",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100}}},
    {"character_pigcrate",5,"sleeping",{{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100},{11, 100},{12, 100},{13, 100}}},
    {"character_pigcrate",14,"loitering",{{14, 100},{15, 100},{16, 100},{17, 100},{18, 100},{19, 100}}},
    {"character_pigcrate",20,"throwing",{{20, 100},{21, 100},{22, 100},{23, 100},{24, 100},{36, 100}}},
    {"character_pigcrate",25,"looking_out",{{25, 100},{26, 100},{27, 100}}},
    {"character_pigcrate",28,"jumping",{{28, 100},{29, 100},{30, 100},{31, 100},{32, 100},{33, 100}}},
    {"character_pigcrate",34,"crate",{{34, 100}}},
    {"character_pigcrate",37,"bubbling",{{37, 100},{38, 100}}},
    {"character_cannonsnpigs",0,"idling",{{0, 100}}},
    {"character_cannonsnpigs",1,"shooting",{{1, 100},{2, 100},{3, 100},{4, 100}}},
    {"character_cannonsnpigs",5,"muzzle_flash",{{5, 100},{6, 100},{7, 100}}},
    {"character_cannonsnpigs",8,"cannonball",{{8, 100}}},
    {"common_items_bomb",0,"bomb_idle",{{0, 100}}},
    {"common_items_bomb",1,"bomb_lit",{{1, 100},{2, 100},{3, 100},{4, 100}}},
    {"common_items_bomb",5,"boom",{{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100}}},
};

static const size_t level_2_tileanimations_len = 51;

// Layers ------------------------------------------------

// layer tilelayer: lvl_2_bg
static const dang::tmx_tile level_2_lvl_2_bg_tiles[] = {

    {4, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {5, 0}, {4, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {5, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0},
    {14, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {50, 0}, {12, 0}, {14, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {76, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {77, 0}, {49, 0}, {49, 0}, {50, 0}, {24, 0}, {25, 0}, {5, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0},
    {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {52, 0}, {73, 0}, {73, 0}, {53, 0}, {61, 0}, {62, 0}, {0, 0}, {28, 0}, {37, 0}, {38, 0}, {60, 0}, {61, 0}, {61, 0}, {64, 0}, {76, 0}, {86, 0}, {12, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0},
    {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {62, 0}, {0, 0}, {2, 0}, {60, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {84, 0}, {77, 0}, {65, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {36, 0}, {6, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0},
    {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {62, 0}, {24, 0}, {26, 0}, {60, 0}, {61, 0}, {62, 0}, {12, 0}, {19, 0}, {38, 0}, {78, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {88, 0}, {86, 0}, {12, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0},
    {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {24, 0}, {26, 0}, {60, 0}, {61, 0}, {64, 0}, {49, 0}, {49, 0}, {65, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {48, 0}, {91, 0}, {36, 0}, {37, 0}, {37, 0}, {37, 0}, {37, 0}, {37, 0}, {41, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0},
    {14, 0}, {78, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {88, 0}, {85, 0}, {85, 0}, {89, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {74, 0}, {12, 0}, {14, 0}, {72, 0}, {88, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0},
    {14, 0}, {63, 0}, {0, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}, {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0},
    {14, 0}, {63, 0}, {24, 0}, {18, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {5, 0}, {4, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {5, 0}, {4, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {18, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {18, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0}, {25, 0},
    {14, 0}, {90, 0}, {50, 0}, {15, 0}, {82, 0}, {85, 0}, {85, 0}, {77, 0}, {50, 0}, {12, 0}, {14, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {76, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {85, 0}, {77, 0}, {49, 0}, {76, 0}, {85, 0}, {86, 0}, {12, 0}, {14, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {50, 0}, {15, 0}, {82, 0}, {85, 0}, {85, 0}, {77, 0}, {50, 0}, {15, 0}, {48, 0}, {49, 0}, {49, 0}, {49, 0}, {76, 0}, {85, 0}, {85, 0}, {77, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {49, 0}, {76, 0}, {85, 0},
    {14, 0}, {60, 0}, {62, 0}, {15, 0}, {63, 0}, {0, 0}, {2, 0}, {60, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {36, 0}, {37, 0}, {29, 0}, {1, 0}, {1, 0}, {1, 0}, {2, 0}, {78, 0}, {73, 0}, {74, 0}, {0, 0}, {1, 0}, {17, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {15, 0}, {63, 0}, {0, 0}, {2, 0}, {60, 0}, {62, 0}, {15, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {0, 0}, {2, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {0, 0},
    {14, 0}, {60, 0}, {62, 0}, {15, 0}, {63, 0}, {12, 0}, {14, 0}, {60, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {67, 0}, {85, 0}, {86, 0}, {12, 0}, {13, 0}, {13, 0}, {4, 0}, {26, 0}, {63, 0}, {34, 0}, {37, 0}, {41, 0}, {5, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {15, 0}, {63, 0}, {12, 0}, {14, 0}, {60, 0}, {62, 0}, {15, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0},
    {14, 0}, {60, 0}, {62, 0}, {15, 0}, {63, 0}, {12, 0}, {14, 0}, {60, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {52, 0}, {73, 0}, {74, 0}, {0, 0}, {1, 0}, {17, 0}, {13, 0}, {13, 0}, {14, 0}, {82, 0}, {95, 0}, {15, 0}, {48, 0}, {50, 0}, {12, 0}, {13, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {15, 0}, {63, 0}, {12, 0}, {14, 0}, {60, 0}, {62, 0}, {15, 0}, {72, 0}, {53, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0},
    {14, 0}, {60, 0}, {62, 0}, {27, 0}, {63, 0}, {12, 0}, {14, 0}, {60, 0}, {62, 0}, {24, 0}, {26, 0}, {60, 0}, {52, 0}, {73, 0}, {74, 0}, {0, 0}, {1, 0}, {17, 0}, {13, 0}, {13, 0}, {4, 0}, {25, 0}, {26, 0}, {63, 0}, {36, 0}, {47, 0}, {60, 0}, {62, 0}, {24, 0}, {25, 0}, {26, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {27, 0}, {63, 0}, {12, 0}, {14, 0}, {60, 0}, {62, 0}, {46, 0}, {38, 0}, {60, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0}, {14, 0}, {60, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {61, 0}, {62, 0}, {12, 0},
    {14, 0}, {72, 0}, {88, 0}, {85, 0}, {95, 0}, {12, 0}, {14, 0}, {72, 0}, {88, 0}, {85, 0}, {85, 0}, {89, 0}, {74, 0}, {0, 0}, {1, 0}, {17, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {14, 0}, {84, 0}, {85, 0}, {93, 0}, {85, 0}, {85, 0}, {89, 0}, {88, 0}, {85, 0}, {85, 0}, {85, 0}, {89, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {88, 0}, {85, 0}, {95, 0}, {12, 0}, {14, 0}, {72, 0}, {88, 0}, {85, 0}, {85, 0}, {89, 0}, {73, 0}, {73, 0}, {74, 0}, {12, 0}, {14, 0}, {72, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {73, 0}, {74, 0}, {12, 0},
    {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}, {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {13, 0}, {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}, {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}, {16, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {17, 0}

};

static const size_t level_2_lvl_2_bg_tiles_len = 960;

static const dang::tmx_layer level_2_lvl_2_bg = {
    .name = "lvl_2_bg",
    .type = dang::tmx_layerType::ltTile,
    .opacity = 1,
    .visible = true,
    .z_order = 0,
    .tl_width = 60,
    .tl_height = 16,
    .tl_tileset = "gfx_levels_castle_tiles",
    .tl_tiles = level_2_lvl_2_bg_tiles,
    .tl_tiles_len = level_2_lvl_2_bg_tiles_len,
    .spriteobjects = nullptr,
    .spriteobejcts_len = 0
};

// Objects for layer: lvl_2_mood
static const dang::tmx_spriteobject level_2_lvl_2_mood_objects[] = {

    {50,"50","",1632,352,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {51,"51","",1856,352,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {52,"52","",1632,448,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {53,"53","",1824,448,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {54,"54","",1856,448,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {55,"55","",1664,448,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {56,"56","",1792,352,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {57,"57","",1792,384,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {58,"58","",1792,416,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {59,"59","",1696,352,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {60,"60","",1696,384,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {61,"61","",1696,416,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {62,"62","",1728,352,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {63,"63","",1760,352,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {64,"64","",1760,384,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {65,"65","",1728,384,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {66,"66","",1728,416,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {67,"67","",1760,416,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {68,"68","",1408,352,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {69,"69","",1440,352,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {70,"70","",1536,352,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {71,"71","",1536,384,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {72,"72","",1536,416,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {73,"73","",1536,448,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {74,"74","",1344,448,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {75,"75","",1312,416,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {76,"76","",1344,384,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {77,"77","",1312,352,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {78,"78","",736,448,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {79,"79","",768,384,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {82,"82","",768,320,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {83,"83","",800,320,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {84,"84","",832,320,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {85,"85","",512,384,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {86,"86","",448,416,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {87,"87","",416,416,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {88,"88","",416,384,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {89,"89","",448,384,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {90,"90","",480,384,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {91,"91","",224,448,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {92,"92","",224,416,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {93,"93","",256,384,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {94,"94","",224,352,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {95,"95","",32,448,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {96,"96","",64,416,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {97,"97","",32,384,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {98,"98","",64,352,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {99,"99","",32,320,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {100,"100","",32,288,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {101,"101","",32,256,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {102,"102","",32,224,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {103,"103","",32,192,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {104,"104","",32,160,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {105,"105","",32,128,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {106,"106","",32,96,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {107,"107","",32,64,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {108,"108","",256,64,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {109,"109","",256,96,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {111,"111","",256,128,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {112,"112","",256,160,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {113,"113","",256,192,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {114,"114","",416,160,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {115,"115","",416,96,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {116,"116","",512,96,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {117,"117","",512,160,32,32,true,"gfx_castle_decorations",4,"",0b0000},
    {119,"119","",96,64,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {120,"120","",96,96,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {121,"121","",96,128,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {122,"122","",96,160,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {123,"123","",96,192,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {124,"124","",352,128,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {125,"125","",544,128,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {126,"126","",384,192,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {127,"127","",192,64,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {128,"128","",192,96,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {129,"129","",192,128,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {130,"130","",192,160,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {131,"131","",192,192,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {132,"132","",384,128,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {133,"133","",576,128,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {134,"134","",544,192,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {135,"135","",416,192,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {136,"136","",448,192,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {137,"137","",480,192,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {138,"138","",512,192,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {139,"139","",128,64,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {140,"140","",160,64,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {141,"141","",160,96,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {142,"142","",128,96,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {143,"143","",128,128,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {144,"144","",160,128,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {145,"145","",160,160,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {146,"146","",128,160,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {147,"147","",128,192,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {148,"148","",160,192,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {149,"149","",736,128,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {150,"150","",768,128,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {151,"151","",800,128,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {153,"153","",832,96,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {154,"154","",800,96,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {155,"155","",768,64,32,32,true,"gfx_castle_decorations",2,"",0b0000},
    {157,"157","",736,64,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {158,"158","",768,96,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {159,"159","",704,128,32,32,true,"gfx_castle_decorations",1,"",0b0000},
    {161,"161","",864,96,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {162,"162","",800,64,32,32,true,"gfx_castle_decorations",3,"",0b0000},
    {163,"163","",832,128,32,32,true,"gfx_castle_decorations",3,"",0b0000}

};

static const size_t level_2_lvl_2_mood_objects_len = 107;

static const dang::tmx_layer level_2_lvl_2_mood = {
    .name = "lvl_2_mood",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 1,
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = level_2_lvl_2_mood_objects,
    .spriteobejcts_len = level_2_lvl_2_mood_objects_len
};

// Objects for layer: lvl_2_obj
static const dang::tmx_spriteobject level_2_lvl_2_obj_objects[] = {

    {1,"1","bubble_proto",512,448,32,32,true,"gfx_bubbles",0,"",0b0000},
    {2,"2","crate_proto",608,448,32,32,true,"character_pigcrate",34,"",0b0000},
    {3,"3","bomb_proto",576,448,32,32,true,"character_pigbomb",26,"",0b0000},
    {4,"4","pigpoof_proto",544,448,32,32,true,"gfx_pig",24,"",0b0000},
    {5,"5","cannonmuzzle_proto",608,416,32,32,true,"character_cannonsnpigs",5,"",0b0000},
    {6,"6","cannonball_proto",576,416,32,32,true,"character_cannonsnpigs",8,"",0b0000},
    {7,"7","boom_proto",640,416,64,64,true,"common_items_bomb",5,"",0b0000},
    {8,"8","king",1888,288,32,32,true,"gfx_king",0,"",0b0000},
    {9,"9","coin_silver",832,369,32,48,true,"gfx_items",0,"",0b0000},
    {10,"10","coin_gold",1024,274,32,48,true,"gfx_items",4,"",0b0000},
    {11,"11","hotrect",1888,320,32,160,true,"",0,"",0b0000},
    {12,"12","hotrect",640,480,1280,32,true,"",0,"",0b0000},
    {13,"13","hotrect",1568,320,64,160,true,"",0,"",0b0000},
    {14,"14","hotrect",1376,288,32,128,true,"",0,"",0b0000},
    {15,"15","hotrect",64,224,1856,64,true,"",0,"",0b0000},
    {16,"16","hotrect",1184,288,32,160,true,"",0,"",0b0000},
    {17,"17","hotrect",928,288,64,96,true,"",0,"",0b0000},
    {18,"18","hotrect",896,384,96,64,true,"",0,"",0b0000},
    {19,"19","hotrect",864,320,64,64,true,"",0,"",0b0000},
    {20,"20","hotrect",1376,416,64,32,true,"",0,"",0b0000},
    {21,"21","hotrect",1248,320,64,160,true,"",0,"",0b0000},
    {22,"22","hotrect",160,320,64,160,true,"",0,"",0b0000},
    {23,"23","hotrect",288,288,64,160,true,"",0,"",0b0000},
    {24,"24","hotrect",96,288,32,160,true,"",0,"",0b0000},
    {25,"25","hotrect",0,0,32,480,true,"",0,"",0b0000},
    {26,"26","hotrect",288,32,64,160,true,"",0,"",0b0000},
    {27,"27","hotrect",448,96,64,64,true,"",0,"",0b0000},
    {28,"28","hotrect",608,96,64,128,true,"",0,"",0b0000},
    {29,"29","hotrect",32,0,928,32,true,"",0,"",0b0000},
    {30,"30","hotrect",736,160,224,32,true,"",0,"",0b0000},
    {31,"31","hotrect",864,32,64,32,true,"",0,"",0b0000},
    {32,"32","hotrect",928,0,32,160,true,"",0,"",0b0000},
    {33,"33","hotrect",608,64,128,32,true,"",0,"",0b0000},
    {34,"34","hotrect",672,128,32,32,true,"",0,"",0b0000},
    {35,"35","hotrect",896,96,32,32,true,"",0,"",0b0000},
    {36,"36","hotrect",768,416,64,32,true,"",0,"",0b0000},
    {37,"37","hotrect",800,352,64,32,true,"",0,"",0b0000},
    {38,"38","hotrect",800,384,32,32,true,"",0,"",0b0000},
    {39,"39","hotrect",544,320,224,32,true,"",0,"",0b0000},
    {40,"40","hotrect",704,352,64,32,true,"",0,"",0b0000},
    {41,"41","hotrect",640,416,64,32,true,"",0,"",0b0000},
    {42,"42","hotrect",704,384,32,64,true,"",0,"",0b0000},
    {43,"43","hotrect",640,448,32,32,true,"",0,"",0b0000},
    {44,"44","hotrect",416,448,96,32,true,"",0,"",0b0000},
    {45,"45","hotrect",480,416,96,32,true,"",0,"",0b0000},
    {46,"46","hotrect",544,384,96,32,true,"",0,"",0b0000},
    {47,"47","hotrect",608,352,32,32,true,"",0,"",0b0000},
    {48,"48","hotrect",0,480,448,32,true,"",0,"",0b0000},
    {49,"49","hotrect_platform",1632,352,32,4,true,"",0,"",0b0000},
    {164,"164","hotrect_platform",1856,352,32,4,true,"",0,"",0b0000},
    {165,"165","hotrect_platform",1696,352,128,4,true,"",0,"",0b0000},
    {166,"166","hotrect_platform",1632,448,64,4,true,"",0,"",0b0000},
    {167,"167","hotrect_platform",1824,448,64,4,true,"",0,"",0b0000},
    {168,"168","hotrect_platform",1696,384,128,4,true,"",0,"",0b0000},
    {169,"169","hotrect_platform",1696,416,128,4,true,"",0,"",0b0000},
    {170,"170","hotrect_platform",1536,352,32,4,true,"",0,"",0b0000},
    {171,"171","hotrect_platform",1536,384,32,4,true,"",0,"",0b0000},
    {172,"172","hotrect_platform",1536,416,32,4,true,"",0,"",0b0000},
    {173,"173","hotrect_platform",1536,448,32,4,true,"",0,"",0b0000},
    {174,"174","hotrect_platform",1408,352,64,4,true,"",0,"",0b0000},
    {175,"175","hotrect_platform",1312,352,32,4,true,"",0,"",0b0000},
    {176,"176","hotrect_platform",1344,384,32,4,true,"",0,"",0b0000},
    {177,"177","hotrect_platform",1312,416,32,4,true,"",0,"",0b0000},
    {178,"178","hotrect_platform",1344,448,32,4,true,"",0,"",0b0000},
    {179,"179","hotrect_platform",736,448,32,4,true,"",0,"",0b0000},
    {180,"180","hotrect_platform",768,384,32,4,true,"",0,"",0b0000},
    {181,"181","hotrect_platform",768,320,96,4,true,"",0,"",0b0000},
    {182,"182","hotrect_platform",416,384,128,4,true,"",0,"",0b0000},
    {183,"183","hotrect_platform",416,416,64,4,true,"",0,"",0b0000},
    {184,"184","hotrect_platform",736,64,96,4,true,"",0,"",0b0000},
    {185,"185","hotrect_platform",768,96,128,4,true,"",0,"",0b0000},
    {186,"186","hotrect_platform",704,128,160,4,true,"",0,"",0b0000},
    {188,"188","hotrect_platform",512,96,32,4,true,"",0,"",0b0000},
    {189,"189","hotrect_platform",416,96,32,4,true,"",0,"",0b0000},
    {190,"190","hotrect_platform",352,128,64,4,true,"",0,"",0b0000},
    {191,"191","hotrect_platform",544,128,64,4,true,"",0,"",0b0000},
    {192,"192","hotrect_platform",512,160,32,4,true,"",0,"",0b0000},
    {193,"193","hotrect_platform",416,160,32,4,true,"",0,"",0b0000},
    {194,"194","hotrect_platform",384,192,192,4,true,"",0,"",0b0000},
    {195,"195","hotrect_platform",32,320,32,4,true,"",0,"",0b0000},
    {196,"196","hotrect_platform",64,352,32,4,true,"",0,"",0b0000},
    {197,"197","hotrect_platform",32,384,32,4,true,"",0,"",0b0000},
    {198,"198","hotrect_platform",64,416,32,4,true,"",0,"",0b0000},
    {199,"199","hotrect_platform",32,448,32,4,true,"",0,"",0b0000},
    {200,"200","hotrect_platform",224,352,32,4,true,"",0,"",0b0000},
    {201,"201","hotrect_platform",256,384,32,4,true,"",0,"",0b0000},
    {202,"202","hotrect_platform",224,416,32,4,true,"",0,"",0b0000},
    {203,"203","hotrect_platform",224,448,32,4,true,"",0,"",0b0000},
    {204,"204","hotrect_platform",32,288,32,4,true,"",0,"",0b0000},
    {205,"205","hotrect_platform",32,256,32,4,true,"",0,"",0b0000},
    {206,"206","hotrect_platform",32,224,32,4,true,"",0,"",0b0000},
    {207,"207","hotrect_platform",32,192,32,4,true,"",0,"",0b0000},
    {208,"208","hotrect_platform",32,160,32,4,true,"",0,"",0b0000},
    {209,"209","hotrect_platform",32,128,32,4,true,"",0,"",0b0000},
    {210,"210","hotrect_platform",32,96,32,4,true,"",0,"",0b0000},
    {211,"211","hotrect_platform",32,96,32,4,true,"",0,"",0b0000},
    {212,"212","hotrect_platform",32,64,32,4,true,"",0,"",0b0000},
    {213,"213","hotrect_platform",256,64,32,4,true,"",0,"",0b0000},
    {214,"214","hotrect_platform",256,96,32,4,true,"",0,"",0b0000},
    {215,"215","hotrect_platform",256,128,32,4,true,"",0,"",0b0000},
    {216,"216","hotrect_platform",256,160,32,4,true,"",0,"",0b0000},
    {217,"217","hotrect_platform",256,192,32,4,true,"",0,"",0b0000},
    {218,"218","hotrect_platform",96,64,128,4,true,"",0,"",0b0000},
    {219,"219","hotrect_platform",96,96,128,4,true,"",0,"",0b0000},
    {220,"220","hotrect_platform",96,128,128,4,true,"",0,"",0b0000},
    {221,"221","hotrect_platform",96,160,128,4,true,"",0,"",0b0000},
    {222,"222","hotrect_platform",96,192,128,4,true,"",0,"",0b0000},
    {223,"dan","pig_box",960,192,32,32,true,"character_pigcrate",5,"",0b0000},
    {224,"224","pig_bomb",1440,320,32,32,true,"character_pigbomb",4,"",0b0000},
    {225,"225","pig_normal",1344,448,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {226,"226","pig_bomb",1408,384,32,32,true,"character_pigbomb",4,"",0b0000},
    {227,"227","pig_normal",1312,320,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {228,"228","pig_normal",1696,320,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {229,"229","pig_normal",1696,352,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {230,"230","pig_normal",1792,384,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {231,"231","pig_normal",1856,416,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {232,"232","pig_normal",1632,416,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {233,"233","pig_normal",1024,448,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {234,"234","pig_normal",800,448,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {235,"235","pig_normal",832,320,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {236,"236","pig_normal",640,288,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {237,"237","pig_normal",192,288,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {238,"238","pig_normal",256,448,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {239,"239","pig_normal",32,448,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {240,"240","pig_normal",32,416,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {241,"241","pig_normal",64,384,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {242,"242","pig_normal",32,352,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {243,"243","pig_normal",64,320,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {244,"244","pig_normal",32,288,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {245,"245","pig_normal",96,192,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {246,"246","pig_normal",96,32,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {247,"247","pig_normal",96,96,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {248,"248","pig_normal",352,96,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {249,"249","pig_normal",576,96,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {250,"250","pig_normal",576,192,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {251,"251","pig_normal",352,192,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {252,"252","pig_normal",672,192,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {253,"253","pig_normal",896,192,32,32,true,"gfx_pig",0,"loiter",0b0000},
    {254,"254","hotrect",1920,272,8,64,true,"",0,"",0b0000},
    {255,"0","room_trigger",1603,288,2,32,true,"",0,"",0b0000},
    {256,"1","room_trigger",1597,288,2,32,true,"",0,"",0b0000},
    {257,"1","room_trigger",1285,288,2,32,true,"",0,"",0b0000},
    {258,"2","room_trigger",1274,288,2,32,true,"",0,"",0b0000},
    {259,"2","room_trigger",973,448,2,32,true,"",0,"",0b0000},
    {260,"3","room_trigger",960,448,2,32,true,"",0,"",0b0000},
    {261,"3","room_trigger",645,288,2,32,true,"",0,"",0b0000},
    {262,"4","room_trigger",630,288,2,32,true,"",0,"",0b0000},
    {263,"4","room_trigger",325,448,2,32,true,"",0,"",0b0000},
    {264,"5","room_trigger",309,448,2,32,true,"",0,"",0b0000},
    {265,"5","room_trigger",309,192,2,32,true,"",0,"",0b0000},
    {266,"6","room_trigger",328,192,2,32,true,"",0,"",0b0000},
    {267,"6","room_trigger",629,32,2,32,true,"",0,"",0b0000},
    {268,"7","room_trigger",661,32,2,32,true,"",0,"",0b0000},
    {269,"7","room_trigger",949,192,2,32,true,"",0,"",0b0000}

};

static const size_t level_2_lvl_2_obj_objects_len = 154;

static const dang::tmx_layer level_2_lvl_2_obj = {
    .name = "lvl_2_obj",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 2,
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = level_2_lvl_2_obj_objects,
    .spriteobejcts_len = level_2_lvl_2_obj_objects_len
};

// Objects for layer: lvl_2_fg
static const dang::tmx_spriteobject level_2_lvl_2_fg_objects[] = {


};

static const size_t level_2_lvl_2_fg_objects_len = 0;

static const dang::tmx_layer level_2_lvl_2_fg = {
    .name = "lvl_2_fg",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 3,
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = level_2_lvl_2_fg_objects,
    .spriteobejcts_len = level_2_lvl_2_fg_objects_len
};

// Objects for layer: lvl_2_hud
static const dang::tmx_spriteobject level_2_lvl_2_hud_objects[] = {

    {226,"226","",15,18,32,32,true,"gfx_king",26,"",0b0000},
    {227,"227","",46,37,16,16,true,"hud_ui",61,"",0b0000},
    {228,"228","",62,37,16,16,true,"hud_ui",62,"",0b0000},
    {229,"229","",78,37,16,16,true,"hud_ui",63,"",0b0000}

};

static const size_t level_2_lvl_2_hud_objects_len = 4;

static const dang::tmx_layer level_2_lvl_2_hud = {
    .name = "lvl_2_hud",
    .type = dang::tmx_layerType::ltObjects,
    .opacity = 1,
    .visible = true,
    .z_order = 4,
    .tl_width = 0,
    .tl_height = 0,
    .tl_tileset = "",
    .tl_tiles = nullptr,
    .tl_tiles_len = 0,
    .spriteobjects = level_2_lvl_2_hud_objects,
    .spriteobejcts_len = level_2_lvl_2_hud_objects_len
};

// layer with points - path layer
static const dang::tmx_waypoint level_2_waypoints[] = {
    {270, 1680,440, 0x0},
    {271, 1712,472, 0x0},
    {272, 1808,472, 0x0},
    {273, 1840,440, 0x0},
    {274, 1648,440, 0x0},
    {275, 1872,440, 0x0},
    {276, 1776,472, 0x0},
    {277, 1744,472, 0x0},
    {278, 1648,472, 0x0},
    {279, 1872,472, 0x0},
    {280, 1808,408, 0x0},
    {281, 1712,408, 0x0},
    {282, 1712,376, 0x0},
    {283, 1808,376, 0x0},
    {284, 1712,344, 0x0},
    {285, 1808,344, 0x0},
};

static const size_t level_2_waypoints_len = 16;

static const dang::tmx_waypoint_connection level_2_connections[] = {
    {270, 271, 0x1},
    {270, 274, 0x1},
    {271, 278, 0x1},
    {271, 277, 0x1},
    {271, 270, 0x2},
    {272, 276, 0x1},
    {272, 279, 0x1},
    {272, 273, 0x2},
    {273, 272, 0x1},
    {273, 275, 0x1},
    {274, 270, 0x1},
    {275, 273, 0x1},
    {276, 272, 0x1},
    {277, 271, 0x1},
    {278, 271, 0x1},
    {279, 272, 0x1},
    {280, 281, 0x1},
    {281, 280, 0x1},
    {282, 283, 0x1},
    {283, 282, 0x1},
    {284, 285, 0x1},
    {285, 284, 0x1},
};

static const size_t level_2_connections_len = 22;

const static dang::tmx_layer level_2_layers[] = {
    level_2_lvl_2_bg,
    level_2_lvl_2_mood,
    level_2_lvl_2_obj,
    level_2_lvl_2_hud,
};

const static size_t level_2_layers_len = 6;

// Level ---------------------------------------------------

static const dang::tmx_level level_2_level {
    .w = &level_2_world,
    .tilesets = level_2_tilesets,
    .tilesets_len = level_2_tilesets_len,
    .tileanimations = level_2_tileanimations,
    .tileanimations_len = level_2_tileanimations_len,
    .layers = level_2_layers,
    .layers_len = level_2_layers_len,
    .waypoints = level_2_waypoints,
    .waypoints_len = level_2_waypoints_len,
    .waypoint_connections = level_2_connections,
    .waypoint_connections_len = level_2_connections_len
};


