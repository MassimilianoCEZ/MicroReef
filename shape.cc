#include "shape.h"

Segment::Segment(double angleIpt, double lengthIpt)
    : angle(angleIpt), length(lengthIpt) {}

double Segment::getAngle() const
{
    return angle;
}
