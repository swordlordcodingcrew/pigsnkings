// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <vector>

#include <DangFwdDecl.h>

#include <CollisionSprite.hpp>
#include <bt/BTNode.h>

namespace pnk
{
    enum e_state
            {
        SLEEPING = 0,
        HIDING,
        LOITERING,
        BERSERK,
        THROWING,
        FIRING,
        PICKING_UP,
        BUBBLED,
        REMOVE_SELF,
        DEAD
            };

    class Enemy : public dang::CollisionSprite
    {
    public:
        Enemy();
        Enemy(const dang::tmx_spriteobject* so, const dang::spImagesheet& is);
        ~Enemy() override;

        virtual void    init();

        // pure virtual enemy class
        void update(uint32_t dt) override = 0;
        void collide(const dang::CollisionSpriteLayer::manifold &mf) override = 0;
        dang::CollisionSpriteLayer::eCollisionResponse getCollisionResponse(const dang::spCollisionSprite& other) override = 0;

        virtual bool isBubbled() = 0;
        virtual void bubble() = 0;
        virtual void endBubble() = 0;

        /**
         * Behaviour tree functions
         */
        dang::BTNode::Status checkPathCompleted();
        dang::BTNode::Status checkWaypointReached();
        virtual void startOutToWaypoint();

        /** these functions are used to set a destination wapoint */
        dang::BTNode::Status setDestinationWaypointByDepot(uint32_t depot_type);
        dang::BTNode::Status setRandNeighbourWaypoint();
        dang::BTNode::Status setRandPath();
        dang::BTNode::Status setWPHNearHero();

        /** this function are used if the sprite missed the dest waypoint and has to get back somehow to the path system */
        dang::BTNode::Status findNearestWaypoint(bool only_horizontally);

        /** static hooks for the behaviour tree */
        static dang::BTNode::Status NTcheckPathCompleted(dang::spSprite s);
        static dang::BTNode::Status NTsetRandNeighbourWaypoint(dang::spSprite s);
        static dang::BTNode::Status NTsetRandomPath(dang::spSprite s);
        static dang::BTNode::Status NTsetWPNearHero(dang::spSprite s);

        static dang::BTNode::Status NTcheckWaypointReached(dang::spSprite s);
        static dang::BTNode::Status NTsetDestinationBombDepot(dang::spSprite s);
        static dang::BTNode::Status NTsetDestinationCrateDepot(dang::spSprite s);
        static dang::BTNode::Status NTfindNearestWaypoint(dang::spSprite s);
        static dang::BTNode::Status NTfindNearestWaypointH(dang::spSprite s);

        /** path params & funcs*/
        dang::spSceneGraph                  _scene_graph{nullptr};
        std::vector<const dang::Waypoint*>  _path;
        const dang::Waypoint*               _current_wp{nullptr};
        size_t                              _path_index{0};
        uint32_t                            _max_time_to_wp{0};         //!< time in ms
        uint32_t                            _time_elapsed_to_wp{0};     //!< time in ms
        void        resetPathVars();

    protected:
        bool _on_ground = false;
        float _walkSpeed{2.0};

        dang::spTwVel _tw_long_horiz_jump{nullptr};
        dang::spTwVelY _tw_short_jump{nullptr};

    };

}
