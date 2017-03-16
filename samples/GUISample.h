// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class GUISample: public ouzel::scene::Scene
{
public:
    GUISample(Samples& aSamples);

private:
    bool handleGamepad(ouzel::Event::Type type, const ouzel::GamepadEvent& event);
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event) const;

    Samples& samples;

    ouzel::EventHandler eventHandler;

    ouzel::scene::Node buttonNode;
    std::unique_ptr<ouzel::gui::Button> button;
    ouzel::scene::Node fullscreenButtonNode;
    std::unique_ptr<ouzel::gui::Button> fullscreenButton;
    ouzel::scene::Node checkBoxNode;
    std::unique_ptr<ouzel::gui::CheckBox> checkBox;

    ouzel::scene::Node label1Node;
    std::unique_ptr<ouzel::gui::Label> label1;
    ouzel::scene::Node label2Node;
    std::unique_ptr<ouzel::gui::Label> label2;
    ouzel::scene::Node label3Node;
    std::unique_ptr<ouzel::gui::Label> label3;

    ouzel::scene::Layer guiLayer;
    ouzel::scene::Camera guiCamera;
    ouzel::scene::Node menuNode;
    ouzel::gui::Menu menu;
    ouzel::scene::Node backButtonNode;
    ouzel::gui::Button backButton;
};
