// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework
#pragma once

#include <list>

#include "Layer.hpp"
#include "32blit.hpp"

#include "GameState.h"


namespace pnk
{
    class SettingsLayer : public dang::Layer
    {
    public:
        SettingsLayer();
        ~SettingsLayer() override;

        void update(uint32_t dt, const dang::Gear &gear) override;

        void render(const dang::Gear &gear) override;

    protected:
        explicit SettingsLayer(Layer::E_TYPE type) : Layer(type){};

        void paintSlider(const dang::Gear& gear, uint8_t x, uint8_t y, float val);
        void paintGameslot(const dang::Gear& gear, uint8_t x, uint8_t y, uint8_t val);
        void paintBackground(const dang::Gear& gear);

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

        uint8_t _selectedPref{_pnk.TRACKS};

        std::shared_ptr<dang::Imagesheet> _is_castle;
        std::shared_ptr<dang::Imagesheet> _is_hud;
        std::shared_ptr<dang::Imagesheet> _is_king;

    };
}


