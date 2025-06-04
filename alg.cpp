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
    else if (ix >= img.width) ix = img.width-1;
    
    if (iy < 0) iy = 0;
    else if (iy >= img.height) iy = img.height-1;

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
            throw std::runtime_error("Cannot write color to image with number of channels: " + std::to_string(img.channels));
        } break;
    }
}


std::vector<BezieCurve2> generate_curves(const Image &img, int p_count, int crv_count, 
    float MIDDLEPIXEL_DEVIATION,
    float ENDPIXEL_DEVIATION
) {
    std::vector<BezieCurve2> crvs(crv_count);
    for (int i=0; i<crv_count; i++) {
        std::vector<Point2> pts(p_count);

        Point2 p0 = random_point_nrm();
        pts.push_back(p0);

        Point2 pj = p0;
        for (int j=0; j<p_count-2; j++) {
            pj = pj + random_point_nrm()*MIDDLEPIXEL_DEVIATION;
            pts.push_back(pj);
        }

        Point2 pend = p0 + random_point_nrm()*ENDPIXEL_DEVIATION;
        pts.push_back(pend);

        BezieCurve2 crv(pts);
        crvs.push_back(crv);
    }
    return crvs;
}


ColorRGBA get_color_from_crv(const Image &img, Curve2 &crv, int SAMPLES) {
    ColorRGBA sum;
    for (int i=0; i<=SAMPLES; i++) {
        double t = (double)i / (double)SAMPLES;
        Point2 p = crv(t);
        ColorRGBA clr = get_color_at_uv(img, p);
    }
    sum = sum/SAMPLES;
    return sum;
}

void draw_curve(Image &img, Curve2 &crv, ColorRGBA base_clr, float width,
    float COLOR_MERGE,
    float COLOR_DEVIATION,
    float COLOR_STRENGTH,
    int SAMPLES) 
{
    Point2 prev_p = crv(0);
    for (int i=1; i<=SAMPLES; i++) {
        double t = (double)i / (double)SAMPLES;
        Point2 uv = crv(t);

        ColorRGBA current_clr = get_color_at_uv(img, uv);

        ColorRGBA color = (COLOR_MERGE*current_clr + (1-COLOR_MERGE)*base_clr)
            + COLOR_DEVIATION*random_color_nrm();

        Point2 prev_xy = uv2xy(img, prev_p);
        Point2 xy = uv2xy(img, uv);

        switch (img.channels) {
            case 3: {
                add_line<PixelRGB>(
                    img, 
                    prev_xy.x, prev_xy.y, 
                    xy.x, xy.y,
                    (PixelRGB)color, 
                    width, 
                    COLOR_STRENGTH
                );
            } break;
            
            case 4: {
                add_line<PixelRGBA>(
                    img, 
                    prev_xy.x, prev_xy.y, 
                    xy.x, xy.y,
                    (PixelRGBA)color, 
                    width, 
                    COLOR_STRENGTH
                );
            } break;

            default: {
                throw std::runtime_error("Cannot write color to image with number of channels: " + std::to_string(img.channels));
            } break;
        }
        

        prev_p = uv;
    }
}