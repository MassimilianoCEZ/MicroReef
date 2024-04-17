/*
 * shape.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
 */
#include "shape.h"
#include <cmath>
using namespace std;

Circle::Circle(double radiusInp, S2d baseInp) : radius(radiusInp), base(baseInp) {}

double Circle::getRadius() const { return radius; }

S2d Circle::getBase() const { return base; }

Segment::Segment(double angleInp, double lengthInp, S2d baseInp)
    : angle(angleInp), length(lengthInp), base(baseInp)
{
    end.x = cos(angle) * length + base.x;
    end.y = sin(angle) * length + base.y;
}

double Segment::getAngle() const { return angle; }

double Segment::getLength() const { return length; }

S2d Segment::getBase() const { return base; }

S2d Segment::getEnd() const { return end; }

double angularDifference(double alpha, double beta)
{
    double phi;
    if (alpha < 0)
    {
        alpha += 2 * M_PI;
    }
    if (beta < 0)
    {
        beta += 2 * M_PI;
    }
    phi = M_PI - (beta - alpha);
    if (phi > M_PI)
    {
        phi -= 2 * M_PI;
    }
    return phi;
}

bool onSegment(S2d p, S2d q, S2d r, bool approx)
{
    // if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) &&
    //     q.y >= min(p.y, r.y)) {
    //     return true;
    //}

    // scalar product pr.pq
    double s = (r.x - p.x) * (q.x - p.x) + (r.y - p.y) * (q.y - p.y);
    // norme pr
    double norme = sqrt(pow(r.x - p.x, 2) + pow(r.y - p.y, 2));
    // projection pr on pq
    double x = s / norme;
    
    if ((0 <= x) && (x <= norme))
    {
        return true;
    }

    if ((-epsil_zero <= x) && (x <= norme + epsil_zero) && (approx == 1))
    {
        return true;
    }

    return false;
}

double orientation(S2d p, S2d q, S2d r, bool approx)
{
    double val = ((q.y - p.y) * (r.x - q.x)) - ((q.x - p.x) * (r.y - q.y));
    double distR = val / sqrt(pow(q.x - p.x, 2) + pow(q.y - p.y, 2));
    if (approx)
    {
        if (fabs(distR) < epsil_zero)
            return 0;
    }
    else
    {
        if (distR == 0)
            return 0;
    }
    return (distR > 0) ? 1 : 2;
}

bool doIntersect(S2d p1, S2d q1, S2d p2, S2d q2, bool approx)
{
    int o1 = orientation(p1, q1, p2, approx);
    int o2 = orientation(p1, q1, q2, approx);
    int o3 = orientation(p2, q2, p1, approx);
    int o4 = orientation(p2, q2, q1, approx);

    if (o1 != o2 && o3 != o4)
        return true;
    if (o1 == 0 && onSegment(p1, p2, q1, approx))
        return true;
    if (o2 == 0 && onSegment(p1, q2, q1, approx))
        return true;
    if (o3 == 0 && onSegment(p2, p1, q2, approx))
        return true;
    if (o4 == 0 && onSegment(p2, q1, q2, approx))
        return true;

    return false;
}

bool supSegment(Segment segInp1, Segment segInp2)
{
    if (abs(angularDifference(segInp1.getAngle(), segInp2.getAngle())) < 0.1)
    {
        return true;
    }
    return false;
}