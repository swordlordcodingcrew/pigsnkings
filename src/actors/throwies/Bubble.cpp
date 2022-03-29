// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <cassert>

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <tween/TwSequence.hpp>
#include <tween/TwVel.hpp>
#include <libs/DANG/src/snd/SndGear.hpp>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"
#include "Bubble.h"
#include "src/actors/npc/Enemy.h"
#include "src/PnkEvent.h"
#include "src/pnk_globals.h"
#include "src/GSPlay.h"
#include "sfx/bubble_pop_22050_mono.h"
#include "sfx/coin_22050_mono.h"
#include "src/SpriteFactory.hpp"

namespace pnk
{
    using spTwSeq = std::shared_ptr<dang::TwSequence>;
    using spTwVel = std::shared_ptr<dang::TwVel>;

    extern PigsnKings _pnk;

    Bubble::Bubble()
    {

    }

    Bubble::Bubble(const dang::tmx_spriteobject* so, spImagesheet is) : dang::CollisionSprite(so, is)
    {

    }

    Bubble::Bubble(const Bubble &bub) : CollisionSprite(bub)
    {
//        std::cout << "bubble copy constructor" << std::endl;

        _to_the_left = bub._to_the_left;
        _state = bs_hatch;
//        _state = bub._state;
        _anim_blow = std::make_shared<dang::TwAnim>(*(bub._anim_blow));
        _anim_bobble = std::make_shared<dang::TwAnim>(*(bub._anim_bobble));
        _anim_poof = std::make_shared<dang::TwAnim>(*(bub._anim_poof));
        _anim_catched = std::make_shared<dang::TwAnim>(*(bub._anim_catched));

        _catched_en.reset();
        _anim_blow->reset();
        _anim_bobble->reset();
        _anim_poof->reset();
        _anim_catched->reset();

        removeTweens(true);
        removeAnimation(true);
    }

    Bubble::~Bubble()
    {
        if(!_catched_en.expired())
        {
            _catched_en.reset();
        }
#ifdef PNK_DEBUG_PRINT
        std::cout << "bubble destructor" << std::endl;
#endif
    }

    void Bubble::init()
    {
        _hotrect = {10, 10, 12, 12};

        // animation sequence
        spTwSeq tw_seq_anim = std::make_shared<dang::TwSequence>();
        // add finished_callback to grow anim
        _anim_blow->setFinishedCallback([=] ()
             {
                if (_state == bs_growing)
                {
                    _state = bs_wobbling;
                }
             });
        tw_seq_anim->addTween(_anim_blow);

        // bubble bobbles
        _anim_bobble->setFinishedCallback([=] ()
             {
                 if (_state == bs_wobbling)
                 {
                     _state = bs_bursting;
                     dang::SndGear::playSfx(bubble_pop_22050_mono_wav, bubble_pop_22050_mono_wav_length, _pnk._prefs.volume_sfx);
                     _vel = {0,0};
                     removeTweens(true);
                 }
             });
        tw_seq_anim->addTween(_anim_bobble);

        // bubble poofs
        tw_seq_anim->addTween(_anim_poof);
        tw_seq_anim->setFinishedCallback(std::bind(&Bubble::removeSelf, this));
        setAnimation(tw_seq_anim);
        _state = bs_growing;

        // movement sequence
        float velx = _to_the_left ? -BUBBLE_VEL : BUBBLE_VEL;
        spTwSeq tw_seq = std::make_shared<dang::TwSequence>();
        spTwVel twv1 = std::make_shared<dang::TwVel>(dang::Vector2F(velx, 0), dang::Vector2F(0, 0), 400, &dang::Ease::InQuad);
        spTwVel twv2 = std::make_shared<dang::TwVel>(dang::Vector2F(0, 0), dang::Vector2F(0, BUBBLE_VEL_UP), 100, &dang::Ease::Linear);
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

    void Bubble::collide(const dang::manifold &mf)
    {
        dang::spCollisionSprite sprOther = getOther(mf, this);
//        dang::spCollisionSprite sprOther = std::static_pointer_cast<CollisionSprite>(mf.me.get() == this ? mf.other : mf.me);

        if ((sprOther->_type_num > ST_ENEMIES && sprOther->_type_num < ST_ENEMIES_END)
            && (_state == bs_growing || _state == bs_wobbling))
        {   // an enemy is catched

            // yeah, could be added to the check above, but if we add a few more exceptions, would it still be readable?
            // or do some whitelist instead of ranges...
            if (sprOther->_type_num == ST_CANNON || sprOther->_type_num == ST_PIG_CANNON || sprOther->_type_num == ST_PIG_BOSS)
            {
                return; // cannoneers, cannons and royals dont get bubbled
            }

            _catched_en = std::static_pointer_cast<Enemy>(sprOther);
//            _catched_en = std::static_pointer_cast<Enemy>(mf.me.get() == this ? mf.other : mf.me);
            std::shared_ptr<Enemy> en = _catched_en.lock();
            if (en)
            {
                _pos = en->getPos() - _delta_catch;
                en->bubble();
            }
            _state = bs_enemy_catched;

            // set vel
            removeTweens(true);
            _vel = {0, -0.1};

            // alter animation
            removeAnimation(true);
            spTwSeq tw_seq_anim = std::make_shared<dang::TwSequence>();
            _anim_catched->reset();
            _anim_catched->setFinishedCallback([=] ()
                 {
                     _state = bs_bursting;
                     dang::SndGear::playSfx(bubble_pop_22050_mono_wav, bubble_pop_22050_mono_wav_length, _pnk._prefs.volume_sfx);

                     std::shared_ptr<Enemy> en = std::static_pointer_cast<Enemy>(_catched_en.lock());
                     if (en)
                     { // enemy becomes free again
                         en->endBubble();
                         _catched_en.reset();
                     }
                 });
            tw_seq_anim->addTween(_anim_catched);
            _anim_poof->reset();
            _anim_poof->setFinishedCallback(nullptr);
            tw_seq_anim->addTween(_anim_poof);
            tw_seq_anim->setFinishedCallback(std::bind(&Bubble::removeSelf, this));
            setAnimation(tw_seq_anim);

        }
        else if (sprOther->_type_num == ST_KING)
        {
            const dang::Vector2F& normal = mf.me.get() == this ? mf.normalMe : mf.normalOther;

            if (normal.y >= 0 || _state == bs_enemy_catched)   // hero is not on top or bubble has an enemy catched
            {
                if (_state == bs_enemy_catched)
                {
                    dang::SndGear::playSfx(coin_22050_mono_wav, coin_22050_mono_wav_length, _pnk._prefs.volume_sfx);

                    // reward
                    std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REWARD_HIT));
                    e->_payload = ST_PIG_REWARD;
                    e->_spr = shared_from_this();
                    pnk::_pnk._dispatcher.queueEvent(std::move(e));

                    // remove enemy with a poof
                    std::shared_ptr<Enemy> en = _catched_en.lock();
                    if (en)
                    {
                        en->markRemove();

                        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_SPR_CONSUMED_BY_HERO, en->_id));
                        pnk::_pnk._dispatcher.queueEvent(std::move(e));

                        // remove enemy
//                        std::unique_ptr<PnkEvent> er(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
//                        er->_spr = _catched_en;
//                        pnk::_pnk._dispatcher.queueEvent(std::move(er));

                        // poof
                        std::unique_ptr<PnkEvent> ep(new PnkEvent(EF_GAME, ETG_NEW_POOF));
                        ep->_pos = en->getPos();
                        ep->_pos.y += 5;
                        pnk::_pnk._dispatcher.queueEvent(std::move(ep));
                    }

                    // wash hands
                    _catched_en.reset();

                }

