// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <string>
#include <memory>
#include "Reward.h"

namespace dang
{
    // forward declarations of DANG framework
    class CollisionSprite;
    class Imagesheet;
    class TmxExtruder;

    struct tmx_spriteobject;
}

namespace pnk
{
    // forward declarations
    class Hero;
    class Enemy;
    class Bubble;
}

// using assignments
using spCollisionSprite = std::shared_ptr<dang::CollisionSprite>;
using spImagesheet = std::shared_ptr<dang::Imagesheet>;
using spHero = std::shared_ptr<pnk::Hero>;
using spEnemy = std::shared_ptr<pnk::Enemy>;
using spReward = std::shared_ptr<pnk::Reward>;
using spBubble = std::shared_ptr<pnk::Bubble>;

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
        static inline const std::string T_HOTRECT{"hotrect"};
        static inline const std::string T_HOTRECT_PLATFORM{"hotrect_platform"};
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

        /**
         * definition of _type_num of sprites.
         */
        enum TN
        {
            // 0 - 99 king and associated stuff to king
            TN_KING = 10,
            TN_BUBBLE = 50,

            // 100 - 199 enemies
            TN_PIG_NORMAL = 101,
            TN_NORMAL_PIG_HIVE = 102,
            TN_PIG_BETTER = 103,
            TN_PIG_BOX = 104,
            TN_PIG_BOMB = 105,
            TN_PIG_CANNON = 106,

            // 200 - 299 hotrects
            TN_HOTRECT = 201,
            TN_HOTRECT_PLATFORM = 202,

            // 300 - 399 coins and rewards in general
            TN_REWARDS = 300,
            TN_COIN_SILVER = 301,
            TN_COIN_GOLD = 302,
            TN_GEM_BLUE = 303,
            TN_GEM_GREEN = 304,
            TN_GEM_RED = 305,
            TN_POTION_BLUE = 306,
            TN_POTION_RED = 307,
            TN_POTION_GREEN = 308,
            TN_REWARDS_END = 399
        };

        static spHero King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is);
        static spEnemy NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is);
        static spEnemy PigBox(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is);
        static spEnemy PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is);
        static spBubble Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is, bool to_the_left);
        static spReward Reward(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is);
        static spCollisionSprite Hotrect(const dang::tmx_spriteobject& so);
        static spCollisionSprite HotrectPlatform(const dang::tmx_spriteobject& so);
    };

}

