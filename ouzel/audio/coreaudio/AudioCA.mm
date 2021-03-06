// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "core/CompileConfig.h"

#if OUZEL_SUPPORTS_COREAUDIO

#if OUZEL_PLATFORM_IOS || OUZEL_PLATFORM_TVOS
#import <AVFoundation/AVFoundation.h>
#include <AudioToolbox/AudioToolbox.h>
#endif

#include "AudioCA.h"
#include "utils/Log.h"

#if OUZEL_PLATFORM_MACOS
static const AudioObjectPropertyAddress devlistAddress = {
    kAudioHardwarePropertyDevices,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMaster
};

static OSStatus deviceListChanged(AudioObjectID, UInt32, const AudioObjectPropertyAddress*, void*)
{
    return 0;
}

static const AudioObjectPropertyAddress aliveAddress =
{
    kAudioDevicePropertyDeviceIsAlive,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMaster
};

static OSStatus deviceUnplugged(AudioObjectID, UInt32, const AudioObjectPropertyAddress*, void*)
{
    return noErr;
}
#endif

static OSStatus outputCallback(void* inRefCon,
                               AudioUnitRenderActionFlags*,
                               const AudioTimeStamp*,
                               UInt32, UInt32,
                               AudioBufferList* ioData)
{
    ouzel::audio::AudioCA* audioCA = static_cast<ouzel::audio::AudioCA*>(inRefCon);

    audioCA->outputCallback(ioData);

    return noErr;
}

namespace ouzel
{
    namespace audio
    {
        AudioCA::AudioCA():
            Audio(Audio::Driver::COREAUDIO)
        {
        }

        AudioCA::~AudioCA()
        {
            if (audioUnit)
            {
                AudioOutputUnitStop(audioUnit);

                AURenderCallbackStruct callback;
                callback.inputProc = nullptr;
                callback.inputProcRefCon = nullptr;

                const AudioUnitElement bus = 0;

                AudioUnitSetProperty(audioUnit,
                                     kAudioUnitProperty_SetRenderCallback,
                                     kAudioUnitScope_Input, bus, &callback, sizeof(callback));

                AudioComponentInstanceDispose(audioUnit);
            }

#if OUZEL_PLATFORM_MACOS
            AudioObjectAddPropertyListener(kAudioObjectSystemObject, &devlistAddress, deviceListChanged, this);

            if (deviceId)
            {
                AudioObjectRemovePropertyListener(deviceId, &aliveAddress, deviceUnplugged, this);
            }
#endif
        }

        bool AudioCA::init()
        {
            if (!Audio::init())
            {
                return false;
            }

            OSStatus result;

#if OUZEL_PLATFORM_MACOS
            AudioObjectAddPropertyListener(kAudioObjectSystemObject, &devlistAddress, deviceListChanged, this);

            AudioObjectPropertyAddress addr = {
                0,
                kAudioObjectPropertyScopeGlobal,
                kAudioObjectPropertyElementMaster
            };

            UInt32 size = sizeof(AudioDeviceID);
            addr.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
            result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &addr,
                                                         0, NULL, &size, &deviceId);

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to get CoreAudio output device, error: " << result;
                return false;
            }

            addr.mSelector = kAudioDevicePropertyDeviceIsAlive;
            addr.mScope = kAudioDevicePropertyScopeOutput;

            UInt32 alive = 0;
            size = sizeof(alive);
            result = AudioObjectGetPropertyData(deviceId, &addr, 0, NULL, &size, &alive);

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to get CoreAudio device status, error: " << result;
                return false;
            }

            if (!alive)
            {
                Log(Log::Level::ERR) << "Requested CoreAudio device is not alive, error: " << result;
                return false;
            }

            addr.mSelector = kAudioDevicePropertyHogMode;
            pid_t pid = 0;
            size = sizeof(pid);
            result = AudioObjectGetPropertyData(deviceId, &addr, 0, NULL, &size, &pid);

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to check if CoreAudio device is in hog mode, error: " << result;
                return false;
            }

            if (pid != -1)
            {
                Log(Log::Level::ERR) << "Requested CoreAudio device is being hogged";
                return false;
            }
