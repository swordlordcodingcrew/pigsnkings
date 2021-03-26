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
#include "src/Enemy.h"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    using spTwSeq = std::shared_ptr<dang::TwSequence>;
    using spTwVel = std::shared_ptr<dang::TwVel>;

    extern PigsnKings _pnk;

    Craties::Craties()
    {

    }

    Craties::Craties(const dang::tmx_spriteobject &so, spImagesheet is) : Throwies(so, is)
    {

    }

    Craties::Craties(const Throwies &crate) : Throwies(crate)
    {
        std::cout << "craties copy constructor" << std::endl;

        _to_the_left = crate._to_the_left;
        _anim_flying = std::make_shared<dang::TwAnim>(*(crate._anim_flying));

        removeTweens(true);
        removeAnimation(true);
    }

    Craties::~Craties()
    {
        std::cout << "Craties destructor" << std::endl;
    }

    void Craties::init()
    {
        this->Throwies::init();

        _hotrect = {6, 9, 20, 20};
    }

    void Craties::update(uint32_t dt)
    {
        // will remove this Cratie when not needed anymore
        this->Throwies::update(dt);

        // add special stuff here
    }

    void Craties::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == SpriteFactory::TN_HOTRECT || mf.me->_type_num == SpriteFactory::TN_HOTRECT)
        {
            // me destroys
            this->removeSelf();
        }
        else if (mf.other->_type_num == SpriteFactory::TN_KING || mf.me->_type_num == SpriteFactory::TN_KING)
        {
            // King hurt
            tellTheKingWeHitHim();

            // me destroys
            _remove_me = true;
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse Craties::getCollisionResponse(spSprite other)
    {
        if (other->_type_num == SpriteFactory::TN_KING || other->_type_num == SpriteFactory::TN_HOTRECT)
        {
            return dang::CollisionSpriteLayer::CR_TOUCH;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Craties::tellTheKingWeHitHim()
    {
        //
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_KING_HIT));
        e->_spr = shared_from_this();
        e->_payload = SpriteFactory::TN_FLYING_CRATE;
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}
