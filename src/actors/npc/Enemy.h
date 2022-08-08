// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <DangFwdDecl.h>
#include <sprite/FullColSpr.hpp>
#include <bt/BTNode.h>

#include <vector>

namespace dang
{
    class TmxExtruder;
}

namespace pnk
{
    class ScreenPlay;
    using spScreenPlay = std::shared_ptr<ScreenPlay>;

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

    class Enemy : public dang::FullColSpr
    {
    public:
        Enemy();
        Enemy(const dang::tmx_spriteobject* so, const dang::spImagesheet& is);
        ~Enemy() override;

        virtual void    init();

        // pure virtual enemy class
        void collide(const dang::manifold &mf) override = 0;
        uint8_t  getCollisionResponse(const dang::CollisionObject* other) const override = 0;

        virtual bool isBubbled() = 0;
        virtual void bubble() = 0;
        virtual void endBubble() = 0;

        /**
         * Behaviour tree functions
         */
        dang::BTNode::Status checkPathCompleted(uint32_t dt);
        dang::BTNode::Status checkWaypointReached(uint32_t dt);
        virtual void startOutToWaypoint();
        void initSceneGraph(const spScreenPlay &sp, dang::TmxExtruder& extr);

        /** these functions are used to set a destination wapoint */
        dang::BTNode::Status setDestinationWaypointByType(uint32_t wp_type);
        dang::BTNode::Status setRandNeighbourWaypoint();
        dang::BTNode::Status setRandPath();
        dang::BTNode::Status setWPHNearHero();
        dang::BTNode::Status setWPNearHero();
        dang::BTNode::Status setPathToHero();

        /** this function is used if the sprite missed the dest waypoint and has to get back somehow to the path system */
        dang::BTNode::Status findNearestWaypoint(bool only_horizontally);

        /** static hooks for the behaviour tree */
        static dang::BTNode::Status NTcheckPathCompleted(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTsetRandNeighbourWaypoint(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTsetRandomPath(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTsetWPNearHero(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTsetPathToHero(dang::FullColSpr& s, uint32_t dt);

        static dang::BTNode::Status NTcheckWaypointReached(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTsetDestinationBombDepot(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTsetDestinationCrateDepot(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTsetDestinationPOI(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTfindNearestWaypoint(dang::FullColSpr& s, uint32_t dt);
        static dang::BTNode::Status NTfindNearestWaypointH(dang::FullColSpr& s, uint32_t dt);


    protected:
        /** path params & funcs*/
        void                                resetPathVars();
        const dang::Waypoint*               _current_wp{nullptr};
        dang::spSceneGraph                  _scene_graph{nullptr};
        std::vector<const dang::Waypoint*>  _path;
        size_t                              _path_index{0};
        uint32_t                            _max_time_to_wp{0};         //!< time in ms
        uint32_t                            _time_elapsed_to_wp{0};     //!< time in ms


        bool _on_ground = false;
        float _walkSpeed{2.0};

        dang::spTwVel _tw_long_horiz_jump{nullptr};
        dang::spTwVelY _tw_short_jump{nullptr};

    };

}
