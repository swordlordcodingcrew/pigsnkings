// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "HUDLayer.hpp"
#include "pnk_globals.h"
#include "fonts/hud_font_small.h"
#include "Gear.hpp"
#include "pigsnkings.hpp"

#include <sprite/ImgSpr.hpp>
#include <sprite/FullImgSpr.hpp>
#include <TmxExtruder.hpp>
#include <SprIterator.hpp>
#include <Imagesheet.hpp>
#include <tween/TwAnim.hpp>
#include <cassert>

namespace pnk
{
    extern PigsnKings _pnk;

    HUDLayer::HUDLayer() : dang::BaseHUDLayer()
    {
    }

    void HUDLayer::fillSprites(dang::Gear& gear)
    {
        for (size_t j = 0; j < tmxLayer()->spriteobejcts_len; j++)
        {
            const dang::tmx_spriteobject* so = tmxLayer()->spriteobjects + j;

            if (so->type == "hud_l1")
            {
                auto spr = std::make_shared<dang::FullImgSpr>(so, gear.getImagesheet(so->tileset));
                spr->setTypeNum(ST_HUD_L1);
                l1 = spr;
                addSprite(std::static_pointer_cast<dang::ImgSpr>(spr));
            }
            else if (so->type == "hud_l2")
            {
                auto spr = std::make_shared<dang::FullImgSpr>(so, gear.getImagesheet(so->tileset));
                spr->setTypeNum(ST_HUD_L2);
                l2 = spr;
                addSprite(std::static_pointer_cast<dang::ImgSpr>(spr));
            }
            else if (so->type == "hud_l3")
            {
                auto spr = std::make_shared<dang::FullImgSpr>(so, gear.getImagesheet(so->tileset));
                spr->setTypeNum(ST_HUD_L3);
                l3 = spr;
                addSprite(std::static_pointer_cast<dang::ImgSpr>(spr));
            }
            /*
            else if (so->type == "hud_l4")
            {
                spr->setTypeNum(ST_HUD_L4);
                spr->setVisible(false); // bonus life
                l4 = spr;
            }
             */
            else
            {
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
                else if (so->type == "hud_boss_health_p1")
                {
                    spr->setTypeNum(ST_HUD_BOSS_HEALTH_P1);
                    spr->setVisible(false); // as long as no boss battle is running
                }
                else if (so->type == "hud_boss_health_p2")
                {
                    spr->setTypeNum(ST_HUD_BOSS_HEALTH_P2);
                    spr->setVisible(false); // as long as no boss battle is running
                }
                else if (so->type == "hud_boss_health_p3")
                {
                    spr->setTypeNum(ST_HUD_BOSS_HEALTH_P3);
                    spr->setVisible(false); // as long as no boss battle is running
                }

                addSprite(spr);

            }
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
        spr = getImgSprByTypeNum(ST_HUD_BOSS_HEALTH_P1);
        spr->setVisible(true);
        spr = getImgSprByTypeNum(ST_HUD_BOSS_HEALTH_P2);
        spr->setVisible(true);
        spr = getImgSprByTypeNum(ST_HUD_BOSS_HEALTH_P3);
        spr->setVisible(true);

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
        spr = getImgSprByTypeNum(ST_HUD_BOSS_HEALTH_P1);
        spr->setVisible(false);
        spr = getImgSprByTypeNum(ST_HUD_BOSS_HEALTH_P2);
        spr->setVisible(false);
        spr = getImgSprByTypeNum(ST_HUD_BOSS_HEALTH_P3);
        spr->setVisible(false);

        isBossVisible = false;
    }


    void HUDLayer::render(const dang::Gear &gear)
    {
        l1->setVisible(_pnk._gamestate.lives > 0);
        l2->setVisible(_pnk._gamestate.lives > 1);
        l3->setVisible(_pnk._gamestate.lives > 2);
        //l4->setVisible(_pnk._gamestate.lives > 3);

        this->dang::BaseHUDLayer::render(gear);

        std::string score =  std::to_string(_pnk._gamestate.score);
        std::string prefixedScore = std::string(5 - score.length(), '0') + score;

//        std::string highScore =  std::to_string(_pnk._gamestate.high_score);
//        std::string formattedHighScore = "HS:" + std::string(8 - highScore.length(), '0') + highScore;

//        std::string lives =  std::to_string(_pnk._gamestate.lives);
//        std::string prefixedLives = std::string(2 - lives.length(), '0') + lives;

        blit::screen.pen = backgroundColour;
        blit::screen.text(prefixedScore, hud_font_small, blit::Point(86 + shadowOffset, 4 + shadowOffset), true, blit::TextAlign::left);
//        blit::screen.text(prefixedScore, hud_font_small, blit::Point(49 + shadowOffset, 18 + shadowOffset), true, blit::TextAlign::left);
//        blit::screen.text(formattedHighScore, hud_font_small, blit::Point(160 + shadowOffset, 18 + shadowOffset), true, blit::TextAlign::center_h);
//        blit::screen.text(prefixedLives, hud_font_small, blit::Point(34 + shadowOffset, 49 + shadowOffset), true, blit::TextAlign::center_h);

        blit::screen.pen = background2Colour;
        blit::screen.text(prefixedScore, hud_font_small, blit::Point(86 - shadowOffset, 4 - shadowOffset), true, blit::TextAlign::left);

        blit::screen.pen = foregroundColour;
        blit::screen.text(prefixedScore, hud_font_small, blit::Point(86, 4), true, blit::TextAlign::left);
//        blit::screen.text(prefixedScore, hud_font_small, blit::Point(49, 18), true, blit::TextAlign::left);
//        blit::screen.text(formattedHighScore, hud_font_small, blit::Point(160, 18), true, blit::TextAlign::center_h);
//        blit::screen.text(prefixedLives, hud_font_small, blit::Point(34, 49), true, blit::TextAlign::center_h);

        // health bar
        blit::screen.pen = heroHealthColour;
        float width = _pnk._gamestate.health * 0.3;
        blit::screen.rectangle(blit::Rect(40, 23, width > 30 ? 30 : width, 2));

        if(isBossVisible)
        {
            // boss health bar
            blit::screen.pen = bossHealthColour;
            float width = _pnk._gamestate.boss_health * 0.3;
            blit::screen.rectangle(blit::Rect(249, 23, width > 30 ? 30 : width, 2));
        }
    }

    void HUDLayer::setAnimations(dang::TmxExtruder &txtr)
    {
        dang::spTwAnim heartAnim1 = txtr.getAnimation(l1->getImagesheet(), "heart_in_hud");
        assert(heartAnim1 != nullptr);
        heartAnim1->delay(2000);
        l1->setAnimation(heartAnim1);
        dang::spTwAnim heartAnim2 = std::make_shared<dang::TwAnim>(*heartAnim1);
        l2->setAnimation(heartAnim2);
        dang::spTwAnim heartAnim3 = std::make_shared<dang::TwAnim>(*heartAnim1);
        l3->setAnimation(heartAnim3);
    }
}
