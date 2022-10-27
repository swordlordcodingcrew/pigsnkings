// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "GameState.h"
#include <vector>

namespace pnk
{

    class GSEndScene : public GameState
    {
    public:
        std::shared_ptr<GameState> update(dang::Gear &gear, uint32_t time) override;
        void enter(dang::Gear &gear, uint32_t time) override;
        void exit(dang::Gear &gear, uint32_t time) override;

    protected:

        /**
         * rsrc for the end-scene state
         */
        const dang::tmx_level *_tmx{nullptr};

        /**
         * rsrc-names
         */
        const std::string tmx_bg_layer_name = "end_scene_bg";
        const std::string tmx_deco_layer_name = "end_scene_mood";
        const std::string tmx_obj_layer_name = "end_scene_obj";


        dang::spFullImgSpr _sprHero{nullptr};
        dang::spTwAnim _twaWalk{nullptr};
        dang::spTwAnim _twaWait{nullptr};
        dang::spTwAnim _twaJump{nullptr};

        enum uint8 {
            WALK = 1,
            JUMP,
            WAIT
        };

        struct slice {
            uint8           type;
            uint16_t        time;
            dang::Vector2F  pos;
            bool            to_the_left;
        };
        std::vector<slice>  _slices
        {
            {WAIT, 2000, {0,0}, false},
            {WALK, 6000, {482, 320}, false},
            {WAIT, 1000, {0, 0}, false},
            {WAIT, 1000, {0, 0}, true},
            {WALK, 500, {460, 320}, true},
            {JUMP, 500, {418, 288}, true},

            {WAIT, 500, {464, 288}, true},
            {JUMP, 500, {386, 256}, true},

            {WAIT, 1500, {386, 256}, true},
            {JUMP, 500, {354, 224}, true},

            {WAIT, 300, {354, 224}, true},
            {JUMP, 500, {322, 192}, true},

            {WAIT, 300, {322, 192}, true},
            {JUMP, 500, {290, 160}, true},

            {WAIT, 2000, {322, 192}, true},
            {WALK, 200, {258, 160}, true},
            {WAIT, 5000, {258, 160}, true},
        };

        uint8_t  _slice_pos{0}, _prev_slice_pos{0};

        void nextSlice();

    };

}
