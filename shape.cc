#include "shape.h"

struct S2d // (better against bugs; according to "specification" page 10/12 rather than using class)
{
    double x = 0.;
    double y = 0.;
};

class Segment
{
private:
    S2d base;
    double angle;
    double length;

public:
    Segment(S2d baseIpt, double angleIpt = 0.0, double lengthIpt = 0.0)
        : base(), angle(angleIpt), length(lengthIpt) // can you put a struct in the initialization list? base???
    {
    }

    const double getAngle()
    {
        return angle;
    }
};


