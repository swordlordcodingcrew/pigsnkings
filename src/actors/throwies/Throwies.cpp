// (c) 2019-20 by SwordLord - the coding crew
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

    Throwies::Throwies()
    {

    }

    Throwies::Throwies(const dang::tmx_spriteobject* so, spImagesheet is) : dang::CollisionSprite(so, is)
    {

    }

    Throwies::Throwies(const Throwies &crate) : CollisionSprite(crate)
    {
        // let the lower classes do the actual copying
    }

    Throwies::~Throwies()
    {
#ifdef PNK_DEBUG
        std::cout << "Throwies destructor" << std::endl;
#endif
    }

    void Throwies::init()
    {
        _hotrect = {6, 9, 20, 20};

        // movement sequence
        float velx = _to_the_left ? -GSPlay::CRATE_VEL : GSPlay::CRATE_VEL;
        spTwSeq tw_seq = std::make_shared<dang::TwSequence>();
        spTwVel twv1 = std::make_shared<dang::TwVel>(dang::Vector2F(velx, -6), _pnk._gravity, 600, &dang::Ease::InQuad, 1, false, 100);
        tw_seq->addTween(twv1);
        addTween(tw_seq);

        if(_anim_flying != nullptr)
        {
            setAnimation(_anim_flying);
        }
    }

    void Throwies::update(uint32_t dt)
    {
        if(_remove_me)
        {
            removeSelf();
        }
    }

    void Throwies::collide(const dang::CollisionSpriteLayer::manifold &mf)
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

    dang::CollisionSpriteLayer::eCollisionResponse Throwies::getCollisionResponse(spSprite other)
    {
        if (other->_type_num == dang::SpriteType::KING || other->_type_num == dang::SpriteType::HOTRECT)
        {
            return dang::CollisionSpriteLayer::CR_TOUCH;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Throwies::tellTheKingWeHitHim()
    {
        // no we dont
        std::cout << "Throwies wont tell the King anything!" << std::endl;
    }

    void Throwies::removeSelf()
    {
         // remove throwie
         std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
         e->_spr = shared_from_this();
         pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}
