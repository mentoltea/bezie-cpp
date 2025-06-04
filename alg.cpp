#include "alg.h"

#define STB_IMAGE_WRAPPER_IMPLEMENTATION
#include "stbimg/image.hpp"
#undef STB_IMAGE_WRAPPER_IMPLEMENTATION

#define STB_IMAGE_WRAPPER_EDIT_IMPLEMENTATION
#include "stbimg/image_edit.hpp"

Point2 xy2uv(const Image &img, Point2 xy) {
    double u = xy.x / (double)img.width;
    double v = xy.y / (double)img.height;
    return Point2(u, v);
}

Point2 uv2xy(const Image &img, Point2 uv) {
    double x = uv.x * img.width;
    double y = uv.y * img.height;
    return Point2(x, y);
}

std::pair<int,int> xy2int(const Image &img, Point2 xy) {
    int ix = (int)round(xy.x);
    int iy = (int)round(xy.y);

    if (ix < 0) ix = 0;
    else if (ix > img.width) ix = img.width-1;
    
    if (iy < 0) iy = 0;
    else if (iy > img.height) iy = img.height-1;

    return {ix, iy};
}

Point2 int2xy(const Image &img, std::pair<int,int> ixy) {
    int ix = ixy.first;
    int iy = ixy.second;
    return Point2((double)ix, (double)iy);
}

double random_uniform_double() {
    // std::uniform_real_distribution<double> unif(0,1);
    // std::default_random_engine rng;
    // return unif(rng);
    return (double)rand() / (double)RAND_MAX;
}

ColorRGBA random_color_nrm() {
    return ColorRGBA(random_uniform_double(), random_uniform_double(), random_uniform_double(), random_uniform_double());
}

Point2 random_point_nrm() {
    return Point2(random_uniform_double(), random_uniform_double());
}

ColorRGBA get_color_at_uv(const Image &img, Point2 uv) {
    std::pair<int,int> ixy = xy2int(img, uv2xy(img, uv));
    int x = ixy.first;
    int y = ixy.second;
    const uint8_t *data = img.at(x, y);

    ColorRGBA color;

    switch (img.channels) {
        case 3: {
            PixelRGB pix = *(PixelRGB*)data;
            color = (pix);
        } break;
        
        case 4: {
            PixelRGBA pix = *(PixelRGBA*)data;
            color = (pix);
        } break;

        default: {
            throw std::runtime_error("Cannot take color from image with number of channels: " + std::to_string(img.channels));
        } break;
    }

    return color;
}

void set_color_at_uv(Image &img, Point2 uv, ColorRGBA color) {
    std::pair<int,int> ixy = xy2int(img, uv2xy(img, uv));
    int x = ixy.first;
    int y = ixy.second;
    uint8_t *data = img.at(x, y);

    switch (img.channels) {
        case 3: {
            *(PixelRGB*)data = (PixelRGB)color;
        } break;
        
        case 4: {
            *(PixelRGBA*)data = (PixelRGBA)color;
        } break;

        default: {
            throw std::runtime_error("Cannot take color from image with number of channels: " + std::to_string(img.channels));
        } break;
    }
}