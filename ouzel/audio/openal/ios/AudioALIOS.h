// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "core/CompileConfig.h"

#if OUZEL_PLATFORM_IOS && OUZEL_SUPPORTS_OPENAL

#include "audio/openal/AudioAL.h"

namespace ouzel
{
    namespace audio
    {
        class AudioALIOS: public AudioAL
        {
        public:
            virtual bool init() override;
        };
    }
}

#endif
