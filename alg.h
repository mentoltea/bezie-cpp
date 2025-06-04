#ifndef ALG_H
#define ALG_H

#include "stbimg/image.hpp"
#include "stbimg/image_edit.hpp"
#include "bezie.h"

#include <utility>
#include <vector>
#include <thread>
#include <random>

Point2 xy2uv(const Image &img, Point2 xy);

Point2 uv2xy(const Image &img, Point2 uv);

std::pair<int,int> xy2int(const Image &img, Point2 xy);

Point2 int2xy(const Image &img, std::pair<int,int> ixy);


double random_uniform_double();

// r -> [0, 1)
// g -> [0, 1)
// b -> [0, 1)
// a -> [0, 1)
ColorRGBA random_color_nrm();

// x -> [0, 1)
// y -> [0, 1)
Point2 random_point_nrm();



ColorRGBA get_color_at_uv(const Image &img, Point2 uv);

void set_color_at_uv(Image &img, Point2 uv, ColorRGBA clr);



std::vector<BezieCurve2> generate_curves(const Image &img, int p_count, int crv_count, 
    float ENDPIXEL_DEVIATION
);

ColorRGBA get_color_from_crv(const Image &img, Curve2 &crv, int SAMPLES=100);

void draw_curve(Image &img, Curve2 &crv, ColorRGBA base_clr, float width,
    float COLOR_MERGE,
    float COLOR_DEVIATION,
    int SAMPLES=100);


Image algorythm(const Image &original,
    int CURVE_COUNT_PER_10x10,
    int THREADS_COUNT,
    float ENDPIXEL_DEVIATION,
    float COLOR_MERGE,
    float COLOR_DEVIATION,
    float BASE_WIDTH,
    float WIDTH_DEVIATION
);


#endif // ALG_H