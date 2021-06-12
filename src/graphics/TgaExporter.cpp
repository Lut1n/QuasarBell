#include "Graphics/TgaExporter.hpp"

#include <fstream>


//--------------------------------------------------------------
void qb::exportTGA(const std::string& filename, const ImageData& image)
{
    std::ofstream ostrm(filename,std::ios::binary);
    unsigned char chnl = image.channels;
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
    d = image.width;
    ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // width (2o)
    d = image.height;
    ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // height (2o)
    // v = 8;	// default for black and white
    v = chnl * 8;	// 24 for RGB; 32 for RGBA
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // px depth (1o)
    v = 0;
    ostrm.write(reinterpret_cast<char*>(&v), sizeof v); // description (1o)
    
    int totalSize = image.width * image.height * image.channels;

    /*Image::Ptr tosave = Image::create(image.width,image.height,chnl);
    if(chnl == 1) tosave->copy(image);
    else if(chnl == 2) tosave->copy(img, {1,0});
    else if(chnl == 3) tosave->copy(img, {2,1,0});
    else if(chnl == 4) tosave->copy(img, {2,1,0,3});*/
    ostrm.write(image.data.data(),totalSize); // image data
}
