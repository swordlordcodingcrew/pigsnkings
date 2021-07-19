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
#include "Moodies.h"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    using spTwSeq = std::shared_ptr<dang::TwSequence>;
    using spTwVel = std::shared_ptr<dang::TwVel>;

    extern PigsnKings _pnk;

    Moodies::Moodies()
    {

    }

    Moodies::Moodies(const dang::tmx_spriteobject* so, spImagesheet is) : dang::CollisionSprite(so, is)
    {

    }

    Moodies::Moodies(const Moodies& moodie) : CollisionSprite(moodie)
    {
        // let the lower classes do the actual copying
        std::cout << "Moodies copy constructor" << std::endl;

        if(moodie._anim_m_standard != nullptr)
        {
            _anim_m_standard = std::make_shared<dang::TwAnim>(*(moodie._anim_m_standard));
            _anim_m_standard->reset();
        }

        removeTweens(true);
        removeAnimation(true);
    }

    Moodies::~Moodies()
    {
        _anim_m_standard = nullptr;
        _animation = nullptr;
        _tweens.clear();

        std::cout << "Moodies destructor" << std::endl;
    }

    void Moodies::init()
    {
        _hotrect = {0, 0, 0, 0};

        if(_anim_m_standard != nullptr)
        {
            setAnimation(_anim_m_standard);
        }
    }

    void Moodies::update(uint32_t dt)
    {
        if(_remove_me)
        {
            removeSelf();
        }
    }

    void Moodies::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == dang::SpriteType::HOTRECT || mf.me->_type_num == dang::SpriteType::HOTRECT)
        {
            // me destroys
            //this->removeSelf();
        }
        else if (mf.other->_type_num == dang::SpriteType::KING || mf.me->_type_num == dang::SpriteType::KING)
        {
            // me destroys
            //_remove_me = true;
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse Moodies::getCollisionResponse(const spCollisionSprite& other)
    {
        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Moodies::removeSelf()
    {
         // remove throwie
         std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
         e->_spr = shared_from_this();
         pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}
