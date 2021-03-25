// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "RoomTrigger.h"
#include "SpriteFactory.hpp"
#include "PnkEvent.h"
#include "pnk_globals.h"
#include "pigsnkings.hpp"

#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    RoomTrigger::RoomTrigger() : dang::CollisionSprite()
    {
    }

    RoomTrigger::RoomTrigger(const dang::tmx_spriteobject &so) : dang::CollisionSprite(so, nullptr)
    {

        size_t pos{0};
        _room1 = std::stoul(so.name, &pos);
        _room2 = std::stoul(so.name.substr(pos+1, so.name.size()));

        _pos12 = _pos;
        _pos21 = _pos;
        _pos21.x -= 36;

/*        size_t pos = so.name.find('-');
        if (pos != std::string::npos)
        {
            std::string r1 = so.name.substr(0, pos);
            std::string r2 = so.name.substr(pos+1, so.name.size());

        }
*/    }


    void RoomTrigger::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.me->_type_num == SpriteFactory::TN_KING || mf.other->_type_num == SpriteFactory::TN_KING)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_CHANGE_ROOM));
            if (_is_pos12)
            {
                _is_pos12 = false;
                _pos = _pos21;
                e->_pos.x = _room2;
            }
            else
            {
                _is_pos12 = true;
                _pos = _pos12;
                e->_pos.x = _room1;

            }

            _pnk._dispatcher.queueEvent(std::move(e));

        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse RoomTrigger::getCollisionResponse(spSprite other)
    {
        if (other->_type_num == SpriteFactory::TN_KING)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }
        return dang::CollisionSpriteLayer::CR_NONE;
    }

}
