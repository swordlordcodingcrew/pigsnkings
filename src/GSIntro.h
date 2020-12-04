// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>
#include "GameState.h"

namespace pnk
{

    class GSIntro : public GameState
    {
    public:
        std::shared_ptr<GameState> update(dang::Gear& gear, uint32_t time) override;
        void enter(dang::Gear& gear, uint32_t time) override;
        void exit(dang::Gear& gear, uint32_t time) override;
        const std::string& getInfotext() override { return _infoText; }

    protected:
        uint32_t _last_time{0};
        const std::string _infoText = "";
    };

}
