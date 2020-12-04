// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>

#include <Event.h>
#include <Dispatcher.h>
#include <TmxExtruder.h>
#include <Vector2T.h>

#include "GameState.h"


namespace pnk
{

    class Hero;

    class GSPlay : public GameState
    {
    public:
        std::shared_ptr<GameState> update(dang::Gear& gear, uint32_t time) override;
        void enter(dang::Gear& gear, uint32_t time) override;
        void exit(dang::Gear& gear, uint32_t time) override;
        const std::string& getInfotext() override { return _infoText; }

    protected:
        const std::string _infoText = "RIGHT: move right\n"
                                      "LEFT: move left\n"
                                      "Btn A ('z'): jump\n"
                                      "Btn B ('x'): bubble\n"
                                      "HOME ('1'): back to main\n"
                                      "MENU ('2'): pause";

        dang::tmx_level _lvl;

        std::shared_ptr<pnk::Hero> _spr_hero{nullptr};
        std::shared_ptr<dang::CollisionSpriteLayer> _csl{nullptr};
        spImagesheet _obj_is{nullptr};
        dang::tmx_spriteobject _bubble_prototype;

        // vp handling per room
        // assuming that 1 room is the size of the screen
        dang::Vector2F      _room_center{0,0};
        const uint32_t      _room_buffer = 4;  // "hotrect" when the room shall change

        // define level to be loaded (according to the tmx-file)
        const std::string bg_name = "lvl1_bg";
        const std::string playfield_name = "lvl1_obj";
        const std::string img_name_bg = "tiles_bg_32_png";
        const std::string img_name_obj = "players_png";

        // reference to subscriber
        u_int32_t _sub_ref{0};

        void gameEventReceived(dang::Event &e);

    };

}