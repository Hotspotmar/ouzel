// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <cstdint>
#include <vector>
#include "Stream.h"

struct stb_vorbis;

namespace ouzel
{
    namespace audio
    {
        class StreamVorbis: public Stream
        {
        public:
            StreamVorbis(const std::vector<uint8_t>& data);
            virtual ~StreamVorbis();
            virtual void reset() override;

            stb_vorbis* getVorbisStream() const { return vorbisStream; }

        private:
            stb_vorbis* vorbisStream = nullptr;
        };
    } // namespace audio
} // namespace ouzel
