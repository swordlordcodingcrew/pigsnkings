// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "PigBomb.h"
#include "src/pnk_globals.h"
#include "src/Enemy.h"
#include "src/PnkEvent.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{

    extern PigsnKings _pnk;

    PigBomb::PigBomb() : pnk::HenchPig()
    {
    }

    PigBomb::PigBomb(const dang::tmx_spriteobject &so, spImagesheet is) : pnk::HenchPig(so, is)
    {
    }

    void PigBomb::init()
    {
        this->HenchPig::init();
    }

    PigBomb::~PigBomb()
    {
        std::cout << "PigBomb destructor" << std::endl;
    }

    void PigBomb::update(uint32_t dt)
    {
        this->HenchPig::update(dt);
    }

    dang::CollisionSpriteLayer::eCollisionResponse PigBomb::getCollisionResponse(spSprite other)
    {
        return this->HenchPig::getCollisionResponse(other);
    }

    void PigBomb::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        this->HenchPig::collide(mf);
    }
}