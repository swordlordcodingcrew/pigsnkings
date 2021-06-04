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
#include "Cannonball.h"
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

    Cannonball::Cannonball()
    {

    }

    Cannonball::Cannonball(const dang::tmx_spriteobject* so, spImagesheet is) : Throwies(so, is)
    {

    }

    Cannonball::Cannonball(const Throwies &can) : Throwies(can)
    {
#ifdef PNK_DEBUG
        std::cout << "Cannonball copy constructor" << std::endl;
#endif
        _to_the_left = can._to_the_left;
        _anim_flying = std::make_shared<dang::TwAnim>(*(can._anim_flying));

        _anim_flying->reset();

        removeTweens(true);
        removeAnimation(true);
    }

    Cannonball::~Cannonball()
    {
#ifdef PNK_DEBUG
        std::cout << "Cannonball destructor" << std::endl;
#endif
    }

    void Cannonball::init()
    {
        _hotrect = {10, 10, 8, 8};
    }

    void Cannonball::update(uint32_t dt)
    {
        // will remove this Cratie when not needed anymore
        this->Throwies::update(dt);

        // add special stuff here
    }

    void Cannonball::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == dang::SpriteType::HOTRECT || mf.me->_type_num == dang::SpriteType::HOTRECT)
        {
            // me destroys
            this->removeSelf();
        }
        else if (mf.other->_type_num == dang::SpriteType::KING || mf.me->_type_num == dang::SpriteType::KING)
        {
            // King hurt
            tellTheKingWeHitHim();

            // me destroys
            _remove_me = true;
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse Cannonball::getCollisionResponse(spSprite other)
    {
        if (other->_type_num == dang::SpriteType::KING || other->_type_num == dang::SpriteType::HOTRECT)
        {
            return dang::CollisionSpriteLayer::CR_TOUCH;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Cannonball::tellTheKingWeHitHim()
    {
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = static_cast<uint16_t>(dang::SpriteType::FLYING_CANNONBALL);
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}
