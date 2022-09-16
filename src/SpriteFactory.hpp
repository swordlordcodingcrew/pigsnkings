// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <Vector2T.hpp>

#include <string>
#include <memory>
#include <unordered_map>

namespace dang
{
    // forward declarations of DANG framework
    class SpriteObject;
    class ColSpr;
    class FullColSpr;
    class Imagesheet;
    class TmxExtruder;
    class NTreeState;

    struct tmx_spriteobject;

    using spColSpr = std::shared_ptr<ColSpr>;
    using spFullSpr = std::shared_ptr<FullColSpr>;
    using spSprObj = std::shared_ptr<SpriteObject>;
    using spImagesheet = std::shared_ptr<Imagesheet>;
    using spNTreeState = std::shared_ptr<NTreeState>;
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
    class Cannonmuzzle;
    class Moodies;
    class MoodiesThatHurt;
    class ScreenPlay;

    // using assignments
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
    using spCannonmuzzle = std::shared_ptr<pnk::Cannonmuzzle>;
    using spMoodies = std::shared_ptr<pnk::Moodies>;
    using spMoodiesThatHurt = std::shared_ptr<pnk::MoodiesThatHurt>;
    using spScreenPlay = std::shared_ptr<pnk::ScreenPlay>;


    class SpriteFactory
    {
    public:
        /**
         * type names. These should correspond to the object-types in the tiled-files
         */
        static inline const std::string_view T_KING{"king"};
        static inline const std::string_view T_BOSS{"boss"};
        static inline const std::string_view T_BUBBLE_PROTO{"bubble_proto"};
        static inline const std::string_view T_CRATE_PROTO{"crate_proto"};
        static inline const std::string_view T_BOMB_PROTO{"bomb_proto"};
        static inline const std::string_view T_EXPLOSION_PROTO{"boom_proto"};
        static inline const std::string_view T_PIG_POOF_PROTO{"pigpoof_proto"};
        static inline const std::string_view T_CANNONMUZZLE_PROTO{"cannonmuzzle_proto"};
        static inline const std::string_view T_CANNONBALL_PROTO{"cannonball_proto"};
        static inline const std::string_view T_HOTRECT{"hotrect"};
        static inline const std::string_view T_HOTRECT_PLATFORM{"hotrect_platform"};
        static inline const std::string_view T_ROOM_TRIGGER{"room_trigger"};
        static inline const std::string_view T_BOSSBATTLE_TRIGGER{"bossbattle_trigger"};
        static inline const std::string_view T_SAVEPOINT_TRIGGER{"savepoint_trigger"};
        static inline const std::string_view T_LEVEL_TRIGGER{"level_trigger"};
        static inline const std::string_view T_WARP_ROOM_TRIGGER{"warp_room_trigger"};
        static inline const std::string_view T_NORMAL_PIG_HIVE{"normal_pig_hive"};
        static inline const std::string_view T_PIG_NORMAL{"pig_normal"};
        static inline const std::string_view T_PIG_BETTER{"pig_better"};
        static inline const std::string_view T_PIG_BOX{"pig_box"};
        static inline const std::string_view T_PIG_BOMB{"pig_bomb"};
        static inline const std::string_view T_PIG_CANNON{"pig_cannon"};
        static inline const std::string_view T_CANNON{"cannon"};
        static inline const std::string_view T_COIN_SILVER{"coin_silver"};
        static inline const std::string_view T_COIN_GOLD{"coin_gold"};
        static inline const std::string_view T_GEM_BLUE{"gem_blue"};
        static inline const std::string_view T_GEM_GREEN{"gem_green"};
        static inline const std::string_view T_GEM_RED{"gem_red"};
        static inline const std::string_view T_POTION_BLUE{"potion_blue"};
        static inline const std::string_view T_POTION_GREEN{"potion_green"};
        static inline const std::string_view T_POTION_RED{"potion_red"};

//        static void attachBehaviourTree(const spScreenPlay& sp, const dang::tmx_spriteobject* so, const dang::spFullSpr& cs);
        static dang::spNTreeState getBT(const spScreenPlay& sp, const dang::tmx_spriteobject* so);

//        static void initSceneGraph(const spScreenPlay& sp, const spEnemy& spr);
//        static size_t findNearestGraph(const std::vector<dang::spSceneGraph>& sgs, const dang::Vector2F& pos);

        static spHero King(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is);
        static spBoss Boss(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp);
        static spHenchPig NormalPig(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, spScreenPlay& sp);
        static spHenchPig PigCrate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp);
        static spHenchPig PigBomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, const std::unordered_map<std::string, dang::spImagesheet> &iss, spScreenPlay& sp);

        static spPigCannon PigCannon(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, spScreenPlay& sp);
        static dang::spFullSpr Cannonmuzzle(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is);

        /** throwies */
        static spBubble Bubble(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left, uint8_t num_bubble_loops);
        static spCraties Crate(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left);
        static spBombies Bomb(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is);
        static spCannonball Cannonball(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is, bool to_the_left);

        /** moodies */
        static dang::spFullSpr PigPoof(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is);
        static spReward Reward(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is);

        /** moodies that hurt */
        static dang::spFullSpr Explosion(dang::TmxExtruder& txtr, const dang::tmx_spriteobject* so, dang::spImagesheet is);

        /** throwies form proto throwie */
        static spCraties CrateFromProto(const spCraties& proto, const dang::Vector2F& pos, bool to_the_left);
        static spBombies BombFromProto(const spBombies& proto, const dang::Vector2F& pos, bool to_the_left);
        static spCannonball CannonballFromProto(const spCannonball& proto, const dang::Vector2F& pos, bool to_the_left);
        static spMoodies CannonmuzzleFromProto(const spMoodies& proto, const dang::Vector2F& pos, bool to_the_left);



        static dang::spColSpr RoomTrigger(const dang::tmx_spriteobject* so, bool warp);
        static dang::spColSpr LevelTrigger(const dang::tmx_spriteobject* so);
        static dang::spColSpr BossbattleTrigger(const dang::tmx_spriteobject* so);
        static dang::spColSpr SavepointTrigger(const dang::tmx_spriteobject* so);
        static dang::spColSpr RigidObj(const dang::tmx_spriteobject* so);

        static void setTypeNum(dang::spSprObj spo, const std::string& type);


    };

}

