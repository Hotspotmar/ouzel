// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "Scene.h"
#include "Layer.h"
#include "Camera.h"
#include "SceneManager.h"
#include "core/Engine.h"
#include "events/EventDispatcher.h"

namespace ouzel
{
    namespace scene
    {
        Scene::Scene():
            eventHandler(EventHandler::PRIORITY_MAX + 1)
        {
            eventHandler.windowHandler = std::bind(&Scene::handleWindow, this, std::placeholders::_1, std::placeholders::_2);
            eventHandler.mouseHandler = std::bind(&Scene::handleMouse, this, std::placeholders::_1, std::placeholders::_2);
            eventHandler.touchHandler = std::bind(&Scene::handleTouch, this, std::placeholders::_1, std::placeholders::_2);
        }

        Scene::~Scene()
        {
            if (entered)
            {
                for (Layer* layer : layers)
                {
                    layer->leave();
                }
            }

            sharedEngine->getSceneManager()->removeScene(this);
        }

        void Scene::draw()
        {
            std::stable_sort(layers.begin(), layers.end(), [](Layer* a, Layer* b) {
                return a->getOrder() > b->getOrder();
            });

            for (Layer* layer : layers)
            {
                layer->draw();
            }
        }

        void Scene::addLayer(Layer* layer)
        {
            if (layer && !hasLayer(layer))
            {
                layers.push_back(layer);

                if (entered) layer->enter();

                layer->scene = this;
            }
        }

        void Scene::removeLayer(Layer* layer)
        {
            std::vector<Layer*>::iterator i = std::find(layers.begin(), layers.end(), layer);

            if (i != layers.end())
            {
                if (entered)
                {
                    layer->leave();
                }

                layers.erase(i);

                layer->scene = nullptr;
            }
        }

        void Scene::removeAllLayers()
        {
            if (entered)
            {
                for (Layer* layer : layers)
                {
                    layer->leave();
                }
            }

            layers.clear();
        }

        bool Scene::hasLayer(Layer* layer) const
        {
            std::vector<Layer*>::const_iterator i = std::find(layers.begin(), layers.end(), layer);

            return i != layers.end();
        }

        void Scene::recalculateProjection()
        {
            for (Layer* layer : layers)
            {
                layer->recalculateProjection();
            }
        }

        Node* Scene::pickNode(const Vector2& position) const
        {
            for (std::vector<Layer*>::const_reverse_iterator i = layers.rbegin(); i != layers.rend(); ++i)
            {
                Layer* layer = *i;

                if (Node* result = layer->pickNode(position))
                {
                    return result;
                }
            }

            return nullptr;
        }

        std::vector<Node*> Scene::pickNodes(const Vector2& position) const
        {
            std::vector<Node*> result;

            for (auto i = layers.rbegin(); i != layers.rend(); ++i)
            {
                std::vector<Node*> nodes = (*i)->pickNodes(position);

                result.insert(result.end(), nodes.begin(), nodes.end());
            }

            return result;
        }

        std::vector<Node*> Scene::pickNodes(const std::vector<Vector2>& edges) const
        {
            std::vector<Node*> result;

            for (auto i = layers.rbegin(); i != layers.rend(); ++i)
            {
                std::vector<Node*> nodes = (*i)->pickNodes(edges);

                result.insert(result.end(), nodes.begin(), nodes.end());
            }

            return result;
        }

        Component* Scene::pickComponent(const Vector2& position) const
        {
            for (std::vector<Layer*>::const_reverse_iterator i = layers.rbegin(); i != layers.rend(); ++i)
            {
                Layer* layer = *i;

                if (Component* result = layer->pickComponent(position))
                {
                    return result;
                }
            }

            return nullptr;
        }

        std::vector<Component*> Scene::pickComponents(const Vector2& position) const
        {
            std::vector<Component*> result;

            for (auto i = layers.rbegin(); i != layers.rend(); ++i)
            {
                std::vector<Component*> nodes = (*i)->pickComponents(position);

                result.insert(result.end(), nodes.begin(), nodes.end());
            }
            
            return result;
        }

        std::vector<Component*> Scene::pickComponents(const std::vector<Vector2>& edges) const
        {
            std::vector<Component*> result;

            for (auto i = layers.rbegin(); i != layers.rend(); ++i)
            {
                std::vector<Component*> nodes = (*i)->pickComponents(edges);

                result.insert(result.end(), nodes.begin(), nodes.end());
            }
            
            return result;
        }

        void Scene::enter()
        {
            entered = true;

            recalculateProjection();
            sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

            for (Layer* layer : layers)
            {
                layer->enter();
            }
        }

        void Scene::leave()
        {
            entered = false;

            sharedEngine->getEventDispatcher()->removeEventHandler(&eventHandler);

            for (Layer* layer : layers)
            {
                layer->leave();
            }
        }

