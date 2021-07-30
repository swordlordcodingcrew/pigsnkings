// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <DangFwdDecl.h>

#include <memory>

namespace pnk
{
    class HUDLayer;

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

        virtual void updateCheatKeyStream(uint32_t pressed);
    };

}
