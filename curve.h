#ifndef CURVE_H
#define CURVE_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <cstdint>

struct Point2 {
    double x, y;
    
    Point2();
    Point2(double x, double y);
    
    double distance2(const Point2 &other = Point2(0,0));
    double distance(const Point2 &other = Point2(0,0));
    
    Point2 operator-() const;
    
    Point2 operator+(const Point2 &other) const;
    Point2 operator-(const Point2 &other) const;

    Point2& operator+=(const Point2 &other) ;
    Point2& operator-=(const Point2 &other) ;
    
    Point2 operator*(double k) const;
    Point2 operator/(double k) const;
    
    bool operator==(const Point2 &other) const;
    bool operator!=(const Point2 &other) const;
};
Point2 operator*(double k, const Point2 &other);

std::ostream& operator<<(std::ostream& os, const Point2& p);

struct CurveMetainfo {
    // last 2 - for x ( < & >)
    // next 2 - for y ( < & >)
    uint8_t quarter = 0b0000;
};

class Curve2 {
public:
    CurveMetainfo meta;
    virtual Point2 operator()(double t) = 0;
};


#endif // CURVE_H