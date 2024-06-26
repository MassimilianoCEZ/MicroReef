/*
 * lifeform.h
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 38
 */


#ifndef LIFEFORM_H
#define LIFEFORM_H
#include "constantes.h"
#include "message.h"
#include "shape.h"
#include <cmath>
#include <iostream>
#include <vector>


class Lifeform {
public:
    Lifeform(
        S2d posInp = {0, 0},
        int ageInp = 0); 
    S2d getPos() const;
    int getAge() const;
    bool getNotAge() const;
    bool getNotCentered() const;
    void setPos(const S2d &posInp); 
    void setPos(const double& x, const double& y);
    void setAge(const int &ageInp);

protected:
    S2d pos;
    int age;
    bool notAge;
    bool notCentered;
    void checkAge();
    void checkCenter();

private:
};

class Algae : public Lifeform {
public:
    Algae(S2d posInp = {0, 0}, unsigned ageInp = 0); 
private:
};

class Coral : public Lifeform {
private:
    unsigned int id;
    Status_cor statusCor;
    Dir_rot_cor dirRotCor;
    Status_dev statusDev;
    unsigned int nbSeg;
    std::vector<Segment> corSegments;
    bool beingEaten;

public:
    Coral(S2d posInp = {0, 0}, unsigned ageInp = 0, unsigned int idInp = 0,
          bool statusCorInp = 1, bool dirRotCorInp = 0, bool statusDevInp = 0,
          unsigned int nbSegInp = 0, 
          std::vector<Segment> segments = std::vector<Segment>(), bool beingEaten = 0);
    void changeDirSup(); 
    void updateAngle(double deltaRot = delta_rot); 
    void growSegment(double growth); 
    void setDirRotCor(Dir_rot_cor dirInp); 
    void setSegment(Segment segInp);
    const std::vector<Segment> &getCorSegments() const;
    Segment getSegment(unsigned i);
    unsigned int getId() const;
    Status_cor getStatusCor() const;
    Dir_rot_cor getDirRotCor() const;
    Status_dev getStatusDev() const;
    unsigned int getNbSeg() const;
    void setStatusCor(Status_cor status = DEAD);
    void setStatusDev (Status_dev status);
    void setBeingEaten(bool beingTasted);
    bool getBeingEaten() const;
    void popBackSegment();
};

class Scavenger : public Lifeform {
private:
    double radius;
    Status_sca statusSca;
    unsigned int corIdCib;
    bool notRadius;
    void checkRadius();
    bool onCoral;
    size_t whichSegmentEating;
    std::vector<unsigned int> possiblePrey;

public:
    Scavenger(S2d posInp = {0, 0}, unsigned ageInp = 0, double radiusInp = 0,
              bool statusScaInp = 0, unsigned int corIdInp = 0,bool onCoralInp = false,
              size_t segmentEating = 0);
    unsigned int getCorIdCib() const;
    Status_sca getStatusSca() const;
    double getRadius() const;
    bool getNotRadius() const;
    void setStatusSca(Status_sca status);
    void setCorIdCib(unsigned int id);
    unsigned int getPossiblePrey(size_t i) const;
    const std::vector<unsigned int> &getPossiblePreyVect() const;
    void addPossiblePrey(unsigned int id);
    bool getOnCoral() const;
    void setOnCoral(bool isItOn);
    size_t getWhichSegment() const;
    void setWhichSegment(const size_t &index);
    void clearPossiblePrey();
    void popBackPrey(size_t i);
};




void drawAlgae(Algae algInp);
void drawCoral(const Coral& coralInp);
void drawSca(const Scavenger& scaInp);
void changeDir(Dir_rot_cor& dirInp);


#endif