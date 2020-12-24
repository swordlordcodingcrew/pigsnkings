// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include <engine/input.hpp>
#include <iostream>

#include "pnk_globals.h"
#include "ActionState.h"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "Hero.h"
#include <Event.h>

namespace pnk
{
    extern PigsnKings _pnk;

    /************************************************************************
     * class ActionState base class
     */

    std::shared_ptr<BubbleState> ActionState::_bubble = std::make_shared<BubbleState>();
    std::shared_ptr<NoActionState> ActionState::_no_action = std::make_shared<NoActionState>();

    void BubbleState::enter(Hero &hero, uint32_t dt)
    {
        _bubbling = true;
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_NEW_BUBBLE));
        e->_to_the_left = (hero._transform == blit::SpriteTransform::HORIZONTAL);
        e->_pos = hero.getPos();
        e->_pos.x = e->_pos.x + (e->_to_the_left ? -hero.getSize().x/2 : hero.getSize().x/2);
        e->_pos.y += 6;
        _pnk._dispatcher.queueEvent(std::move(e));
    }

    std::shared_ptr<ActionState> BubbleState::update(Hero &hero, uint32_t dt)
    {
        if (_bubbling)
        {
            return ActionState::_bubble;
        }
        return ActionState::_no_action;
    }

    void NoActionState::enter(Hero &hero, uint32_t dt)
    {

    }

    std::shared_ptr<ActionState> NoActionState::update(Hero &hero, uint32_t dt)
    {
        // SDLK_x
        if (pressed(blit::Button::B))
        {
            return ActionState::_bubble;
        }
        return ActionState::_no_action;
    }
}