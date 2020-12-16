// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.h>
#include <Imagesheet.h>
#include <iostream>
#include <tween/TwSequence.h>
#include <tween/TwVel.h>

#include "TmxExtruder.h"
#include "pigsnkings.hpp"

#include "Bubble.h"
#include "Enemy.h"
#include "PnkEvent.h"
#include "pnk_globals.h"

namespace pnk
{
    using spTwSeq = std::shared_ptr<dang::TwSequence>;
    using spTwVel = std::shared_ptr<dang::TwVel>;

    extern PigsnKings _pnk;

    Bubble::Bubble()
    {

    }

    Bubble::Bubble(const dang::tmx_spriteobject &so, spImagesheet is) : dang::CollisionSprite(so, is)
    {

    }

    Bubble::~Bubble()
    {
        std::cout << "bubble destructor" << std::endl;
    }

    void Bubble::init()
    {
        setCOType(dang::CollisionSpriteLayer::COT_DYNAMIC);
        _type_num = TN_BUBBLE;

        _hotrect = {4, 4, 8, 8};

        spTwSeq tw_seq_anim = std::make_shared<dang::TwSequence>();
        spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{19, 20, 21, 22, 23}, 800, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), 0);
        tw_seq_anim->addTween(twa);
        twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{22, 23}, 500, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), 4);
        tw_seq_anim->addTween(twa);
        tw_seq_anim->setFinishedCallback(std::bind(&Bubble::removeSelf, this));

        setAnimation(tw_seq_anim);

        float velx = _to_the_left ? -BUBBLE_VEL : BUBBLE_VEL;
        spTwSeq tw_seq = std::make_shared<dang::TwSequence>();
        spTwVel twv1 = std::make_shared<dang::TwVel>(dang::Vector2F(velx, 0), dang::Vector2F(0, 0), 500, std::unique_ptr<dang::Ease>(new dang::EaseInQuad()));
        spTwVel twv2 = std::make_shared<dang::TwVel>(dang::Vector2F(0, 0), dang::Vector2F(0, -3), 100, std::unique_ptr<dang::Ease>(new dang::EaseLinear()));

        tw_seq->addTween(twv1);
        tw_seq->addTween(twv2);

        addTween(tw_seq);
    }

    void Bubble::update(uint32_t dt)
    {
        if (_enemy_catched)
        {
            std::shared_ptr<Enemy> en = _catched_en.lock();
            if (en)
            {
                en->setPos(_pos);
            }
        }
    }

    void Bubble::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == TN_ENEMY1 || mf.me->_type_num == TN_ENEMY1)
        {
            _catched_en = std::static_pointer_cast<Enemy>(mf.other->_type_num == TN_ENEMY1 ? mf.other : mf.me);
            std::shared_ptr<Enemy> en = _catched_en.lock();
            if (en)
            {
                _pos = (_pos + en->getPos()) / 2.0f;
                en->setPos(_pos);
            }
            _enemy_catched = true;

            removeTweens(true);
            removeAnimation(true);
            _vel = {0, -0.5};
            spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{22, 23}, 500, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), 12);
            twa->setFinishedCallback(std::bind(&Bubble::removeSelf, this));
            setAnimation(twa);
        }
        else if (mf.other->_type_num == TN_HERO || mf.me->_type_num == TN_HERO)
        {
            if (_enemy_catched)
            {
                // TODO: reward
            }
            removeSelf();
        }
        else
        {
            removeTweens(true);
            removeAnimation(true);
            _vel = {0, 0};
            spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{22, 23}, 500, std::unique_ptr<dang::Ease>(new dang::EaseLinear()), 4);
            twa->setFinishedCallback(std::bind(&Bubble::removeSelf, this));
            setAnimation(twa);
        }
    }

    dang::CollisionSpriteLayer::eCollisionResponse Bubble::getCollisionResponse(spSprite other)
    {
        return dang::CollisionSpriteLayer::CR_TOUCH;
//        return _enemy_catched ? dang::CollisionSpriteLayer::CR_NONE : dang::CollisionSpriteLayer::CR_TOUCH;
    }

    void Bubble::removeSelf()
    {
        // remove bubble
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
        e->_spr = shared_from_this();
        pnk::_pnk._dispatcher.queueEvent(std::move(e));

        // remove enemy if catched
        if (_enemy_catched)
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
            e->_spr = _catched_en;
            pnk::_pnk._dispatcher.queueEvent(std::move(e));
        }
    }


}
