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
#ifdef PNK_DEBUG
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
#ifdef PNK_DEBUG
        std::cout << "Bombies destructor" << std::endl;
#endif
    }

    void Bombies::init()
    {
        this->Throwies::init();

        _hotrect = {6, 9, 20, 20};
    }

    void Bombies::update(uint32_t dt)
    {
        // will remove this Cratie when not needed anymore
        this->Throwies::update(dt);

        // add special stuff here
    }

    void Bombies::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (!_bIsOnFire && (mf.other->_type_num == dang::SpriteType::HOTRECT || mf.me->_type_num == dang::SpriteType::HOTRECT))
        {
            // have the animation sequence triggered
            setBombOnFire();
        }
        else if (_bIsOnFire && (mf.other->_type_num == dang::SpriteType::KING || mf.me->_type_num == dang::SpriteType::KING))
        {
            // King hurt
            tellTheKingWeHitHim();
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse Bombies::getCollisionResponse(const spCollisionSprite& other)
    {
        if (other->_type_num == dang::SpriteType::KING || other->_type_num == dang::SpriteType::HOTRECT)
        {
            return dang::CollisionSpriteLayer::CR_BOUNCE;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Bombies::tellTheKingWeHitHim()
    {
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = static_cast<uint16_t>(dang::SpriteType::FLYING_BOMB);
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
        e->_payload = static_cast<uint16_t>(dang::SpriteType::FLYING_BOMB);
        pnk::_pnk._dispatcher.queueEvent(std::move(e));

        // me destroys in the next cycle, we need the pointer in this cycle for the event
        _remove_me = true;
    }
}
