// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <Vector2T.hpp>
#include <Event.hpp>
#include <memory>
#include "GameState.h"

namespace dang
{
    class Sprite;
}

namespace pnk
{
    class PnkEvent : public dang::Event
    {
    public:
        /**
         * Creates (and moves) a PnkEvent with filter EF_GAME
         * @param type of event (ETG)
         * @param payload paylod as uint16
         * @return unique_ptr of PnkEvent
         */
        static std::unique_ptr<PnkEvent> createGE(int32_t type, uint16_t payload);

        PnkEvent(uint16_t filter, int32_t type);
        PnkEvent(uint16_t filter, int32_t type, uint16_t payload);
        ~PnkEvent() override = default;
        dang::Vector2F _pos{0,0};
        bool            _to_the_left{false};
        uint16_t        _payload{0}; // contains event specific details, optional
    };

}
