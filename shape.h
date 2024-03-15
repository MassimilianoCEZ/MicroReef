#include <iostream>
#include <string>


constexpr double epsil_zero(0.5);
struct S2d // (better against bugs; according to "specification" page 10/12 rather than using class)
{
    double x = 0.;
    double y = 0.;
};

class Segment
{
private:
    double angle;
    double length;

public:
    Segment(double angleIpt = 0.0, double lengthIpt = 0.0);
    double getAngle() const;
};