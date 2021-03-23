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

    Throwies::Throwies()
    {

    }

    Throwies::Throwies(const dang::tmx_spriteobject &so, spImagesheet is) : dang::CollisionSprite(so, is)
    {

    }

    Throwies::Throwies(const Throwies &crate) : CollisionSprite(crate)
    {
        std::cout << "throwies copy constructor" << std::endl;

        _to_the_left = crate._to_the_left;
        _anim_flying = std::make_shared<dang::TwAnim>(*(crate._anim_flying));

        removeTweens(true);
        removeAnimation(true);
    }

    Throwies::~Throwies()
    {
        std::cout << "Throwies destructor" << std::endl;
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
    }

    void Throwies::update(uint32_t dt)
    {
        //
    }

    void Throwies::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == SpriteFactory::TN_HOTRECT || mf.me->_type_num == SpriteFactory::TN_HOTRECT)
        {
            // me destroys
            this->removeSelf();
        }
        else if (mf.other->_type_num == SpriteFactory::TN_KING || mf.me->_type_num == SpriteFactory::TN_KING)
        {
            // King hurts
            _pnk.removeHealth(10);

            // me destroys
            // TODO probably an error in the remove, game crashes when hit here...
            this->removeSelf();
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse Throwies::getCollisionResponse(spSprite other)
    {
        if (other->_type_num == SpriteFactory::TN_KING || other->_type_num == SpriteFactory::TN_HOTRECT)
        {
            return dang::CollisionSpriteLayer::CR_TOUCH;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Throwies::removeSelf()
    {
         // remove throwie
         std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
         e->_spr = shared_from_this();
         pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}
