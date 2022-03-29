// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "LevelTrigger.h"
#include "src/SpriteFactory.hpp"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/pigsnkings.hpp"

#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    LevelTrigger::LevelTrigger() : dang::CollisionSprite()
    {
        _level = 1;
    }

    LevelTrigger::LevelTrigger(const dang::tmx_spriteobject* so) : dang::CollisionSprite(so, nullptr)
    {
        // name of the sprite object from the tiled level will be used as the level number to switch to
        _level = std::stoul(so->name);
    }

    void LevelTrigger::activateTrigger()
    {
        _is_activated = true;
    }

    void LevelTrigger::collide(const dang::manifold &mf)
    {
        dang::spCollisionSprite sprOther = getOther(mf, this);
//        dang::spCollisionSprite sprOther = std::static_pointer_cast<CollisionSprite>(mf.me.get() == this ? mf.other : mf.me);

        if (sprOther->_type_num == ST_KING)
        {
                std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_CHANGE_LEVEL));
                e->_payload = _level;
                _pnk._dispatcher.queueEvent(std::move(e));
        }
    }

    uint8_t  LevelTrigger::getCollisionResponse(const dang::spCollisionObject& other)
    {
        dang::spCollisionSprite cs_other = std::static_pointer_cast<CollisionSprite>(other);

        if (_is_activated && cs_other->_type_num == ST_KING)
        {
            return dang::CR_CROSS;
        }
        return dang::CR_NONE;
    }
}
