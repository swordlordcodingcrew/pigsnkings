// This file is auto generated by the SwordLord DANG Tiled Exporter.
// It will get overwritten once you re-export your tile map!
// (c) 2019-2021 by SwordLord - the coding crew


dang::tmx_level init_level_1() {

    dang::tmx_level lvl = {};

// World ------------------------------------------------

    lvl.w.width = 30;
    lvl.w.height = 24;
    lvl.w.tileWidth = 32;
    lvl.w.tileHeight = 32;

// Tilesets ------------------------------------------------

    // Tileset: gfx_levels_castle_tiles has ID: 0
    lvl.tilesets[0] = {"gfx_levels_castle_tiles",96,32,32,384,256,12,8};
    // Tileset: gfx_castle_decorations has ID: 1
    lvl.tilesets[1] = {"gfx_castle_decorations",40,32,32,320,128,10,4};
    // Tileset: gfx_items has ID: 2
    lvl.tilesets[2] = {"gfx_items",53,32,48,1696,48,53,1};
    // Tileset: gfx_bubbles has ID: 3
    lvl.tilesets[3] = {"gfx_bubbles",11,32,32,352,32,11,1};
    // Tileset: hud_ui has ID: 4
    lvl.tilesets[4] = {"hud_ui",105,16,16,240,112,15,7};
    // Tileset: gfx_king has ID: 5
    lvl.tilesets[5] = {"gfx_king",27,32,32,864,32,27,1};
    // Tileset: gfx_pig has ID: 6
    lvl.tilesets[6] = {"gfx_pig",16,32,32,512,32,16,1};
    // Tileset: character_pigbomb has ID: 7
    lvl.tilesets[7] = {"character_pigbomb",33,32,32,1056,32,33,1};
    // Tileset: character_pigcrate has ID: 8
    lvl.tilesets[8] = {"character_pigcrate",39,32,32,1248,32,39,1};

// Animations ------------------------------------------------

    // Animation: coin_silver
    lvl.tileanimation["gfx_items_coin_silver"] = {"gfx_items",0,"coin_silver",{{0, 100},{1, 100},{2, 100},{3, 100}}};
    // Animation: coin_gold
    lvl.tileanimation["gfx_items_coin_gold"] = {"gfx_items",4,"coin_gold",{{4, 100},{5, 100},{6, 100},{7, 100}}};
    // Animation: coin_poof
    lvl.tileanimation["gfx_items_coin_poof"] = {"gfx_items",8,"coin_poof",{{8, 100},{9, 100},{10, 100},{11, 100}}};
    // Animation: gem_blue
    lvl.tileanimation["gfx_items_gem_blue"] = {"gfx_items",12,"gem_blue",{{12, 100},{13, 100},{14, 100},{15, 100}}};
    // Animation: gem_green
    lvl.tileanimation["gfx_items_gem_green"] = {"gfx_items",16,"gem_green",{{16, 100},{17, 100},{18, 100},{19, 100}}};
    // Animation: gem_red
    lvl.tileanimation["gfx_items_gem_red"] = {"gfx_items",20,"gem_red",{{20, 100},{21, 100},{22, 100},{23, 100}}};
    // Animation: gem_poof
    lvl.tileanimation["gfx_items_gem_poof"] = {"gfx_items",24,"gem_poof",{{24, 100},{25, 100},{26, 100},{27, 100}}};
    // Animation: potion_blue
    lvl.tileanimation["gfx_items_potion_blue"] = {"gfx_items",28,"potion_blue",{{28, 100},{29, 100},{30, 100},{31, 100},{32, 100},{33, 100},{34, 100}}};
    // Animation: potion_red
    lvl.tileanimation["gfx_items_potion_red"] = {"gfx_items",35,"potion_red",{{35, 100},{36, 100},{37, 100},{38, 100},{39, 100},{40, 100},{41, 100}}};
    // Animation: potion_green
    lvl.tileanimation["gfx_items_potion_green"] = {"gfx_items",42,"potion_green",{{42, 100},{43, 100},{44, 100},{45, 100},{46, 100},{47, 100},{48, 100}}};
    // Animation: potion_poof
    lvl.tileanimation["gfx_items_potion_poof"] = {"gfx_items",49,"potion_poof",{{49, 100},{50, 100},{51, 100},{52, 100}}};
    // Animation: bubble_blow
    lvl.tileanimation["gfx_bubbles_bubble_blow"] = {"gfx_bubbles",0,"bubble_blow",{{0, 120},{1, 120},{2, 120},{3, 120},{4, 100}}};
    // Animation: bubble_bobble
    lvl.tileanimation["gfx_bubbles_bubble_bobble"] = {"gfx_bubbles",4,"bubble_bobble",{{3, 120},{5, 120},{4, 120},{3, 120},{6, 120}}};
    // Animation: bubble_poof
    lvl.tileanimation["gfx_bubbles_bubble_poof"] = {"gfx_bubbles",7,"bubble_poof",{{7, 100},{8, 100},{9, 100}}};
    // Animation: wait
    lvl.tileanimation["gfx_king_wait"] = {"gfx_king",0,"wait",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100},{5, 100},{6, 100},{0, 800}}};
    // Animation: blink
    lvl.tileanimation["gfx_king_blink"] = {"gfx_king",1,"blink",{{0, 100},{25, 100}}};
    // Animation: jump
    lvl.tileanimation["gfx_king_jump"] = {"gfx_king",7,"jump",{{9, 100},{7, 100}}};
    // Animation: on_air
    lvl.tileanimation["gfx_king_on_air"] = {"gfx_king",8,"on_air",{{8, 100}}};
    // Animation: walk
    lvl.tileanimation["gfx_king_walk"] = {"gfx_king",10,"walk",{{10, 100},{11, 100},{12, 100},{13, 100},{14, 100},{15, 100},{16, 100},{17, 100}}};
    // Animation: bubble
    lvl.tileanimation["gfx_king_bubble"] = {"gfx_king",18,"bubble",{{18, 100},{19, 100},{20, 100},{19, 100},{18, 100}}};
    // Animation: hit
    lvl.tileanimation["gfx_king_hit"] = {"gfx_king",21,"hit",{{21, 200},{22, 200},{23, 200}}};
    // Animation: sleeping
    lvl.tileanimation["gfx_pig_sleeping"] = {"gfx_pig",0,"sleeping",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100},{5, 100}}};
    // Animation: loitering
    lvl.tileanimation["gfx_pig_loitering"] = {"gfx_pig",6,"loitering",{{6, 100},{7, 100},{8, 100},{9, 100},{10, 100},{11, 100}}};
    // Animation: bubbling
    lvl.tileanimation["gfx_pig_bubbling"] = {"gfx_pig",13,"bubbling",{{12, 100},{13, 100}}};
    // Animation: hitting
    lvl.tileanimation["gfx_pig_hitting"] = {"gfx_pig",14,"hitting",{{14, 100},{15, 100}}};
    // Animation: picking_up
    lvl.tileanimation["character_pigbomb_picking_up"] = {"character_pigbomb",0,"picking_up",{{0, 100},{1, 100},{2, 100},{3, 100}}};
    // Animation: sleeping
    lvl.tileanimation["character_pigbomb_sleeping"] = {"character_pigbomb",4,"sleeping",{{4, 100},{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100},{11, 100},{12, 100},{13, 100}}};
    // Animation: loitering
    lvl.tileanimation["character_pigbomb_loitering"] = {"character_pigbomb",14,"loitering",{{14, 100},{15, 100},{16, 100},{17, 100},{18, 100},{19, 100}}};
    // Animation: throwing
    lvl.tileanimation["character_pigbomb_throwing"] = {"character_pigbomb",20,"throwing",{{20, 100},{21, 100},{22, 100},{23, 100},{24, 100}}};
    // Animation: bubbling
    lvl.tileanimation["character_pigbomb_bubbling"] = {"character_pigbomb",23,"bubbling",{{23, 100},{24, 100}}};
    // Animation: bomb_off
    lvl.tileanimation["character_pigbomb_bomb_off"] = {"character_pigbomb",25,"bomb_off",{{25, 100}}};
    // Animation: bomb_on
    lvl.tileanimation["character_pigbomb_bomb_on"] = {"character_pigbomb",26,"bomb_on",{{26, 100},{27, 100},{28, 100},{29, 100}}};
    // Animation: bubbling
    lvl.tileanimation["character_pigbomb_bubbling"] = {"character_pigbomb",31,"bubbling",{{31, 100},{32, 100}}};
    // Animation: picking_up
    lvl.tileanimation["character_pigcrate_picking_up"] = {"character_pigcrate",0,"picking_up",{{0, 100},{1, 100},{2, 100},{3, 100},{4, 100}}};
    // Animation: sleeping
    lvl.tileanimation["character_pigcrate_sleeping"] = {"character_pigcrate",5,"sleeping",{{5, 100},{6, 100},{7, 100},{8, 100},{9, 100},{10, 100},{11, 100},{12, 100},{13, 100}}};
    // Animation: loitering
    lvl.tileanimation["character_pigcrate_loitering"] = {"character_pigcrate",14,"loitering",{{14, 100},{15, 100},{16, 100},{17, 100},{18, 100},{19, 100}}};
    // Animation: throwing
    lvl.tileanimation["character_pigcrate_throwing"] = {"character_pigcrate",20,"throwing",{{20, 100},{21, 100},{22, 100},{23, 100},{24, 100},{36, 100}}};
    // Animation: looking_out
    lvl.tileanimation["character_pigcrate_looking_out"] = {"character_pigcrate",25,"looking_out",{{25, 100},{26, 100},{27, 100}}};
    // Animation: jumping
    lvl.tileanimation["character_pigcrate_jumping"] = {"character_pigcrate",28,"jumping",{{28, 100},{29, 100},{30, 100},{31, 100},{32, 100},{33, 100}}};
    // Animation: bubbling
    lvl.tileanimation["character_pigcrate_bubbling"] = {"character_pigcrate",37,"bubbling",{{37, 100},{38, 100}}};

