// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <cassert>

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwVel.hpp>

#include "TmxExtruder.hpp"
#include "pigsnkings.hpp"
#include "Bubble.h"
#include "Enemy.h"
#include "PnkEvent.h"
#include "pnk_globals.h"
#include "GSPlay.h"
#include "sfx/bubble_pop_22050_mono.h"
#include "sfx/coin_22050_mono.h"
#include "SpriteFactory.hpp"

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
        _hotrect = {10, 10, 12, 12};

        // animation sequence
        spTwSeq tw_seq_anim = std::make_shared<dang::TwSequence>();
        // bobble grows
        spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{41, 42, 43, 44, 45}, 600, dang::Ease::OutQuad, 0);
        twa->setFinishedCallback([=] ()
             {
                if (_state == bs_growing)
                {
                    _state = bs_wobbling;
                }
             });
        tw_seq_anim->addTween(twa);
        // bubble wobbles
        twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{44, 46, 45, 44, 47}, 600, dang::Ease::Linear, 3);
        twa->setFinishedCallback([=] ()
             {
                 if (_state == bs_wobbling)
                 {
                     _state = bs_bursting;
                     PigsnKings::playSfx(bubble_pop_22050_mono_wav, bubble_pop_22050_mono_wav_length);
                     _vel = {0,0};
                     removeTweens(true);
                 }
             });
        tw_seq_anim->addTween(twa);
        twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{48, 49, 50}, 300, dang::Ease::Linear, 1);
        tw_seq_anim->setFinishedCallback(std::bind(&Bubble::removeSelf, this));
        tw_seq_anim->addTween(twa);

        setAnimation(tw_seq_anim);
        _state = bs_growing;

        // movement sequence
        float velx = _to_the_left ? -GSPlay::BUBBLE_VEL : GSPlay::BUBBLE_VEL;
        spTwSeq tw_seq = std::make_shared<dang::TwSequence>();
        spTwVel twv1 = std::make_shared<dang::TwVel>(dang::Vector2F(velx, 0), dang::Vector2F(0, 0), 400, dang::Ease::InQuad);
        spTwVel twv2 = std::make_shared<dang::TwVel>(dang::Vector2F(0, 0), dang::Vector2F(0, -3), 100, dang::Ease::Linear);

        tw_seq->addTween(twv1);
        tw_seq->addTween(twv2);

        addTween(tw_seq);

    }

    void Bubble::update(uint32_t dt)
    {
        if (_state == bs_enemy_catched)
        {
            std::shared_ptr<Enemy> en = _catched_en.lock();
            if (en)
            {
                en->setPos(_pos + _delta_catch);
            }
        }
    }

    void Bubble::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if ((mf.other->_type_num == SpriteFactory::TN_NORMAL_PIG || mf.me->_type_num == SpriteFactory::TN_NORMAL_PIG)
            && (_state == bs_growing || _state == bs_wobbling))
        {   // an enemy is catched
            _catched_en = std::static_pointer_cast<Enemy>(mf.other->_type_num == SpriteFactory::TN_NORMAL_PIG ? mf.other : mf.me);
            std::shared_ptr<Enemy> en = _catched_en.lock();
            if (en)
            {
                _pos = en->getPos() - _delta_catch;
            }
            _state = bs_enemy_catched;

            // set vel
            removeTweens(true);
            _vel = {0, -0.5};

            // alter animation
            removeAnimation(true);
            spTwSeq tw_seq_anim = std::make_shared<dang::TwSequence>();
            spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{45, 46, 44, 45, 47}, 600, dang::Ease::Linear, 12);
            twa->setFinishedCallback([=] ()
                 {
                     _state = bs_bursting;
                     PigsnKings::playSfx(bubble_pop_22050_mono_wav, bubble_pop_22050_mono_wav_length);

                     std::shared_ptr<Enemy> en = std::static_pointer_cast<Enemy>(_catched_en.lock());
                     if (en)
                     { // enemy becomes free again
                         en->deBubble();
                         _catched_en.reset();

/*                         std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
                         e->_spr = _catched_en;
                         pnk::_pnk._dispatcher.queueEvent(std::move(e));
                         _catched_en.reset();
*/
                     }
                 });
            tw_seq_anim->addTween(twa);
            twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{48, 49, 50}, 300, dang::Ease::Linear, 1);
            tw_seq_anim->addTween(twa);
            tw_seq_anim->setFinishedCallback(std::bind(&Bubble::removeSelf, this));
            setAnimation(tw_seq_anim);

        }
        else if (mf.other->_type_num == SpriteFactory::TN_KING || mf.me->_type_num == SpriteFactory::TN_KING)
        {
            const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.y >= 0 || _state == bs_enemy_catched)   // hero is not on top or bubble has an enemy catched
            {
                if (_state == bs_enemy_catched)
                {
                    PigsnKings::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length);
                    // TODO: reward
                }

                removeTweens(true);

                if (!_catched_en.expired())
                {
                    std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
                    e->_spr = _catched_en;
                    pnk::_pnk._dispatcher.queueEvent(std::move(e));
                    _catched_en.reset();
                }

                _vel = {0,0};
                removeTweens(true);
                _state = bs_bursting;
                PigsnKings::playSfx(bubble_pop_22050_mono_wav, bubble_pop_22050_mono_wav_length);

                // alter animation
                removeAnimation(true);
                spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{48, 49, 50}, 300, dang::Ease::Linear, 1);
                twa->setFinishedCallback([=] ()
                     {
                         removeSelf();
                     });
                setAnimation(twa);

            }

        }
/*        else if (mf.other->_type_num == TN_HOTRECT || mf.me->_type_num == TN_HOTRECT)
        {
        }
*/    }


    dang::CollisionSpriteLayer::eCollisionResponse Bubble::getCollisionResponse(spSprite other)
    {
        if (_state == bs_bursting)
        {
            return dang::CollisionSpriteLayer::CR_NONE;
        }

        if (other->_type_num == SpriteFactory::TN_KING)
        {
            if (_state == bs_enemy_catched)
            {
                return dang::CollisionSpriteLayer::CR_CROSS;
            }

            return dang::CollisionSpriteLayer::CR_SLIDE;

        }

        if (other->_type_num == SpriteFactory::TN_NORMAL_PIG)
        {
            return _state == bs_enemy_catched ? dang::CollisionSpriteLayer::CR_NONE : dang::CollisionSpriteLayer::CR_CROSS;
        }

        if (other->_type_num == SpriteFactory::TN_HOTRECT)
        {
            return dang::CollisionSpriteLayer::CR_TOUCH;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Bubble::removeSelf()
    {
        // remove enemy if catched
        if (!_catched_en.expired())
        {
            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
            e->_spr = _catched_en;
            pnk::_pnk._dispatcher.queueEvent(std::move(e));
        }

         // remove bubble
         std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
         e->_spr = shared_from_this();
         pnk::_pnk._dispatcher.queueEvent(std::move(e));

    }


}
