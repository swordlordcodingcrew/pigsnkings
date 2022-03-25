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
//        std::cout << "Moodies copy constructor" << std::endl;

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

#ifdef PNK_DEBUG_PRINT
        std::cout << "Moodies destructor" << std::endl;
#endif
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
            markRemove();
        }
    }

    uint8_t  Moodies::getCollisionResponse(const dang::spCollisionObject& other)
    {
        return dang::CR_NONE;
    }

}
