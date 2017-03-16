// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include "utils/Noncopyable.h"
#include "math/Vector2.h"
#include "events/EventHandler.h"

namespace ouzel
{
    namespace scene
    {
        class SceneManager;
        class Layer;
        class Component;

        class Scene: public Noncopyable
        {
            friend SceneManager;
            friend Layer;
        public:
            Scene();
            virtual ~Scene();

            virtual void draw();

            bool hasLayer(Layer* layer) const;
            const std::vector<Layer*>& getLayers() const { return layers; }

            virtual void recalculateProjection();

            Node* pickNode(const Vector2& position) const;
            std::vector<Node*> pickNodes(const Vector2& position) const;
            std::vector<Node*> pickNodes(const std::vector<Vector2>& edges) const;

            Component* pickComponent(const Vector2& position) const;
            std::vector<Component*> pickComponents(const Vector2& position) const;
            std::vector<Component*> pickComponents(const std::vector<Vector2>& edges) const;

        protected:
            void addLayer(Layer* layer);
            bool removeLayer(Layer* layer);

            virtual void enter();
            virtual void leave();

            bool handleWindow(Event::Type type, const WindowEvent& event);
            bool handleMouse(Event::Type type, const MouseEvent& event);
            bool handleTouch(Event::Type type, const TouchEvent& event);

            void pointerEnterComponent(uint64_t pointerId, Component* component, const Vector2& position);
            void pointerLeaveComponent(uint64_t pointerId, Component* component, const Vector2& position);
            void pointerDownOnComponent(uint64_t pointerId, Component* component, const Vector2& position);
            void pointerUpOnComponent(uint64_t pointerId, Component* component, const Vector2& position);
            void pointerDragComponent(uint64_t pointerId, Component* component, const Vector2& position);

            std::vector<Layer*> layers;
            ouzel::EventHandler eventHandler;

            std::unordered_map<uint64_t, Component*> pointerDownOnComponents;

            bool entered = false;
        };
    } // namespace scene
} // namespace ouzel
