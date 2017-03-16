// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SoundSample.h"
#include "MainMenu.h"

using namespace std;
using namespace ouzel;

SoundSample::SoundSample(Samples& aSamples):
    samples(aSamples),
    test8BitButton("button.png", "button_selected.png", "button_down.png", "", "8-bit", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK),
    test24BitButton("button.png", "button_selected.png", "button_down.png", "", "24-bit", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK),
    jumpButton("button.png", "button_selected.png", "button_down.png", "", "Jump", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK),
    ambientButton("button.png", "button_selected.png", "button_down.png", "", "Ambient", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK)
{
    eventHandler.gamepadHandler = bind(&SoundSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&SoundSample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&SoundSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    std::shared_ptr<audio::SoundDataWave> test8BitData(new audio::SoundDataWave());
    test8BitData->initFromFile("8-bit.wav");

    test8BitSound = sharedEngine->getAudio()->createSound();
    test8BitSound->init(test8BitData);

    std::shared_ptr<audio::SoundDataWave> test24BitData(new audio::SoundDataWave());
    test24BitData->initFromFile("24-bit.wav");

    test24BitSound = sharedEngine->getAudio()->createSound();
    test24BitSound->init(test24BitData);

    std::shared_ptr<audio::SoundDataWave> jumpData(new audio::SoundDataWave());
    jumpData->initFromFile("jump.wav");

    jumpSound = sharedEngine->getAudio()->createSound();
    jumpSound->init(jumpData);

    std::shared_ptr<audio::SoundDataWave> ambientData(new audio::SoundDataWave());
    ambientData->initFromFile("ambient.wav");

    ambientSound = sharedEngine->getAudio()->createSound();
    ambientSound->init(ambientData);

    guiCamera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera.setTargetContentSize(Size2(800.0f, 600.0f));
    guiCamera.setLayer(&guiLayer);
    guiLayer.setScene(this);

    menuNode.setParent(&guiLayer);
    menu.setNode(&menuNode);

    test8BitButton.setMenu(&menu);
    test8BitButton.setNode(&test8BitButtonNode);
    test8BitButtonNode.setPosition(Vector2(0.0f, 80.0f));
    test8BitButtonNode.setParent(&menuNode);

    test24BitButton.setMenu(&menu);
    test24BitButton.setNode(&test24BitButtonNode);
    test24BitButtonNode.setPosition(Vector2(0.0f, 40.0f));
    test24BitButtonNode.setParent(&menuNode);

    jumpButton.setMenu(&menu);
    jumpButton.setNode(&jumpButtonNode);
    jumpButtonNode.setPosition(Vector2(0.0f, 0.0f));
    jumpButtonNode.setParent(&menuNode);

    ambientButton.setMenu(&menu);
    ambientButton.setNode(&ambientButtonNode);
    ambientButtonNode.setPosition(Vector2(0.0f, -40.0f));
    ambientButtonNode.setParent(&menuNode);

    backButton.setMenu(&menu);
    backButton.setNode(&backButtonNode);
    backButtonNode.setPosition(Vector2(-200.0f, -200.0f));
    backButtonNode.setParent(&menuNode);
}

bool SoundSample::handleGamepad(Event::Type type, const GamepadEvent& event)
{
    if (type == Event::Type::GAMEPAD_BUTTON_CHANGE)
    {
        if (event.pressed &&
            event.button == input::GamepadButton::B)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new MainMenu(samples)));
        }
    }

    return true;
}

bool SoundSample::handleUI(Event::Type type, const UIEvent& event) const
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.component == &backButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new MainMenu(samples)));
        }
        else if (event.component == &test8BitButton)
        {
            test8BitSound->play();
        }
        else if (event.component == &test24BitButton)
        {
            test24BitSound->play();
        }
        else if (event.component == &jumpButton)
        {
            jumpSound->play();
        }
        else if (event.component == &ambientButton)
        {
            ambientSound->play();
        }
    }

    return true;
}

bool SoundSample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
{
    if (type == Event::Type::KEY_DOWN)
    {
        switch (event.key)
        {
            case input::KeyboardKey::ESCAPE:
                samples.setScene(std::unique_ptr<scene::Scene>(new MainMenu(samples)));
                break;
            default:
                break;
        }
    }

    return true;
}
