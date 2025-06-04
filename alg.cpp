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

// -> [0, 1)
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



BezieCurve2 generate_curve(
    int p_count,  
    float MIDDLEPIXEL_DEVIATION,
    float ENDPIXEL_DEVIATION
) {
    std::vector<Point2> pts;

    Point2 p0 = random_point_nrm();
    pts.push_back(p0);
    // std::cout << p0 << std::endl;
    Point2 pj = p0;
    for (int j=0; j<p_count-2; j++) {
        pj = pj + random_point_nrm()*MIDDLEPIXEL_DEVIATION;
        pts.push_back(pj);
        // std::cout << pj << std::endl;
    }
    
    Point2 pend = p0 + random_point_nrm()*ENDPIXEL_DEVIATION;
    pts.push_back(pend);
    // std::cout << pend << std::endl;

    BezieCurve2 crv(pts);

    return crv;
}

std::vector<BezieCurve2> generate_curves(
    int crv_count, int p_count, 
    float MIDDLEPIXEL_DEVIATION,
    float ENDPIXEL_DEVIATION
) {
    std::vector<BezieCurve2> crvs(crv_count);
    for (int i=0; i<crv_count; i++) {
        BezieCurve2 crv = generate_curve(p_count, MIDDLEPIXEL_DEVIATION, ENDPIXEL_DEVIATION);
        crvs.push_back(crv);
    }
    return crvs;
}

void generate_curves_into_array(
    BezieCurve2 *arr, 
    int crv_count, int p_count,  
    float MIDDLEPIXEL_DEVIATION,
    float ENDPIXEL_DEVIATION
) {
    for (int i=0; i<crv_count; i++) {
        arr[i] = generate_curve(p_count, MIDDLEPIXEL_DEVIATION, ENDPIXEL_DEVIATION);
    }
}








ColorRGBA get_color_from_crv(const Image &img, Curve2 &crv, int SAMPLES) {
    ColorRGBA sum;
    for (int i=0; i<=SAMPLES; i++) {
        double t = (double)i / (double)SAMPLES;
        Point2 p = crv(t);
        ColorRGBA clr = get_color_at_uv(img, p);
        sum += clr;

        // std::cout << t << ":\t" << clr << std::endl;

        // if (p.x < 0.5)  crv.meta.quarter = crv.meta.quarter | 0b0001;
        // else            crv.meta.quarter = crv.meta.quarter | 0b0010;

        // if (p.y < 0.5)  crv.meta.quarter = crv.meta.quarter | 0b0100; 
        // else            crv.meta.quarter = crv.meta.quarter | 0b1000; 
        
    }
    sum = sum/SAMPLES;
    return sum;
}

void get_colors_from_bcrvs_array_to_array(
    const Image &img, 
    BezieCurve2 *crvs,
    ColorRGBA *clrs,
    int count,
    int SAMPLES
) {
    for (int i=0; i < count; i++) {
        clrs[i] = get_color_from_crv(img, crvs[i], SAMPLES);
    }
}








