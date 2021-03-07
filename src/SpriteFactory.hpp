// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <string>
#include <memory>

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

// using assignements
using spCollisionSprite = std::shared_ptr<dang::CollisionSprite>;
using spImagesheet = std::shared_ptr<dang::Imagesheet>;
using spHero = std::shared_ptr<pnk::Hero>;
using spEnemy = std::shared_ptr<pnk::Enemy>;
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
        static inline const std::string T_NORMAL_PIG{"normal_pig"};
        static inline const std::string T_NORMAL_PIG_HIVE{"normal_pig_hive"};
        static inline const std::string T_BETTER_PIG{"better_pig"};
        static inline const std::string T_COIN_SILVER{"coin_silver"};

        /**
         * definition of _type_num of sprites.
         */
        enum TN
        {
            // 0 - 99 king and associated stuff to king
            TN_KING = 10,
            TN_BUBBLE = 50,

            // 100 - 199 enemies
            TN_NORMAL_PIG = 101,
            TN_NORMAL_PIG_HIVE = 102,
            TN_BETTER_PIG = 102,

            // 200 - 299 hotrects
            TN_HOTRECT = 201,
            TN_HOTRECT_PLATFORM = 202,

            // 300 - 399 coins
            TN_COIN_SILVER = 300
        };


        static spHero King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is);
        static spEnemy NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is);
        static spBubble Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject& so, spImagesheet is, bool to_the_left);
        static spCollisionSprite Hotrect(const dang::tmx_spriteobject& so);
        static spCollisionSprite HotrectPlatform(const dang::tmx_spriteobject& so);
    };

}

