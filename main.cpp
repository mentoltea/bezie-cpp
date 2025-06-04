#include "alg.h"
#include "time.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    Image img("img.jpg", 3);

    std::cout << "Image loaded: " << img.width << " x " << img.height << " x " << img.channels << " channels" << std::endl;

    double epsilon = 1e-3;
    for (double u=0.0; u < 1.0/2.0; u += epsilon/2) {
        for (double v=1/2; v < 3.0/4.0; v += epsilon) {
            // std::cout << u << " " << v << std::endl;
            set_color_at_uv(img, Point2(u,v), ColorRGBA(0.5, 0.5, 0.5));
        }
    }

    img.save_jpg("out.jpg");

    return 0;
}