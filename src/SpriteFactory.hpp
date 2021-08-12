// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <string>
#include <memory>
#include "src/actors/others/Reward.h"
#include "src/levels/ScreenPlay.h"

namespace dang
{
    // forward declarations of DANG framework
    class CollisionSprite;
    class Imagesheet;
    class TmxExtruder;
    class Sprite;
    class ScreenPlay;

    struct tmx_spriteobject;
}

namespace pnk
{
    // forward declarations
    class Hero;
    class PigBoss;
    class Reward;
    class Enemy;
    class HenchPig;
    class Bubble;
    class Throwies;
    class Craties;
    class Bombies;
    class PigCannon;
    class Cannon;
    class Cannonball;
    class Moodies;
    class MoodiesThatHurt;
}

// using assignments
using spCollisionSprite = std::shared_ptr<dang::CollisionSprite>;
using spImagesheet = std::shared_ptr<dang::Imagesheet>;
using spHero = std::shared_ptr<pnk::Hero>;
using spBoss = std::shared_ptr<pnk::PigBoss>;
using spEnemy = std::shared_ptr<pnk::Enemy>;
using spHenchPig = std::shared_ptr<pnk::HenchPig>;
using spReward = std::shared_ptr<pnk::Reward>;
using spBubble = std::shared_ptr<pnk::Bubble>;
using spThrowies = std::shared_ptr<pnk::Throwies>;
using spCraties = std::shared_ptr<pnk::Craties>;
using spBombies = std::shared_ptr<pnk::Bombies>;
using spPigCannon = std::shared_ptr<pnk::PigCannon>;
using spCannon = std::shared_ptr<pnk::Cannon>;
using spCannonball = std::shared_ptr<pnk::Cannonball>;
using spMoodies = std::shared_ptr<pnk::Moodies>;
using spMoodiesThatHurt = std::shared_ptr<pnk::MoodiesThatHurt>;
using spScreenPlay = std::shared_ptr<pnk::ScreenPlay>;

namespace pnk
{
    class SpriteFactory
    {
    public:
        /**
         * type names. These should correspond to the object-types in the tiled-files
         */
        static inline const std::string T_KING{"king"};
        static inline const std::string T_BOSS{"boss"};
        static inline const std::string T_BUBBLE_PROTO{"bubble_proto"};
        static inline const std::string T_CRATE_PROTO{"crate_proto"};
        static inline const std::string T_BOMB_PROTO{"bomb_proto"};
        static inline const std::string T_EXPLOSION_PROTO{"boom_proto"};
        static inline const std::string T_PIG_POOF_PROTO{"pigpoof_proto"};
        static inline const std::string T_CANNONMUZZLE_PROTO{"cannonmuzzle_proto"};
        static inline const std::string T_CANNONBALL_PROTO{"cannonball_proto"};
        static inline const std::string T_HOTRECT{"hotrect"};
        static inline const std::string T_HOTRECT_PLATFORM{"hotrect_platform"};
        static inline const std::string T_ROOM_TRIGGER{"room_trigger"};
        static inline const std::string T_BOSSBATTLE_TRIGGER{"bossbattle_trigger"};
        static inline const std::string T_LEVEL_TRIGGER{"level_trigger"};
        static inline const std::string T_WARP_ROOM_TRIGGER{"warp_room_trigger"};
        static inline const std::string T_NORMAL_PIG_HIVE{"normal_pig_hive"};
        static inline const std::string T_PIG_NORMAL{"pig_normal"};
        static inline const std::string T_PIG_BETTER{"pig_better"};
        static inline const std::string T_PIG_BOX{"pig_box"};
        static inline const std::string T_PIG_BOMB{"pig_bomb"};
        static inline const std::string T_PIG_CANNON{"pig_cannon"};
        static inline const std::string T_CANNON{"cannon"};
        static inline const std::string T_COIN_SILVER{"coin_silver"};
        static inline const std::string T_COIN_GOLD{"coin_gold"};
        static inline const std::string T_GEM_BLUE{"gem_blue"};
        static inline const std::string T_GEM_GREEN{"gem_green"};
        static inline const std::string T_GEM_RED{"gem_red"};
        static inline const std::string T_POTION_BLUE{"potion_blue"};
        static inline const std::string T_POTION_GREEN{"potion_green"};
        static inline const std::string T_POTION_RED{"potion_red"};

        static void attachBehaviourTree(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const spCollisionSprite& cs);
        static void initSceneGraph(const spScreenPlay& sp, const spEnemy& spr);
        static size_t findNearestGraph(const std::vector<dang::spSceneGraph>& sgs, const dang::Vector2F& pos);

        static spHero King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spBoss Boss(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spHenchPig NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp);
        static spHenchPig PigCrate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp);
        static spHenchPig PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp);
        static spHenchPig PigCannoneer(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp);
        static spPigCannon PigCannoneerWCannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp);
        static spBubble Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left);
        static spReward Reward(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spThrowies Crate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left);
        static spThrowies Bomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spCollisionSprite Explosion(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spThrowies Cannonball(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left);
        static spCollisionSprite PigPoof(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spCollisionSprite Cannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spCannon CannonForCannoneer(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spCollisionSprite Cannonmuzzle(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spCollisionSprite Hotrect(const dang::tmx_spriteobject* so);
        static spCollisionSprite HotrectPlatform(const dang::tmx_spriteobject* so);
        static spCollisionSprite RoomTrigger(const dang::tmx_spriteobject* so);
        static spCollisionSprite WarpRoomTrigger(const dang::tmx_spriteobject* so);
        static spCollisionSprite LevelTrigger(const dang::tmx_spriteobject* so);
        static spCollisionSprite BossbattleTrigger(const dang::tmx_spriteobject* so);
    };

}

