// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>
#include <vector>

namespace dang
{
    struct tmx_spriteobject;
    class Imagesheet;
    class SceneGraph;
    class Waypoint;
}

namespace pnk
{
    using spSprite = std::shared_ptr<dang::Sprite>;
    using spImagesheet = std::shared_ptr<dang::Imagesheet>;
    using spTweenable = std::shared_ptr<dang::Tweenable>;
    using spSceneGraph = std::shared_ptr<dang::SceneGraph>;
    using wpWaypoint = std::weak_ptr<dang::Waypoint>;
    using spWaypoint = std::shared_ptr<dang::Waypoint>;

    class Enemy : public dang::CollisionSprite
    {
    public:
        Enemy();
        Enemy(const dang::tmx_spriteobject* so, spImagesheet is);
        ~Enemy() override;

        virtual void    init();

        void update(uint32_t dt) override;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override;
        dang::CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other) override;

        virtual bool isBubbled() = 0;
        virtual void bubble() = 0;
        virtual void deBubble() = 0;

        virtual void handlePath();
        virtual void randomNextWaypoint();
        virtual void checkPathProgress();
        virtual void startOutToWaypoint();

        spSceneGraph            _scene_graph{nullptr};
        std::vector<wpWaypoint> _path;
        wpWaypoint              _current_wp;
        size_t                  _path_index{0};

    protected:
        bool _on_ground = false;
        float _walkSpeed{2.0};

    };

}
