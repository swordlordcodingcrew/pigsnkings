// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "GameState.h"
#include "GSIntro.h"
#include "GSHome.h"
#include "GSPlay.h"
#include "GSAbout.h"
#include "GSPrefs.h"
#include "pigsnkings.hpp"

#include <32blit.hpp>


namespace pnk
{
    extern PigsnKings _pnk;

    std::shared_ptr<GSIntro> GameState::_gs_intro = std::make_shared<GSIntro>();
    std::shared_ptr<GSHome> GameState::_gs_home = std::make_shared<GSHome>();
    std::shared_ptr<GSPlay> GameState::_gs_play = std::make_shared<GSPlay>();
    std::shared_ptr<GSAbout> GameState::_gs_about = std::make_shared<GSAbout>();
    std::shared_ptr<GSPrefs> GameState::_gs_prefs = std::make_shared<GSPrefs>();

    void GameState::updateCheatKeyStream(uint32_t pressed)
    {
        char nc = '0';

        if (pressed & blit::Button::A)
        {
            nc = 'A';
        }
        else if (pressed & blit::Button::B)
        {
            nc = 'B';
        }
        else if (pressed & blit::Button::X)
        {
            nc = 'X';
        }
        else if (pressed & blit::Button::Y)
        {
            nc = 'Y';
        }
        else if (pressed & blit::Button::DPAD_UP)
        {
            nc = 'U';
        }
        else if (pressed & blit::Button::DPAD_DOWN)
        {
            nc = 'D';
        }
        else if (pressed & blit::Button::DPAD_LEFT)
        {
            nc = 'L';
        }
        else if (pressed & blit::Button::DPAD_RIGHT)
        {
            nc = 'R';
        }

        if(nc != '0')
        {
            for (int i = 0; i < 7; ++i)
            {
                _pnk.cheatKeyStream[i] = _pnk.cheatKeyStream[i+1];
            }
            _pnk.cheatKeyStream[7] = nc;

#ifdef PNK_DEBUG_COMMON
            DEBUG_PRINT("CheatStream %s\r\n", _pnk.cheatKeyStream.c_str());
#endif
        }
    }
}
