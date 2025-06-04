#include "alg.h"
#include "time.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    Image img("img.jpg", 3);

    std::cout << "Image loaded: " << img.width << " x " << img.height << " x " << img.channels << " channels" << std::endl;

    BezieCurve2 crv({
        Point2(0,0),
        Point2(0.5,2),
        Point2(1,0)
    });

    ColorRGBA clr(0.5, 0.1, 0.25);

    draw_curve(img, crv, clr,
        5,
        0.2, 0.1, 0.85,
        100
    );

    img.save_jpg("out.jpg");

    return 0;
}