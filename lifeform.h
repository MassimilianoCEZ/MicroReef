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
    Algae(S2d posInp = {0, 0}, unsigned ageInp = 0); // default constructor
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

public:
    Coral(S2d posInp = {0, 0}, unsigned ageInp = 0, unsigned int idInp = 0,
          bool statusCorInp = 1, bool dirRotCorInp = 0, bool statusDevInp = 0,
          unsigned int nbSegInp = 0, 
          std::vector<Segment> segments = std::vector<Segment>());
    void setSegment(Segment segInp);
    const std::vector<Segment> &getCorSegments() const;
    Segment getSegment(unsigned i);
    unsigned int getId() const;
    Status_cor getStatusCor() const;
    Dir_rot_cor getDirRotCor() const;
    Status_dev getStatusDev() const;
    unsigned int getNbSeg() const;
};

class Scavenger : public Lifeform {
private:
    double radius;
    Status_sca statusSca;
    unsigned int corIdCib;
    bool notRadius;
    void checkRadius();
public:
    Scavenger(S2d posInp = {0, 0}, unsigned ageInp = 0, double radiusInp = 0,
              bool statusScaInp = 0, unsigned int corIdInp = 0);
    unsigned int getCorIdCib() const;
    Status_sca getStatusSca() const;
    double getRadius() const;
    bool getNotRadius() const;
};

void drawAlgae(Algae algInp);
void drawCoral(const Coral& coralInp);
void drawSca(const Scavenger& scaInp);

#endif