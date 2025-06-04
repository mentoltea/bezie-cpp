#include "alg.h"
#include "time.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    Image img("img.jpg", 3);
    std::cout << "Image loaded: " << img.width << " x " << img.height << " x " << img.channels << " channels" << std::endl;

    Image mod = algorythm(
        img, true, 
        4, 
        16, 4, 
        0.05, 0.025,
        0.1, 0.1, 0.9,
        7, 6,
        50, 25
    );
    
    
    // Image mod = img;
    // BezieCurve2 crv = generate_curve(4, 0.1, 0.05);
    // ColorRGBA clr = get_color_from_crv(mod, crv);
    // std::cout << clr << std::endl;
    // draw_curve(mod, crv, clr, 4, 0, 0.1, 1);


    mod.save_jpg("modified.jpg");

    return 0;
}