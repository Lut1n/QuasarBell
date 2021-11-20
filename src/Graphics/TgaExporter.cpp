#include "Graphics/TgaExporter.hpp"

#include "Core/Math.hpp"

#include <fstream>

//--------------------------------------------------------------
unsigned int qb::ImageData::sample(float u, float v)
{
    int x = (int)(u * (float)width-1.0f);
    int y = (int)(v * (float)height-1.0f);
    x = (size_t)qb::clamp(x, 0, (int)width-1);
    y = (size_t)qb::clamp(y, 0, (int)height-1);

    size_t idx = y * width*channels + x*channels;
    unsigned int color = 0;
    color = data[idx + 0] << 24;
    if (channels > 1)
        color |= data[idx + 1] << 16;
    if (channels > 2)
        color |= data[idx + 2] << 8;
    if (channels > 3)
        color |= data[idx + 3];
    return color;
}

//--------------------------------------------------------------
void qb::exportTGA(const std::string& filename, const ImageData& image)
{
    std::ofstream ostrm(filename,std::ios::binary);
    unsigned char chnl = (unsigned char)image.channels;
    unsigned char v = 0;
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); //ID length (1o)
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // Color map type (1o)
    if(chnl == 1) v = 3; // black and white
    if(chnl == 3 || chnl == 4) v = 2;	// TrueColor
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // Image type (1o)
    v = 0;	// no color map
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // Colormap spec (5o)
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // Colormap spec (5o)
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // Colormap spec (5o)
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // Colormap spec (5o)
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // Colormap spec (5o)
    unsigned short d = 0;
    ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // X-origin (2o)
    ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // Y-origin (2o)
    d = (unsigned short)image.width;
    ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // width (2o)
    d = (unsigned short)image.height;
    ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // height (2o)
    // v = 8;	// default for black and white
    v = chnl * 8;	// 24 for RGB; 32 for RGBA
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // px depth (1o)
    v = 0;
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // description (1o)
    
    size_t totalSize = image.width * image.height * image.channels;

    ostrm.write(image.data.data(),totalSize); // image data
}
