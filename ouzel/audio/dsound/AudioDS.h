// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "core/CompileConfig.h"

#if OUZEL_SUPPORTS_DIRECTSOUND

#include <thread>

#include <dsound.h>
#include "audio/Audio.h"

namespace ouzel
{
    namespace audio
    {
        class AudioDS: public Audio
        {
            friend Engine;
        public:
            virtual ~AudioDS();

            virtual bool update() override;

            IDirectSound8* getDirectSound() const { return directSound; }

        protected:
            AudioDS();
            virtual bool init() override;

            void run();

            IDirectSound8* directSound = nullptr;
            
            IDirectSoundBuffer* primaryBuffer = nullptr;
            IDirectSoundBuffer8* buffer = nullptr;

            uint32_t nextBuffer = 0;

            bool running = true;
#if OUZEL_MULTITHREADED
            std::thread audioThread;
#endif
        };
    } // namespace audio
} // namespace ouzel

#endif
