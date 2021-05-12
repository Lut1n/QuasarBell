#ifndef DEFAULT_FONT_H
#define DEFAULT_FONT_H

#include "Font/FontMaker.h"

struct DefaultFontMaker : public FontMaker
{
    void generateCharset();
};

#endif // DEFAULT_FONT_H
