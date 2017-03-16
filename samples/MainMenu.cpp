// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "MainMenu.h"
#include "SpritesSample.h"
#include "GUISample.h"
#include "RTSample.h"
#include "AnimationsSample.h"
#include "InputSample.h"
#include "SoundSample.h"
#include "PerspectiveSample.h"

using namespace std;
using namespace ouzel;

MainMenu::MainMenu(Samples& aSamples):
    samples(aSamples),
    gitHubButton("button.png", "button_selected.png", "button_down.png", "", "GitHub", "arial.fnt", Color(20, 0, 0, 255), Color::BLACK, Color::BLACK),
    spritesButton("button.png", "button_selected.png", "button_down.png", "", "Sprites", "arial.fnt", Color(20, 0, 0, 255), Color::BLACK, Color::BLACK),
    guiButton("button.png", "button_selected.png", "button_down.png", "", "GUI", "arial.fnt", Color(20, 0, 0, 255), Color::BLACK, Color::BLACK),
    renderTargetButton("button.png", "button_selected.png", "button_down.png", "", "Render target", "arial.fnt", Color(20, 0, 0, 255), Color::BLACK, Color::BLACK),
    animationsButton("button.png", "button_selected.png", "button_down.png", "", "Animations", "arial.fnt", Color(20, 0, 0, 255), Color::BLACK, Color::BLACK),
    inputButton("button.png", "button_selected.png", "button_down.png", "", "Input", "arial.fnt", Color(20, 0, 0, 255), Color::BLACK, Color::BLACK),
    soundButton("button.png", "button_selected.png", "button_down.png", "", "Sound", "arial.fnt", Color(20, 0, 0, 255), Color::BLACK, Color::BLACK),
    perspectiveButton("button.png", "button_selected.png", "button_down.png", "", "Perspective", "arial.fnt", Color(20, 0, 0, 255), Color::BLACK, Color::BLACK)
{
    eventHandler.uiHandler = bind(&MainMenu::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&MainMenu::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    layer.setScene(this);

    camera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    camera.setTargetContentSize(Size2(400.0f, 600.0f));
    camera.setLayer(&layer);

    menu.setNode(&menuNode);
    menuNode.setParent(&layer);

    gitHubButton.setMenu(&menu);
    gitHubButton.setNode(&gitHubButtonNode);
    gitHubButtonNode.setPosition(Vector2(0.0f, 120.0f));
    gitHubButtonNode.setParent(&menuNode);

    spritesButton.setMenu(&menu);
    spritesButton.setNode(&spritesButtonNode);
    spritesButtonNode.setPosition(Vector2(0.0f, 80.0f));
    spritesButtonNode.setParent(&menuNode);

    guiButton.setMenu(&menu);
    guiButton.setNode(&guiButtonNode);
    guiButtonNode.setPosition(Vector2(0.0f, 40.0f));
    guiButtonNode.setParent(&menuNode);

    renderTargetButton.setMenu(&menu);
    renderTargetButton.setNode(&renderTargetButtonNode);
    renderTargetButtonNode.setPosition(Vector2(0.0f, 0.0f));
    renderTargetButtonNode.setParent(&menuNode);

    animationsButton.setMenu(&menu);
    animationsButton.setNode(&animationsButtonNode);
    animationsButtonNode.setPosition(Vector2(0.0f, -40.0f));
    animationsButtonNode.setParent(&menuNode);

    inputButton.setMenu(&menu);
    inputButton.setNode(&inputButtonNode);
    inputButtonNode.setPosition(Vector2(0.0f, -80.0f));
    inputButtonNode.setParent(&menuNode);

    soundButton.setMenu(&menu);
    soundButton.setNode(&soundButtonNode);
    soundButtonNode.setPosition(Vector2(0.0f, -120.0f));
    soundButtonNode.setParent(&menuNode);

    perspectiveButton.setMenu(&menu);
    perspectiveButton.setNode(&perspectiveButtonNode);
    perspectiveButtonNode.setPosition(Vector2(0.0f, -160.0f));
    perspectiveButtonNode.setParent(&menuNode);
}

bool MainMenu::handleKeyboard(Event::Type type, const KeyboardEvent& event)
{
    if (event.key == ouzel::input::KeyboardKey::ESCAPE)
    {
        if (type == Event::Type::KEY_DOWN)
        {
            sharedApplication->exit();
        }

        return false;
    }

    return true;
}

bool MainMenu::handleUI(Event::Type type, const UIEvent& event)
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.component == &gitHubButton)
        {
            sharedApplication->openURL("https://github.com/elnormous/ouzel");
        }
        else if (event.component == &spritesButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new SpritesSample(samples)));
        }
        else if (event.component == &guiButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new GUISample(samples)));
        }
        else if (event.component == &renderTargetButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new RTSample(samples)));
        }
        else if (event.component == &animationsButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new AnimationsSample(samples)));
        }
        else if (event.component == &inputButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new InputSample(samples)));
        }
        else if (event.component == &soundButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new SoundSample(samples)));
        }
        else if (event.component == &perspectiveButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new PerspectiveSample(samples)));
        }
    }

    return true;
}
