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
    using spTwSeq = std::shared_ptr<dang::TwSequence>;
    using spTwVel = std::shared_ptr<dang::TwVel>;

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

    void MoodiesThatHurt::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (!_has_hurt && (mf.other->_type_num == ST_KING || mf.me->_type_num == ST_KING))
        {
            // only hurts once
            _has_hurt = true;
//            _hotrect = {0, 0, 0, 0};
            // King hurt
            tellTheKingWeHitHim();

        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse MoodiesThatHurt::getCollisionResponse(const spCollisionSprite& other)
    {
        if (other->_type_num == ST_KING && !_has_hurt)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
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