// Layers ------------------------------------------------

    // layer tilelayer: lvl_1_bg
    dang::tmx_tile tlvl_1_bg[] = {
 {4,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {5,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {4,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {5,0,0}, {4,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {5,0,0},
 {14,0,0}, {48,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {50,0,0}, {24,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {18,0,0}, {26,0,0}, {48,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {50,0,0}, {24,0,0}, {26,0,0}, {48,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {50,0,0}, {24,0,0},
 {14,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {64,0,0}, {76,0,0}, {85,0,0}, {85,0,0}, {85,0,0}, {83,0,0}, {27,0,0}, {48,0,0}, {65,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {64,0,0}, {49,0,0}, {49,0,0}, {65,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {52,0,0}, {73,0,0}, {88,0,0}, {85,0,0},
 {14,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {0,0,0}, {1,0,0}, {2,0,0}, {94,0,0}, {77,0,0}, {65,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {36,0,0}, {37,0,0}, {29,0,0},
 {14,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {52,0,0}, {74,0,0}, {12,0,0}, {13,0,0}, {16,0,0}, {2,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {52,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {53,0,0}, {61,0,0}, {64,0,0}, {49,0,0}, {50,0,0}, {12,0,0},
 {14,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {52,0,0}, {74,0,0}, {0,0,0}, {17,0,0}, {13,0,0}, {13,0,0}, {14,0,0}, {60,0,0}, {61,0,0}, {52,0,0}, {74,0,0}, {0,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {2,0,0}, {72,0,0}, {53,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {12,0,0},
 {14,0,0}, {72,0,0}, {73,0,0}, {73,0,0}, {66,0,0}, {74,0,0}, {0,0,0}, {17,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {14,0,0}, {72,0,0}, {73,0,0}, {74,0,0}, {0,0,0}, {17,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {16,0,0}, {2,0,0}, {72,0,0}, {73,0,0}, {73,0,0}, {74,0,0}, {12,0,0},
 {16,0,0}, {1,0,0}, {1,0,0}, {2,0,0}, {63,0,0}, {0,0,0}, {17,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {16,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {17,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {16,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {17,0,0},
 {13,0,0}, {13,0,0}, {13,0,0}, {14,0,0}, {63,0,0}, {12,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {4,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {5,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0},
 {4,0,0}, {25,0,0}, {25,0,0}, {26,0,0}, {63,0,0}, {24,0,0}, {25,0,0}, {25,0,0}, {5,0,0}, {13,0,0}, {14,0,0}, {48,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {50,0,0}, {12,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0},
 {14,0,0}, {48,0,0}, {49,0,0}, {49,0,0}, {55,0,0}, {49,0,0}, {49,0,0}, {50,0,0}, {12,0,0}, {13,0,0}, {14,0,0}, {72,0,0}, {73,0,0}, {53,0,0}, {61,0,0}, {61,0,0}, {52,0,0}, {73,0,0}, {74,0,0}, {12,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0},
 {14,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {12,0,0}, {13,0,0}, {19,0,0}, {37,0,0}, {38,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {36,0,0}, {37,0,0}, {6,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0},
 {14,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {52,0,0}, {74,0,0}, {12,0,0}, {13,0,0}, {14,0,0}, {84,0,0}, {85,0,0}, {54,0,0}, {61,0,0}, {61,0,0}, {67,0,0}, {85,0,0}, {86,0,0}, {12,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0},
 {14,0,0}, {72,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {74,0,0}, {0,0,0}, {17,0,0}, {13,0,0}, {19,0,0}, {37,0,0}, {38,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {36,0,0}, {37,0,0}, {41,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0},
 {16,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {17,0,0}, {13,0,0}, {13,0,0}, {14,0,0}, {84,0,0}, {85,0,0}, {89,0,0}, {73,0,0}, {73,0,0}, {88,0,0}, {85,0,0}, {85,0,0}, {85,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0},
 {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {16,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0}, {13,0,0},
 {4,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {26,0,0}, {63,0,0}, {24,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {5,0,0}, {13,0,0}, {13,0,0}, {4,0,0}, {25,0,0}, {25,0,0}, {5,0,0}, {13,0,0}, {4,0,0}, {25,0,0}, {25,0,0}, {25,0,0}, {25,0,0},
 {14,0,0}, {84,0,0}, {85,0,0}, {77,0,0}, {49,0,0}, {76,0,0}, {85,0,0}, {93,0,0}, {85,0,0}, {85,0,0}, {85,0,0}, {85,0,0}, {77,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {50,0,0}, {24,0,0}, {25,0,0}, {25,0,0}, {26,0,0}, {48,0,0}, {50,0,0}, {24,0,0}, {25,0,0}, {26,0,0}, {48,0,0}, {49,0,0}, {76,0,0}, {85,0,0},
 {19,0,0}, {37,0,0}, {38,0,0}, {60,0,0}, {61,0,0}, {62,0,0}, {36,0,0}, {37,0,0}, {37,0,0}, {29,0,0}, {1,0,0}, {2,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {64,0,0}, {76,0,0}, {85,0,0}, {85,0,0}, {85,0,0}, {89,0,0}, {88,0,0}, {85,0,0}, {85,0,0}, {85,0,0}, {54,0,0}, {61,0,0}, {62,0,0}, {0,0,0},
 {14,0,0}, {48,0,0}, {49,0,0}, {65,0,0}, {61,0,0}, {64,0,0}, {49,0,0}, {49,0,0}, {50,0,0}, {30,0,0}, {25,0,0}, {26,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {36,0,0}, {29,0,0}, {28,0,0}, {37,0,0}, {37,0,0}, {37,0,0}, {37,0,0}, {38,0,0}, {60,0,0}, {61,0,0}, {62,0,0}, {12,0,0},
 {26,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {15,0,0}, {85,0,0}, {77,0,0}, {65,0,0}, {52,0,0}, {73,0,0}, {73,0,0}, {53,0,0}, {64,0,0}, {50,0,0}, {12,0,0}, {14,0,0}, {48,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {49,0,0}, {65,0,0}, {61,0,0}, {62,0,0}, {12,0,0},
 {85,0,0}, {89,0,0}, {53,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {52,0,0}, {74,0,0}, {42,0,0}, {2,0,0}, {60,0,0}, {61,0,0}, {62,0,0}, {0,0,0}, {2,0,0}, {60,0,0}, {61,0,0}, {62,0,0}, {12,0,0}, {14,0,0}, {60,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {61,0,0}, {62,0,0}, {12,0,0},
 {1,0,0}, {2,0,0}, {72,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {74,0,0}, {0,0,0}, {17,0,0}, {14,0,0}, {72,0,0}, {73,0,0}, {74,0,0}, {12,0,0}, {14,0,0}, {72,0,0}, {73,0,0}, {74,0,0}, {12,0,0}, {14,0,0}, {72,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {73,0,0}, {74,0,0}, {12,0,0},
 {13,0,0}, {16,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {17,0,0}, {13,0,0}, {16,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {17,0,0}, {16,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {17,0,0}, {16,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {17,0,0}
 };
    dang::tmx_tilelayer tllvl_1_bg = {"lvl_1_bg",720,tlvl_1_bg,30,24,0};
    lvl.layers.push_back(std::make_shared<dang::tmx_tilelayer>(tllvl_1_bg));

    // Objects for layer: lvl_1_mood
    dang::tmx_spriteobject solvl_1_mood[] = {
        {3,"3","",61,654,32,32,true,1,20},
        {4,"4","",61,622,32,32,true,1,0},
        {5,"5","",238,623,32,32,true,1,21},
        {6,"6","",270,623,32,32,true,1,22},
        {7,"7","",238,655,32,32,true,1,31},
        {8,"8","",270,655,32,32,true,1,32},
        {64,"64","",166,134,32,32,true,1,21},
        {65,"65","",198,134,32,32,true,1,22},
        {66,"66","",166,166,32,32,true,1,31},
        {67,"67","",198,166,32,32,true,1,32},
        {86,"86","",399,625,32,32,true,1,0},
        {87,"87","",528,624,32,32,true,1,0},
        {88,"88","",399,657,32,32,true,1,30},
        {89,"89","",528,656,32,32,true,1,30},
        {98,"98","",448,553,32,32,true,1,23},
        {99,"99","",480,553,32,32,true,1,24},
        {100,"100","",448,585,32,32,true,1,33},
        {101,"101","",480,585,32,32,true,1,34},
        {121,"121","",460,307,32,32,true,1,21},
        {122,"122","",492,307,32,32,true,1,22},
        {123,"123","",460,339,32,32,true,1,31},
        {124,"124","",492,339,32,32,true,1,32},
        {129,"129","",460,400,32,32,true,1,23},
        {130,"130","",492,400,32,32,true,1,24},
        {131,"131","",460,432,32,32,true,1,33},
        {133,"133","",492,432,32,32,true,1,34},
        {134,"134","",64,328,32,32,true,1,0},
        {135,"135","",64,360,32,32,true,1,10},
        {136,"136","",64,392,32,32,true,1,20},
        {137,"137","",192,326,32,32,true,1,0},
        {138,"138","",192,358,32,32,true,1,10},
        {139,"139","",192,390,32,32,true,1,20},
        {52,"52","",160,672,32,32,true,1,1},
        {53,"53","",192,672,32,32,true,1,3},
        {47,"47","",128,576,32,32,true,1,4},
        {50,"50","",128,640,32,32,true,1,4},
        {51,"51","",128,608,32,32,true,1,4},
        {232,"232","",128,704,32,32,true,1,1},
        {233,"233","",160,704,32,32,true,1,2},
        {234,"234","",192,704,32,32,true,1,3},
        {255,"255","",864,576,32,32,true,1,0},
        {256,"256","",864,608,32,32,true,1,10},
        {257,"257","",864,640,32,32,true,1,10},
        {258,"258","",864,672,32,32,true,1,20},
        {259,"259","",832,608,32,32,true,1,4},
        {260,"260","",832,640,32,32,true,1,4},
        {261,"261","",896,608,32,32,true,1,4},
        {262,"262","",896,640,32,32,true,1,4},
        {263,"263","",896,672,32,32,true,1,4},
        {264,"264","",896,704,32,32,true,1,4},
        {278,"278","",704,672,32,32,true,1,5},
        {279,"279","",768,672,32,32,true,1,5},
        {82,"82","",384,576,32,32,true,1,4},
        {83,"83","",544,608,32,32,true,1,4},
        {84,"84","",544,704,32,32,true,1,4},
        {85,"85","",384,704,32,32,true,1,4},
        {102,"102","",448,608,32,32,true,1,1},
        {103,"103","",448,640,32,32,true,1,1},
        {104,"104","",480,608,32,32,true,1,3},
        {105,"105","",480,640,32,32,true,1,3}
    };
    dang::tmx_objectlayer llvl_1_mood = {"lvl_1_mood",60,solvl_1_mood,1};
    lvl.layers.push_back(std::make_shared<dang::tmx_objectlayer>(llvl_1_mood));

    // Objects for layer: lvl_1_obj
    dang::tmx_spriteobject solvl_1_obj[] = {
        {54,"54","",96,192,32,32,true,1,4},
        {55,"55","",64,160,32,32,true,1,4},
        {56,"56","",96,128,32,32,true,1,4},
        {57,"57","",64,96,32,32,true,1,4},
        {58,"58","",96,64,32,32,true,1,1},
        {59,"59","",128,64,32,32,true,1,3},
        {62,"62","",192,96,32,32,true,1,11},
        {63,"63","",224,96,32,32,true,1,13},
        {71,"71","",32,384,32,32,true,1,4},
        {72,"72","",224,352,32,32,true,1,4},
        {73,"73","",96,352,32,32,true,1,1},
        {74,"74","",96,384,32,32,true,1,1},
        {75,"75","",96,416,32,32,true,1,1},
        {76,"76","",160,352,32,32,true,1,3},
        {77,"77","",160,384,32,32,true,1,3},
        {78,"78","",160,416,32,32,true,1,3},
        {79,"79","",128,416,32,32,true,1,2},
        {80,"80","",128,384,32,32,true,1,2},
        {81,"81","",128,352,32,32,true,1,2},
        {108,"108","",448,448,32,32,true,1,1},
        {109,"109","",448,384,32,32,true,1,1},
        {110,"110","",352,320,32,32,true,1,1},
        {111,"111","",544,320,32,32,true,1,1},
        {112,"112","",480,384,32,32,true,1,3},
        {113,"113","",480,448,32,32,true,1,3},
        {114,"114","",576,320,32,32,true,1,3},
        {115,"115","",384,320,32,32,true,1,3},
        {117,"117","",416,352,32,32,true,1,4},
        {118,"118","",416,416,32,32,true,1,4},
        {119,"119","",512,416,32,32,true,1,4},
        {120,"120","",512,352,32,32,true,1,4},
        {143,"143","coin_silver",384,534,32,48,true,2,0},
        {144,"144","coin_silver",448,598,32,48,true,2,0},
        {145,"145","coin_silver",480,598,32,48,true,2,0},
        {146,"146","coin_silver",544,662,32,48,true,2,0},
        {147,"147","coin_silver",384,662,32,48,true,2,0},
        {148,"148","coin_silver",128,598,32,48,true,2,0},
        {149,"149","coin_silver",128,566,32,48,true,2,0},
        {150,"150","coin_silver",128,534,32,48,true,2,0},
        {151,"151","coin_silver",352,310,32,48,true,2,0},
        {152,"152","coin_silver",384,310,32,48,true,2,0},
        {153,"153","coin_silver",544,310,32,48,true,2,0},
        {154,"154","coin_silver",576,310,32,48,true,2,0},
        {155,"155","coin_gold",352,374,32,48,true,2,4},
        {156,"156","coin_gold",576,374,32,48,true,2,4},
        {157,"157","coin_gold",544,566,32,48,true,2,4},
        {158,"158","coin_gold",352,278,32,48,true,2,4},
        {159,"159","coin_gold",384,278,32,48,true,2,4},
        {160,"160","coin_gold",544,278,32,48,true,2,4},
        {161,"161","coin_gold",576,278,32,48,true,2,4},
        {162,"162","coin_gold",32,22,32,48,true,2,4},
        {163,"163","gem_blue",463,321,32,48,true,2,12},
        {164,"164","",448,64,32,32,true,1,4},
        {165,"165","",384,192,32,32,true,1,4},
        {166,"166","",832,192,32,32,true,1,1},
        {167,"167","",864,192,32,32,true,1,3},
        {168,"168","",448,160,32,32,true,1,3},
        {169,"169","",416,160,32,32,true,1,1},
        {170,"170","",800,128,32,32,true,1,4},
        {179,"179","hotrect",0,704,64,64,true,0,0},
        {180,"180","hotrect_platform",128,704,96,4,true,0,0},
        {182,"182","hotrect",256,704,64,32,true,0,0},
        {183,"183","hotrect",0,512,32,160,true,0,0},
        {184,"184","hotrect",32,512,192,32,true,0,0},
        {185,"185","hotrect",32,576,64,32,true,0,0},
        {186,"186","hotrect",192,576,192,32,true,0,0},
        {187,"187","hotrect",288,608,32,96,true,0,0},
        {189,"189","hotrect_platform",128,576,32,4,true,0,0},
        {190,"190","hotrect_platform",128,608,32,4,true,0,0},
        {191,"191","hotrect_platform",128,640,32,4,true,0,0},
        {195,"195","hotrect",216,504,56,8,true,0,0},
        {196,"196","king",0,672,32,32,true,5,0},
        {197,"197","bubble_proto",0,480,32,32,true,3,0},
        {198,"198","hotrect_platform",160,672,64,4,true,0,0},
        {199,"199","coin_silver",64,694,32,48,true,2,0},
        {200,"200","gem_blue",96,694,32,48,true,2,12},
        {202,"202","potion_green",224,694,32,48,true,2,42},
        {235,"235","hotrect",256,512,704,32,true,0,0},
        {236,"236","hotrect",64,736,896,32,true,0,0},
        {237,"237","hotrect",320,672,32,64,true,0,0},
        {238,"238","hotrect",320,608,64,32,true,0,0},
        {239,"239","hotrect",448,672,64,64,true,0,0},
        {240,"240","hotrect",608,288,32,160,true,0,0},
        {241,"241","hotrect",544,544,128,32,true,0,0},
        {243,"243","hotrect_platform",384,576,32,4,true,0,0},
        {244,"244","hotrect_platform",448,608,64,4,true,0,0},
        {245,"245","hotrect_platform",448,640,64,4,true,0,0},
        {246,"246","hotrect_platform",384,704,32,4,true,0,0},
        {247,"247","hotrect_platform",544,704,32,4,true,0,0},
        {248,"248","hotrect_platform",544,608,32,4,true,0,0},
        {280,"280","hotrect",736,544,96,32,true,0,0},
        {283,"283","hotrect",608,640,64,96,true,0,0},
        {284,"284","hotrect",928,576,32,160,true,0,0},
        {286,"286","hotrect",576,608,256,32,true,0,0},
        {288,"288","hotrect_platform",832,608,32,4,true,0,0},
        {289,"289","hotrect_platform",832,640,32,4,true,0,0},
        {290,"290","hotrect_platform",896,608,32,4,true,0,0},
        {291,"291","hotrect_platform",896,640,32,4,true,0,0},
        {292,"292","hotrect_platform",896,672,32,4,true,0,0},
        {293,"293","hotrect_platform",896,704,32,4,true,0,0},
        {265,"265","coin_silver",672,566,32,48,true,2,0},
        {266,"266","coin_silver",704,566,32,48,true,2,0},
        {267,"267","coin_silver",672,534,32,48,true,2,0},
        {268,"268","coin_silver",704,534,32,48,true,2,0},
        {269,"269","coin_gold",672,630,32,48,true,2,4},
        {270,"270","coin_silver",704,630,32,48,true,2,0},
        {271,"271","coin_silver",736,630,32,48,true,2,0},
        {272,"272","coin_silver",768,630,32,48,true,2,0},
        {273,"273","coin_silver",800,630,32,48,true,2,0},
        {274,"274","coin_silver",832,534,32,48,true,2,0},
        {275,"275","coin_silver",864,534,32,48,true,2,0},
        {276,"276","coin_silver",896,534,32,48,true,2,0},
        {277,"277","coin_silver",928,534,32,48,true,2,0},
        {294,"294","hotrect_platform",416,416,32,4,true,0,0},
        {295,"295","hotrect_platform",416,352,32,4,true,0,0},
        {296,"296","hotrect_platform",512,352,32,4,true,0,0},
        {297,"297","hotrect_platform",512,416,32,4,true,0,0},
        {298,"298","hotrect_platform",448,384,64,4,true,0,0},
        {299,"299","hotrect_platform",448,448,64,4,true,0,0},
        {300,"300","hotrect_platform",352,320,64,4,true,0,0},
        {301,"301","hotrect_platform",544,320,64,4,true,0,0},
        {302,"302","hotrect",320,480,320,32,true,0,0},
        {303,"303","hotrect",320,256,320,32,true,0,0},
        {305,"305","hotrect",320,288,32,192,true,0,0},
        {306,"306","hotrect",352,416,64,32,true,0,0},
        {307,"307","hotrect",352,352,64,32,true,0,0},
        {308,"308","hotrect",544,352,64,32,true,0,0},
        {309,"309","hotrect",544,416,64,32,true,0,0},
        {310,"310","hotrect",512,160,256,32,true,0,0},
        {311,"311","hotrect",352,224,608,32,true,0,0},
        {312,"312","hotrect",768,192,32,32,true,0,0},
        {313,"313","hotrect_platform",480,192,32,4,true,0,0},
        {314,"314","hotrect",384,0,576,32,true,0,0},
        {315,"315","hotrect",608,32,64,32,true,0,0},
        {316,"316","hotrect",864,96,96,32,true,0,0},
        {317,"317","hotrect",928,32,32,32,true,0,0},
        {318,"318","hotrect",928,128,32,96,true,0,0},
        {319,"319","hotrect",352,128,32,96,true,0,0},
        {320,"320","hotrect_platform",384,192,32,4,true,0,0},
        {321,"321","hotrect_platform",416,160,64,4,true,0,0},
        {322,"322","hotrect_platform",832,192,64,4,true,0,0},
        {323,"323","hotrect_platform",800,128,32,4,true,0,0},
        {324,"324","hotrect_platform",416,64,64,4,true,0,0},
        {326,"326","hotrect",320,96,32,64,true,0,0},
        {328,"328","",352,96,32,32,true,1,4},
        {329,"329","hotrect_platform",352,96,32,4,true,0,0},
        {330,"330","hotrect",384,64,32,32,true,0,0},
        {331,"331","hotrect",320,32,64,32,true,0,0},
        {333,"dan","pig_box",320,640,32,32,true,8,5},
        {334,"334","pig_bomb",768,704,32,32,true,7,4},
        {335,"335","pig_normal",576,704,32,32,true,6,0},
        {336,"336","pig_normal",800,576,32,32,true,6,0},
        {340,"boris","pig_box",192,640,32,32,true,8,5},
        {341,"fred","pig_bomb",256,672,32,32,true,7,4},
        {342,"sam","pig_normal",192,672,32,32,true,6,0},
        {344,"344","crate_proto",32,480,32,32,true,8,34},
        {345,"345","hotrect",-8,648,8,64,true,0,0},
        {346,"346","hotrect",960,528,8,64,true,0,0},
        {347,"1","room_trigger",317,544,2,32,true,0,0},
        {348,"0","room_trigger",288,544,2,32,true,0,0},
        {349,"2","room_trigger",640,576,2,32,true,0,0},
        {350,"1","room_trigger",608,576,2,32,true,0,0},
        {351,"351","bomb_proto",64,480,32,32,true,7,26}
    };
    dang::tmx_objectlayer llvl_1_obj = {"lvl_1_obj",163,solvl_1_obj,2};
    lvl.layers.push_back(std::make_shared<dang::tmx_objectlayer>(llvl_1_obj));

    // Objects for layer: lvl_1_hud
    dang::tmx_spriteobject solvl_1_hud[] = {
        {250,"250","",18,538,32,32,true,5,26},
        {251,"251","",49,557,16,16,true,4,61},
        {252,"252","",65,557,16,16,true,4,62},
        {254,"254","",81,557,16,16,true,4,63}
    };
    dang::tmx_objectlayer llvl_1_hud = {"lvl_1_hud",4,solvl_1_hud,3};
    lvl.layers.push_back(std::make_shared<dang::tmx_objectlayer>(llvl_1_hud));

// Images ------------------------------------------------

    lvl.images["gfx_levels_castle_tiles"] = gfx_levels_castle_tiles;
    lvl.images["gfx_castle_decorations"] = gfx_castle_decorations;
    lvl.images["gfx_items"] = gfx_items;
    lvl.images["gfx_bubbles"] = gfx_bubbles;
    lvl.images["hud_ui"] = hud_ui;
    lvl.images["gfx_king"] = gfx_king;
    lvl.images["gfx_pig"] = gfx_pig;
    lvl.images["character_pigbomb"] = character_pigbomb;
    lvl.images["character_pigcrate"] = character_pigcrate;

    return lvl;
}
