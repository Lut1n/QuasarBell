#include "Font/GlyphRenderer.h"


#include <cmath>
#include <algorithm>

#define ATLAS_FONT_SIZE 8.0f

GlyphRenderer::GlyphRenderer()
{
}

GlyphRenderer::~GlyphRenderer()
{
}

void GlyphRenderer::initialize()
{
    _font.generateCharset();
    
    char c_beg = 32; // ' '
    char c_end = 126; // '~'
    // 'A' = 65; 'a' = 97
    
    setBaseSize(ATLAS_FONT_SIZE);
    
    resetCharset();
    for(char c = c_beg; c<=c_end; ++c)
        addChar(c);
    generateUVs();
    
    unsigned atlasSize = size();
    
    _atlasTexture = RenderInterface::createTarget(atlasSize,atlasSize,false);
    renderAtlas(_atlasTexture);
}

void GlyphRenderer::renderAtlas(unsigned target)
{
    RenderInterface::setTarget(target);
    RenderInterface::clear(0x00000000);
    RenderInterface::setThickness(2.0f);
    RenderInterface::setPrecision(5);
    RenderInterface::setColor(0xFFFFFFFF);
    
    char c_beg = 32; // ' '
    char c_end = 126; // '~'
    // 'A' = 65; 'a' = 97
    
    vec2 offset = vec2(0.f, mix(1.f, _glyphRatio, 0.5)) * ATLAS_FONT_SIZE;
    for(char c = c_beg; c<=c_end; ++c)
    {
        Rect uv = getUV(c);
        _font.drawGlyph(uv.p0 + offset, c, ATLAS_FONT_SIZE);
    }
}

void GlyphRenderer::renderText(const std::string& str, const vec2& pos, float size, unsigned color)
{
    int oft = 0;
    for(char c : str)
    {
        vec2 char_p = pos + vec2(oft++ * size, 0.0f);
        renderChar(c, char_p, size, color);
    }
}

void GlyphRenderer::renderChar(char c, const vec2& pos, float size, unsigned color)
{
    if(has(c))
    {
        Rect uv = getUV(c);
        float scale = size/(uv.p1-uv.p0).x;
        vec2 dst_size{size, scale*(uv.p1-uv.p0).y};
        Rect dstRect{pos, pos + dst_size};
        
        RenderInterface::setColor(color);
        RenderInterface::copy(_atlasTexture, uv, dstRect, true, true);
    }
}

void GlyphRenderer::renderClippedText(const std::string& str, const Rect& clip, const vec2& pos, float size, unsigned color)
{
    int oft = 0;
    for(char c : str)
    {
        vec2 char_p = pos + vec2(oft++ * size, 0.0f);
        renderClippedChar(c, clip, char_p, size, color);
    }
}

void GlyphRenderer::renderClippedChar(char c, const Rect& clip, const vec2& pos, float size, unsigned color)
{
    if(has(c))
    {
        Rect uv = getUV(c);
        float scale = size/(uv.p1-uv.p0).x;
        vec2 dst_size{size, scale*(uv.p1-uv.p0).y};
        Rect dstRect{pos, pos + dst_size};
        
        bool clipped = dstRect.p0.x > clip.p1.x || dstRect.p1.x < clip.p0.x;
        clipped = clipped || dstRect.p0.y > clip.p1.y || dstRect.p1.y < clip.p0.y;
        if(clipped) return;
        
        // test clipping on text
        // Rect clip{pos, pos + vec2(size*0.5,dst_size.y)};
        if(dstRect.p1.x > clip.p1.x)
        {
            float d = dstRect.p1.x - clip.p1.x;
            dstRect.p1.x -= d;
            uv.p1.x -= d/scale;
        }
        else if(dstRect.p0.x < clip.p0.x)
        {
            float d = clip.p0.x - dstRect.p0.x;
            dstRect.p0.x += d;
            uv.p0.x += d/scale;
        }
        if(dstRect.p1.y > clip.p1.y)
        {
            float d = dstRect.p1.y - clip.p1.y;
            dstRect.p1.y -= d;
            uv.p1.y -= d/scale;
        }
        else if(dstRect.p0.y < clip.p0.y)
        {
            float d = clip.p0.y - dstRect.p0.y;
            dstRect.p0.y += d;
            uv.p0.y += d/scale;
        }

        RenderInterface::setColor(color);
        RenderInterface::copy(_atlasTexture, uv, dstRect, true, true);
    }
}

void GlyphRenderer::save()
{
    RenderInterface::debugSave("./debug.png");
}

void GlyphRenderer::setBaseSize(float size)
{
    _fontBaseSize = size;
}

void GlyphRenderer::resetCharset()
{
    _charset.clear();
}

void GlyphRenderer::addChar(char c)
{
    _charset.push_back(c);
}

unsigned GlyphRenderer::size()
{
    unsigned count = (unsigned)_charset.size();
    return (unsigned)(_fontBaseSize * (std::sqrt((float)count * _glyphRatio) + 1.0f));
}

void GlyphRenderer::generateUVs()
{
    unsigned atlasSize = size();
    
    vec2 glyphSize(_fontBaseSize, _fontBaseSize * _glyphRatio);
    vec2 cursor;
    
    _uvs.clear();
    for(unsigned i = 0; i<_charset.size(); ++i)
    {
        _uvs.push_back( Rect{cursor, cursor + glyphSize} );
        cursor.x += _fontBaseSize;
        if(cursor.x+_fontBaseSize > atlasSize)
        {
            cursor.x = 0;
            cursor.y += glyphSize.y;
        }
    }
}

bool GlyphRenderer::has(char c) const
{
    auto it = std::find(_charset.begin(), _charset.end(), c);
    return it != _charset.end();
}

Rect GlyphRenderer::getUV(char c) const
{
    auto it = std::find(_charset.begin(), _charset.end(), c);
    if(it == _charset.end()) return Rect();
    
    size_t idx = it-_charset.begin();
    return _uvs[idx];
}
void GlyphRenderer::renderTexture(unsigned /*srcTarget*/)
{
    Rect uv;
    uv.p0 = vec2(0.0,0.0);
    uv.p1 = vec2(1.0,1.0);
    
    Rect dst;
    dst.p0 = vec2(0.0, 0.0);
    dst.p1 = vec2((float)size(), (float)size());
    
    RenderInterface::setColor(0xFFFFFFFF);
    RenderInterface::copy(_atlasTexture, uv, dst, true, true);
}

vec2 GlyphRenderer::computeSize(const std::string& str, float size)
{
    return vec2( (float)str.size(), mix(1.f, _glyphRatio, 0.5f) ) * size;
}