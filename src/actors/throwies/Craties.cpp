// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <cassert>

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwVel.hpp>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"
#include "Throwies.h"
#include "Craties.h"
#include "src/actors/npc/Enemy.h"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    extern PigsnKings _pnk;

    Craties::Craties()
    {

    }

    Craties::Craties(const dang::tmx_spriteobject* so, spImagesheet is) : Throwies(so, is)
    {
        _cr = dang::CR_TOUCH;
    }

    Craties::Craties(const Craties &crate) : Throwies(crate)
    {
#ifdef PNK_DEBUG_PRINT
        std::cout << "craties copy constructor" << std::endl;
#endif
        _to_the_left = crate._to_the_left;
        _anim_flying = std::make_shared<dang::TwAnim>(*(crate._anim_flying));
        _anim_destruction = std::make_shared<dang::TwAnim>(*(crate._anim_destruction));

        removeTweens(true);
        removeAnimation(true);
    }

    Craties::~Craties()
    {
#ifdef PNK_DEBUG_PRINT
        std::cout << "Craties destructor" << std::endl;
#endif
    }

    void Craties::init()
    {
        this->Throwies::init();

        _hotrect = {6, 9, 20, 20};
    }


    void Craties::collide(const dang::manifold &mf)
    {
        dang::spCollisionSprite sprOther = getOther(mf, this);
//        dang::spCollisionSprite sprOther = std::static_pointer_cast<CollisionSprite>(mf.me.get() == this ? mf.other : mf.me);

        if (sprOther->_type_num == ST_HOTRECT || sprOther->_type_num == ST_HOTRECT_PLATFORM)
        {
            _cr = dang::CR_NONE;
            _gravity = {0,0};
            _vel = {0,0};
            removeAnimation();
            _anim_destruction->setFinishedCallback([=]()
            {
                _remove_me = true;
            });
            setAnimation(_anim_destruction);

            triggerExplosion(); // does only do sound, no explosion fx
        }
        else if (sprOther->_type_num == ST_KING)
        {
            // King hurt
            tellTheKingWeHitHim();

            _cr = dang::CR_NONE;
            _gravity = {0,0};
            _vel = {0,0};
            removeAnimation();
            _anim_destruction->setFinishedCallback([=]()
            {
                _remove_me = true;
            });
            setAnimation(_anim_destruction);

            // no sfx, the hero already plays one
        }
    }

    uint8_t  Craties::getCollisionResponse(const dang::spCollisionObject& other)
    {
        dang::spCollisionSprite cs_other = std::static_pointer_cast<CollisionSprite>(other);
        if (_cr != dang::CR_NONE && (cs_other->_type_num == ST_KING || cs_other->_type_num == ST_HOTRECT))
        {
            return dang::CR_TOUCH;
        }

        return dang::CR_NONE;
    }

    void Craties::tellTheKingWeHitHim()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = ST_FLYING_CRATE;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Craties::triggerExplosion()
    {
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_CRATE_EXPLODES));
        e->_spr = shared_from_this();
        e->_pos = this->getPos();
        e->_payload = ST_FLYING_CRATE;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}
