#include <iostream>
#include <string>

#include "shape.h" // NOTE this Massi ;)
                   // how to make sure that whatever is on this file has acces to shape.cc
using namespace std;
int main()
{
    S2d point;
    // checks wether field of structure S2d can be accessed = CONFIRMED
    cout << point.x << endl;

    Segment segmentOne;
    // checks constructor and getter for angle field
    cout << segmentOne.getAngle() << endl;
}