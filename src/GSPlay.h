// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "GameState.h"
#include "levels/ScreenPlay.h"

#include <engine/input.hpp>

#include <Vector2T.hpp>
#include <bt/BTNode.h>

namespace pnk
{
    class Hero;
    using spHero = std::shared_ptr<Hero>;
    class PigBoss;
    using spBoss = std::shared_ptr<PigBoss>;
    class ScreenPlay;
    using spScreenPlay = std::shared_ptr<ScreenPlay>;
    class PnkEvent;

    class GSPlay : public GameState
    {
    public:
        std::shared_ptr<GameState> update(dang::Gear& gear, uint32_t time) override;
        void enter(dang::Gear& gear, uint32_t time) override;
        void exit(dang::Gear& gear, uint32_t time) override;

        void checkCheatActivation();

    protected:
        const dang::tmx_level*          _tmx{nullptr};          //!< level definition
        spScreenPlay                    _screenplay{nullptr};   //!< paths
        spHero                          _spr_hero{nullptr};     //!< the king
        spBoss                          _spr_boss{nullptr};     //!< the respective boss (or nullptr)
        dang::spColSprLayer             _csl{nullptr};          //!< the layer in which the collision detection takes place
        dang::spMessageLayer            _txtl{nullptr};
        std::unordered_map<std::string, dang::spColSpr> _hives{};

        // viewport pos (top left)
        dang::Vector2F          _vp_pos{0, 0};

        // zone stuff
        dang::RectF             _active_room{0,0,0,0};
        int8_t                  _active_room_index{-1}; // which room are we in?
        bool                    _warp{false};
        bool                    _leaveGame{false};
        bool                    _endScene{false};

        // reference to subscriber
        uint32_t _sub_ref{0};
        void gameEventReceived(dang::Event &e);

        void handleKingHealth(PnkEvent& pe);
        void handleNewThrowie(PnkEvent& pe);
        void handleExplodingThrowie(PnkEvent& pe);
        void handleNewPoof(PnkEvent& pe);
        void handleRewardCollected(PnkEvent& pe);

        void addScore(uint8_t score);
        void addHealth(uint8_t health);

        void handleKingLoosesLife();
        void resetRoomFromSave();

        void updateVpPos();
        void changeRoom(int32_t room_nr, bool warp);
        void changeLevel(int8_t level_nr);

        void saveGamestate();

        void loadLevel(int8_t level_nr);
        void freeCurrentLevel();

        void userIsCheating();

        void startBossBattle();
        void endBossBattle();
        void handleBossHit(PnkEvent& pe);

        void showInfoLayer(bool pause, uint32_t ttl, const std::string_view& message);
        void showGameOverInfo();
        void hideInfoLayer(blit::Button btn);
        void leaveTheGameCallback(blit::Button btn);

    public:
        /** bt hooks */
        dang::BTNode::Status NTheroInSightH(dang::FullColSpr& s, uint32_t dt);
        dang::BTNode::Status NTheroInSight(dang::FullColSpr& s, uint32_t dt);
        dang::BTNode::Status NTheroPos(dang::FullColSpr& s, uint32_t dt);

    };

}