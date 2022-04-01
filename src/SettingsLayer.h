// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "pigsnkings.hpp"

#include <32blit.hpp>

#include <DangFwdDecl.h>
#include <Layer.hpp>

#include <list>

namespace pnk
{
    class SettingsLayer : public dang::Layer
    {
    public:
        SettingsLayer();
        ~SettingsLayer() override;

        void update(uint32_t dt, const dang::Gear &gear) override;

        void render(const dang::Gear &gear) override;

        uint8_t getSelectedPrefs() const { return _selectedPref; }
        void refreshTempGamestateFromSave(const uint8_t slot);

    private:
        explicit SettingsLayer(Layer::E_TYPE type) : Layer(type){};

        void paintSlider(const dang::Gear& gear, uint8_t x, uint8_t y, float val);
        void paintGameslot(const dang::Gear& gear, uint8_t x, uint8_t y, uint8_t val);
        void paintStats(const dang::Gear& gear, uint8_t x, uint8_t y);


        blit::Pen backgroundColour;
        blit::Pen foregroundColour;
        blit::Pen highlightColour;

        blit::Rect displayRect;

        /**
         * buttons & animations
         */
        enum e_prefsType
        {
            STEP_10,
            STEP_3,
            GAMESLOT,
            YESNO
        };

        struct prefs_struct
        {
            std::string caption;
            float curVal;
            e_prefsType type;
        };

        std::vector<prefs_struct> _prefs;

//        uint8_t _selectedPref{_pnk.TRACKS};
        uint8_t _selectedPref{PigsnKings::TRACKS};

        dang::spImagesheet _is_castle;
        dang::spImagesheet _is_hud;
        dang::spImagesheet _is_king;

        PigsnKings::gamestate _temp_gamestate; // state of selected game (so that we can show it)

    };
}


