#ifndef QUASAR_BELL_TGA_EXPORTER_HPP
#define QUASAR_BELL_TGA_EXPORTER_HPP

#include <vector>
#include <string>

namespace qb
{
    struct ImageData
    {
        std::vector<char> data;
        size_t width = 256;
        size_t height = 256;
        size_t channels = 3;
    };

    void exportTGA(const std::string& filename, const ImageData& image);
};


#endif // QUASAR_BELL_TGA_EXPORTER_HPP