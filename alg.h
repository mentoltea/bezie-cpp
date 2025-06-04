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


BezieCurve2 generate_curve(
    int p_count,  
    float MIDDLEPIXEL_DEVIATION,
    float ENDPIXEL_DEVIATION
);

std::vector<BezieCurve2> generate_curves(
    int crv_count, int p_count,  
    float MIDDLEPIXEL_DEVIATION,
    float ENDPIXEL_DEVIATION
);

void generate_curves_into_array(
    BezieCurve2 *arr, 
    int crv_count, int p_count,  
    float MIDDLEPIXEL_DEVIATION,
    float ENDPIXEL_DEVIATION
);


ColorRGBA get_color_from_crv(const Image &img, Curve2 &crv, int SAMPLES=100);

void get_colors_from_bcrvs_array_to_array(
    const Image &img, 
    BezieCurve2 *crvs,
    ColorRGBA *clrs,
    int count,
    int SAMPLES=100
);

// COLOR_MERGE - koef to merge color with current sample
// k*given + (1-k)*current
//
// COLOR_DEVIATION - multiply to random color shift
//
// COLOR_STRENGTH - koef to pass to add_line
// the bigger - the more it affects on surface
// if 1 - just paints solid color
// if 0 - does not paint anything
void draw_curve(Image &img, Curve2 &crv, ColorRGBA base_clr, float width,
    float COLOR_MERGE,
    float COLOR_DEVIATION,
    float COLOR_STRENGTH,
    int SAMPLES=100);


Image algorythm(
    const Image &original, 
    bool verbose,

    int THREADS_COUNT,

    int CURVE_COUNT_PER_10x10,
    
    int P_COUNT,
    float MIDDLEPIXEL_DEVIATION,
    float ENDPIXEL_DEVIATION,
    
    float COLOR_MERGE,
    float COLOR_DEVIATION,
    float COLOR_STRENGTH,
    
    float BASE_WIDTH,
    float WIDTH_DEVIATION,

    int COLLECT_SAMPLES = 100,
    int DRAW_SAMPLES = 50
);


#endif // ALG_H