                _vel = {0,0};
                removeTweens(true);
                _state = bs_bursting;
                dang::SndGear::playSfx(bubble_pop_22050_mono_wav, bubble_pop_22050_mono_wav_length, _pnk._prefs.volume_sfx);

                // alter animation
                removeAnimation(true);
                _anim_poof->reset();
                _anim_poof->setFinishedCallback(std::bind(&Bubble::removeSelf, this));
                setAnimation(_anim_poof);
            }

        }
    }


    uint8_t  Bubble::getCollisionResponse(const dang::spCollisionObject& other)
    {
        dang::spCollisionSprite cs_other = std::static_pointer_cast<CollisionSprite>(other);
        if (cs_other->_type_num == ST_KING)
        {
            if (_state == bs_enemy_catched)
            {
                return dang::CR_CROSS;
            }

            if (_state == bs_bursting)
            {
                return dang::CR_NONE;
            }

            return dang::CR_SLIDE;

        }

        if (_state == bs_bursting || _state == bs_wobbling)
        {
            return dang::CR_NONE;
        }

        if (cs_other->_type_num > ST_ENEMIES && cs_other->_type_num < ST_ENEMIES_END)
        {
            return _state == bs_enemy_catched ? dang::CR_NONE : dang::CR_CROSS;
        }

        if (cs_other->_type_num == ST_HOTRECT)
        {
            return dang::CR_TOUCH;
        }

        return dang::CR_NONE;
    }

    void Bubble::removeSelf()
    {
        std::cout << "Bubble: removeSelf" << std::endl;
        // remove enemy if catched
        dang::spCollisionSprite spr = _catched_en.lock();
        if (spr != nullptr)
        {
            spr->markRemove();

            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_SPR_CONSUMED_BY_HERO, spr->_id));
            pnk::_pnk._dispatcher.queueEvent(std::move(e));

/*            std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
            e->_spr = _catched_en;
            pnk::_pnk._dispatcher.queueEvent(std::move(e));
*/        }

         // remove bubble
         markRemove();
/*         std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
         e->_spr = shared_from_this();
         pnk::_pnk._dispatcher.queueEvent(std::move(e));
*/
    }



}
