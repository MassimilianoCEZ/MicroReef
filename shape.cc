#include <iostream>  //only preliminary REMOVE AFTER TEST
using namespace std; // only preliminary REMOVE AFTER TEST

constexpr double epsil_zero(0.5);

struct S2d // (plus robuste au bugs; according to "specification" page 10/12)
{
    double x = 0.;
    double y = 0.;
};


// pleliminary main function to test code
int main()
{
    S2d testPoint;
    cout << testPoint.x << " is x and" << endl;
    cout << testPoint.y << " is y";
}