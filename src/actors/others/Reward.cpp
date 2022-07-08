// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Reward.h"

#include "pigsnkings.hpp"
#include "PnkEvent.h"

#include <tween/TwAnim.hpp>
#include <Imagesheet.hpp>

#include <sfx/coin_22050_mono.h>

namespace pnk
{
    extern PigsnKings _pnk;

    Reward::Reward() : dang::FullColSpr()
    {
    }

    Reward::Reward(const dang::tmx_spriteobject* so, dang::spImagesheet is) : dang::FullColSpr(so, is)
    {
    }

    void Reward::init()
    {

        // todo: get it from the definition in the tmx file
        // the problem is, that if we set it in the sprite factory, we have an instance of an animation in every
        // single reward eating memory. if we set it if we need it, we only have a few...
        // lets discuss the problem
        switch (this->typeNum())
        {
            case ST_COIN_SILVER:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3}, 500, &dang::Ease::Linear, -1)));
                _hotrect = {11, 26, 12, 12};
                break;
            case ST_COIN_GOLD:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{4, 5, 6, 7}, 500, &dang::Ease::Linear, -1)));
                _hotrect = {11, 26, 12, 12};
                break;
            case ST_GEM_BLUE:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{12, 13, 14, 15}, 500, &dang::Ease::Linear, -1)));
                _hotrect = {10, 24, 12, 16};
                break;
            case ST_GEM_GREEN:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{16, 17, 18, 19}, 500, &dang::Ease::Linear, -1)));
                _hotrect = {10, 24, 12, 16};
                break;
            case ST_GEM_RED:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{20, 21, 22, 23}, 500, &dang::Ease::Linear, -1)));
                _hotrect = {10, 24, 12, 16};
                break;
            case ST_POTION_BLUE:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{28, 29, 30, 31, 32, 33, 34}, 500, &dang::Ease::Linear, -1)));
                _hotrect = {10, 24, 12, 16};
                break;
            case ST_POTION_RED:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{35, 36, 37, 38, 39, 40, 41}, 500, &dang::Ease::Linear, -1)));
                _hotrect = {10, 24, 12, 16};
                break;
            case ST_POTION_GREEN:
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{42, 43, 44, 45, 46, 47, 48}, 500, &dang::Ease::Linear, -1)));
                _hotrect = {10, 24, 12, 16};
                break;
                // Default gets silver coin
            default:
                _hotrect = {10, 24, 12, 16};
                setAnimation(std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{0, 1, 2, 3}, 500, &dang::Ease::Linear, -1)));
                break;
        }

        setVel({0,0});
    }

    Reward::~Reward()
    {
#ifdef PNK_DEBUG_PRINT
        std::cout << "reward destructor" << std::endl;
#endif
    }

    void Reward::update(uint32_t dt)
    {
    }

    uint8_t Reward::getCollisionResponse(const dang::CollisionObject* other) const
    {
        const dang::ColSpr* cs_other = dynamic_cast<const dang::ColSpr*>(other);

        if (cs_other->typeNum() == ST_KING && !_collected)
        {
            return dang::CR_CROSS;
        }

        return dang::CR_NONE;
    }

    void Reward::collide(const dang::manifold &mf)
    {
        dang::spColSpr sprOther = getOther(mf, this);

        if (sprOther->typeNum() == ST_KING)
        {
            this->_collected = true;

            removeTweens(true);

            // alter animation
            removeAnimation(true);

            dang::spTwAnim anim_poof;

            switch (this->typeNum())
            {
                case ST_COIN_SILVER:
                case ST_COIN_GOLD:
                    anim_poof = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{8, 9, 10, 11}, 400, &dang::Ease::Linear, 0));
                    break;
                case ST_GEM_BLUE:
                case ST_GEM_GREEN:
                case ST_GEM_RED:
                    anim_poof = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{24, 25, 26, 27}, 400, &dang::Ease::Linear, 0));
                    break;
                case ST_POTION_BLUE:
                case ST_POTION_RED:
                case ST_POTION_GREEN:
                    anim_poof = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{49, 50, 51, 52}, 400, &dang::Ease::Linear, 0));
                    break;
                    // Default gets silver coin
                default:
                    anim_poof = std::make_shared<dang::TwAnim>(dang::TwAnim(std::vector<uint16_t>{8, 9, 10, 11}, 400, &dang::Ease::Linear, 0));
                    break;
            }

            createRewardEvent(this->typeNum());

            anim_poof->setFinishedCallback(std::bind(&Reward::removeSelf, this));
            setAnimation(anim_poof);
        }
    }

    void Reward::createRewardEvent(uint8_t rewardType)
    {
        // remove reward
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REWARD_HIT));
        e->_payload = static_cast<uint16_t>(rewardType);
        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }

    void Reward::removeSelf()
    {
        markRemove();
        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_SPR_CONSUMED_BY_HERO, id()));
        pnk::_pnk._dispatcher.queueEvent(std::move(e));

        // remove reward
//        std::unique_ptr<PnkEvent> e(new PnkEvent(EF_GAME, ETG_REMOVE_SPRITE));
//        e->_spr = shared_from_this();
//        pnk::_pnk._dispatcher.queueEvent(std::move(e));
    }
}