// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "GUISample.h"
#include "MainMenu.h"

using namespace std;
using namespace ouzel;

GUISample::GUISample(Samples& aSamples):
    samples(aSamples),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK)
{
    eventHandler.gamepadHandler = bind(&GUISample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&GUISample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&GUISample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    guiCamera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera.setTargetContentSize(Size2(800.0f, 600.0f));
    guiCamera.setLayer(&guiLayer);
    guiLayer.setScene(this);

    menuNode.setParent(&guiLayer);
    menu.setNode(&menuNode);

    button.reset(new gui::Button("button.png", "button_selected.png", "button_down.png", "", "Button", "arial.fnt", Color::RED, Color::RED, Color::BLACK));
    button->setNode(&buttonNode);
    button->setMenu(&menu);
    buttonNode.setPosition(Vector2(-200.0f, 100.0f));
    buttonNode.setParent(&menuNode);

    checkBox.reset(new gui::CheckBox("checkbox.png", "", "", "", "tick.png"));
    checkBox->setNode(&checkBoxNode);
    checkBox->setMenu(&menu);
    checkBoxNode.setPosition(Vector2(-100.0f, 100.0f));
    checkBoxNode.setParent(&menuNode);

    fullscreenButton.reset(new gui::Button("button.png", "button_selected.png", "button_down.png", "", "Fullscreen", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK));
    fullscreenButton->setNode(&fullscreenButtonNode);
    fullscreenButton->setMenu(&menu);
    fullscreenButtonNode.setPosition(Vector2(-200.0f, 40.0f));
    fullscreenButtonNode.setParent(&menuNode);

    label1.reset(new gui::Label("checkbox", "arial.fnt"));
    label1->setColor(Color::CYAN);
    label1->setNode(&label1Node);
    label1Node.setPosition(Vector2(-60.0f, 100.0f));
    label1Node.setParent(&menuNode);

    sharedEngine->getLocalization()->addLanguage("latvian", "lv.mo");
    sharedEngine->getLocalization()->setLanguage("latvian");

    label2.reset(new gui::Label(sharedEngine->getLocalization()->getString("Ouzel"), "ArialBlack.fnt"));
    label2->setNode(&label2Node);
    label2Node.setPosition(Vector2(10.0f, 0.0f));
    label2Node.setParent(&menuNode);

    label3.reset(new gui::Label("UTF-8 ĀāČč\nNew line", "ArialBlack.fnt", true, Color::WHITE, Vector2(0.0f, 0.5f)));
    label3->setColor(Color::BLUE);
    label3->setNode(&label3Node);
    label3Node.setPosition(Vector2(-100.0f, -100.0f));
    label3Node.setScale(Vector3(0.5f, 0.5f, 1.0f));
    label3Node.setParent(&menuNode);

    backButton.setNode(&backButtonNode);
    backButton.setMenu(&menu);
    backButtonNode.setParent(&menuNode);
    backButtonNode.setPosition(Vector2(-200.0f, -200.0f));
}

bool GUISample::handleGamepad(Event::Type type, const GamepadEvent& event)
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

bool GUISample::handleUI(Event::Type type, const UIEvent& event) const
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.component == &backButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new MainMenu(samples)));
        }
        else if (event.component == button.get())
        {
            checkBox->setChecked(!checkBox->isChecked());
        }
        else if (event.component == fullscreenButton.get())
        {
            bool fullscreen = ouzel::sharedEngine->getWindow()->isFullscreen();
            ouzel::sharedEngine->getWindow()->setFullscreen(!fullscreen);
        }
    }

    return true;
}

bool GUISample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
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
