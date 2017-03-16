// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <vector>
#include "utils/Noncopyable.h"
#include "math/Vector2.h"

namespace ouzel
{
    namespace scene
    {
        class Node;
        class Component;

        class NodeContainer: public Noncopyable
        {
            friend Node;
        public:
            NodeContainer();
            virtual ~NodeContainer();

            virtual bool hasChild(Node* node, bool recursive = false) const;
            virtual const std::vector<Node*>& getChildren() const { return children; }

        protected:
            virtual void addChild(Node* node);
            virtual bool removeChild(Node* node);

            virtual void enter();
            virtual void leave();

            virtual void findNodes(const Vector2& position, std::vector<Node*>& nodes) const;
            virtual void findNodes(const std::vector<Vector2>& edges, std::vector<Node*>& nodes) const;

            virtual void findComponents(const Vector2& position, std::vector<Component*>& components) const;
            virtual void findComponents(const std::vector<Vector2>& edges, std::vector<Component*>& components) const;

            std::vector<Node*> children;
            bool entered = false;
        };
    } // namespace scene
} // namespace ouzel
