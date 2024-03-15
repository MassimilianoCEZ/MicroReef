#include "shape.h"

struct S2d // (better against bugs; according to "specification" page 10/12 rather than using class)
{
    double x = 0.;
    double y = 0.;
};

class Segment{
private:
    S2d base;
    double angle;
    double lenght;

public:
};
