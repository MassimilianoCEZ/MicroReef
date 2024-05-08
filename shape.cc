/*
 * shape.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 27
 * 50% Massimiliano 50% Carl
 */
#include "shape.h"
#include <cmath>
using namespace std;

static double zero(0);

Circle::Circle(double radiusInp, S2d baseInp) : radius(radiusInp), base(baseInp) {}

double Circle::getRadius() const { return radius; }

S2d Circle::getBase() const { return base; }

Rect::Rect(double heightInp, double widthInp, S2d centerInp) 
    : height(heightInp), width(widthInp), center(centerInp) {}

double Rect::getHeight() const { return height; }

double Rect::getWidth() const { return width; }

S2d Rect::getCenter() const { return center; }

S2d Rect::getCorner() const {
    S2d corner;
    corner.x = center.x - width/2.;
    corner.y = center.y - height/2.;
    return corner;
 }

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


bool onSegment(S2d p, S2d q, S2d r, bool withEpsil){
	double s(dotP(p, r, p, q));
	double norm(sqrt(dotP(p, r, p, r)));
	double x(s / norm);
    double zero(0);

    if(withEpsil){
        zero = epsil_zero;
    }

	if ((-zero <= x) && (x <= (norm + zero))){
		return true;
	}
    
	return false;
}

double orientation(S2d p, S2d q, S2d r, bool withEpsil){
	double val = (q.y - p.y) * (r.x - q.x) -
				 (q.x - p.x) * (r.y - q.y);
    double distance = val / (sqrt(pow((q.y - p.y), 2)+pow((q.x - p.x), 2)));
    double zero(0);

    if(withEpsil){
        zero = epsil_zero;
    }

	if (abs(distance) <= zero){
		return 0;
	}

	return (distance >= zero) ? 1 : 2;
}

bool doIntersect(Segment s1, Segment s2, bool withEpsil){
	S2d p1 = s1.getBase();
	S2d q1 = s1.getEnd();
	S2d p2 = s2.getBase();
	S2d q2 = s2.getEnd();
	double o1 = orientation(p1, q1, p2, withEpsil);
	double o2 = orientation(p1, q1, q2, withEpsil);
	double o3 = orientation(p2, q2, p1, withEpsil);
	double o4 = orientation(p2, q2, q1, withEpsil);
	if (o1 != o2 && o3 != o4)  return true;
	if (o1 == 0 && onSegment(p1, p2, q1, withEpsil))  return true;
	if (o2 == 0 && onSegment(p1, q2, q1, withEpsil))  return true;
	if (o3 == 0 && onSegment(p2, p1, q2, withEpsil))  return true;
	if (o4 == 0 && onSegment(p2, q1, q2, withEpsil))  return true;
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

void drawSegment(const Segment& segInp, const size_t& colorIndex) {
    graphic_draw_segment(segInp.getBase().x, segInp.getBase().y, segInp.getEnd().x, 
                          segInp.getEnd().y, colorIndex);
}

void drawCircle(const Circle& circleInp, const size_t& colorIndex) {
    graphicDrawCircle(circleInp.getBase().x, circleInp.getBase().y,
                       circleInp.getRadius(), colorIndex);
}

void drawRect(const Rect& rectInp, const size_t& colorIndex) {
    graphicDrawRect(rectInp.getCenter().x, rectInp.getCenter().y, rectInp.getWidth(), 
                     rectInp.getHeight(), colorIndex);
}


double dotP(S2d a, S2d b, S2d c, S2d d){
	double x1 = b.x - a.x;
	double y1 = b.y - a.y;
	double x2 = d.x - c.x;
	double y2 = d.y - c.y;
	return x1 * x2 + y1 * y2;
};