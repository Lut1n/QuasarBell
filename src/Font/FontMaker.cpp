#include "font/FontMaker.h"


CornerData CornerData::mirrored(bool h, bool v)
{
    if(!h && !v) return *this;
    
    CornerData r = *this;
    if(h)
    {
        r.x = 2 - x;
        if(r.d == TL) r.d = TR;
        else if(r.d == TR) r.d = TL;
        else if(r.d == BL) r.d = BR;
        else if(r.d == BR) r.d = BL;
    }
    if(v)
    {
        r.y = -2 - y;
        if(r.d == TL) r.d = BL;
        else if(r.d == TR) r.d = BR;
        else if(r.d == BL) r.d = TL;
        else if(r.d == BR) r.d = TR;
    }
    return r;
}

LineData LineData::mirrored(bool h, bool v)
{
    if(!h && !v) return *this;
    
    LineData r = *this;
    if(h)
    {
        r.p0x = 2 - p0x;
        r.p1x = 2 - p1x;
    }
    if(v)
    {
        r.p0y = -2 - p0y;
        r.p1y = -2 - p1y;
    }
    return r;
}

DotData DotData::mirrored(bool h, bool v)
{
    if(!h && !v) return *this;
    
    DotData r = *this;
    if(h)
        r.x = 2 - x;
    if(v)
        r.y = -2 - y;
    return r;
}


void FontMaker::drawCorner(const vec2& p, const CornerData& corner)
{
    float pad_f = 0.2;
    float padding = pad_f * _size;
    float r = _radius * _size;
    float h = _size;
    float w = h - padding * 2.0;
    
    vec2 padded_p = p + vec2(padding,0.0) + vec2(corner.x * w, corner.y * h)*0.5;
    
    float a_step = 1.57;
    
    float a1 = 0.0;
    float a2 = a1 + a_step;
    float a3 = a2 + a_step;
    float a4 = a3 + a_step;
    float a5 = a4 + a_step;
    
    if(corner.d == TL)
        RenderInterface::arc(padded_p+vec2(-r,-r), r, a1, a2);
    else if(corner.d == TR)
        RenderInterface::arc(padded_p+vec2(r,-r), r, a2, a3);
    else if(corner.d == BR)
        RenderInterface::arc(padded_p+vec2(r,r), r, a3, a4);
    else if(corner.d == BL)
        RenderInterface::arc(padded_p+vec2(-r,r), r, a4, a5);
}

void FontMaker::drawDot(const vec2& p, float ix, float iy)
{
    float pad_f = 0.2;
    float padding = pad_f * _size;
    float h = _size;
    float w = h - padding * 2.0;
    
    float x1 = p.x + padding + ix * w * 0.5;
    float y1 = p.y + iy * h * 0.5;
    
    RenderInterface::dot(vec2(x1, y1));
}

void FontMaker::drawLine(const vec2& p, const LineData& line)
{
    float pad_f = 0.2;
    float padding = pad_f * _size;
    float r = _radius * _size;
    float h = _size;
    float w = h - padding * 2.0;
    
    vec2 pa = p + vec2(padding,0.f) + vec2(line.p0x,line.p0y)*vec2(w,h)*0.5;
    vec2 pb = p + vec2(padding,0.f) + vec2(line.p1x,line.p1y)*vec2(w,h)*0.5;
    
    vec2 dif = pb - pa;
    
    float len = dif.length();
    vec2 dir = dif / len;
    
    if(line.jn1)
    {
        len -= r;
        pa += dir * r;
    }
    if(line.jn2)
        len -= r;
    
    if(len <=0.0) return;
    
    pb = pa + dir * len;
    
    RenderInterface::line(pa, pb);
}

void FontMaker::drawDigits(const vec2& p, const std::vector<int>& digits)
{
    for(int di : digits)
    {
        LineData l;
        if(di == 0)
            l = {0, 0, 2, 0};
        if(di == 1)
            l = {0, -1, 2, -1};
        if(di == 2)
            l = {0, -2, 2, -2};
        if(di == 3)
            l = {0, 0, 0, -1};
        if(di == 4)
            l = {0, -1, 0, -2};
        if(di == 5)
            l = {2, 0, 2, -1};
        if(di == 6)
            l = {2, -1, 2, -2};
        
        drawLine(p,l);
    }
}

void FontMaker::drawGlyph(const vec2& p, char c, float size)
{
    if(!hasGlyph(c)) return;
    
    _size = size;
    CharData data = _charset[c];
    for(auto& corner : data.corners)
        drawCorner(p,corner);
    for(auto& line : data.lines)
        drawLine(p,line);
    for(auto& dot : data.dots)
        drawDot(p,dot.x, dot.y);
    drawDigits(p, data.digits);
}

bool FontMaker::hasGlyph(char c)
{
    return _charset.find(c) != _charset.end();
}

void FontMaker::mkClear()
{
    _charset.clear();
}

void FontMaker::mkCurr(char c)
{
    _current_data = &_charset[c];
    _current_char = c;
}

void FontMaker::mkCorner(const CornerData& data)
{
    _current_data->corners.push_back(data);
}
void FontMaker::mkLine(const LineData& data)
{
    _current_data->lines.push_back(data);
}
void FontMaker::mkDigits(const std::vector<int>& data)
{
    _current_data->digits = data;
}
void FontMaker::mkDot(const DotData& data)
{
    _current_data->dots.push_back(data);
}

void FontMaker::mkCopy(char c, bool h, bool v)
{
    if(_charset.find(c) == _charset.end())
        return;
    
    auto& src_data = _charset[c];
    for(auto& c : src_data.corners)
        _current_data->corners.push_back(c.mirrored(h,v));
    for(auto& l : src_data.lines)
        _current_data->lines.push_back(l.mirrored(h,v));
    for(auto& d : src_data.digits)
        _current_data->digits.push_back(d);
    for(auto& d : src_data.dots)
        _current_data->dots.push_back(d.mirrored(h,v));
}
