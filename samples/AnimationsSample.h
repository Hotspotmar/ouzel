// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Samples.h"

class AnimationsSample: public ouzel::scene::Scene
{
public:
    AnimationsSample(Samples& aSamples);

private:
    bool handleGamepad(ouzel::Event::Type type, const ouzel::GamepadEvent& event);
    bool handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event) const;
    bool handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event) const;

    Samples& samples;

    ouzel::scene::Layer layer;
    ouzel::scene::Camera camera;

    ouzel::scene::ShapeDrawable shapeDrawable;
    ouzel::scene::Node drawNode;

    std::unique_ptr<ouzel::scene::Shake> shake;

    ouzel::scene::Sprite witchSprite;
    ouzel::scene::Node witch;

    std::unique_ptr<ouzel::scene::Animator> witchScale;
    std::unique_ptr<ouzel::scene::Animator> witchFade;
    std::unique_ptr<ouzel::scene::Animator> witchRotate;
    std::unique_ptr<ouzel::scene::Animator> witchRepeat;
    std::unique_ptr<ouzel::scene::Animator> witchParallel;
    std::unique_ptr<ouzel::scene::Animator> witchSequence;

    ouzel::scene::Sprite ballSprite;
    ouzel::scene::Node ball;

    std::unique_ptr<ouzel::scene::Animator> ballDelay;
    std::unique_ptr<ouzel::scene::Animator> ballEase;
    std::unique_ptr<ouzel::scene::Animator> ballMove;
    std::unique_ptr<ouzel::scene::Animator> ballSequence;
    
    ouzel::EventHandler eventHandler;

    ouzel::scene::Layer guiLayer;
    ouzel::scene::Camera guiCamera;
    ouzel::scene::Node menuNode;
    ouzel::gui::Menu menu;
    ouzel::scene::Node backButtonNode;
    ouzel::gui::Button backButton;
};
