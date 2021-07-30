// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>

#include <Event.hpp>
#include <Dispatcher.hpp>
#include <TmxExtruder.hpp>

#include "GameState.h"

namespace pnk
{

    class GSHome : public GameState
    {
    public:
        std::shared_ptr<GameState> update(dang::Gear& gear, uint32_t time) override;
        void enter(dang::Gear& gear, uint32_t time) override;
        void exit(dang::Gear& gear, uint32_t time) override;

    protected:

        void positionCandles();
        void playOink();
        void checkCheatActivation();

        /**
         * rsrc for the home state
         */
        const dang::tmx_level* _tmx{nullptr};

        /**
         * rsrc-names
         */
        const std::string tmx_bg_layer_name = "main_bg";
        const std::string tmx_deco_layer_name = "main_decoration";
        const std::string tmx_obj_layer_name = "main_obj";

        /**
         * buttons & animations
         */
         struct sel_struct
         {
             dang::spSprite btn;
             dang::spTwAnim anim;
             int img_index;
         };
        std::vector<sel_struct> _btns;

        dang::spSprite _sprLeftCandle{nullptr};
        dang::spSprite _sprRightCandle{nullptr};
    };
}
