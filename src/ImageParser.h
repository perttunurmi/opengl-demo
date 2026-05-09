#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>

struct PPM_IMAGE
{
    unsigned char *data;
    int width;
    int height;
    int max_val;
    int BPP;
};

// TODO: IMPROVE THIS! handle commnets, max_val > 256, etc,...
// https://netpbm.sourceforge.net/doc/ppm.html
inline PPM_IMAGE ParsePPM(std::string &filename)
{
    std::ifstream ifs(filename, std::ios::binary);

    std::string magic_numer;
    int width, height, max_val;

    ifs >> magic_numer >> width >> height >> max_val;
    if (!ifs)
        throw std::runtime_error("Failed to read PPM IMAGE header");

    if (magic_numer != "P6")
        throw std::runtime_error("Only P6 supported");

    ifs.get();

    const int BPP = (max_val < 256) ? 1 : 2;
    assert(BPP == 1);
    // Lazy: If max_val > 255 means two bytes are used per pixel.
    //  I dont't want to handle that

    size_t pixel_count = width * height;
    size_t buffer_size = 3 * pixel_count;

    unsigned char *data = (unsigned char *)(std::malloc(buffer_size));
    if (!data)
    {
        throw std::bad_alloc();
    }

    ifs.read((char *)data, (std::streamsize)buffer_size);
    if (ifs.gcount() != buffer_size)
    {
        std::free(data);
        throw std::runtime_error("Reading PPM IMAGE failed");
    }

    ifs.close();

    return {data, width, height, max_val, BPP};
}
