#ifndef BEZIE_H
#define BEZIE_H

#include "curve.h"
#include <vector>
#include <initializer_list>

unsigned long C(unsigned int n, unsigned int k);

class BezieCurve2: public Curve2 {
    std::vector<Point2> points;
public:
    BezieCurve2(std::initializer_list<Point2> list);
    BezieCurve2(std::vector<Point2> &list);

    
    Point2 operator()(double t) override;
};

#endif // BEZIE_H