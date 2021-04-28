#ifndef DEFAULT_FONT_H
#define DEFAULT_FONT_H

#include "font/FontMaker.h"

struct DefaultFontMaker : public FontMaker
{
    void generateCharset();
};

#endif // DEFAULT_FONT_H
