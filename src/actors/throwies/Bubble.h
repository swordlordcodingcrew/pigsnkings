// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <sprite/FullSpr.hpp>

namespace pnk
{
    class Enemy;

    class Bubble : public dang::FullSpr
    {
    public:
        Bubble();
        Bubble(const Bubble& bub);
        Bubble(const dang::tmx_spriteobject* so, dang::spImagesheet is);
        ~Bubble() override;
        void    init();

        void    update(uint32_t dt) override;
        void    collide(const dang::manifold &mf) override;
        uint8_t getCollisionResponse(const dang::CollisionObject* other) const override;
        void postSolve() override;

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
        bool _pos_changed_in_collide{false};
        void removeSelf();

    protected:
        friend class SpriteFactory;
        // animations depot
        dang::spTwAnim _anim_blow; //spTwAnim twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{41, 42, 43, 44, 45}, 600, &dang::Ease::OutQuad, 0);
        dang::spTwAnim _anim_bobble; //       twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{44, 46, 45, 44, 47}, 600, &dang::Ease::Linear, 3);
        dang::spTwAnim _anim_poof; //        twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{48, 49, 50}, 300, &dang::Ease::Linear, 1);
        dang::spTwAnim _anim_catched; //        twa = std::make_shared<dang::TwAnim>(std::vector<uint16_t>{45, 46, 44, 45, 47}, 600, &dang::Ease::Linear, 12);

    };


}

