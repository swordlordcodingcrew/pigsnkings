// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework



#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"
#include "Throwies.h"
#include "src/actors/npc/Enemy.h"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

#include <Imagesheet.hpp>
#include <tween/TwAnim.hpp>
#include <tween/TwVel.hpp>

#include <cassert>

namespace pnk
{
    extern PigsnKings _pnk;

    Throwies::Throwies() : FullSpr()
    {

    }

    Throwies::Throwies(const dang::tmx_spriteobject* so, dang::spImagesheet is) : dang::FullSpr(so, is)
    {

    }

    Throwies::Throwies(const Throwies &crate) : FullSpr(crate)
    {
        // let the lower classes do the actual copying
    }

    Throwies::~Throwies()
    {
        _anim_flying = nullptr;

#ifdef PNK_DEBUG_PRINT
        std::printf("Throwies destructor\n");
#endif
    }

    void Throwies::init()
    {
        _hotrect = {6, 9, 20, 20};

        if(_anim_flying != nullptr)
        {
            setAnimation(_anim_flying);
        }
    }

    void Throwies::update(uint32_t dt)
    {
        if (_remove_me)
        {
            markRemove();
        }
    }

    void Throwies::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_HOTRECT)
        {
            // me destroys
            markRemove();
        }
        else if (sprOther->typeNum() == ST_KING)
        {
            // King hurt
            tellTheKingWeHitHim();

            // me destroys
            _remove_me = true;
        }
    }

    uint8_t  Throwies::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = dynamic_cast<const dang::ColSpr*>(other);
        if (cs_other->typeNum() == ST_KING || cs_other->typeNum() == ST_HOTRECT)
        {
            return dang::CR_TOUCH;
        }

        return dang::CR_NONE;
    }

    void Throwies::tellTheKingWeHitHim()
    {
        // no we dont
        std::printf("Throwies wont tell the King anything!\n");
    }

}
