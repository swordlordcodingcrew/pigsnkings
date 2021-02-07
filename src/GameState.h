// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <memory>
#include "pigsnkings.hpp"

namespace dang
{
    class Gear;
    class Layer;
    class Sprite;
    class CollisionSpriteLayer;
    class SpriteLayer;
    class TileLayer;
    class CollisionSprite;
    class Imagesheet;
    class TwAnim;
}

using spSprite = std::shared_ptr<dang::Sprite>;
using spLayer = std::shared_ptr<dang::Layer>;
using spCollisionSpriteLayer = std::shared_ptr<dang::CollisionSpriteLayer>;
using spSpriteLayer = std::shared_ptr<dang::SpriteLayer>;
using spTileLayer = std::shared_ptr<dang::TileLayer>;
using spCollisionSprite = std::shared_ptr<dang::CollisionSprite>;
using spImagesheet = std::shared_ptr<dang::Imagesheet>;
using spTwAnim = std::shared_ptr<dang::TwAnim>;

namespace pnk
{
    extern PigsnKings _pnk;

    class GSIntro;
    class GSHome;
    class GSPlay;
    class GSAbout;
    class GSPrefs;

    class GameState
    {
    public:
        static std::shared_ptr<GSIntro> _gs_intro;
        static std::shared_ptr<GSHome> _gs_home;
        static std::shared_ptr<GSPlay> _gs_play;
        static std::shared_ptr<GSAbout> _gs_about;
        static std::shared_ptr<GSPrefs> _gs_prefs;


        virtual std::shared_ptr<GameState> update(dang::Gear& gear, uint32_t time) = 0;
        virtual void enter(dang::Gear& gear, uint32_t time) = 0;
        virtual void exit(dang::Gear& gear, uint32_t time) = 0;
        virtual const std::string& getInfotext() = 0;
    };

}
