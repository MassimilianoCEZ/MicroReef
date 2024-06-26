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
#include <algorithm>

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
    int genUniqueId(); // new
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
    void checkAlgMaxAge(size_t i);
    void checkCorMaxAge(size_t i); 
    void checkScaMaxAge(size_t i);
    size_t closestCoral(size_t i);
    void closestScavenger(size_t i);
    void closestScavenger(size_t i, std::vector<int> scaIndexVect);
    S2d LastSegmentBase(size_t i);
    S2d LastSegmentEnd(size_t i);
    void updateScavenger();
    void updateAgeSca();
    void getToTheCoral(size_t i);
    void assignSingleTarget();
    void setIndexVect(std::vector<int>& scaIndexVect, std::vector<int>& corIndexVect,
                      bool& firstTime);
    bool haveTheScavengerChose();
    size_t idToIndex(unsigned int id);
    Segment LastSegment(size_t i); 
    void checkCollision(const int& i); 
    void closeAlg(Segment lastSeg, int direction, int& index, double& angularDist);
    void corRepro(int corIndex);
    void searchEatAlg(int i); 
    void moveOnTheCoral(size_t i);
    void setPos(size_t i, double distance, double angle);
    void setEnd(size_t i);

};

void drawSimulation(const Simulation& simInp);

#endif

