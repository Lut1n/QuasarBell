#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include <iostream>

#include "Core/Math.hpp"
#include "Core/Vec2.h"
#include "Graphics/TgaExporter.hpp"

struct RenderInterface
{
    static unsigned s_currentTarget;
    static unsigned s_color;
    static float s_thickness;
    static unsigned s_precision;
    
    // todo: deleteTarget and eventually call glfwTerminate()
    static void deleteTarget(unsigned id);
    static unsigned createTarget(unsigned width, unsigned height, bool win = false, const std::string& title = "sfml window");
    static vec2 getTargetSize(unsigned id);
    static vec2 getCurrentTargetSize();
    static void setTarget(unsigned target);
    static void* getTargetResource(unsigned target);
    static qb::ImageData downloadTargetImage(unsigned id);
    static void clear(unsigned color);
    static void setColor(unsigned color);
    static void setThickness(float th);
    static void setPrecision(unsigned precision);
    static void line(float x1, float y1, float x2, float y2, bool smooth = false);
    static void line(const vec2& p1, const vec2& p2, bool smooth = false);
    static void dot(const vec2& p);
    static void arc(const vec2& center, float r, float a1 , float a2);
    static void rect(const vec2& tl, const vec2& br);
    static void fill(const vec2& tl, const vec2& br);
    static void copy(unsigned srcTarget, const Rect& srcRect, const Rect& dstRect, bool ySrcInverted = false, bool yDstInverted = false);
    static bool begin();
    static void end();
    static void debugSave(const std::string& filename);
    static void updateTime();

    static unsigned createCustomProgram();
    static void destroyCustomProgram(unsigned id);
    static void updateCustomProgram(unsigned customId, const std::string& fragCode, bool bindUv);
    static void setInputFrameCustomProgram(unsigned customId, size_t srcId, size_t textureUnit, size_t uniformId);
    static void setInputCustomProgram(unsigned customId, size_t uniformId, const vec4& v4);
    static void setInputCustomProgram(unsigned customId, size_t uniformId, const Kernel& kernel);
    static void applyCustomProgram(unsigned customId, const vec2& tl, const vec2& br);
    static void resetCustomProgram(unsigned customId);

    static void debugCheck(const char* msg);
    
    static double getTime();

    static void shutdown();
};

#endif // RENDER_INTERFACE_H
