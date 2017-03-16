// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class InputSample: public ouzel::scene::Scene
{
public:
    InputSample(Samples& aSamples);

private:
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event);
    bool handleMouse(ouzel::Event::Type type, const ouzel::MouseEvent& event);
    bool handleTouch(ouzel::Event::Type type, const ouzel::TouchEvent& event);
    bool handleGamepad(ouzel::Event::Type type, const ouzel::GamepadEvent& event);
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;

    Samples& samples;

    ouzel::scene::Layer layer;
    ouzel::scene::Camera camera;
    
    ouzel::EventHandler eventHandler;

    ouzel::scene::Node showHideButtonNode;
    ouzel::gui::Button showHideButton;
    ouzel::scene::ParticleSystem flameParticleSystem;
    ouzel::scene::Node flame;

    ouzel::scene::Layer guiLayer;
    ouzel::scene::Camera guiCamera;
    ouzel::scene::Node menuNode;
    ouzel::gui::Menu menu;
    ouzel::scene::Node backButtonNode;
    ouzel::gui::Button backButton;
};
