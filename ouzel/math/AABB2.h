// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <cstdint>
#include "math/Vector2.h"
#include "math/Size2.h"

#pragma once

namespace ouzel
{
    /**
     * Axis Aligned Bounding Box (AABB), usually calculate some rough but fast collision detection.
     */
    class AABB2
    {
    public:
        Vector2 min;
        Vector2 max;

        /**
         * Constructor.
         */
        AABB2()
        {
        }

        /**
         * Constructor.
         */
        AABB2(const Vector2& pMin, const Vector2& pMax):
            min(pMin), max(pMax)
        {
        }

        /**
         * Constructor.
         */
        AABB2(const AABB2& box):
            min(box.min), max(box.max)
        {
        }

        /**
         * Gets the center point of the bounding box.
         */
        Vector2 getCenter();

        /* Near face, specified counter-clockwise looking towards the origin from the positive z-axis.
         * verts[0] : left top
         * verts[1] : left bottom
         * verts[2] : right bottom
         * verts[3] : right top
         */
        void getCorners(Vector2* dst) const;

        /**
         * Tests whether this bounding box intersects the specified bounding object.
         */
        bool intersects(const AABB2& aabb) const
        {
            return ((min.x >= aabb.min.x && min.x <= aabb.max.x) || (aabb.min.x >= min.x && aabb.min.x <= max.x)) &&
            ((min.y >= aabb.min.y && min.y <= aabb.max.y) || (aabb.min.y >= min.y && aabb.min.y <= max.y));
        }

        /**
         * check whether the point is in.
         */
        bool containsPoint(const Vector2& point) const
        {
            if (point.x < min.x) return false;
            if (point.y < min.y) return false;
            if (point.x > max.x) return false;
            if (point.y > max.y) return false;
            return true;
        }

        /**
         * Sets this bounding box to the smallest bounding box
         * that contains both this bounding object and the specified bounding box.
         */
        void merge(const AABB2& box);

        /**
         * Sets this bounding box to the specified values.
         */
        void set(const Vector2& pMin, const Vector2& pMax)
        {
            min = pMin;
            max = pMax;
        }

        /**
         * Reset min and max value.If you invoke this method, isEmpty() shall return true.
         */
        void reset();

        /**
         * check the AABB object is empty(reset).
         */
        bool isEmpty() const
        {
            return min.x > max.x || min.y > max.y;
        }

        /**
         * update the min and max from the given points.
         */
        void updateMinMax(const Vector2* points, uint32_t num)
        {
            for (uint32_t i = 0; i < num; ++i)
            {
                // Leftmost point.
                if (points[i].x < min.x)
                    min.x = points[i].x;

                // Lowest point.
                if (points[i].y < min.y)
                    min.y = points[i].y;

                // Rightmost point.
                if (points[i].x > max.x)
                    max.x = points[i].x;

                // Highest point.
                if (points[i].y > max.y)
                    max.y = points[i].y;
            }
        }

        void insertPoint(const Vector2& point)
        {
            if (point.x < min.x) min.x = point.x;
            if (point.x > max.x) max.x = point.x;
            if (point.y < min.y) min.y = point.y;
            if (point.y > max.y) max.y = point.y;
        }

        inline AABB2 operator+(const Vector2& v) const
        {
            AABB2 result(*this);
            result += v;
            return result;
        }

        inline AABB2& operator+=(const Vector2& v)
        {
            min += v;
            max += v;
            return *this;
        }

        inline AABB2 operator-(const Vector2& v) const
        {
            AABB2 result(*this);
            result -= v;
            return result;
        }

        inline AABB2& operator-=(const Vector2& v)
        {
            min -= v;
            max -= v;
            return *this;
        }

        inline Size2 getSize() const
        {
            return Size2(max.x - min.x, max.y - min.y);
        }
    };
}
