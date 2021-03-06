// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>

#include <Event.hpp>
#include <Dispatcher.hpp>
#include <TmxExtruder.hpp>
#include <Vector2T.hpp>

#include "GameState.h"

#include "ScreenPlay.h"
#include "PnkEvent.h"

namespace dang
{
    class BehaviourTree;
    class TreeState;
    class SpriteContext;
}

namespace pnk
{
    class Hero;

    class GSPlay : public GameState
    {
    public:
        /**
         * hero consts
         */
        static inline const float H_WALK_VEL = 7;
        static inline const float H_JUMP_VEL = -16;
        static inline const uint32_t H_JUMP_COUNT = 30;
        static inline const float BUBBLE_VEL = 20;
        static inline const float BUBBLE_VEL_UP = -1.5;
        static inline const float E_WALK_VEL = 2;
        static inline const float CRATE_VEL = 20;


    public:
        std::shared_ptr<GameState> update(dang::Gear& gear, uint32_t time) override;
        void enter(dang::Gear& gear, uint32_t time) override;
        void exit(dang::Gear& gear, uint32_t time) override;

        void createBehaviourTrees(dang::Gear& gear);


    protected:
        const dang::tmx_level*          _tmx;
        std::shared_ptr<ScreenPlay>     _screenplay;

        // the king
        std::shared_ptr<pnk::Hero> _spr_hero{nullptr};

        // the layer in which the game takes place
        std::shared_ptr<dang::CollisionSpriteLayer> _csl{nullptr};

        std::map<std::string, spCollisionSprite> _hives;

        // viewport pos (top left)
        dang::Vector2F          _vp_pos{0, 0};

        // flow stuff
        ScreenPlay::act*        _active_act{nullptr};
        int32_t                 _active_act_index{-1};
        int8_t                 _active_level_index{-1};
        uint32_t                _last_time{0};
        bool                    _warp{false};

        // reference to subscriber
        u_int32_t _sub_ref{0};
        void gameEventReceived(dang::Event &e);


        void handleKingHealth(PnkEvent& pe);
        void handleNewThrowie(PnkEvent& pe);
        void handleExplodingThrowie(PnkEvent& pe);
        void handleNewPoof(PnkEvent& pe);
        void handleRewardCollected(PnkEvent& pe);

        void addScore(uint8_t score);
        void addHealth(uint8_t health);

        void handleKingLoosesLife();

        void initGameVars();
        void updateVpPos();
        void changeRoom(int32_t room_nr, bool warp);
        void changeLevel(int8_t level_nr);

    };

}