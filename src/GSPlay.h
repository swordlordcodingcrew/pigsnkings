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
        const std::string& getInfotext() override { return _infoText; }

    protected:
        const std::string _infoText = "RIGHT: move right\n"
                                      "LEFT: move left\n"
                                      "Btn A ('z'): jump\n"
                                      "Btn B ('x'): bubble\n"
                                      "HOME ('1'): back to main\n"
                                      "MENU ('2'): pause";


        dang::tmx_level             _tmx;
        std::shared_ptr<ScreenPlay>  _screenplay;

        // the king
        std::shared_ptr<pnk::Hero> _spr_hero{nullptr};

        // the layer in which the game takes place
        std::shared_ptr<dang::CollisionSpriteLayer> _csl{nullptr};

        std::map<std::string, spCollisionSprite> _hives;

        // viewport pos (top left)
        dang::Vector2F          _vp_pos{0, 0};
        const uint32_t          _room_buffer = 2;  // "hotrect" when the room shall change
        dang::RectF             _room_extent{0,0,0,0};

        // flow stuff
        ScreenPlay::act*        _active_act{nullptr};
        int32_t                 _active_act_index{0};
        uint32_t                _last_time{0};

        // reference to subscriber
        u_int32_t _sub_ref{0};
        void gameEventReceived(dang::Event &e);

        void initGameVars();
        void updateVpPos();
        void changeRoom(uint32_t room_nr);

    };

}