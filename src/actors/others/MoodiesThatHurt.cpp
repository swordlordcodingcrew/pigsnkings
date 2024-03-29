// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "MoodiesThatHurt.h"

#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "GSPlay.h"

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    MoodiesThatHurt::MoodiesThatHurt() : Moodies()
    {
        _cr = dang::CR_CROSS;
        _hotrect = {16, 16, 32, 32};
    }

    MoodiesThatHurt::MoodiesThatHurt(const dang::tmx_spriteobject* so, dang::spImagesheet is) : Moodies(so, is)
    {
        _cr = dang::CR_CROSS;
        _hotrect = {16, 16, 32, 32};
    }

    MoodiesThatHurt::MoodiesThatHurt(const Moodies& mth): Moodies(mth)
    {
    }

    MoodiesThatHurt::~MoodiesThatHurt()
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("MoodiesThatHurt destructor\n");
#endif
    }

    void MoodiesThatHurt::init()
    {
        this->Moodies::init();

    }

    void MoodiesThatHurt::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (!_has_hurt && (sprOther->typeNum() == ST_KING))
        {
            // only hurts once
            _has_hurt = true;
            _cr = dang::CR_NONE;
            // King hurt
            tellTheKingWeHitHim();

        }
    }

    uint8_t  MoodiesThatHurt::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const ColSpr*>(other);

        if (cs_other->typeNum() == ST_KING)
        {
            return _cr;
        }

        return dang::CR_NONE;
    }

    void MoodiesThatHurt::tellTheKingWeHitHim()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_payload = ST_EXPLOSION;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

}
