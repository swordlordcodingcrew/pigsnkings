// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>

namespace pnk
{
    struct tmx_spriteobject;
    class Imagesheet;
    class Enemy;

    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spTwAnim = std::shared_ptr<dang::TwAnim>;

    class Bubble : public dang::CollisionSprite
    {
    public:
        Bubble();
        Bubble(const Bubble& bub);
        Bubble(const dang::tmx_spriteobject &so, spImagesheet is);
        ~Bubble() override;
        void    init();
        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;


        enum bubble_state
        {
            bs_hatch,
            bs_growing,
            bs_wobbling,
            bs_enemy_catched,
            bs_bursting
        };

        bool    _to_the_left{true};
        std::weak_ptr<Enemy> _catched_en;
        bubble_state _state{bs_hatch};

    protected:
        const dang::Vector2F _delta_catch{-1, -8};
        void removeSelf();

    protected:
        friend class SpriteFactory;
        // animations depot
        spTwAnim _anim_blow; //spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{41, 42, 43, 44, 45}, 600, &dang::Ease::OutQuad, 0);
        spTwAnim _anim_bobble; //       twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{44, 46, 45, 44, 47}, 600, &dang::Ease::Linear, 3);
        spTwAnim _anim_poof; //        twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{48, 49, 50}, 300, &dang::Ease::Linear, 1);
        spTwAnim _anim_catched; //        twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{45, 46, 44, 45, 47}, 600, &dang::Ease::Linear, 12);

    };


}

