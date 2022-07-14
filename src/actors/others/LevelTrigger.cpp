// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "LevelTrigger.h"
//#include "SpriteFactory.hpp"
#include "PnkEvent.h"
#include "pnk_globals.h"
#include "pigsnkings.hpp"

#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    LevelTrigger::LevelTrigger() : dang::ColSpr()
    {
        _cr = dang::CR_NONE;
        _level = 1;
    }

    LevelTrigger::LevelTrigger(const dang::tmx_spriteobject* so) : dang::ColSpr(so)
    {
        // name of the sprite object from the tiled level will be used as the level number to switch to
        _level = std::stoul(so->name);
        _cr = dang::CR_NONE;
    }

    void LevelTrigger::activateTrigger()
    {
        _cr = dang::CR_CROSS;
        _is_activated = true;
    }

    void LevelTrigger::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_KING)
        {
                std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_CHANGE_LEVEL));
                e->_payload = _level;
                _pnk._dispatcher.queueEvent(std::move(e));
                _cr = dang::CR_NONE;
        }
    }

    uint8_t  LevelTrigger::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const ColSpr*>(other);

        if (cs_other->typeNum() == ST_KING)
        {
            return _cr;
        }

        return dang::CR_NONE;
    }
}
