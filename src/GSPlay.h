// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>

#include <Event.h>
#include <Dispatcher.h>
#include <TmxExtruder.h>
#include <Vector2T.h>

#include "GameState.h"

#include "LevelFlow.h"


namespace pnk
{

    class Hero;

    class GSPlay : public GameState
    {
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

        const uint32_t      _room_buffer = 4;  // "hotrect" when the room shall change
        const uint32_t      _spawn_delay = 1000;    // in whatever unit, some kind of subseconds

        dang::tmx_level             _tmx;
        std::shared_ptr<LevelFlow>  _lvl_flow;

        std::shared_ptr<pnk::Hero> _spr_hero{nullptr};
        std::shared_ptr<dang::CollisionSpriteLayer> _csl{nullptr};
        spImagesheet _obj_is{nullptr};

        // vp handling per room
        // assuming that 1 room is the size of the screen
        dang::Vector2F          _active_room_center{0, 0};
        std::map<uint16_t, dang::tmx_spriteobject> _prototypes;

        // flow stuff
        LevelFlow::roomflow*    _active_room_flow{nullptr};
        uint32_t                _last_time{0};
        bool                    _spawn_ready{true};
        uint32_t                _spawned{0};
        bool                    _room_transition{false};

        // reference to subscriber
        u_int32_t _sub_ref{0};
        void gameEventReceived(dang::Event &e);



    };

}