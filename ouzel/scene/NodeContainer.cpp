// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "NodeContainer.h"
#include "Node.h"

namespace ouzel
{
    namespace scene
    {
        NodeContainer::NodeContainer()
        {
        }

        NodeContainer::~NodeContainer()
        {
            for (Node* node : children)
            {
                if (entered) node->leave();
                node->parent = nullptr;
            }
        }

        void NodeContainer::addChild(Node* node)
        {
            if (node)
            {
                children.push_back(node);
                node->parent = this;
                if (entered) node->enter();
            }
        }

        bool NodeContainer::removeChild(Node* node)
        {
            std::vector<Node*>::iterator i = std::find(children.begin(), children.end(), node);

            if (i != children.end())
            {
                if (entered) node->leave();
                node->parent = nullptr;
                children.erase(i);

                return true;
            }

            return false;
        }

        bool NodeContainer::hasChild(Node* node, bool recursive) const
        {
            for (std::vector<Node*>::const_iterator i = children.begin(); i != children.end(); ++i)
            {
                Node* child = *i;

                if (child == node || (recursive && child->hasChild(node, true)))
                {
                    return true;
                }
            }

            return false;
        }

        void NodeContainer::enter()
        {
            entered = true;

            for (Node* node : children)
            {
                node->enter();
            }
        }

        void NodeContainer::leave()
        {
            entered = false;

            for (Node* node : children)
            {
                node->leave();
            }
        }

        void NodeContainer::findNodes(const Vector2& position, std::vector<Node*>& nodes) const
        {
            for (auto i = children.rbegin(); i != children.rend(); ++i)
            {
                Node* node = *i;

                if (!node->isHidden())
                {
                    if (node->isPickable() && node->pointOn(position))
                    {
                        auto upperBound = std::upper_bound(nodes.begin(), nodes.end(), node,
                                                           [](Node* a, Node* b) {
                                                               return a->worldOrder < b->worldOrder;
                                                           });

                        nodes.insert(upperBound, node);
                    }

                    node->findNodes(position, nodes);
                }
            }
        }

        void NodeContainer::findNodes(const std::vector<Vector2>& edges, std::vector<Node*>& nodes) const
        {
            for (auto i = children.rbegin(); i != children.rend(); ++i)
            {
                Node* node = *i;

                if (!node->isHidden())
                {
                    if (node->isPickable() && node->shapeOverlaps(edges))
                    {
                        auto upperBound = std::upper_bound(nodes.begin(), nodes.end(), node,
                                                           [](Node* a, Node* b) {
                                                               return a->worldOrder < b->worldOrder;
                                                           });

                        nodes.insert(upperBound, node);
                    }

                    node->findNodes(edges, nodes);
                }
            }
        }

        void NodeContainer::findComponents(const Vector2& position, std::vector<Component*>& components) const
        {
            for (auto i = children.rbegin(); i != children.rend(); ++i)
            {
                Node* node = *i;

                if (!node->isHidden())
                {
                    node->findComponents(position, components);
                }
            }
        }

        void NodeContainer::findComponents(const std::vector<Vector2>& edges, std::vector<Component*>& components) const
        {
            for (auto i = children.rbegin(); i != children.rend(); ++i)
            {
                Node* node = *i;

                if (!node->isHidden())
                {
                    node->findComponents(edges, components);
                }
            }
        }
    } // namespace scene
} // namespace ouzel
