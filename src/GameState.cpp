// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the pnk game

#include "GameState.h"
#include "GSIntro.h"
#include "GSHome.h"
#include "GSPlay.h"
#include "GSAbout.h"
#include "GSPrefs.h"

namespace pnk
{

    std::shared_ptr<GSIntro> GameState::_gs_intro = std::make_shared<GSIntro>();
    std::shared_ptr<GSHome> GameState::_gs_home = std::make_shared<GSHome>();
    std::shared_ptr<GSPlay> GameState::_gs_play = std::make_shared<GSPlay>();
    std::shared_ptr<GSAbout> GameState::_gs_about = std::make_shared<GSAbout>();
    std::shared_ptr<GSPrefs> GameState::_gs_prefs = std::make_shared<GSPrefs>();

}
