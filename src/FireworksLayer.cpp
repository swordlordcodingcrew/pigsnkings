// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the pnk game

#include <iostream>
#include "Gear.hpp"
#include "FireworksLayer.h"
#include "libs/DANG/src/layer/Layer.hpp"

namespace pnk
{
    FireworksLayer::FireworksLayer() : Layer(E_TYPE::LT_UNDEFINED, {0,0}, 200, "FW", true, true)
    {
        // setup firework data structure
        for (uint8_t i = 0; i < _numof_rockets; i++)
        {
            // populate fireworks properties
            initFirework(i);
        }
    }

    FireworksLayer::~FireworksLayer()
    {
    }

    void FireworksLayer::update(uint32_t dt, const dang::Gear &gear)
    {
        for (uint8_t fw = 0; fw < _numof_rockets; fw++)
        {
            // rocket has reached its peak
            if (fireworks[fw].property.vel.y >= 0)
            {
                fireworks[fw].property.alpha = 0;

                // calculate particle physics
                for (uint8_t j = 0; j < _numof_particles; j++)
                {
                    if (fireworks[fw].particles[j].alpha >= fireworks[fw].particles[j].alpha_rate)
                    {
                        fireworks[fw].particles[j].alpha -= fireworks[fw].particles[j].alpha_rate;
                    }
                    else
                    {
                        // particles have faded away
                        fireworks[fw].particles[j].alpha = 0;
                    }

                    float prev_pos_x = fireworks[fw].particles[j].pos.x;
                    float prev_pos_y = fireworks[fw].particles[j].pos.y;

                    // change position based on velocity
                    fireworks[fw].particles[j].pos.add(fireworks[fw].particles[j].vel);

                    // change velocity based on acceleration
                    fireworks[fw].particles[j].vel.add(gravity);

                    // slow down particle after ever frame
                    fireworks[fw].particles[j].vel.multiply(0.97);

                    // keep track of the rocket particles previous positions
                    for (uint8_t k = 0; k < _numof_tail_elements; k++)
                    {
                        // store current cell position in a temp variable
                        // to be pushed to the tail
                        float temp_x = fireworks[fw].particles[j].tail[k].x;
                        float temp_y = fireworks[fw].particles[j].tail[k].y;

                        //update current cell with the last known prev position
                        fireworks[fw].particles[j].tail[k].x = prev_pos_x;
                        fireworks[fw].particles[j].tail[k].y = prev_pos_y;

                        //update the prev position for the next iteration of the loop
                        prev_pos_x = temp_x;
                        prev_pos_y = temp_y;
                    }
                }

                // particle 0 always has the shortest alpha_rate. so id alpha of particle 0 is 0, then all particles have an alpha == 0
                // which means: reset the rocket.
                if (fireworks[fw].particles[0].alpha == 0)
                {
                    initFirework(fw);
                }
            }
            else
            {
                // save the rockets previous position
                float prev_pos_x = fireworks[fw].property.pos.x;
                float prev_pos_y = fireworks[fw].property.pos.y;

                // change position based on velocity
                fireworks[fw].property.pos.add(fireworks[fw].property.vel);

                // change velocity based on acceleration
                fireworks[fw].property.vel.add(gravity);

                // make all particles follow the rocket
                for (uint8_t j = 0; j < _numof_particles; j++)
                {
                    fireworks[fw].particles[j].pos.x = fireworks[fw].property.pos.x;
                    fireworks[fw].particles[j].pos.y = fireworks[fw].property.pos.y;
                }

                // keep track of rockets previous positions
                for (uint8_t k = 0; k < _numof_tail_elements; k++)
                {
                    float temp_x = fireworks[fw].property.tail[k].x;
                    float temp_y = fireworks[fw].property.tail[k].y;

                    // update current cell with the last known prev position
                    fireworks[fw].property.tail[k].x = prev_pos_x;
                    fireworks[fw].property.tail[k].y = prev_pos_y;

                    // update the prev position for the next iteration of the loop
                    prev_pos_x = temp_x;
                    prev_pos_y = temp_y;
                }
            }
        }
    }

