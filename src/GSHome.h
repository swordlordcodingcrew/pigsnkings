// (c) 2019-22 by SwordLord - the coding crew
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
        void initTweens();

        /**
         * rsrc for the home state
         */
        const dang::tmx_level* _tmx{nullptr};

        /**
         * rsrc-names
         */
        const std::string tmx_bg_layer_name = "main_bg";
        const std::string tmx_deco_layer_name = "main_mood";
        const std::string tmx_obj_layer_name = "main_obj";

        /**
         * buttons & animations
         */
         struct sel_struct
         {
             dang::spFullImgSpr btn;
             dang::spTwAnim anim;
             int img_index;
         };
        std::vector<sel_struct> _btns;

        dang::spImgSpr _sprLeftCandle{nullptr};
        dang::spImgSpr _sprRightCandle{nullptr};

        dang::spFullImgSpr _king{nullptr};
        dang::spFullImgSpr _pig{nullptr};

        // animations for npc
        dang::spTwAnim _animKing{nullptr};
        dang::spTwAnim _animPig{nullptr};

        dang::spTwPos _twpKingL{nullptr};
        dang::spTwPos _twpKingR{nullptr};
        dang::spTwPos _twpPigL{nullptr};
        dang::spTwPos _twpPigR{nullptr};

        const dang::Vector2F _endPosPigR{360, 192};
        const dang::Vector2F _endPosKingR{340, 192};
        const dang::Vector2F _endPosPigL{-50, 192};
        const dang::Vector2F _endPosKingL{-32, 192};

        const dang::Vector2F _startPosPigR{-32, 192};
        const dang::Vector2F _startPosKingR{-100, 192};
        const dang::Vector2F _startPosPigL{340, 192};
        const dang::Vector2F _startPosKingL{400, 192};
    };
}
