// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SpritesSample.h"
#include "MainMenu.h"

using namespace std;
using namespace ouzel;

SpritesSample::SpritesSample(Samples& aSamples):
    samples(aSamples),
    hideButton("button.png", "button_selected.png", "button_down.png", "", "Show/hide", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK),
    wireframeButton("button.png", "button_selected.png", "button_down.png", "", "Wireframe", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", "arial.fnt", Color::BLACK, Color::BLACK, Color::BLACK)
{
    eventHandler.gamepadHandler = bind(&SpritesSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&SpritesSample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&SpritesSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    camera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    camera.setTargetContentSize(Size2(800.0f, 600.0f));
    camera.setLayer(&layer);
    layer.setScene(this);

    // character
    characterSprite.initFromFile("run.json");
    characterSprite.play(true);

    characterSprite.setNode(&character);
    character.setParent(&layer);
    character.setPosition(Vector2(-300.0f, 0.0f));

    move.reset(new scene::Move(4.0f, Vector2(300.0f, 0.0f)));
    move->setNode(&character);
    move->start();

    // fire
    fireSprite.initFromFile("fire.json");
    fireSprite.setOffset(Vector2(0.0f, 20.0f));
    fireSprite.play(true);

    fireSprite.setNode(&fireNode);
    fireNode.setPosition(Vector2(-100.0f, -140.0f));
    fireNode.setParent(&layer);

    // triangle
    triangleSprite.initFromFile("triangle.json");
    triangleSprite.setNode(&triangleNode);
    triangleNode.setPosition(Vector2(100.0f, -140.0f));
    triangleNode.setParent(&layer);

    guiCamera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera.setTargetContentSize(Size2(800.0f, 600.0f));
    guiCamera.setLayer(&guiLayer);
    guiLayer.setScene(this);

    menu.setNode(&menuNode);
    menuNode.setParent(&guiLayer);

    hideButton.setNode(&backButtonNode);
    hideButton.setMenu(&menu);
    hideButtonNode.setParent(&menuNode);
    hideButtonNode.setPosition(Vector2(-200.0f, 200.0f));

    wireframeButton.setNode(&backButtonNode);
    wireframeButton.setMenu(&menu);
    wireframeButtonNode.setParent(&menuNode);
    wireframeButtonNode.setPosition(Vector2(-200.0f, 160.0f));

    backButton.setNode(&backButtonNode);
    backButton.setMenu(&menu);
    backButtonNode.setParent(&menuNode);
    backButtonNode.setPosition(Vector2(-200.0f, -200.0f));
}

bool SpritesSample::handleGamepad(Event::Type type, const GamepadEvent& event)
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

bool SpritesSample::handleUI(Event::Type type, const UIEvent& event)
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.component == &backButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new MainMenu(samples)));
        }
        else if (event.component == &hideButton)
        {
            character.setHidden(!character.isHidden());
        }
        else if (event.component == &wireframeButton)
        {
            camera.setWireframe(!camera.getWireframe());
        }
    }

    return true;
}

bool SpritesSample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
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
