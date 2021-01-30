// (c) 2019-20 by SwordLord - the coding crew
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
        const std::string& getInfotext() override { return _infoText; }

        static uint32_t _last_time;
    protected:
        /**
         * info text to show on the screen (development only - should be removed later on)
         */
        const std::string _infoText = "DOWN and UP to choose.\n X-Btn ('c') to select";

        static void buff_callback(blit::AudioChannel &channel);


        enum e_selection
        {
            PLAY = 0,
            PREFS,
            ABOUT,
            _ESIZE
        };
        uint8_t _sel{PLAY};

        /**
         * rsrc for the home state
         */
        dang::tmx_level _lvl;

        /**
         * rsrc-names
         */
        const std::string tmx_bg_layer_name = "menu_bg";
        const std::string tmx_obj_layer_name = "menu_obj";
        const std::string img_name_spr = "menus_png";
        const std::string img_name_bg = "tiles_bg_png";

        /**
         * buttons & animations
         */
         struct sel_struct
         {
             spSprite btn;
             spTwAnim anim;
             int img_index;
         };
        std::vector<sel_struct> _btns;
    };

}
