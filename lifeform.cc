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

void Lifeform::setPos(const double& x, const double& y) {
     pos.x = x;
     pos.y = y; 
}

void Lifeform::setAge(const int &ageInp) { 
    age = ageInp; 
}

void Coral::setStatusCor(Status_cor status){
    statusCor = status;
}

void Coral::setStatusDev (Status_dev status) {
    statusDev = status;
}

void Scavenger::setStatusSca(Status_sca status){
    statusSca = status;
}

void Scavenger::setCorIdCib(unsigned int id){
    corIdCib = id;
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


void Coral::setBeingEaten(bool beingTasted){
    beingEaten = beingTasted;
}
bool Coral::getBeingEaten() const{
    return beingEaten;
}


size_t Scavenger::getWhichSegment() const{
    return whichSegmentEating;
}
void Scavenger::setWhichSegment(const size_t &index){
    whichSegmentEating = index;
}




Algae::Algae(S2d posInp, unsigned ageInp) : Lifeform(posInp, ageInp) {}

Coral::Coral(S2d posInp, unsigned ageInp, unsigned int idInp, bool statusCorInp,
             bool dirRotCorInp, bool statusDevInp, unsigned int nbSegInp,
             vector<Segment> segments, bool beingEaten)
    : Lifeform(posInp, ageInp), id(idInp), nbSeg(nbSegInp),
      corSegments(move(segments)), beingEaten(beingEaten){
    statusCor = (statusCorInp) ? ALIVE : DEAD;
    dirRotCor = (dirRotCorInp) ? INVTRIGO : TRIGO;
    statusDev = (statusDevInp) ? REPRO : EXTEND;
}

void Coral::changeDirSup() {
    if(corSegments.size() > 1) {
        Segment lastSeg (corSegments[corSegments.size()-1]);
        Segment otherSeg (corSegments[corSegments.size()-2]);
        bool superpos = supSegment(lastSeg, otherSeg);
        if(superpos){
            changeDir(dirRotCor);
            }
        }
}

void Coral::updateAngle(double deltaRot) { 
    Segment* lastSeg (&corSegments[corSegments.size()-1]);
    double a(0);
    double l((*lastSeg).getLength());
    S2d b((*lastSeg).getBase());

    if(dirRotCor){
        a = (*lastSeg).getAngle() - deltaRot;

    } else { 
        a = (*lastSeg).getAngle() + deltaRot;
    }
    (*lastSeg) = Segment(a,l,b);

}

void Coral::growSegment(double growth) {
    Segment* lastSeg (&corSegments[corSegments.size()-1]);
    double a ((*lastSeg).getAngle());
    double l((*lastSeg).getLength() + growth);
    S2d b((*lastSeg).getBase());
    (*lastSeg) = Segment(a,l,b);
}


void Coral::setDirRotCor(Dir_rot_cor dirInp) { dirRotCor = dirInp; }

void Coral::setSegment(Segment segInp) { corSegments.push_back(segInp); }


const vector<Segment> &Coral::getCorSegments() const { return corSegments; }

void Coral::popBackSegment(){
    corSegments.pop_back();
}

Segment Coral::getSegment(unsigned i) { return corSegments[i]; }

Scavenger::Scavenger(S2d posInp, unsigned ageInp, double radiusInp, bool statusScaInp,
                     unsigned int corIdInp, bool onCoralInp, size_t segmentEating)
    : Lifeform(posInp, ageInp), radius(radiusInp), corIdCib(corIdInp), notRadius(0), 
      onCoral(onCoralInp), whichSegmentEating(segmentEating), possiblePrey() {
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
    Status_cor status(coralInp.getStatusCor());
    if (status == ALIVE) {
        drawRect(coralRect, BLUE);
        for (const auto &segInp : coralInp.getCorSegments()) {
            drawSegment(segInp, BLUE);
        }
    } else {
        drawRect(coralRect, GREY);
        for (const auto &segInp : coralInp.getCorSegments()) {
            drawSegment(segInp, GREY);
        }
    }
}

void drawSca(const Scavenger& scaInp) {
    Circle circleSca (scaInp.getRadius(), scaInp.getPos());
    drawCircle(circleSca, RED);
}

void changeDir(Dir_rot_cor& dirInp) {
    if(dirInp) { 
                dirInp = TRIGO;
            } else {
                dirInp = INVTRIGO;
                }
}



unsigned int Scavenger::getPossiblePrey(size_t i) const{
    return possiblePrey[i];
}


const vector<unsigned int> &Scavenger::getPossiblePreyVect() const {
    return possiblePrey;
}


void Scavenger::addPossiblePrey(unsigned int id){
    possiblePrey.push_back(id);
}


bool Scavenger::getOnCoral() const{
    return onCoral;
}
void Scavenger::setOnCoral(bool isItOn){
    onCoral = isItOn;
}

void Scavenger::clearPossiblePrey(){
    possiblePrey.clear();
}

void Scavenger::popBackPrey(size_t i){
    swap(possiblePrey[i], possiblePrey[possiblePrey.size() - 1]);
    possiblePrey.pop_back();
}

