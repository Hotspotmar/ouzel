// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <jni.h>
#include <memory>
#include "core/android/EngineAndroid.h"
#include "core/android/WindowAndroid.h"
#include "core/Engine.h"
#include "input/Input.h"

std::unique_ptr<ouzel::EngineAndroid> engine;

extern "C" JNIEXPORT jint JNIEXPORT JNI_OnLoad(JavaVM* javaVM, void*)
{
    engine.reset(new ouzel::EngineAndroid(javaVM));
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNIEXPORT JNI_OnUnload(JavaVM*, void*)
{
    engine.reset();
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_onCreate(JNIEnv*, jclass, jobject mainActivity)
{
    engine->onCreate(mainActivity);
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_setSurface(JNIEnv*, jclass, jobject surface)
{
    engine->setSurface(surface);
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_onSurfaceChanged(JNIEnv*, jclass, jobject surface, jint width, jint height)
{
    engine->setSurface(surface);

    if (ouzel::sharedEngine)
    {
        ouzel::WindowAndroid* windowAndroid = static_cast<ouzel::WindowAndroid*>(ouzel::sharedEngine->getWindow());
        windowAndroid->handleResize(ouzel::Size2(static_cast<float>(width), static_cast<float>(height)));
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_onStart(JNIEnv*, jclass)
{
    engine->run();
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_onPause(JNIEnv*, jclass)
{
    engine->pause();
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_onResume(JNIEnv*, jclass)
{
    engine->resume();
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_handleActionDown(JNIEnv*, jclass, jint pointerId, jfloat x, jfloat y)
{
    engine->getInput()->touchBegin(static_cast<uint64_t>(pointerId),
                                   ouzel::sharedEngine->getWindow()->convertWindowToNormalizedLocation(ouzel::Vector2(x, y)));
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_handleActionMove(JNIEnv*, jclass, jint pointerId, jfloat x, jfloat y)
{
    engine->getInput()->touchMove(static_cast<uint64_t>(pointerId),
                                  ouzel::sharedEngine->getWindow()->convertWindowToNormalizedLocation(ouzel::Vector2(x, y)));
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_handleActionUp(JNIEnv*, jclass, jint pointerId, jfloat x, jfloat y)
{
    engine->getInput()->touchEnd(static_cast<uint64_t>(pointerId),
                                 ouzel::sharedEngine->getWindow()->convertWindowToNormalizedLocation(ouzel::Vector2(x, y)));
}

extern "C" JNIEXPORT void JNICALL Java_org_ouzelengine_OuzelLibJNIWrapper_handleActionCancel(JNIEnv*, jclass, jint pointerId, jfloat x, jfloat y)
{
    engine->getInput()->touchCancel(static_cast<uint64_t>(pointerId),
                                    ouzel::sharedEngine->getWindow()->convertWindowToNormalizedLocation(ouzel::Vector2(x, y)));
}