    void FireworksLayer::render(const dang::Gear& gear)
    {
        // draw the firework / rockets
        for (uint8_t fw = 0; fw < _numof_rockets; fw++)
        {
            blit::Rect dest;

            // draw rocket
            dest.x = fireworks[fw].property.pos.x;
            dest.y = fireworks[fw].property.pos.y;
            dest.w = 4;
            dest.h = 4;

            float alpha = fireworks[fw].property.alpha;

            // draw rocket if it has not yet exploded
            if (alpha)
            {
                blit::screen.pen = blit::Pen(fireworks[fw].r, fireworks[fw].g, fireworks[fw].b, (int)alpha);
                blit::screen.rectangle(dest);
            }

            // draw rockets trail
            for (uint8_t k = 0; k < _numof_tail_elements; k++)
            {

                dest.x = fireworks[fw].property.tail[k].x;
                dest.y = fireworks[fw].property.tail[k].y;
                dest.w = 3;
                dest.h = 3;

                // change size of each tail
                float size = (float) k / _numof_tail_elements;
                size *= 3;

                if (size >= 1)
                {

                    dest.w = 3 - size;
                    dest.h = 3 - size;
                }

                // TODO calculate alpha
                blit::screen.pen = blit::Pen(fireworks[fw].r, fireworks[fw].g, fireworks[fw].b, (int)alpha);
                blit::screen.rectangle(dest);
            }

            // draw particles
            for (uint8_t j = 0; j < _numof_particles; j++)
            {
                dest.x = fireworks[fw].particles[j].pos.x;
                dest.y = fireworks[fw].particles[j].pos.y;
                dest.w = 4;
                dest.h = 4;

                // draw particles if the firework has exploded (==has no alpha)
                if (!alpha)
                {

                    // TODO calculate alpha
                    blit::screen.pen = blit::Pen(fireworks[fw].r, fireworks[fw].g, fireworks[fw].b, (int)fireworks[fw].particles[j].alpha);
                    blit::screen.rectangle(dest);

                    // draw particle tails
                    for (uint8_t k = 0; k < _numof_tail_elements; k++)
                    {
                        dest.x = fireworks[fw].particles[j].tail[k].x;
                        dest.y = fireworks[fw].particles[j].tail[k].y;
                        dest.w = 3;
                        dest.h = 3;

                        // change size of each tail
                        float size = (float) k / _numof_tail_elements;
                        size *= 3;

                        if (size >= 1)
                        {
                            dest.w = 3 - size;
                            dest.h = 3 - size;
                        }

                        // TODO calculate alpha
                        blit::screen.pen = blit::Pen(fireworks[fw].r, fireworks[fw].g, fireworks[fw].b, (int)fireworks[fw].particles[j].alpha);
                        blit::screen.rectangle(dest);
                    }
                }
            }
        }
    }

    void FireworksLayer::initFirework(uint8_t i)
    {
        float vel_scale = _rising_height * _height;

        // initialise / set up firework properties
        fireworks[i].property.pos.x = rand() % _width;
        fireworks[i].property.pos.y = _height;
        fireworks[i].property.vel.x = sinf(rand());
        fireworks[i].property.vel.y = (float) (rand() % 5 + vel_scale)  * -1;
        fireworks[i].property.alpha = 255;
        fireworks[i].r = rand() % 155 + 100;
        fireworks[i].g = rand() % 155 + 100;
        fireworks[i].b = rand() % 155 + 100;

        // move tail according new position of rocket
        for (uint8_t k = 0; k < _numof_tail_elements; k++) {

            fireworks[i].property.tail[k].x = fireworks[i].property.pos.x;
            fireworks[i].property.tail[k].y = fireworks[i].property.pos.y;
        }

        for (uint8_t j = 0; j < _numof_particles; j++) {

            int angle = rand();
            float vel = (float) rand() / RAND_MAX * 5; //random velocity for explosion
            float flip = (float) rand() / RAND_MAX;

            // set up rockets particles
            fireworks[i].particles[j].pos.x = fireworks[i].property.pos.x;
            fireworks[i].particles[j].pos.y = fireworks[i].property.pos.y;
            fireworks[i].particles[j].vel.x = sinf(angle);	//will explode is a circular fashion
            fireworks[i].particles[j].vel.y = cosf(angle);	//will explode is a circular fashion
            fireworks[i].particles[j].alpha = 255;
            fireworks[i].particles[j].alpha_rate = static_cast<double>(flip) > double(.8) ? 4 : 6;

            fireworks[i].particles[j].vel.multiply(vel);    //change to a random velocity, so it will not explode in a perfect circle

            // adjust tail based on current position
            for (uint8_t k = 0; k < _numof_tail_elements; k++)
            {
                fireworks[i].particles[j].tail[k].x = fireworks[i].property.pos.x;
                fireworks[i].particles[j].tail[k].y = fireworks[i].property.pos.y;
            }
        }

        // always give the particle 0 the smallest alpha rate to determine when all other particles have faded out.
        fireworks[i].particles[0].alpha_rate = 4;
    }
}
// EOF