// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "HUDLayer.hpp"
#include "pnk_globals.h"
#include "fonts/hud_font_big.h"
#include "fonts/hud_font_small.h"
#include "Gear.hpp"
#include "pigsnkings.hpp"

#include <sprite/ImgSpr.hpp>
#include <TmxExtruder.hpp>

namespace pnk
{
    extern PigsnKings _pnk;

    HUDLayer::HUDLayer() : ImgSprLayer()
    {
        foregroundColour = blit::Pen(63, 56, 81, 255);
        backgroundColour = blit::Pen(152, 152, 152, 200);
        healthColour = blit::Pen(78, 110, 197, 255);
    }

    void HUDLayer::fillSprites(dang::Gear& gear)
    {
        for (size_t j = 0; j < tmxLayer()->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = tmxLayer()->spriteobjects + j;

            auto spr = std::make_shared<dang::ImgSpr>(so, gear.getImagesheet(so->tileset));
            if (so->type == "hud_hero")
            {
                spr->setTypeNum(ST_HUD_HERO);
            }
            else if (so->type == "hud_boss")
            {
                spr->setTypeNum(ST_HUD_BOSS);
                spr->setVisible(false); // as long as no boss battle is running
            }
            else if (so->type == "hud_boss_health")
            {
                spr->setTypeNum(ST_HUD_BOSS_HEALTH);
                spr->setVisible(false); // as long as no boss battle is running
            }

            addSprite(spr);
        }

    }


    void HUDLayer::changeCheatSprite()
    {
        // we already did, chickening out
        if(isCheating)
        {
            return;
        }

        dang::spImgSpr spr = getImgSprByTypeNum(ST_HUD_HERO);
        spr->setImgIndex(27);

/*        for (dang::spSprite& spr : _sprites)
        {
            if(spr->_type_name == "hud_hero")
            {
                // a bit of a hack, needs to be changed when grafx changes
                spr->_img_index = 27;
            }
        }
*/
        isCheating = true;
    }

    void HUDLayer::activateBossHUD()
    {
        // we already did
        if(isBossVisible)
        {
            return;
        }

        dang::spImgSpr spr = getImgSprByTypeNum(ST_HUD_BOSS);
        spr->setVisible(true);
        spr = getImgSprByTypeNum(ST_HUD_BOSS_HEALTH);
        spr->setVisible(true);

/*        for (dang::spSprite& spr : _sprites)
        {
            if(spr->_type_name == "hud_boss" || spr->_type_name == "hud_boss_health")
            {
                spr->_visible = true;
            }
        }
*/
        isBossVisible = true;
    }

    void HUDLayer::deactivateBossHUD()
    {
        // we already did
        if(!isBossVisible)
        {
            return;
        }

        dang::spImgSpr spr = getImgSprByTypeNum(ST_HUD_BOSS);
        spr->setVisible(false);
        spr = getImgSprByTypeNum(ST_HUD_BOSS_HEALTH);
        spr->setVisible(false);

/*        for (dang::spSprite& spr : _sprites)
        {
            if(spr->_type_name == "hud_boss" || spr->_type_name == "hud_boss_health")
            {
                // a bit of a hack, needs to be changed when grafx changes
                spr->_visible = false;
            }
        }
*/
        isBossVisible = false;
    }

//    void HUDLayer::renderInternal(const dang::Gear &gear)
    void HUDLayer::render(const dang::Gear &gear)
    {
        this->ImgSprLayer::render(gear);

        std::string score =  std::to_string(_pnk._gamestate.score);
        std::string prefixedScore = std::string(5 - score.length(), '0') + score;

        std::string highScore =  std::to_string(_pnk._gamestate.high_score);
        std::string formattedHighScore = "HS:" + std::string(8 - highScore.length(), '0') + highScore;

        std::string lives =  std::to_string(_pnk._gamestate.lives);
        std::string prefixedLives = std::string(2 - lives.length(), '0') + lives;

        blit::screen.pen = backgroundColour;
        blit::screen.text(prefixedScore, hud_font_small, blit::Point(49 + shadowOffset, 18 + shadowOffset), true, blit::TextAlign::left);
        blit::screen.text(formattedHighScore, hud_font_small, blit::Point(160 + shadowOffset, 18 + shadowOffset), true, blit::TextAlign::center_h);
        blit::screen.text(prefixedLives, hud_font_small, blit::Point(34 + shadowOffset, 49 + shadowOffset), true, blit::TextAlign::center_h);

        blit::screen.pen = foregroundColour;
        blit::screen.text(prefixedScore, hud_font_small, blit::Point(49, 18), true, blit::TextAlign::left);
        blit::screen.text(formattedHighScore, hud_font_small, blit::Point(160, 18), true, blit::TextAlign::center_h);
        blit::screen.text(prefixedLives, hud_font_small, blit::Point(34, 49), true, blit::TextAlign::center_h);

        // health bar
        // TODO: make sure it does not grow bigger than full size
        blit::screen.pen = healthColour;
        float width = _pnk._gamestate.health * 0.3;
        blit::screen.rectangle(blit::Rect(55, 44, width, 2));

        if(isBossVisible)
        {
            // boss health bar
            // TODO: make sure it does not grow bigger than full size
            blit::screen.pen = healthColour;
            float width = _pnk._gamestate.boss_health * 0.3;
            blit::screen.rectangle(blit::Rect(235, 44, width, 2));
        }
    }
}
