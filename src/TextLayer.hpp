// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include <Layer.hpp>

namespace pnk
{
    class TextLayer : public dang::Layer
    {
    public:
        TextLayer();
        ~TextLayer() override;

        void    update(uint32_t dt, const dang::Gear& gear) override;
        void    render(const dang::Gear& gear) override;

        void    setText(std::string_view& txt);

    protected:
        explicit TextLayer(Layer::E_TYPE type) : Layer(type) {};

        std::vector<std::string_view> _text;
    };

}

