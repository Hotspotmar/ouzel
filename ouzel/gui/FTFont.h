#pragma once
#include "gui/BMFont.h"


namespace ouzel
{
    enum UTFChars
    {
        ASCII = 1,
        ASCIIPLUS = 1 << 1

    };
    class FTFont : public BMFont
    {
    public:
        FTFont();
        FTFont(const std::string& filename, uint16_t pt, UTFChars flag = ASCII);

    protected:
        bool parseFont(const std::string& filename, uint16_t pt, UTFChars flag = ASCII);
    };
} 