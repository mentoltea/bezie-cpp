#include "curve.h"

Point2::Point2(): x(0), y(0) {}

Point2::Point2(double x, double y): x(x), y(y) {}
    
double Point2::distance2(const Point2 &other) {
    return (this->x-other.x)*(this->x-other.x) 
    + (this->y-other.y)*(this->y-other.y);
}

double Point2::distance(const Point2 &other) {
    return sqrt(this->distance2(other));
}

Point2 Point2::operator-() const {
    return Point2(-x, -y);
}

Point2 Point2::operator+(const Point2 &other) const {
    return Point2(this->x + other.x, this->y + other.y);
}
Point2 Point2::operator-(const Point2 &other) const {
    return Point2(this->x - other.x, this->y - other.y);
}

Point2& Point2::operator+=(const Point2 &other)  {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Point2& Point2::operator-=(const Point2 &other)  {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Point2 Point2::operator*(double k) const {
    return Point2(x*k, y*k);
}

Point2 Point2::operator/(double k) const {
    return Point2(x/k, y/k);
}

bool Point2::operator==(const Point2 &other) const {
    return (this->x==other.x)&&(this->y==other.y);
}
bool Point2::operator!=(const Point2 &other) const {
    return !(*this==other);
}

Point2 operator*(double k, const Point2 &other) {
    return Point2(other.x*k, other.y*k);
}

std::ostream& operator<<(std::ostream& os, const Point2& p) {
    os << " {x: " << p.x << ", y: " << p.y << "}";
    return os;
}