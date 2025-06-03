#ifndef CURVE_H
#define CURVE_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>

struct Point2 {
    double x, y;
    
    Point2();
    Point2(float x, float y);
    
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

class Curve2 {
public:
    virtual Point2 operator()(double t) = 0;
};


#endif // CURVE_H