// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <iostream>

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwVel.hpp>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"
#include "MoodiesThatHurt.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    extern PigsnKings _pnk;

    MoodiesThatHurt::MoodiesThatHurt()
    {

    }

    MoodiesThatHurt::MoodiesThatHurt(const dang::tmx_spriteobject* so, spImagesheet is): Moodies(so, is)
    {

    }

    MoodiesThatHurt::MoodiesThatHurt(const Moodies& mth): Moodies(mth)
    {
    }

    MoodiesThatHurt::~MoodiesThatHurt()
    {
#ifdef PNK_DEBUG
        std::cout << "MoodiesThatHurt destructor" << std::endl;
#endif
    }

    void MoodiesThatHurt::init()
    {
        this->Moodies::init();

        _hotrect = {16, 16, 32, 32};
    }

    void MoodiesThatHurt::collide(const dang::manifold &mf)
    {
        dang::spCollisionSprite sprOther = std::static_pointer_cast<CollisionSprite>(mf.me.get() == this ? mf.other : mf.me);
        if (!_has_hurt && (sprOther->_type_num == ST_KING))
        {
            // only hurts once
            _has_hurt = true;
//            _hotrect = {0, 0, 0, 0};
            // King hurt
            tellTheKingWeHitHim();

        }
    }

    uint8_t  MoodiesThatHurt::getCollisionResponse(const dang::spCollisionObject& other)
    {
        dang::spCollisionSprite cs_other = std::static_pointer_cast<CollisionSprite>(other);

        if (cs_other->_type_num == ST_KING && !_has_hurt)
        {
            return dang::CR_CROSS;
        }

        return dang::CR_NONE;
    }

    void MoodiesThatHurt::tellTheKingWeHitHim()
    {
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = ST_EXPLOSION;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

}
