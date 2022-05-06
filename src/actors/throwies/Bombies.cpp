// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include <iostream>
#include <cassert>

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwVel.hpp>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"
#include "Throwies.h"
#include "Bombies.h"
#include "src/actors/npc/Enemy.h"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    using spTwSeq = std::shared_ptr<dang::TwSequence>;
    using spTwVel = std::shared_ptr<dang::TwVel>;

    extern PigsnKings _pnk;

    Bombies::Bombies()
    {

    }

    Bombies::Bombies(const dang::tmx_spriteobject* so, spImagesheet is) : Throwies(so, is)
    {

    }

    Bombies::Bombies(const Bombies &bomb) : Throwies(bomb)
    {
#ifdef PNK_DEBUG_PRINT
        std::cout << "Bombies copy constructor" << std::endl;
#endif
        _to_the_left = bomb._to_the_left;
        _anim_flying = std::make_shared<dang::TwAnim>(*(bomb._anim_flying));
        _anim_flying->reset();

        _anim_on_fire = std::make_shared<dang::TwAnim>(*(bomb._anim_on_fire));
        _anim_on_fire->setFinishedCallback(std::bind(&Bombies::triggerExplosion, this));
        _anim_on_fire->reset();
    }

    Bombies::~Bombies()
    {
#ifdef PNK_DEBUG_PRINT
        std::cout << "Bombies destructor" << std::endl;
#endif
    }

    void Bombies::init()
    {
        this->Throwies::init();

        _hotrect = {11, 20, 10, 10};
    }


    void Bombies::collide(const dang::manifold &mf)
    {
        dang::spCollisionSprite sprOther = getOther(mf, this);
//        std::static_pointer_cast<CollisionSprite>(mf.other);
//        sprOther = std::static_pointer_cast<CollisionSprite>(sprOther.get() == this ? mf.me : mf.other);
        if (!_bIsOnFire && (sprOther->_type_num == ST_HOTRECT || sprOther->_type_num == ST_HOTRECT_PLATFORM))
        {
            // have the animation sequence triggered
            setBombOnFire();
        }
        else if (_bIsOnFire && (sprOther->_type_num == ST_KING))
        {
            // King hurt
            tellTheKingWeHitHim();
        }
    }

    uint8_t  Bombies::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::CollisionSprite* cs_other = dynamic_cast<const CollisionSprite*>(other);
//        dang::spCollisionSprite cs_other = std::static_pointer_cast<CollisionSprite>(other);
        if (cs_other->_type_num == ST_KING || cs_other->_type_num == ST_HOTRECT || cs_other->_type_num == ST_HOTRECT_PLATFORM)
        {
            return dang::CR_BOUNCE;
        }

        return dang::CR_NONE;
    }

    void Bombies::tellTheKingWeHitHim()
    {
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = ST_FLYING_BOMB;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Bombies::setBombOnFire()
    {
        // no reset of animation needed, will only be called once and reet is called after initialisation
        setAnimation(_anim_on_fire);
        _bIsOnFire = true;
    }

    void Bombies::triggerExplosion()
    {
        setAnimation(_anim_flying);
        _anim_on_fire = nullptr;
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_BOMB_EXPLODES));
        e->_spr = shared_from_this();
        e->_pos = this->getPos();
        e->_payload = ST_FLYING_BOMB;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));

        // me destroys in the next cycle, we need the pointer in this cycle for the event
        _remove_me = true;
    }
}
