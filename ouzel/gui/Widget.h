// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "scene/Component.h"

namespace ouzel
{
    namespace gui
    {
        class Menu;

        class Widget: public scene::Component
        {
            friend Menu;
        public:
            Widget();
            virtual ~Widget();

            void setMenu(Menu* newMenu);

            virtual void setEnabled(bool newEnabled);
            bool isEnabled() const { return enabled; }

            bool isSelected() const { return selected; }

        protected:
            virtual void setSelected(bool newSelected);

            Menu* menu = nullptr;

            bool enabled = true;
            bool selected = false;
        };
    } // namespace gui
} // namespace ouzel
