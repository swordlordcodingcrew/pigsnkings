// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework


#include "Craties.h"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "pnk_globals.h"

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>

#include <cassert>

namespace pnk
{
    extern PigsnKings _pnk;

    Craties::Craties() : Throwies()
    {
        _cr = dang::CR_TOUCH;
        _hotrect = {6, 9, 20, 20};
    }

    Craties::Craties(const dang::tmx_spriteobject* so, dang::spImagesheet is) : Throwies(so, is)
    {
        _cr = dang::CR_TOUCH;
        _hotrect = {6, 9, 20, 20};
    }

    Craties::Craties(const Craties &crate) : Throwies(crate)
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("Craties copy constructor\n");
#endif
        _to_the_left = crate._to_the_left;
        _anim_flying = std::make_shared<dang::TwAnim>(*(crate._anim_flying));
        _anim_destruction = std::make_shared<dang::TwAnim>(*(crate._anim_destruction));

        removeTweens(true);
        removeAnimation(true);
    }

    Craties::~Craties()
    {
#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("Craties destructor\n");
#endif
    }

/*    void Craties::init()
    {
        this->Throwies::init();

    }
*/

    void Craties::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_HOTRECT || sprOther->typeNum() == ST_HOTRECT_PLATFORM)
        {
            _cr = dang::CR_NONE;
            setGravity({0,0});
            setVel(0,0);
            removeAnimation();
            _anim_destruction->setFinishedCallback([=]()
            {
                _remove_me = true;
            });
            setAnimation(_anim_destruction);

            triggerExplosion(); // does only do sound, no explosion fx
        }
        else if (sprOther->typeNum() == ST_KING)
        {
            // King hurt
            tellTheKingWeHitHim();

            _cr = dang::CR_NONE;
            setGravity({0,0});
            setVel(0,0);
            removeAnimation();
            _anim_destruction->setFinishedCallback([=]()
            {
                _remove_me = true;
            });
            setAnimation(_anim_destruction);

            // no sfx, the hero already plays one
        }
    }

    uint8_t  Craties::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = static_cast<const ColSpr*>(other);
        if (_cr != dang::CR_NONE && (cs_other->typeNum() == ST_KING || cs_other->typeNum() == ST_HOTRECT))
        {
            return dang::CR_TOUCH;
        }

        return dang::CR_NONE;
    }

    void Craties::tellTheKingWeHitHim()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_payload = ST_FLYING_CRATE;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Craties::triggerExplosion()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_CRATE_EXPLODES));
        e->_pos = this->getPos();
        e->_payload = ST_FLYING_CRATE;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}
