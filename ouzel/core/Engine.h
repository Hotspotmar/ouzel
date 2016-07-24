// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <memory>
#include <list>
#include <set>
#include "utils/Types.h"
#include "utils/Noncopyable.h"
#include "graphics/Renderer.h"
#include "scene/SceneManager.h"
#include "files/FileSystem.h"
#include "input/Input.h"
#include "events/EventDispatcher.h"
#include "core/UpdateCallback.h"
#include "core/Settings.h"

void ouzelMain(const std::vector<std::string>& args);

namespace ouzel
{
    extern Engine* sharedEngine;

    class Engine: public Noncopyable
    {
    public:
        Engine();
        virtual ~Engine();

        static std::set<graphics::Renderer::Driver> getAvailableDrivers();

        bool init(Settings& newSettings);
        const Settings& getSettings() const { return settings; }

        const EventDispatcherPtr& getEventDispatcher() const { return eventDispatcher; }
        const CachePtr& getCache() const { return cache; }
        const WindowPtr& getWindow() const { return window; }
        const graphics::RendererPtr& getRenderer() const { return renderer; }
        const audio::AudioPtr& getAudio() const { return audio; }
        const scene::SceneManagerPtr& getSceneManager() const { return sceneManager; }
        const FileSystemPtr& getFileSystem() const { return fileSystem; }
        const input::InputPtr& getInput() const { return input; }
        const LocalizationPtr& getLocalization() const { return localization; }

        void exit();

        void begin();
        void end();
        void pause();
        void resume();
        bool run();

        float getTargetFPS() const { return targetFPS; }
        float getFPS() const { return currentFPS; }

        void scheduleUpdate(const UpdateCallback& callback);
        void unscheduleUpdate(const UpdateCallback& callback);

    protected:
        Settings settings;

        EventDispatcherPtr eventDispatcher;
        input::InputPtr input;
        CachePtr cache;
        WindowPtr window;
        FileSystemPtr fileSystem;
        LocalizationPtr localization;
        graphics::RendererPtr renderer;
        audio::AudioPtr audio;
        scene::SceneManagerPtr sceneManager;

        float targetFPS;
        uint64_t targetFrameInterval;
        float currentFPS = 0.0f;
        uint64_t previousFrameTime;

        std::list<const UpdateCallback*> updateCallbacks;
        std::list<const UpdateCallback*>::iterator updateCallbackIterator;
        bool updateCallbackDeleted = false;

        bool running = false;
        bool active = true;
    };
}
