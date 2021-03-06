// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

class MainMenu: public ouzel::scene::Scene
{
public:
    MainMenu();

private:
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event);
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event);

    std::unique_ptr<ouzel::scene::Layer> layer;
    std::unique_ptr<ouzel::scene::Camera> camera;

    std::unique_ptr<ouzel::gui::Menu> menu;

    ouzel::EventHandler eventHandler;
    std::unique_ptr<ouzel::gui::Button> gitHubButton;
    std::unique_ptr<ouzel::gui::Button> spritesButton;
    std::unique_ptr<ouzel::gui::Button> guiButton;
    std::unique_ptr<ouzel::gui::Button> renderTargetButton;
    std::unique_ptr<ouzel::gui::Button> animationsButton;
    std::unique_ptr<ouzel::gui::Button> inputButton;
    std::unique_ptr<ouzel::gui::Button> soundButton;
    std::unique_ptr<ouzel::gui::Button> perspectiveButton;
};
