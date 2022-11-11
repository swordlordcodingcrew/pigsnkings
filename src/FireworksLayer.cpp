// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "Gear.hpp"
#include "FireworksLayer.h"
#include "libs/DANG/src/layer/Layer.hpp"

namespace pnk
{
    FireworksLayer::FireworksLayer() : Layer(E_TYPE::LT_UNDEFINED, {0,0}, 200, "FW", true, true)
    {
        // setup fireworks

        //define the world gravity
        gravity.x = 0;
        gravity.y = 0.2;

        //populate fireworks property's
        for (uint8_t i = 0; i < _numof_fireworks; i++)
        {
            initFirework(i);
        }
    }

    FireworksLayer::~FireworksLayer()
    {
    }

    void FireworksLayer::update(uint32_t dt, const dang::Gear &gear)
    {
        for (uint8_t i = 0; i < _numof_fireworks; i++)
        {
            //fireworks has reached its peak
            if (fireworks[i].property.vel.y >= 0)
            {
                fireworks[i].property.alpha = 0;

                //calculate particle physics
                for (uint8_t j = 0; j < _numof_particles; j++)
                {
                    if (fireworks[i].particles[j].alpha >= fireworks[i].particles[j].alpha_rate)
                    {
                        fireworks[i].particles[j].alpha -= fireworks[i].particles[j].alpha_rate;
                        //particles have faded away
                    }
                    else
                    {
                        fireworks[i].particles[j].alpha = 0;
                    }

                    //save particle previous position
                    float prev_pos_x = fireworks[i].particles[j].pos.x;
                    float prev_pos_y = fireworks[i].particles[j].pos.y;

                    //change position based on velocity
                    fireworks[i].particles[j].pos.add(fireworks[i].particles[j].vel);

                    //change velocity based on acceleration
                    fireworks[i].particles[j].vel.add(gravity);

                    //slow down particle after ever frame
                    fireworks[i].particles[j].vel.multiply(0.97);

                    //keep track the fireworks particles previous positions
                    for (uint8_t k = 0; k < _numof_trails; k++)
                    {
                        //store current cell position in a temp variable
                        float temp_x = fireworks[i].particles[j].trail[k].x;
                        float temp_y = fireworks[i].particles[j].trail[k].y;

                        //update current cell with the last known prev position
                        fireworks[i].particles[j].trail[k].x = prev_pos_x;
                        fireworks[i].particles[j].trail[k].y = prev_pos_y;

                        //update the prev position for the next iteration of the loop
                        prev_pos_x = temp_x;
                        prev_pos_y = temp_y;
                    }
                }

                //particle 0 always has the shortest alpha_rate. so is alpha is 0, the all particles have a 0 alpha. so reset the firework.
                if (fireworks[i].particles[0].alpha == 0)
                {
                    initFirework(i);
                }
            }
            else
            {
                //save fireworks previous position
                float prev_pos_x = fireworks[i].property.pos.x;
                float prev_pos_y = fireworks[i].property.pos.y;

                //change position based on velocity
                fireworks[i].property.pos.add(fireworks[i].property.vel);

                //change velocity based on acceleration
                fireworks[i].property.vel.add(gravity);

                //make all particle follow the fire work
                for (uint8_t j = 0; j < _numof_particles; j++)
                {
                    fireworks[i].particles[j].pos.x = fireworks[i].property.pos.x;
                    fireworks[i].particles[j].pos.y = fireworks[i].property.pos.y;
                }

                //keep track of fireworks previous positions
                for (uint8_t k = 0; k < _numof_trails; k++)
                {
                    //store current cell position in a temp variable
                    float temp_x = fireworks[i].property.trail[k].x;
                    float temp_y = fireworks[i].property.trail[k].y;

                    //update current cell with the last known prev position
                    fireworks[i].property.trail[k].x = prev_pos_x;
                    fireworks[i].property.trail[k].y = prev_pos_y;

                    //update the prev position for the next iteration of the loop
                    prev_pos_x = temp_x;
                    prev_pos_y = temp_y;
                }
            }
        }
    }

