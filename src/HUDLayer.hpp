// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <layer/BaseHUDLayer.hpp>
#include <32blit.hpp>
#include <DangFwdDecl.h>

namespace pnk
{

    class HUDLayer : public dang::BaseHUDLayer
    {
    public:
        HUDLayer();
        ~HUDLayer() override = default;
        void fillSprites(dang::Gear& gear);

        void changeCheatSprite();

        void activateBossHUD();
        void deactivateBossHUD();

    protected:
//        void updateInternal(uint32_t dt, const dang::Gear& gear);
//        void renderInternal(const dang::Gear& gear);
        void    render(const dang::Gear& gear) override;

//        foregroundColour = blit::Pen(43, 63, 61, 255);
//        foregroundColour = blit::Pen(63, 56, 81, 255);
//        backgroundColour = blit::Pen(152, 152, 152, 200);
//        healthColour = blit::Pen(78, 110, 197, 255);
        constexpr static blit::Pen backgroundColour{152, 152, 152, 200};
        constexpr static blit::Pen background2Colour{200, 200, 200, 200};
        constexpr static blit::Pen foregroundColour{40, 60, 28, 255};
        constexpr static blit::Pen heroHealthColour{78, 110, 197, 255};
        constexpr static blit::Pen bossHealthColour{255,0,0,255};

        const uint8_t shadowOffset{1};

        bool isCheating{false};
        bool isBossVisible{false};
        dang::spImgSpr l1, l2, l3, l4;
    };
}

