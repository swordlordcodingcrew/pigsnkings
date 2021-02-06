// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>
#include "GameState.h"

namespace pnk
{

    class GSAbout : public GameState
    {
    public:
        std::shared_ptr<GameState> update(dang::Gear& gear, uint32_t time) override;
        void enter(dang::Gear& gear, uint32_t time) override;
        void exit(dang::Gear& gear, uint32_t time) override;
        const std::string& getInfotext() override { return _infoText; }

    protected:
        const std::string _infoText = "";

    };

}
