// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class MainMenu: public ouzel::scene::Scene
{
public:
    MainMenu(Samples& aSamples);

private:
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event);
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event);

    Samples& samples;

    ouzel::scene::Layer layer;
    ouzel::scene::Camera camera;

    ouzel::scene::Node menuNode;
    ouzel::gui::Menu menu;

    ouzel::EventHandler eventHandler;

    ouzel::scene::Node gitHubButtonNode;
    ouzel::gui::Button gitHubButton;
    ouzel::scene::Node spritesButtonNode;
    ouzel::gui::Button spritesButton;
    ouzel::scene::Node guiButtonNode;
    ouzel::gui::Button guiButton;
    ouzel::scene::Node renderTargetButtonNode;
    ouzel::gui::Button renderTargetButton;
    ouzel::scene::Node animationsButtonNode;
    ouzel::gui::Button animationsButton;
    ouzel::scene::Node inputButtonNode;
    ouzel::gui::Button inputButton;
    ouzel::scene::Node soundButtonNode;
    ouzel::gui::Button soundButton;
    ouzel::scene::Node perspectiveButtonNode;
    ouzel::gui::Button perspectiveButton;
};
