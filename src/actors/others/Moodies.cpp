// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Moodies.h"
#include "TmxExtruder.hpp"
#include "pigsnkings.hpp"
#include "PnkEvent.h"
#include "pnk_globals.h"
#include "GSPlay.h"
#include "SpriteFactory.hpp"

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwVel.hpp>

#include <cassert>

namespace pnk
{
    extern PigsnKings _pnk;

    Moodies::Moodies() : dang::FullColSpr()
    {
        _cr = dang::CR_NONE;
    }

    Moodies::Moodies(const dang::tmx_spriteobject* so, dang::spImagesheet is) : dang::FullColSpr(so, is)
    {
        _cr = dang::CR_NONE;
    }

    Moodies::Moodies(const Moodies& moodie) : FullColSpr(moodie)
    {

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
        removeAnimation();
        removeTweens(true);

#ifdef PNK_DEBUG_COMMON
        DEBUG_PRINT("Moodies destructor\n");
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

}
