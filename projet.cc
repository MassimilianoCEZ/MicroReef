/*
 * projet.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
 */
#include <fstream>
#include <iostream>
#include "simulation.h"
using namespace std;

int main(int argc, char *argv[])
{
    
    Simulation sim;
    sim.readFile(argv[1]);
    cout << message::success() << endl;
    return 0;
}
