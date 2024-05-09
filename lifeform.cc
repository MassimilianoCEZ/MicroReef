/*
 * lifeform.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 38
 * 50% Massimiliano 50% Carl
 */


#include "lifeform.h"

using namespace std;

Lifeform::Lifeform(
    S2d posInp, int ageInp) 
    : pos(posInp), age(ageInp), notAge(0), notCentered(0) {
    checkAge();
    checkCenter();
}

void Lifeform::checkAge() {
    if (age <= 0) {
        cout << message::lifeform_age(age);
        notAge = 1;
    }
}

void Lifeform::checkCenter() {
    if ((pos.x > dmax - 1.) or (pos.y > dmax - 1.) or (pos.x < 1.) or (pos.y < 1.)) {
        cout << message::lifeform_center_outside(pos.x, pos.y);
        notCentered = 1;
    }
}

bool Lifeform::getNotAge() const { return notAge; }

bool Lifeform::getNotCentered() const { return notCentered; }

S2d Lifeform::getPos() const {
     return pos; 
}

int Lifeform::getAge() const {
     return age; 
}

unsigned int Coral::getId() const { 
    return id; 
}

unsigned int Scavenger::getCorIdCib() const { 
    return corIdCib; 
}

void Lifeform::setPos(const S2d &posInp) {
     pos = posInp; 
}

void Lifeform::setAge(const int &ageInp) { 
    age = ageInp; 
}

Status_cor Coral::getStatusCor() const {
    return statusCor;
}
Dir_rot_cor Coral::getDirRotCor() const { 
    return dirRotCor;
}
unsigned int Coral::getNbSeg() const{ 
    return corSegments.size(); 
}
Status_dev Coral::getStatusDev() const{
    return statusDev;
}
double Scavenger::getRadius() const {
    return radius;
}
bool Scavenger::getNotRadius() const {
    return notRadius;
}


Algae::Algae(S2d posInp, unsigned ageInp) : Lifeform(posInp, ageInp) {}

Coral::Coral(S2d posInp, unsigned ageInp, unsigned int idInp, bool statusCorInp,
             bool dirRotCorInp, bool statusDevInp, unsigned int nbSegInp,
             vector<Segment> segments)
    : Lifeform(posInp, ageInp), id(idInp), nbSeg(nbSegInp),
      corSegments(move(segments)) {
    statusCor = (statusCorInp) ? ALIVE : DEAD;
    dirRotCor = (dirRotCorInp) ? INVTRIGO : TRIGO;
    statusDev = (statusDevInp) ? REPRO : EXTEND;
}

void Coral::updateAngle() { // updates the angle of one coral dependant of its state of dirRotCor (trigo or invtrigo)
    
    
    //if(EXTREM){ change direction}

    Segment* lastSeg (&corSegments[corSegments.size()-1]);
    double a(0);
    double l((*lastSeg).getLength());
    S2d b((*lastSeg).getBase());

    if(dirRotCor){ // if invtrigo
        a = (*lastSeg).getAngle() - delta_rot;

    } else { // if trigo
        a = (*lastSeg).getAngle() + delta_rot;
    }
    (*lastSeg) = Segment(a,l,b);

};

void Coral::setSegment(Segment segInp) { corSegments.push_back(segInp); }


const vector<Segment> &Coral::getCorSegments() const { return corSegments; }

Segment Coral::getSegment(unsigned i) { return corSegments[i]; }

Scavenger::Scavenger(S2d posInp, unsigned ageInp, double radiusInp, bool statusScaInp,
                     unsigned int corIdInp)
    : Lifeform(posInp, ageInp), radius(radiusInp), corIdCib(corIdInp), notRadius(0) {
    statusSca = (statusScaInp) ? EATING : FREE;
    checkRadius();
}

void Scavenger::checkRadius() {
    if ((radius >= r_sca_repro) or (radius < r_sca)) {
        cout << message::scavenger_radius_outside(radius);
        notRadius = 1;
    }
}

Status_sca Scavenger::getStatusSca() const { return statusSca; }

void drawAlgae(Algae algInp) {
    Circle circleAlg (r_alg, algInp.getPos());
    drawCircle(circleAlg, GREEN);
}

void drawCoral(const Coral& coralInp) {
    Rect coralRect(d_cor, d_cor, coralInp.getPos());
    drawRect(coralRect, BLUE);
    for (const auto &segInp : coralInp.getCorSegments()) {
        drawSegment(segInp, BLUE);
    }
}

void drawSca(const Scavenger& scaInp) {
    Circle circleSca (scaInp.getRadius(), scaInp.getPos());
    drawCircle(circleSca, RED);
}