#else
            [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryAmbient error:Nil];
#endif

            AudioComponentDescription desc;
            desc.componentType = kAudioUnitType_Output;

#if OUZEL_PLATFORM_MACOS
            desc.componentSubType = kAudioUnitSubType_DefaultOutput;
#elif OUZEL_PLATFORM_IOS || OUZEL_PLATFORM_TVOS
            desc.componentSubType = kAudioUnitSubType_RemoteIO;
#endif

            desc.componentManufacturer = kAudioUnitManufacturer_Apple;
            desc.componentFlags = 0;
            desc.componentFlagsMask = 0;

            audioComponent = AudioComponentFindNext(NULL, &desc);

            if (!audioComponent)
            {
                Log(Log::Level::ERR) << "Failed to find requested CoreAudio component";
                return false;
            }

#if OUZEL_PLATFORM_MACOS
            result = AudioObjectAddPropertyListener(deviceId, &aliveAddress, deviceUnplugged, this);

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to add CoreAudio property listener, error: " << result;
                return false;
            }
#endif

            result = AudioComponentInstanceNew(audioComponent, &audioUnit);

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to create CoreAudio component instance, error: " << result;
                return false;
            }

#if OUZEL_PLATFORM_MACOS
            result = AudioUnitSetProperty(audioUnit,
                                          kAudioOutputUnitProperty_CurrentDevice,
                                          kAudioUnitScope_Global, 0,
                                          &deviceId,
                                          sizeof(AudioDeviceID));

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to set CoreAudio unit property, error: " << result;
                return false;
            }
#endif

            const AudioUnitElement bus = 0;

            AudioStreamBasicDescription streamDescription;
            streamDescription.mSampleRate = samplesPerSecond;
            streamDescription.mFormatID = kAudioFormatLinearPCM;
            streamDescription.mFormatFlags = kLinearPCMFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
            streamDescription.mChannelsPerFrame = channels;
            streamDescription.mFramesPerPacket = 1;
            streamDescription.mBitsPerChannel = 16;
            streamDescription.mBytesPerFrame = streamDescription.mBitsPerChannel * streamDescription.mChannelsPerFrame / 8;
            streamDescription.mBytesPerPacket = streamDescription.mBytesPerFrame * streamDescription.mFramesPerPacket;
            streamDescription.mReserved = 0;

            result = AudioUnitSetProperty(audioUnit,
                                          kAudioUnitProperty_StreamFormat,
                                          kAudioUnitScope_Input, bus, &streamDescription, sizeof(streamDescription));

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to set CoreAudio unit stream format, error: " << result;
                return false;
            }

            AURenderCallbackStruct callback;
            callback.inputProc = ::outputCallback;
            callback.inputProcRefCon = this;
            result = AudioUnitSetProperty(audioUnit,
                                          kAudioUnitProperty_SetRenderCallback,
                                          kAudioUnitScope_Input, bus, &callback, sizeof(callback));

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to set CoreAudio unit utput callback, error: " << result;
                return false;
            }

            result = AudioUnitInitialize(audioUnit);

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to initialize CoreAudio unit, error: " << result;
                return false;
            }

            result = AudioOutputUnitStart(audioUnit);

            if (result != noErr)
            {
                Log(Log::Level::ERR) << "Failed to start CoreAudio output unit, error: " << result;
                return false;
            }

            return true;
        }

        void AudioCA::outputCallback(AudioBufferList* ioData)
        {
            for (UInt32 i = 0; i < ioData->mNumberBuffers; ++i)
            {
                AudioBuffer* buffer = &ioData->mBuffers[i];

                std::vector<uint8_t> data = getData(buffer->mDataByteSize);

                std::copy(data.begin(), data.end(), static_cast<uint8_t*>(buffer->mData));
            }
        }
    } // namespace audio
} // namespace ouzel

#endif
