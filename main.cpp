#include "curve.h"
#include "bezie.h"

int main(int argc, char** argv) {
    BezieCurve2 curve({
        Point2(0,0),
        Point2(1,2),
        Point2(0,-1),
    });

    double t;

    t = 0;
    std::cout << t << ": " << curve(t) << std::endl;
    
    t = 1.0/2.0;
    std::cout << t << ": " << curve(t) << std::endl;
    
    t = 1;
    std::cout << t << ": " << curve(t) << std::endl;
    
    return 0;
}