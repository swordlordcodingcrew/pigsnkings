// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "PigCrate.h"
#include "src/pnk_globals.h"
#include "src/Enemy.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    extern PigsnKings _pnk;

    PigCrate::PigCrate() : pnk::HenchPig()
    {
    }

    PigCrate::PigCrate(const dang::tmx_spriteobject &so, spImagesheet is) : pnk::HenchPig(so, is)
    {
    }

    void PigCrate::init()
    {
        this->HenchPig::init();
    }

    PigCrate::~PigCrate()
    {
        std::cout << "PigCrate destructor" << std::endl;
    }

    void PigCrate::update(uint32_t dt)
    {
        this->HenchPig::update(dt);
    }

    dang::CollisionSpriteLayer::eCollisionResponse PigCrate::getCollisionResponse(spSprite other)
    {
        return this->HenchPig::getCollisionResponse(other);
    }

    void PigCrate::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        this->HenchPig::collide(mf);
    }
}