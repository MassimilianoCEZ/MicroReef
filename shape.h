/*
 *shape.h
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
 */
#ifndef SHAPE_H
#define SHAPE_H

#include <cmath>
#include <iostream>
#include <string>

constexpr double epsil_zero(0.5);

struct S2d {
    double x, y;
};

class Circle {
private:
    double radius;
    S2d base;

public:
    Circle(double radiusInp = 0.0, S2d baseInp = S2d());
    double getRadius() const;
    S2d getBase() const;
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

double orientation(S2d p, S2d q, S2d r, bool approx = 0);
double angularDifference(double alpha, double beta);
bool onSegment(S2d p, S2d q, S2d r, bool approx = 0);
bool doIntersect(S2d p1, S2d q1, S2d p2, S2d q2, bool approx = 0);
bool supSegment(Segment segInp1, Segment segInp2);
#endif