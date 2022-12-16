// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework
#pragma once

#include <list>

#include "libs/DANG/src/layer/Layer.hpp"
#include "32blit.hpp"

namespace pnk
{
class FireworksLayer : public dang::Layer
    {
    public:
        FireworksLayer();
        ~FireworksLayer() override;

        void    update(uint32_t dt, const dang::Gear& gear) override;
        void    render(const dang::Gear& gear) override;

    protected:
        explicit FireworksLayer(Layer::E_TYPE type) : Layer(type) {};

        void initFirework(uint8_t i);

        // hack remove me
        static const uint16_t _height = 100;    // was the real height, but with gravity this needs to be tested. works best with 100 for now
        static const uint16_t _width = 320;     // real width of screen

        static const uint8_t _numof_rockets = 4;                // number of rockets in the air at any given moment
        static const uint8_t _numof_particles = 50;             // how many particles to spread
        constexpr static const float _rising_height = 0.015;    // how high the firework will rise
        static const uint8_t _numof_tail_elements = 3;          // number of elements which together build a tail
        const dang::Vector2F gravity{0,0.2};			//gravity effecting the particles

        struct particle {
            dang::Vector2F  pos;			            // position
            dang::Vector2F  vel;			            // velocity
            dang::Vector2I  tail[_numof_tail_elements];	// array of previous positions
            float alpha;					            // currrent alpha value of this particle
            float alpha_rate;				            // rate at which the alpha drops per frame (fade out time)
        };

        struct firework {
            particle property;				        // contains rocket position, vel and accel properties
            particle particles[_numof_particles];	// particles rocket will explode into
            uint8_t r;								// rocket colour
            uint8_t g;
            uint8_t b;
        };

        firework fireworks[_numof_rockets];	// the array of rockets for one firework
    };
}

