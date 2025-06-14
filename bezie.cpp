#include "bezie.h"
// #include <stdint.h>

unsigned long C(unsigned int n, unsigned int k) {
    if (k < 0 || k > n) throw std::runtime_error("k must be in range [0, n]");
    if (k==0 || k==n) return 1;
    unsigned long p = 1;
    for (unsigned long i=k+1; i<=n; i++) {
        p *= i;
    }
    for (unsigned long i=1; i<=n-k; i++) {
        p /= i;
    }
    return p;
}

double pow_id(double x, unsigned long n) {
    double p = 1;
    for (unsigned long i=1; i<=n; i++) {
        p = p*x;
    }
    return p;
}

BezieCurve2::BezieCurve2() {}

BezieCurve2::BezieCurve2(std::initializer_list<Point2> list): points(list) {}
    
BezieCurve2::BezieCurve2(std::vector<Point2> &list): points(list) {}


Point2 BezieCurve2::operator()(double t) {
    Point2 s;
    unsigned long n = this->points.size()-1;
    for (unsigned long k=0; k<=n; k++) {
        // std::cout << k << ": " << points[k] << std::endl;
        s += this->points[k] * (double)C(n, k)  * pow_id(t, n-k) * pow_id(1-t, k);
        // std::cout << s << std::endl;
    }
    return s;
}