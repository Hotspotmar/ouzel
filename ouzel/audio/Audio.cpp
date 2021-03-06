// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "Audio.h"
#include "SoundResource.h"
#include "math/MathUtils.h"

namespace ouzel
{
    namespace audio
    {
        Audio::Audio(Driver aDriver):
            driver(aDriver)
        {
        }

        Audio::~Audio()
        {
        }

        bool Audio::init()
        {
            return true;
        }

        bool Audio::update()
        {
            return true;
        }

        void Audio::deleteResource(SoundResource* resource)
        {
            std::lock_guard<std::mutex> lock(resourceMutex);

            std::vector<std::unique_ptr<SoundResource>>::iterator i = std::find_if(resources.begin(), resources.end(), [resource](const std::unique_ptr<SoundResource>& ptr) {
                return ptr.get() == resource;
            });

            if (i != resources.end())
            {
                resourceDeleteSet.push_back(std::move(*i));
                resources.erase(i);
            }
        }

        void Audio::setListenerPosition(const Vector3& newPosition)
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            listenerPosition = newPosition;

            dirty |= DIRTY_LISTENER_POSITION;
        }

        void Audio::setListenerRotation(const Quaternion& newRotation)
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            listenerRotation = newRotation;

            dirty |= DIRTY_LISTENER_ROTATION;
        }

        std::vector<uint8_t> Audio::getData(uint32_t size)
        {
            std::vector<uint8_t> data(size);
            std::vector<int32_t> buffer(size / 2, 0);

            std::lock_guard<std::mutex> lock(resourceMutex);

            for (const auto& resource : resources)
            {
                std::vector<uint8_t> resourceData = resource->getData(size, channels, samplesPerSecond);

                int16_t* resourceDataPtr = reinterpret_cast<int16_t*>(resourceData.data());
                int32_t* bufferPtr = buffer.data();

                for (uint32_t i = 0; i < resourceData.size() / sizeof(int16_t) && i < data.size() / sizeof(int16_t); ++i)
                {
                    // mix the resource sound into the buffer
                    *bufferPtr += *resourceDataPtr;

                    ++resourceDataPtr;
                    ++bufferPtr;
                }
            }

            int16_t* dataPtr = reinterpret_cast<int16_t*>(data.data());
            int32_t* bufferPtr = buffer.data();

            for (uint32_t i = 0; i < data.size() / sizeof(int16_t); ++i)
            {
                *dataPtr = static_cast<int16_t>(clamp(*bufferPtr, static_cast<int32_t>(std::numeric_limits<int16_t>::min()), static_cast<int32_t>(std::numeric_limits<int16_t>::max())));

                ++dataPtr;
                ++bufferPtr;
            }

            return data;
        }

        SoundResource* Audio::createSound()
        {
            std::lock_guard<std::mutex> lock(resourceMutex);

            SoundResource* sound = new SoundResource();
            resources.push_back(std::unique_ptr<SoundResource>(sound));
            return sound;
        }
    } // namespace audio
} // namespace ouzel
