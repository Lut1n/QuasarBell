#ifndef FONT_MAKER_H
#define FONT_MAKER_H

#include <vector>
#include <iostream>
#include <unordered_map>

#include "Graphics/RenderInterface.h"


// font

enum Direction
{
    BL,
    BR,
    TL,
    TR
};

struct CornerData
{
    float x, y;
    Direction d = TL;
    
    CornerData mirrored(bool h, bool v);
};

struct LineData
{
    float p0x, p0y;
    float p1x, p1y;
    bool jn1 = false;
    bool jn2 = false;

    LineData mirrored(bool h, bool v);
};

struct DotData
{
    float x, y;
    DotData mirrored(bool h, bool v);
};

struct CharData
{
    std::vector<CornerData> corners;
    std::vector<LineData> lines;
    std::vector<DotData> dots;
    std::vector<int> digits;
};

struct FontMaker
{
    void drawCorner(const vec2& p, const CornerData& corner);
    void drawDot(const vec2& p, float ix, float iy);
    void drawLine(const vec2& p, const LineData& line);
    void drawDigits(const vec2& p, const std::vector<int>& digits);
    void drawGlyph(const vec2& p, char c, float size = 12.0f);
    bool hasGlyph(char c);

    void mkClear();
    void mkCurr(char c);
    void mkCorner(const CornerData& data);
    void mkLine(const LineData& data);
    void mkDigits(const std::vector<int>& data);
    void mkDot(const DotData& data);
    void mkCopy(char c, bool h, bool v);
        
    float _size = 12.0;
    float _thickness = 2.0;
    float _radius = 0.25;

    std::unordered_map<char, CharData> _charset;
    char _current_char = '\0';
    CharData* _current_data = nullptr;
};

#endif // FONT_MAKER_H
