/*
 * shape.h
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
 */

#ifndef SHAPE_H
#define SHAPE_H

#include <cmath>
#include <iostream>
#include <string>
#include "graphic.h"

constexpr double epsil_zero(0.5);

struct S2d {
    double x, y;
};

class Circle {
private:
    double radius;
    S2d base;

public:
    Circle(double radiusInp = 0., S2d baseInp = S2d());
    double getRadius() const;
    S2d getBase() const;
};

class Rect {
private:
    S2d center; 
    S2d corner;
    double height;
    double width;

public:
    Rect(double height = 0., double width = 0., S2d centerInp = S2d());
    double getHeight() const; 
    double getWidth() const;
    S2d getCenter() const;
    S2d getCorner() const; 
};

class Segment {
private:
    double angle;
    double length;
    S2d base;
    S2d end;

public:
    Segment(double angleInp = 0.0, double lengthInp = 0.0,
            S2d baseInp = S2d());
    double getAngle() const;
    double getLength() const;
    S2d getBase() const;
    S2d getEnd() const;
};

double orientation(S2d p, S2d q, S2d r);
double angularDifference(double alpha, double beta);
bool onSegment(S2d p, S2d q, S2d r);
bool doIntersect(Segment s1, Segment s2);
bool supSegment(Segment segInp1, Segment segInp2);
void drawSegment(const Segment& segInp, const size_t& colorIndex);
void drawCircle(const Circle& circleInp, const size_t& colorIndex);
void drawRect(const Rect& rectInp, const size_t& colorIndex);
double dotP(S2d a, S2d b, S2d c, S2d d);

#endif