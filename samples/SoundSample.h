// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class SoundSample: public ouzel::scene::Scene
{
public:
    SoundSample(Samples& aSamples);

private:
    bool handleGamepad(ouzel::Event::Type type, const ouzel::GamepadEvent& event);
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event) const;

    Samples& samples;

    std::shared_ptr<ouzel::audio::Sound> test8BitSound;
    std::shared_ptr<ouzel::audio::Sound> test24BitSound;
    std::shared_ptr<ouzel::audio::Sound> jumpSound;
    std::shared_ptr<ouzel::audio::Sound> ambientSound;

    ouzel::scene::Node test8BitButtonNode;
    ouzel::gui::Button test8BitButton;
    ouzel::scene::Node test24BitButtonNode;
    ouzel::gui::Button test24BitButton;
    ouzel::scene::Node jumpButtonNode;
    ouzel::gui::Button jumpButton;
    ouzel::scene::Node ambientButtonNode;
    ouzel::gui::Button ambientButton;
    ouzel::EventHandler eventHandler;

    ouzel::scene::Layer guiLayer;
    ouzel::scene::Camera guiCamera;
    ouzel::scene::Node menuNode;
    ouzel::gui::Menu menu;
    ouzel::scene::Node backButtonNode;
    ouzel::gui::Button backButton;
};