    void FireworksLayer::render(const dang::Gear& gear)
    {
        //draw the fireworks
        for (uint8_t i = 0; i < _numof_fireworks; i++)
        {
            blit::Rect dest;

            //draw firework
            dest.x = fireworks[i].property.pos.x;
            dest.y = fireworks[i].property.pos.y;
            dest.w = 4;
            dest.h = 4;

            float alpha = fireworks[i].property.alpha;
            blit::Pen pen = blit::Pen(fireworks[i].r, fireworks[i].g, fireworks[i].b, (int)alpha);

            //draw fireworks if they have not yet exploded
            if (alpha)
            {
                blit::screen.pen = pen;
                blit::screen.rectangle(dest);
            }

            //draw firework trails
            for (uint8_t k = 0; k < _numof_trails; k++)
            {

                dest.x = fireworks[i].property.trail[k].x;
                dest.y = fireworks[i].property.trail[k].y;
                dest.w = 3;
                dest.h = 3;

                //change size of each trail
                float size = (float) k / _numof_trails;
                size *= 3;

                if (size >= 1)
                {

                    dest.w = 3 - size;
                    dest.h = 3 - size;
                }

                blit::screen.pen = pen;
                blit::screen.rectangle(dest);
            }

            //draw particles
            for (uint8_t j = 0; j < _numof_particles; j++)
            {
                dest.x = fireworks[i].particles[j].pos.x;
                dest.y = fireworks[i].particles[j].pos.y;
                dest.w = 4;
                dest.h = 4;

                //draw particles if the firework has exploded and has no alpha
                if (!alpha)
                {

                    blit::screen.pen = pen;
                    blit::screen.rectangle(dest);

                    //draw particle trails
                    for (uint8_t k = 0; k < _numof_trails; k++)
                    {
                        dest.x = fireworks[i].particles[j].trail[k].x;
                        dest.y = fireworks[i].particles[j].trail[k].y;
                        dest.w = 3;
                        dest.h = 3;

                        //change size of each trail
                        float size = (float) k / _numof_trails;
                        size *= 3;

                        if (size >= 1)
                        {
                            dest.w = 3 - size;
                            dest.h = 3 - size;
                        }

                        blit::screen.pen = pen;
                        blit::screen.rectangle(dest);
                    }
                }
            }
        }
    }

    void FireworksLayer::initFirework(uint8_t i)
    {
        float vel_scale = _rising_height * height;

        //set up firework properties
        fireworks[i].property.pos.x = rand() % width;
        fireworks[i].property.pos.y = height;
        fireworks[i].property.vel.x = sin(rand());
        fireworks[i].property.vel.y = (float) (rand() % 5 + vel_scale)  * -1;
        fireworks[i].property.alpha = 255;
        fireworks[i].r = rand() % 155 + 100;
        fireworks[i].g = rand() % 155 + 100;
        fireworks[i].b = rand() % 155 + 100;

        //record of previous positions for each firework
        for (uint8_t k = 0; k < _numof_trails; k++) {

            fireworks[i].property.trail[k].x = fireworks[i].property.pos.x;
            fireworks[i].property.trail[k].y = fireworks[i].property.pos.y;
        }

        for (uint8_t j = 0; j < _numof_particles; j++) {

            int angle = rand();
            float vel = (float) rand() / RAND_MAX * 5; //random velocity for explosion
            float flip = (float) rand() / RAND_MAX;

            //set up firework's particle
            fireworks[i].particles[j].pos.x = fireworks[i].property.pos.x;
            fireworks[i].particles[j].pos.y = fireworks[i].property.pos.y;
            fireworks[i].particles[j].vel.x = sin(angle);	//will explode is a circular fashion
            fireworks[i].particles[j].vel.y = cos(angle);	//will explode is a circular fashion
            fireworks[i].particles[j].alpha = 255;
            fireworks[i].particles[j].alpha_rate = flip > .8 ? 4 : 6;

            fireworks[i].particles[j].vel.multiply(vel); //change to a random velocity so it wont explode in a perfect circle

            //record of previous positions for each particle
            for (uint8_t k = 0; k < _numof_trails; k++)
            {
                fireworks[i].particles[j].trail[k].x = fireworks[i].property.pos.x;
                fireworks[i].particles[j].trail[k].y = fireworks[i].property.pos.y;
            }
        }

        //always give the first paticle the shortest alpha rate to determine when all other particles have faded out.
        fireworks[i].particles[0].alpha_rate = 4;
    }

}
// EOF