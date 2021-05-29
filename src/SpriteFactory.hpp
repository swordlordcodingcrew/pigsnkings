// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <string>
#include <memory>
#include "src/actors/others/Reward.h"
#include "ScreenPlay.h"

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
    class Enemy;
    class HenchPig;
    class Bubble;
    class Throwies;
    class Craties;
    class Bombies;
    class Moodies;
}

// using assignments
using spCollisionSprite = std::shared_ptr<dang::CollisionSprite>;
using spImagesheet = std::shared_ptr<dang::Imagesheet>;
using spHero = std::shared_ptr<pnk::Hero>;
using spEnemy = std::shared_ptr<pnk::Enemy>;
using spHenchPig = std::shared_ptr<pnk::HenchPig>;
using spReward = std::shared_ptr<pnk::Reward>;
using spBubble = std::shared_ptr<pnk::Bubble>;
using spThrowies = std::shared_ptr<pnk::Throwies>;
using spCraties = std::shared_ptr<pnk::Craties>;
using spBombies = std::shared_ptr<pnk::Bombies>;
//using spCannonball = std::shared_ptr<pnk::Cannonball>;
using spMoodies = std::shared_ptr<pnk::Moodies>;
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
        static inline const std::string T_BUBBLE_PROTO{"bubble_proto"};
        static inline const std::string T_CRATE_PROTO{"crate_proto"};
        static inline const std::string T_BOMB_PROTO{"bomb_proto"};
        static inline const std::string T_PIG_POOF_PROTO{"pigpoof_proto"};
        static inline const std::string T_HOTRECT{"hotrect"};
        static inline const std::string T_HOTRECT_PLATFORM{"hotrect_platform"};
        static inline const std::string T_ROOM_TRIGGER{"room_trigger"};
        static inline const std::string T_WARP_ROOM_TRIGGER{"warp_room_trigger"};
        static inline const std::string T_NORMAL_PIG_HIVE{"normal_pig_hive"};
        static inline const std::string T_PIG_NORMAL{"pig_normal"};
        static inline const std::string T_PIG_BETTER{"pig_better"};
        static inline const std::string T_PIG_BOX{"pig_box"};
        static inline const std::string T_PIG_BOMB{"pig_bomb"};
        static inline const std::string T_PIG_CANNON{"pig_cannon"};
        static inline const std::string T_COIN_SILVER{"coin_silver"};
        static inline const std::string T_COIN_GOLD{"coin_gold"};
        static inline const std::string T_GEM_BLUE{"gem_blue"};
        static inline const std::string T_GEM_GREEN{"gem_green"};
        static inline const std::string T_GEM_RED{"gem_red"};
        static inline const std::string T_POTION_BLUE{"potion_blue"};
        static inline const std::string T_POTION_GREEN{"potion_green"};
        static inline const std::string T_POTION_RED{"potion_red"};

        static void attachBehaviourTree(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::shared_ptr<dang::Sprite>& sprite);
        static void initSceneGraph(const spScreenPlay& sp, const spEnemy& spr);

        static spHero King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spHenchPig NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp);
        static spHenchPig PigCrate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp);
        static spHenchPig PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, spScreenPlay& sp);
        static spBubble Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left);
        static spReward Reward(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spThrowies Crate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left);
        static spThrowies Bomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left);
        static spThrowies Cannonball(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is, bool to_the_left);
        static spCollisionSprite PigPoof(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, spImagesheet is);
        static spCollisionSprite Hotrect(const dang::tmx_spriteobject* so);
        static spCollisionSprite HotrectPlatform(const dang::tmx_spriteobject* so);
        static spCollisionSprite RoomTrigger(const dang::tmx_spriteobject* so);
        static spCollisionSprite WarpRoomTrigger(const dang::tmx_spriteobject* so);
    };

}

