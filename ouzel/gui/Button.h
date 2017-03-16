// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <functional>
#include "gui/Widget.h"
#include "scene/Sprite.h"
#include "scene/TextDrawable.h"
#include "events/Event.h"
#include "events/EventHandler.h"
#include "math/Color.h"

namespace ouzel
{
    namespace gui
    {
        class Button: public Widget
        {
        public:
            Button();
            Button(const std::string& normalImage,
                   const std::string& selectedImage,
                   const std::string& pressedImage,
                   const std::string& disabledImage,
                   const std::string& label = "",
                   const std::string& font = "",
                   const Color& aLabelColor = Color::WHITE,
                   const Color& aLabelSelectedColor = Color::WHITE,
                   const Color& aLabelPressedColor = Color::WHITE,
                   const Color& aLabelDisabledColor = Color::WHITE);

            virtual void setEnabled(bool newEnabled) override;

        protected:
            virtual void setSelected(bool newSelected) override;

            bool handleUI(Event::Type type, const UIEvent& event);

            void updateSprite();

            std::unique_ptr<scene::Sprite> normalSprite;
            std::unique_ptr<scene::Sprite> selectedSprite;
            std::unique_ptr<scene::Sprite> pressedSprite;
            std::unique_ptr<scene::Sprite> disabledSprite;
            BMFont labelFont;

            EventHandler eventHandler;

            bool pointerOver = false;
            bool pressed = false;

            Color labelColor = Color::WHITE;
            Color labelSelectedColor = Color::WHITE;
            Color labelPressedColor = Color::WHITE;
            Color labelDisabledColor = Color::WHITE;
        };
    } // namespace gui
} // namespace ouzel
