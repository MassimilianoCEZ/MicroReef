/*
 * simulation.h
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
 */
#ifndef SIMULATION_H
#define SIMULATION_H

#include "lifeform.h"
#include <fstream>
#include <memory> 
#include <string>
#include <vector>

class Simulation {
public:
    void readFile(char *file_name);
    void setAlgae(std::unique_ptr<Algae> algaeInp);
    void setCoral(std::unique_ptr<Coral> coralInp);
    void setScavenger(std::unique_ptr<Scavenger> scavengerInp);

private:
    std::vector<std::unique_ptr<Algae>> algaeVect;
    std::vector<std::unique_ptr<Coral>> coralVect;
    std::vector<std::unique_ptr<Scavenger>> scavengerVect;
    void lineDecoding(std::string line);
    void checkId();
    void checkScaId();
    void checkLength(const Segment &segmentInp, const int &idInp);
    void checkAngle(const Segment &segmentInp, const int &idInp);
    void checkExtremity(const S2d &posInp, const int &idInp, bool approx = true);
    void checkIntersect();
    void checkSup();

};

#endif
