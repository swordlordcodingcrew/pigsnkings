// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>
#include <iostream>
#include <sfx/coin_22050_mono.h>

#include "TmxExtruder.hpp"
#include "src/pigsnkings.hpp"

#include "Reward.h"
#include "src/GSPlay.h"
#include "src/SpriteFactory.hpp"
#include "src/PnkEvent.h"

namespace pnk
{
    extern PigsnKings _pnk;

    Reward::Reward() : dang::CollisionSprite()
    {
    }

    Reward::Reward(const dang::tmx_spriteobject* so, spImagesheet is) : dang::CollisionSprite(so, is)
    {
    }

    void Reward::init()
    {
        _hotrect = {10, 24, 12, 16};

        // todo: get it from the definition in the tmx file
        // the problem is, that if we set it in the sprite factory, we have an instance of an animation in every
        // single reward eating memory. if we set it if we need it, we only have a few...
        // lets discuss the problem
        switch (this->_type_num)
        {
            case dang::SpriteType::COIN_SILVER:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3}, 500, &dang::Ease::Linear, -1)));
                break;
            case dang::SpriteType::COIN_GOLD:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{4, 5, 6, 7}, 500, &dang::Ease::Linear, -1)));
                break;
            case dang::SpriteType::GEM_BLUE:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{12, 13, 14, 15}, 500, &dang::Ease::Linear, -1)));
                break;
            case dang::SpriteType::GEM_GREEN:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{16, 17, 18, 19}, 500, &dang::Ease::Linear, -1)));
                break;
            case dang::SpriteType::GEM_RED:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{20, 21, 22, 23}, 500, &dang::Ease::Linear, -1)));
                break;
            case dang::SpriteType::POTION_BLUE:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{28, 29, 30, 31, 32, 33, 34}, 500, &dang::Ease::Linear, -1)));
                break;
            case dang::SpriteType::POTION_RED:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{35, 36, 37, 38, 39, 40, 41}, 500, &dang::Ease::Linear, -1)));
                break;
            case dang::SpriteType::POTION_GREEN:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{42, 43, 44, 45, 46, 47, 48}, 500, &dang::Ease::Linear, -1)));
                break;
                // Default gets silver coin
            default:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3}, 500, &dang::Ease::Linear, -1)));
                break;
        }

        setVel({0,0});
    }

    Reward::~Reward()
    {
        //std::cout << "reward destructor" << std::endl;
    }

    void Reward::update(uint32_t dt)
    {
    }

    dang::CollisionSpriteLayer::eCollisionResponse Reward::getCollisionResponse(const spCollisionSprite& other)
    {
        if (other->_type_num == dang::SpriteType::KING && !_collected)
        {
            return dang::CollisionSpriteLayer::CR_CROSS;
        }

        return dang::CollisionSpriteLayer::CR_NONE;
    }

    void Reward::collide(const dang::CollisionSpriteLayer::manifold &mf)
    {
        if (mf.other->_type_num == dang::SpriteType::KING || mf.me->_type_num == dang::SpriteType::KING)
        {
            this->_collected = true;

            removeTweens(true);

            // alter animation
            removeAnimation(true);

            spTwAnim anim_poof;

            switch (this->_type_num)
            {
                case dang::SpriteType::COIN_SILVER:
                case dang::SpriteType::COIN_GOLD:
                    anim_poof = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{8, 9, 10, 11}, 400, &dang::Ease::Linear, 0));
                    break;
                case dang::SpriteType::GEM_BLUE:
                case dang::SpriteType::GEM_GREEN:
                case dang::SpriteType::GEM_RED:
                    anim_poof = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{24, 25, 26, 27}, 400, &dang::Ease::Linear, 0));
                    break;
                case dang::SpriteType::POTION_BLUE:
                case dang::SpriteType::POTION_RED:
                case dang::SpriteType::POTION_GREEN:
                    anim_poof = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{49, 50, 51, 52}, 400, &dang::Ease::Linear, 0));
                    break;
                    // Default gets silver coin
                default:
                    anim_poof = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{8, 9, 10, 11}, 400, &dang::Ease::Linear, 0));
                    break;
            }

            createRewardEvent(this->_type_num);

            anim_poof->setFinishedCallback(std::bind(&Reward::removeSelf, this));
            setAnimation(anim_poof);
        }
    }

    void Reward::createRewardEvent(dang::SpriteType rewardType)
    {
        // remove reward
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REWARD_HIT));
        e->_spr = shared_from_this();
        e->_payload = static_cast<uint16_t>(rewardType);
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Reward::removeSelf()
    {
        // remove reward
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
        e->_spr = shared_from_this();
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}