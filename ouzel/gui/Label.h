// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <memory>
#include <string>
#include "gui/Widget.h"
#include "math/Color.h"
#include "scene/Camera.h"
#include "gui/BMFont.h"
#include "graphics/BlendState.h"
#include "graphics/Buffer.h"
#include "graphics/MeshBuffer.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

namespace ouzel
{
    namespace gui
    {
        class Label: public Widget
        {
        public:
            Label(const std::string& aText,
                  const std::string& fontFile,
                  bool aMipmaps = true,
                  Color aColor = Color::WHITE,
                  const Vector2& aTextAnchor = Vector2(0.5f, 0.5f));

            virtual void draw(const Matrix4& transformMatrix,
                              const Color& drawColor,
                              scene::Camera* camera) override;

            virtual void drawWireframe(const Matrix4& transformMatrix,
                                       const Color& drawColor,
                                       scene::Camera* camera) override;

            virtual void setFont(const std::string& fontFile);

            virtual void setTextAnchor(const Vector2& newTextAnchor);
            virtual const Vector2& getTextAnchor() const { return textAnchor; }

            virtual void setTextScale(const Vector2& newTextScale);
            virtual const Vector2& getTextScale() const { return textScale; }

            virtual void setText(const std::string& newText);
            virtual const std::string& getText() const { return text; }

            virtual const Color& getColor() const { return color; }
            virtual void setColor(const Color& newColor);

            virtual const std::shared_ptr<graphics::Shader>& getShader() const { return shader; }
            virtual void setShader(const std::shared_ptr<graphics::Shader>& newShader) { shader = newShader; }

            virtual const std::shared_ptr<graphics::BlendState>& getBlendState() const { return blendState; }
            virtual void setBlendState(const std::shared_ptr<graphics::BlendState>& newBlendState)  { blendState = newBlendState; }

        protected:
            void updateText();

            std::shared_ptr<graphics::Shader> shader;
            std::shared_ptr<graphics::BlendState> blendState;

            std::shared_ptr<graphics::MeshBuffer> meshBuffer;
            std::shared_ptr<graphics::Buffer> indexBuffer;
            std::shared_ptr<graphics::Buffer> vertexBuffer;

            std::shared_ptr<graphics::Texture> texture;
            std::shared_ptr<graphics::Texture> whitePixelTexture;

            BMFont font;
            std::string text;
            Vector2 textAnchor;
            Vector2 textScale = Vector2(1.0f, 1.0f);

            std::vector<uint16_t> indices;
            std::vector<graphics::VertexPCT> vertices;

            Color color = Color::WHITE;

            bool mipmaps = true;
            bool needsMeshUpdate = false;
        };
    } // namespace gui
} // namespace ouzel
