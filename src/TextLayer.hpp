// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <Layer.hpp>
#include <libs/32blit-sdk/32blit/types/rect.hpp>

namespace pnk
{
    class TextLayer : public dang::Layer
    {
    public:
        TextLayer();
        ~TextLayer() override;

        void    update(uint32_t dt, const dang::Gear& gear) override;
        void    render(const dang::Gear& gear) override;

        void    setText(const std::string_view& txt);
        void    setTtl(uint32_t ttl_ms, std::function<void (void)> cb);

    protected:
        explicit TextLayer(Layer::E_TYPE type) : Layer(type) {};

        std::vector<std::string_view> _text;
        blit::Rect _whitishRect{0,0,0,0};

        uint32_t _ttl{0};
        std::function<void (void)> _ttl_cb{nullptr};

        uint32_t _start_ms{0};

    };

}

