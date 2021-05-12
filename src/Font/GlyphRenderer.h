#ifndef GLYPH_RENDERER_H
#define GLYPH_RENDERER_H

#include <vector>

#include "Core/Vec2.h"
#include "Font/DefaultFontMaker.h"
#include "Graphics/RenderInterface.h"

struct GlyphRenderer
{
public:
    DefaultFontMaker _font;
    std::vector<Rect> _uvs;
    unsigned _atlasTexture;
    
    GlyphRenderer();
    ~GlyphRenderer();
    
    void initialize();
    
    void renderAtlas(unsigned target);
    
    void renderChar(char c, const vec2& pos, float size = 12.0f, unsigned color = 0xFFFFFFFF);
    void renderText(const std::string& str, const vec2& pos, float size = 12.0f, unsigned color = 0xFFFFFFFF);
    void renderClippedChar(char c, const Rect& clip, const vec2& pos, float size = 12.0f, unsigned color = 0xFFFFFFFF);
    void renderClippedText(const std::string& str, const Rect& clip, const vec2& pos, float size = 12.0f, unsigned color = 0xFFFFFFFF);
    void renderTexture(unsigned srcTarget);

    vec2 computeSize(const std::string& str, float size = 12.0f);
    
    void save();
    
private:
    std::vector<char> _charset;
    float _glyphRatio = 2.2f;
    float _fontBaseSize = 32.0f;
    
    void setBaseSize(float size);
    void resetCharset();
    void addChar(char c);
    void generateUVs();
    unsigned size();
    
    bool has(char c) const;
    Rect getUV(char c) const;
};

#endif // GLYPH_RENDERER_H
