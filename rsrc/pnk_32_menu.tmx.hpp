// This file is auto generated by the SwordLord DANG Tiled Exporter.
// It will get overwritten once you re-export your tile map!
// (c) 2019-20 by SwordLord - the coding crew


dang::tmx_level init_pnk_32_menu() {

    dang::tmx_level lvl = {};

// World ------------------------------------------------

    lvl.w.width = 30;
    lvl.w.height = 8;
    lvl.w.tileWidth = 32;
    lvl.w.tileHeight = 32;

// Tilesets ------------------------------------------------

    // Tileset: tiles_bg_32_png has ID: 0
    lvl.tilesets[0] = {"tiles_bg_32_png",96,32,32,384,256,12,8};
    // Tileset: menus_png has ID: 1
    lvl.tilesets[1] = {"menus_png",9,128,32,384,96,3,3};

// Layers ------------------------------------------------

    // layer tilelayer: menu_bg
    dang::tmx_tile tmenu_bg[] = {
        {4,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {5,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {4,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {25,0,0},
        {5,0,0},
        {14,0,0},
        {84,0,0},
        {85,0,0},
        {85,0,0},
        {85,0,0},
        {85,0,0},
        {85,0,0},
        {85,0,0},
        {86,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {48,0,0},
        {49,0,0},
        {49,0,0},
        {49,0,0},
        {49,0,0},
        {49,0,0},
        {49,0,0},
        {50,0,0},
        {12,0,0},
        {19,0,0},
        {37,0,0},
        {37,0,0},
        {37,0,0},
        {37,0,0},
        {37,0,0},
        {37,0,0},
        {37,0,0},
        {37,0,0},
        {6,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {48,0,0},
        {49,0,0},
        {49,0,0},
        {49,0,0},
        {49,0,0},
        {49,0,0},
        {49,0,0},
        {50,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {72,0,0},
        {73,0,0},
        {73,0,0},
        {73,0,0},
        {73,0,0},
        {73,0,0},
        {73,0,0},
        {74,0,0},
        {12,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {14,0,0},
        {72,0,0},
        {73,0,0},
        {73,0,0},
        {73,0,0},
        {73,0,0},
        {73,0,0},
        {73,0,0},
        {74,0,0},
        {12,0,0},
        {16,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {17,0,0},
        {14,0,0},
        {60,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {61,0,0},
        {62,0,0},
        {12,0,0},
        {16,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {1,0,0},
        {17,0,0}
    };
    dang::tmx_tilelayer tlmenu_bg = {"menu_bg",240,tmenu_bg,30,8};
    lvl.layers.push_back(std::make_shared<dang::tmx_tilelayer>(tlmenu_bg));

    // Objects for layer: menu_obj
    dang::tmx_spriteobject somenu_obj[] = {
        {20,"20","play",88,120,128,32,true,1,0},
        {21,"21","prefs",88,152,128,32,true,1,3},
        {22,"22","about",88,184,128,32,true,1,6}
    };
    dang::tmx_objectlayer lmenu_obj = {"menu_obj",3,somenu_obj};
    lvl.layers.push_back(std::make_shared<dang::tmx_objectlayer>(lmenu_obj));

    // make sure to generate every file with the 32blit-tool and link them.
    // tiles_bg_32_png.h
    // menus_png.h

    lvl.images["tiles_bg_32_png"] = tiles_bg_png;
    lvl.images["menus_png"] = menus_png;

    return lvl;
}
