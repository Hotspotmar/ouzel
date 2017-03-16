// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "EditBox.h"

namespace ouzel
{
    namespace gui
    {
        EditBox::EditBox()
        {
        }

        void EditBox::setValue(const std::string& newValue)
        {
            value = newValue;
        }

    } // namespace gui
} // namespace ouzel
