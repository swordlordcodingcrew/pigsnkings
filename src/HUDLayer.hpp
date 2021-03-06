// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <list>
#include <BaseHUDLayer.hpp>
#include <32blit.hpp>

#include "Layer.hpp"

namespace pnk
{
    using spSprite = std::shared_ptr<dang::Sprite>;

    class HUDLayer : public dang::BaseHUDLayer
    {
    public:
        HUDLayer();
        ~HUDLayer() override = default;

    protected:
        void updateInternal(uint32_t dt, const dang::Gear& gear) override;
        void renderInternal(const dang::Gear& gear) override;

        blit::Pen backgroundColour;
        blit::Pen foregroundColour;
        blit::Pen healthColour;

        const uint8_t shadowOffset{1};

    };
}

