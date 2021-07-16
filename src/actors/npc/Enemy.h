// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <CollisionSprite.hpp>
#include <vector>

#include <bt/BTNode.h>
#include <bt/NTreeState.h>

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

        // pure virtual enemy class
        void update(uint32_t dt) override = 0;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override = 0;
        dang::CollisionSpriteLayer::eCollisionResponse getCollisionResponse(spSprite other) override = 0;

        virtual bool isBubbled() = 0;
        virtual void bubble() = 0;
        virtual void deBubble() = 0;

        /**
         * Behaviour tree functions
         */
        dang::BTNodeStatus checkPathCompleted();
        dang::BTNodeStatus checkWaypointReached();
        virtual void startOutToWaypoint();

        /** these functions are used to set a destination wapoint */
        dang::BTNodeStatus setDestinationWaypointByDepot(uint32_t depot_type);
        dang::BTNodeStatus setRandNeighbourWaypoint();

        /** these functions are used if the sprite missed the dest waypoint and has to get back somehow to the path system */
        dang::BTNodeStatus findNearestWaypoint(bool only_horizontally);

        /** static hooks */
        static dang::BTNodeStatus BTcheckPathCompleted(std::shared_ptr<Sprite> s);
        static dang::BTNodeStatus BTsetRandNeighbourWaypoint(std::shared_ptr<Sprite> s);
        static dang::BTNodeStatus BTcheckWaypointReached(std::shared_ptr<Sprite> s);
        static dang::BTNodeStatus BTsetDestinationBombDepot(std::shared_ptr<Sprite> s);
        static dang::BTNodeStatus BTsetDestinationCrateDepot(std::shared_ptr<Sprite> s);
        static dang::BTNodeStatus BTfindNearestWaypoint(std::shared_ptr<Sprite> s);
        static dang::BTNodeStatus BTfindNearestWaypointH(std::shared_ptr<Sprite> s);

        /** new static bt hooks */
        static dang::BTNode::Status NTcheckPathCompleted(std::shared_ptr<Sprite> s);
        static dang::BTNode::Status NTsetRandNeighbourWaypoint(std::shared_ptr<Sprite> s);
        static dang::BTNode::Status NTfindNearestWaypointH(std::shared_ptr<Sprite> s);



        /** path params */
        spSceneGraph            _scene_graph{nullptr};
        std::vector<wpWaypoint> _path;
        wpWaypoint              _current_wp;
        size_t                  _path_index{0};

        // time in ms
        uint32_t                _max_time_to_wp{0};
        uint32_t                _time_elapsed_to_wp{0};

    protected:
        bool _on_ground = false;
        float _walkSpeed{2.0};

    };

}
