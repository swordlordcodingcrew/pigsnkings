// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <CollisionSprite.hpp>

namespace pnk
{
    struct tmx_spriteobject;
    class Sprite;
    using spSprite = std::shared_ptr<dang::Sprite>;

    class RoomTrigger : public dang::CollisionSprite
    {
    public:
        RoomTrigger();
        explicit RoomTrigger(const dang::tmx_spriteobject &so);
//        ~RoomTrigger() override;

        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;
    protected:
        uint32_t _room1{0};
        uint32_t _room2{0};
        dang::Vector2F _pos12{0,0};
        dang::Vector2F _pos21{0,0};
        bool    _is_pos12{true};
    };

}


