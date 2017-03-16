// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "Label.h"
#include "core/Engine.h"
#include "graphics/Renderer.h"
#include "core/Cache.h"
#include "utils/Utils.h"

namespace ouzel
{
    namespace gui
    {
        Label::Label(const std::string& aText,
                     const std::string& fontFile,
                     bool aMipmaps,
                     Color aColor,
                     const Vector2& aTextAnchor):
            text(aText),
            textAnchor(aTextAnchor),
            color(aColor),
            mipmaps(aMipmaps)
        {
            shader = sharedEngine->getCache()->getShader(graphics::SHADER_TEXTURE);
            blendState = sharedEngine->getCache()->getBlendState(graphics::BLEND_ALPHA);
            whitePixelTexture = sharedEngine->getCache()->getTexture(graphics::TEXTURE_WHITE_PIXEL);

            indexBuffer = std::make_shared<graphics::Buffer>();
            indexBuffer->init(graphics::Buffer::Usage::INDEX);

            vertexBuffer = std::make_shared<graphics::Buffer>();
            vertexBuffer->init(graphics::Buffer::Usage::VERTEX);

            meshBuffer = std::make_shared<graphics::MeshBuffer>();
            meshBuffer->init(sizeof(uint16_t), indexBuffer, graphics::VertexPCT::ATTRIBUTES, vertexBuffer);

            font = sharedEngine->getCache()->getBMFont(fontFile);
            texture = sharedEngine->getCache()->getTexture(font.getTexture(), false, mipmaps);

            updateText();
        }

        void Label::setFont(const std::string& fontFile)
        {
            font = sharedEngine->getCache()->getBMFont(fontFile);
            texture = sharedEngine->getCache()->getTexture(font.getTexture(), false, mipmaps);

            updateText();
        }

        void Label::setTextAnchor(const Vector2& newTextAnchor)
        {
            textAnchor = newTextAnchor;

            updateText();
        }

        void Label::setTextScale(const Vector2& newTextScale)
        {
            textScale = newTextScale;

            updateText();
        }

        void Label::draw(const Matrix4& transformMatrix,
                         const Color& drawColor,
                         scene::Camera* camera)
        {
            Component::draw(transformMatrix, drawColor, camera);

            if (needsMeshUpdate)
            {
                indexBuffer->setData(indices.data(), static_cast<uint32_t>(getVectorSize(indices)));
                vertexBuffer->setData(vertices.data(), static_cast<uint32_t>(getVectorSize(vertices)));

                needsMeshUpdate = false;
            }

            Matrix4 modelViewProj = camera->getRenderViewProjection() * transformMatrix;
            float colorVector[] = {drawColor.normR(), drawColor.normG(), drawColor.normB(), drawColor.normA()};

            std::vector<std::vector<float>> pixelShaderConstants(1);
            pixelShaderConstants[0] = {std::begin(colorVector), std::end(colorVector)};

            std::vector<std::vector<float>> vertexShaderConstants(1);
            vertexShaderConstants[0] = {std::begin(modelViewProj.m), std::end(modelViewProj.m)};

            sharedEngine->getRenderer()->addDrawCommand({texture},
                                                        shader,
                                                        pixelShaderConstants,
                                                        vertexShaderConstants,
                                                        blendState,
                                                        meshBuffer,
                                                        static_cast<uint32_t>(indices.size()),
                                                        graphics::Renderer::DrawMode::TRIANGLE_LIST,
                                                        0,
                                                        camera->getRenderTarget(),
                                                        camera->getRenderViewport(),
                                                        camera->getDepthWrite(),
                                                        camera->getDepthTest());
        }

        void Label::drawWireframe(const Matrix4& transformMatrix,
                                  const Color& drawColor,
                                  scene::Camera* camera)
        {
            Component::drawWireframe(transformMatrix, drawColor, camera);

            Matrix4 modelViewProj = camera->getRenderViewProjection() * transformMatrix;
            float colorVector[] = {drawColor.normR(), drawColor.normG(), drawColor.normB(), drawColor.normA()};

            std::vector<std::vector<float>> pixelShaderConstants(1);
            pixelShaderConstants[0] = {std::begin(colorVector), std::end(colorVector)};

            std::vector<std::vector<float>> vertexShaderConstants(1);
            vertexShaderConstants[0] = {std::begin(modelViewProj.m), std::end(modelViewProj.m)};

            sharedEngine->getRenderer()->addDrawCommand({whitePixelTexture},
                                                        shader,
                                                        pixelShaderConstants,
                                                        vertexShaderConstants,
                                                        blendState,
                                                        meshBuffer,
                                                        static_cast<uint32_t>(indices.size()),
                                                        graphics::Renderer::DrawMode::TRIANGLE_LIST,
                                                        0,
                                                        camera->getRenderTarget(),
                                                        camera->getRenderViewport(),
                                                        camera->getDepthWrite(),
                                                        camera->getDepthTest(),
                                                        true);
        }

        void Label::setText(const std::string& newText)
        {
            text = newText;

            updateText();
        }

        void Label::setColor(const Color& newColor)
        {
            color = newColor;

            updateText();
        }

        void Label::updateText()
        {
            font.getVertices(text, color, textAnchor, textScale, indices, vertices);
            needsMeshUpdate = true;
            
            boundingBox.reset();
            
            for (const graphics::VertexPCT& vertex : vertices)
            {
                boundingBox.insertPoint(Vector2(vertex.position.v[0], vertex.position.v[1]));
            }
            
        }
    } // namespace gui
} // namespace ouzel