        bool Scene::handleWindow(Event::Type type, const WindowEvent&)
        {
            if (type == Event::Type::WINDOW_SIZE_CHANGE ||
                type == Event::Type::WINDOW_CONTENT_SCALE_CHANGE)
            {
                recalculateProjection();
            }

            return true;
        }

        bool Scene::handleMouse(Event::Type type, const MouseEvent& event)
        {
            switch (type)
            {
                case Event::Type::MOUSE_DOWN:
                {
                    Component* component = pickComponent(event.position);
                    pointerDownOnComponent(0, component, event.position);
                    break;
                }
                case Event::Type::MOUSE_UP:
                {
                    Component* component = pickComponent(event.position);
                    pointerUpOnComponent(0, component, event.position);
                    break;
                }
                case Event::Type::MOUSE_MOVE:
                {
                    Component* previousComponent = pickComponent(event.position - event.difference);
                    pointerLeaveComponent(0, previousComponent, event.position);

                    Component* component = pickComponent(event.position);
                    pointerEnterComponent(0, component, event.position);

                    auto i = pointerDownOnComponents.find(0);

                    if (i != pointerDownOnComponents.end())
                    {
                        pointerDragComponent(0, i->second, event.position);
                    }
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        bool Scene::handleTouch(Event::Type type, const TouchEvent& event)
        {
            switch (type)
            {
                case Event::Type::TOUCH_BEGIN:
                {
                    Component* component = pickComponent(event.position);
                    pointerDownOnComponent(event.touchId, component, event.position);
                    break;
                }
                case Event::Type::TOUCH_END:
                {
                    Component* component = pickComponent(event.position);
                    pointerUpOnComponent(event.touchId, component, event.position);
                    break;
                }
                case Event::Type::TOUCH_MOVE:
                {
                    Component* previousComponent = pickComponent(event.position - event.difference);
                    pointerLeaveComponent(0, previousComponent, event.position);

                    Component* component = pickComponent(event.position);
                    pointerEnterComponent(0, component, event.position);

                    auto i = pointerDownOnComponents.find(event.touchId);

                    if (i != pointerDownOnComponents.end())
                    {
                        pointerDragComponent(event.touchId, i->second, event.position);
                    }
                    break;
                }
                case Event::Type::TOUCH_CANCEL:
                {
                    Component* component = pickComponent(event.position);
                    pointerUpOnComponent(event.touchId, component, event.position);
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        void Scene::pointerEnterComponent(uint64_t pointerId, Component* component, const Vector2& position)
        {
            if (component)
            {
                Event event;
                event.type = Event::Type::UI_ENTER_NODE;

                event.uiEvent.component = component;
                event.uiEvent.touchId = pointerId;
                event.uiEvent.position = position;

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }

        void Scene::pointerLeaveComponent(uint64_t pointerId, Component* component, const Vector2& position)
        {
            if (component)
            {
                Event event;
                event.type = Event::Type::UI_LEAVE_NODE;

                event.uiEvent.component = component;
                event.uiEvent.touchId = pointerId;
                event.uiEvent.position = position;

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }

        void Scene::pointerDownOnComponent(uint64_t pointerId, Component* component, const Vector2& position)
        {
            pointerDownOnComponents[pointerId] = component;

            if (component)
            {
                Event event;
                event.type = Event::Type::UI_PRESS_NODE;

                event.uiEvent.component = component;
                event.uiEvent.touchId = pointerId;
                event.uiEvent.position = position;

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }

        void Scene::pointerUpOnComponent(uint64_t pointerId, Component* component, const Vector2& position)
        {
            auto i = pointerDownOnComponents.find(pointerId);

            if (i != pointerDownOnComponents.end())
            {
                auto pointerDownOnComponent = i->second;

                if (pointerDownOnComponent)
                {
                    Event releaseEvent;
                    releaseEvent.type = Event::Type::UI_RELEASE_NODE;

                    releaseEvent.uiEvent.component = pointerDownOnComponent;
                    releaseEvent.uiEvent.touchId = pointerId;
                    releaseEvent.uiEvent.position = position;

                    sharedEngine->getEventDispatcher()->postEvent(releaseEvent);

                    if (pointerDownOnComponent == component)
                    {
                        Event clickEvent;
                        clickEvent.type = Event::Type::UI_CLICK_NODE;

                        clickEvent.uiEvent.component = pointerDownOnComponent;
                        clickEvent.uiEvent.touchId = pointerId;
                        clickEvent.uiEvent.position = position;

                        sharedEngine->getEventDispatcher()->postEvent(clickEvent);
                    }
                }
            }

            pointerDownOnComponents.erase(pointerId);
        }

        void Scene::pointerDragComponent(uint64_t, Component* component, const Vector2& position)
        {
            if (component)
            {
                Event event;
                event.type = Event::Type::UI_DRAG_NODE;

                event.uiEvent.component = component;
                event.uiEvent.position = position;

                sharedEngine->getEventDispatcher()->postEvent(event);
            }
        }
    } // namespace scene
} // namespace ouzel
