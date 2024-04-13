/*
 * lifeform.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
 */
#include "Lifeform.h"

using namespace std;

Lifeform::Lifeform(
    S2d posInp,
    int ageInp) 
    : pos(posInp), age(ageInp) {
    checkAge();
    checkCenter();
}

void Lifeform::checkAge() {
    if (age <= 0) {
        cout << message::lifeform_age(age);
        std ::exit(EXIT_FAILURE); 
    }
}

void Lifeform::checkCenter() {
    if ((pos.x > dmax - 1.) or (pos.y > dmax - 1.) or (pos.x < 1.) or (pos.y < 1.)) {
        cout << message::lifeform_center_outside(pos.x, pos.y);
        std ::exit(EXIT_FAILURE); 
    }
}

S2d Lifeform::getPos() const { return pos; }

int Lifeform::getAge() const { return age; }

unsigned int Coral::getId() const { return id; }

unsigned int Scavenger::getCorIdCib() const { return corIdCib; }

void Lifeform::setPos(const S2d &posInp) { pos = posInp; }

void Lifeform::setAge(const int &ageInp) { age = ageInp; }


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

void Coral::setSegment(Segment segInp) { corSegments.push_back(segInp); }


const vector<Segment> &Coral::getCorSegments() const { return corSegments; }

Scavenger::Scavenger(S2d posInp, unsigned ageInp, double radiusInp, bool statusScaInp,
                     unsigned int corIdInp)
    : Lifeform(posInp, ageInp), radius(radiusInp), corIdCib(corIdInp) {
    statusSca = (statusScaInp) ? EATING : FREE;
    checkRadius();
}

void Scavenger::checkRadius() {
    if ((radius >= r_sca_repro) or (radius < r_sca)) {
        cout << message::scavenger_radius_outside(radius);
        std ::exit(EXIT_FAILURE); // Rendu1
    }
}

Status_sca Scavenger::getStatusSca() const { return statusSca; }