void draw_curve(Image &img, Curve2 &crv, ColorRGBA base_clr, float width,
    float COLOR_MERGE,
    float COLOR_DEVIATION,
    float COLOR_STRENGTH,
    int SAMPLES) 
{
    Point2 prev_p = crv(0);
    // std::cout << prev_p << std::endl;
    for (int i=1; i<=SAMPLES; i++) {
        double t = (double)i / (double)SAMPLES;
        Point2 uv = crv(t);
        // std::cout << t << ": " << uv << std::endl;

        ColorRGBA current_clr = get_color_at_uv(img, uv);

        ColorRGBA color = (COLOR_MERGE*current_clr + (1-COLOR_MERGE)*base_clr)
            + COLOR_DEVIATION*random_color_nrm();

        Point2 prev_xy = uv2xy(img, prev_p);
        Point2 xy = uv2xy(img, uv);

        switch (img.channels) {
            case 3: {
                add_line_noedge<PixelRGB>(
                    img, 
                    prev_xy.x, prev_xy.y, 
                    xy.x, xy.y,
                    (PixelRGB)color, 
                    width, 
                    COLOR_STRENGTH
                );
            } break;
            
            case 4: {
                add_line_noedge<PixelRGBA>(
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

    int COLLECT_SAMPLES,
    int DRAW_SAMPLES
) {
    Image modified(original.width, original.height, original.channels);
    // Image modified = original;
    
    std::vector<std::thread> thread_pull(THREADS_COUNT);
    int available_threads = THREADS_COUNT;

    int CURVE_COUNT = modified.width * modified.height * CURVE_COUNT_PER_10x10 / (10*10);

    // -----------
    // GENERATING
    // -----------
    if (verbose) std::cout << "GENERATING IN " << THREADS_COUNT << " THREADS..." << std::endl;
    
    BezieCurve2 *curves = new BezieCurve2[CURVE_COUNT];
    
    int CURVES_PER_THREAD = CURVE_COUNT/THREADS_COUNT;
    int REMAINING = CURVE_COUNT - THREADS_COUNT*CURVES_PER_THREAD;

    for (int i=0; i < THREADS_COUNT; i++) {
        thread_pull[i] = std::thread(
            generate_curves_into_array,
            curves + i*CURVES_PER_THREAD,
            CURVES_PER_THREAD,
            P_COUNT,
            MIDDLEPIXEL_DEVIATION,
            ENDPIXEL_DEVIATION
        );
    }

    if (REMAINING > 0) {
        generate_curves_into_array(
            curves + THREADS_COUNT*CURVES_PER_THREAD,
            REMAINING,
            P_COUNT,
            MIDDLEPIXEL_DEVIATION,
            ENDPIXEL_DEVIATION
        );
    }

    for (int i=0; i < THREADS_COUNT; i++) {
        if (thread_pull[i].joinable()) thread_pull[i].join();
    }

    if (verbose) std::cout << "GENERATED " << CURVE_COUNT << " CURVES" << std::endl;

    // -----------
    // COLLECTING
    // -----------
    if (verbose) std::cout << "COLLECTING IN " << THREADS_COUNT << " THREADS..." << std::endl;

    ColorRGBA *colors = new ColorRGBA[CURVE_COUNT];

    for (int i=0; i < THREADS_COUNT; i++) {
        thread_pull[i] = std::thread(
            get_colors_from_bcrvs_array_to_array,
            original,
            curves + i*CURVES_PER_THREAD,
            colors + i*CURVES_PER_THREAD,
            CURVES_PER_THREAD,
            COLLECT_SAMPLES
        );
    }

    if (REMAINING > 0) {
        get_colors_from_bcrvs_array_to_array(
            original,
            curves + THREADS_COUNT*CURVES_PER_THREAD,
            colors + THREADS_COUNT*CURVES_PER_THREAD,
            REMAINING,
            COLLECT_SAMPLES
        );
    }

    for (int i=0; i < THREADS_COUNT; i++) {
        if (thread_pull[i].joinable()) thread_pull[i].join();
    }

    if (verbose) std::cout << "COLLECTED" << std::endl;

    // -----------
    // DRAWING
    // -----------

    if (verbose) std::cout << "DRAWING IN " << 1 << " THREAD (Multithread drawing not implemented yet)..." << std::endl;

    // TODO: parallel drawing
    // crv1 meta.quarter = 0b....
    // crv2 meta.quarter = 0b....
    // crv1 meta & crv2 meta == 0 ->
    // -> they dont share any quarter ->
    // -> it is safe to draw them parallel

    for (int i=0; i<CURVE_COUNT; i++) {
        if (verbose && i%10000==0)
            std::cout << "\t" << i << "/" << CURVE_COUNT << std::endl;  
        float width = BASE_WIDTH + random_uniform_double()*WIDTH_DEVIATION;
        draw_curve(modified, curves[i], colors[i], width,
            COLOR_MERGE, COLOR_DEVIATION, COLOR_STRENGTH, DRAW_SAMPLES
        );
    }
    
    if (verbose) std::cout << "DRAWED" << std::endl;


    return modified;
}