/*
 * simulation.h
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 29
 */
#ifndef SIMULATION_H
#define SIMULATION_H

#include "lifeform.h"
#include <fstream>
#include <memory> 
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class Simulation {
public:
    void readFile(char *file_name, bool open);
    void setAlgae(std::unique_ptr<Algae> algaeInp);
    void setCoral(std::unique_ptr<Coral> coralInp);
    void setScavenger(std::unique_ptr<Scavenger> scavengerInp);
    void createAlgae(std::istringstream& data, S2d pAlg, int ageAlg);
    void createAlgae( S2d pAlg, int ageAlg);
    void createScavenger(std::istringstream& data, S2d pSca, int ageSca, int radius,
                        bool statusSca, int corIdCib);
    void createScavenger(std::istringstream& data, S2d pSca, int ageSca, int radius,
                        bool statusSca);
    void createCoral(S2d pCor, int ageCor, int id, bool statusCor, bool dirRotCor,
                     bool statusDev, int nbSeg, std::vector<Segment> corSegments);
    void addSegments(std::vector<Segment>& corSegments, double angle, double length, 
                     S2d pCor, Segment& seg);
    const std::vector<std::unique_ptr<Algae>> &getAlgaeVect() const;
    const std::vector<std::unique_ptr<Coral>> &getCoralVect() const;
    const std::vector<std::unique_ptr<Scavenger>> &getScaVect() const;
    unsigned int getNbCor();
    unsigned int getNbAlg();
    unsigned int getNbSca();
    void clearCoralId(unsigned int id);
    void saveSimulation(std::stringstream& data);
    void saveAlgae(std::stringstream& data);
    void saveCorals(std::stringstream& data);
    void saveScavengers(std::stringstream& data);
    void clearSimulation();
    void update(const bool& spawnAlgae);
    
private:
    std::vector<std::unique_ptr<Algae>> algaeVect;
    std::vector<std::unique_ptr<Coral>> coralVect;
    std::vector<std::unique_ptr<Scavenger>> scavengerVect;
    void lineDecoding(std::string line, bool open, bool& errors);
    void checkId(bool& errors);
    void checkScaId(bool& errors);
    void checkLength(const Segment &segmentInp, const int &idInp, bool& errors);
    void checkAngle(const Segment &segmentInp, const int &idInp, bool& errors);
    void checkExtremity(const S2d &posInp, const int &idInp,bool approx, bool& errors);
    void checkIntersect(bool& errors);
    void checkSup(bool& errors);
    void lifeformChecks(bool& errors);
    void checkPosAgeAlg(bool& errors);
    void checkPosAgeCor(bool& errors);
    void checkPosAgeRadiusSca(bool& errors);
};

void drawSimulation(const Simulation& simInp);

#endif

