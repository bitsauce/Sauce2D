#ifndef SAUCE_GRAPHICS_H
#define SAUCE_GRAPHICS_H

#include <Sauce/Math.h>
#include <Sauce/Graphics/GraphicsContext.h>
#include <Sauce/Graphics/BlendState.h>
#include <Sauce/Graphics/Animation.h>
#include <Sauce/Graphics/Spritebatch.h>
#include <Sauce/Graphics/Font.h>
#include <Sauce/Graphics/RenderTarget.h>
#include <Sauce/Graphics/Pixmap.h>
#include <Sauce/Graphics/Shader.h>
#include <Sauce/Graphics/Sprite.h>
#include <Sauce/Graphics/Texture.h>
#include <Sauce/Graphics/TextureAtlas.h>
#include <Sauce/Graphics/Textureregion.h>
#include <Sauce/Graphics/Vertex.h>
#include <Sauce/Graphics/Vertexbuffer.h>
#include <Sauce/Graphics/Viewport.h>

BEGIN_SAUCE_NAMESPACE

SAUCE_API extern uint QUAD_INDICES[6];
SAUCE_API extern Vector4F QUAD_VERTICES[4];
SAUCE_API extern Vector2F QUAD_TEXCOORD[4];

END_SAUCE_NAMESPACE

#endif // SAUCE_GRAPHICS_H
