/*
 * simulation.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 69
 * 50% Massimiliano 50% Carl
 */
#include "simulation.h"
#include "lifeform.h"
#include "message.h"
#include "shape.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

using namespace std;

static default_random_engine e;

void resetStatic( int& state,  unsigned& ageAlg,  unsigned& ageCor, 
                  unsigned& ageSca,  unsigned& id,  unsigned& corIdCib, 
                  double& angle,  double& length,  double& radius, 
                  int& countAlg,  int& countCor,  int& countSca, 
                  int& nbSeg, int& countSeg,  int& i, int& j, 
                  int& k,  vector<Segment>& corSegments, bool open );



void Simulation::readFile(char *file_name, bool open) {
    ifstream file(file_name);
    if (!file.fail()) {
        string line;
        bool errors = false;
        while (getline(file >> ws, line)) {
            if (line[0] == '#')
                continue;
            lineDecoding(line, open, errors);
            open = false;
        }
        checkId(errors);
        checkScaId(errors);
        checkIntersect(errors);
        lifeformChecks(errors);
        checkSup(errors);
        if(errors == true) {
            clearSimulation();
        }
    } else {
        cout << "failed" << endl;
        clearSimulation();
    }
}

void Simulation::lineDecoding(string line, bool open, bool& errors) {
    enum ReadingState { COUNTALG, ALG, COUNTCOR, COR, SEG, COUNTSCA, SCA };
    static int state(COUNTALG), countAlg, countCor, countSca, nbSeg, countSeg, i, j, k;
    static S2d pAlg, pCor, pSca;
    static unsigned ageAlg, ageCor, ageSca, id, corIdCib;
    static double angle, length, radius;
    static vector<Segment> corSegments;
    static bool statusSca, statusCor, dirRotCor, statusDev;
    S2d previous;
    istringstream data(line);
    e.seed(1);
    resetStatic(state, ageAlg, ageCor,ageSca, id, corIdCib, angle, length, radius, 
                     countAlg, countCor, countSca, nbSeg, countSeg, i, j,  k,
                     corSegments, open);
    switch (state) {
    case COUNTALG:
        if (!(data >> countAlg)) break;
        state = ALG;
        break;
    case ALG:
        while (i < countAlg) {
            createAlgae(data, pAlg, ageAlg);
            ++i;
            break;
        }
        if (i == countAlg) state = COUNTCOR;
        break;
    case COUNTCOR:
        if (!(data >> countCor)) break;
        state = COR;
        break;
    case COR:
        if (j < countCor) {
            if (data >> pCor.x >> pCor.y >> ageCor >> id >> statusCor >>
                dirRotCor >> statusDev >> nbSeg) {
                countSeg = 0;
                state = SEG;
                ++j;
                break;
            }
        }
    case SEG:
    while (countSeg < nbSeg) {
        if (data >> angle >> length) {
            Segment seg;
            if (countSeg == 0) addSegments(corSegments, angle, length, pCor, seg);
            else {
                previous = corSegments.back().getEnd();
                addSegments(corSegments, angle, length, previous, seg);
            }
            checkLength(seg, id, errors);
            checkAngle(seg, id, errors);
            checkExtremity(seg.getEnd(), id, true, errors);
        }
        ++countSeg;
        if (countSeg == nbSeg) {
            createCoral(pCor, ageCor, id, statusCor, dirRotCor, statusDev, nbSeg,
                         corSegments);
            corSegments.clear();
            state = COR;
            if (j == countCor) state = COUNTSCA;
        } break;
    }
    case COUNTSCA:
        if (!(data >> countSca)) break;
        state = SCA;
        break;
    case SCA:
        while (k < countSca) {
            if (data >> pSca.x >> pSca.y >> ageSca >> radius >> statusSca) {
                if (data >> corIdCib) {
                    createScavenger(data,pSca,ageSca,radius,statusSca,corIdCib);
                } else  createScavenger(data,pSca,ageSca,radius,statusSca);
            }
            ++k;
            break;
        } break;
    }
}

void Simulation::setAlgae(unique_ptr<Algae> algaeInp) { 
    this->algaeVect.push_back(move(algaeInp));
}

void Simulation::setCoral(unique_ptr<Coral> coralInp) {
    this->coralVect.push_back(move(coralInp));
}

void Simulation::setScavenger(unique_ptr<Scavenger> scavengerInp) {
    this->scavengerVect.push_back(move(scavengerInp));
}

void Simulation::createAlgae(istringstream& data, S2d pAlg, int ageAlg){
    if (data >> pAlg.x >> pAlg.y >> ageAlg) {
        unique_ptr<Algae> ptr(new Algae({pAlg.x, pAlg.y}, ageAlg)); 
        setAlgae(move(ptr));
    }
}


void Simulation::createAlgae(S2d pAlg, int ageAlg){
        unique_ptr<Algae> ptr(new Algae({pAlg.x, pAlg.y}, ageAlg)); 
        setAlgae(move(ptr));
}



void Simulation::createScavenger(istringstream& data, S2d pSca, int ageSca, int radius,
                                 bool statusSca, int corIdCib) {
    unique_ptr<Scavenger> ptr(new Scavenger(
        {pSca.x, pSca.y}, ageSca, radius, statusSca, corIdCib));
    setScavenger(move(ptr));
}

void Simulation::createScavenger(istringstream& data, S2d pSca, int ageSca, int radius,
                                 bool statusSca){
    unique_ptr<Scavenger> ptr(new Scavenger(
        {pSca.x, pSca.y}, ageSca, radius, statusSca));
    setScavenger(move(ptr));
}

void Simulation::addSegments(vector<Segment>& corSegments, double angle, double length, 
                        S2d pCor, Segment& seg){
    seg =  {angle, length, pCor};
    corSegments.push_back(seg);
}

void Simulation::createCoral(S2d pCor, int ageCor, int id, bool statusCor, 
                              bool dirRotCor, bool statusDev, int nbSeg,
                              vector<Segment> corSegments){
    unique_ptr<Coral> ptr(new Coral({pCor.x, pCor.y}, ageCor, id, statusCor, dirRotCor,
                                    statusDev, nbSeg, corSegments));
    setCoral(move(ptr));
}




void Simulation::checkId(bool& errors) {
    for (auto &coral1 : coralVect) {
        for (auto &coral2 : coralVect) {
            if (coral1 != coral2 && coral1->getId() == coral2->getId()) {
                cout << message::lifeform_duplicated_id(coral2->getId()) << endl;
                errors = true;
            }
        }
    }
}

void Simulation::checkScaId(bool& errors) {
    bool foundError = false;
    for (auto &scavenger : scavengerVect) {
        if (scavenger->getStatusSca() == Status_sca::FREE)
            continue;
        bool foundMatch = false;
        for (auto &coral : coralVect) {
            if (scavenger->getCorIdCib() == coral->getId()) {
                foundMatch = true;
                break;
            }
        }
        if (!foundMatch) {
            cout << message::lifeform_invalid_id(scavenger->getCorIdCib()) << endl;
            foundError = true;
        }
    }
    if (foundError) {
        errors = true;;
    }
}


void Simulation::checkIntersect(bool &errors) {
    for (size_t i = 0; i < coralVect.size(); ++i) {
        for (size_t j = 0; j < coralVect.size(); ++j) {
            int coraliId = coralVect[i]->getId();
            int coraljId = coralVect[j]->getId();
            const auto& segments1 = coralVect[i]->getCorSegments();
            const auto& segments2 = coralVect[j]->getCorSegments();
            for (size_t index1 = 0; index1 < segments1.size(); ++index1) {
                const auto& seg1 = segments1[index1];

                for (size_t index2 = 0; index2 < segments2.size(); ++index2) {
                    const auto& seg2 = segments2[index2];

                    if (index1 == index2 && i == j) {
                        continue;
                    }

                    if (i == j && (index2 == index1 + 1 || index2 == index1 - 1)) {
                        continue;
                    }

                    if (doIntersect(seg1, seg2) == 1) {
                        cout << message::segment_collision(coraliId, index1, coraljId,
                                 index2) << endl;
                        errors = true;
                    }
                }
            }
        }
    }
}




void Simulation::checkSup(bool& errors){
     for (size_t i = 0; i < coralVect.size(); ++i) {
        for (size_t j = 0; j < coralVect[i]->getCorSegments().size()-1; ++j){
            bool superposed(false);
            int coraliId = coralVect[i]->getId();
            superposed = supSegment(coralVect[i]->getCorSegments()[j],
                                     coralVect[i]->getCorSegments()[j+1]);
            if(superposed){
                cout << message::segment_superposition(coraliId, j, j+1);
                errors = true;
            }
        }
     }
}

void Simulation::checkLength(const Segment &segmentInp,const int &idInp, bool& errors){
    if ((segmentInp.getLength() >= l_repro) or
        (segmentInp.getLength() < l_repro - l_seg_interne)) {
        cout << message::segment_length_outside(idInp, segmentInp.getLength());
        errors = true; 
    }
}

void Simulation::checkAngle(const Segment &segmentInp, const int &idInp, bool& errors) {
    if ((segmentInp.getAngle() > M_PI) or (segmentInp.getAngle() < -M_PI)) {
        cout << message::segment_angle_outside(idInp, segmentInp.getAngle());
        errors = true;
    }
}

void Simulation::checkExtremity(const S2d &posInp, const int &idInp, bool approx, 
                                bool& errors) {
    bool error(false);
    if (approx == true) {
        if ((posInp.x > dmax - epsil_zero) or (posInp.y > dmax - epsil_zero) or
            (posInp.x < epsil_zero) or (posInp.y < epsil_zero)) {
            error = true;
        }
    } else {
        if ((posInp.x > dmax - 0.) or (posInp.y > dmax - 0.) or (posInp.x < 0.) or
            (posInp.y < 0.)) {
            error = true;
        }
    }
    if (error) {
        cout << message::lifeform_computed_outside(idInp, posInp.x, posInp.y);
        errors = true; 
    }
}

 const vector<unique_ptr<Algae>> &Simulation::getAlgaeVect() const { 
    return algaeVect; 
}
 const vector<unique_ptr<Coral>> &Simulation::getCoralVect() const {
     return coralVect; 
}
 const vector<unique_ptr<Scavenger>> &Simulation::getScaVect()const{
    return scavengerVect; 
}

void drawSimulation(const Simulation& simInp) {
    for(auto &element : simInp.getAlgaeVect()){
        drawAlgae((*element));
    }
    for(auto &element : simInp.getCoralVect()){
        drawCoral((*element));
    }
    for(auto &element : simInp.getScaVect()){
        drawSca((*element));
    }
}

void Simulation::update(const bool& spawnAlgae) {
    for(size_t i(0); i < algaeVect.size(); ++i){
        algaeVect[i]->setAge(algaeVect[i]->getAge() + 1);
        if(algaeVect[i]->getAge() >= max_life_alg){
           swap(algaeVect[i], algaeVect[algaeVect.size() - 1]);
           algaeVect.pop_back();
        }
    }
    double algX, algY;
    if (spawnAlgae) {
        double p = alg_birth_rate; 
        bernoulli_distribution b(p);
        if (b(e)) {
            uniform_int_distribution<unsigned> random(1, dmax - 1);
            algX = random(e);
            algY = random(e);
            createAlgae({algX, algY}, 1);
        }
    }
}



unsigned int Simulation::getNbCor(){
    return coralVect.size();
}
unsigned int Simulation::getNbAlg(){
    return algaeVect.size();
}
unsigned int Simulation::getNbSca(){
    return scavengerVect.size();
}



void Simulation::clearCoralId(unsigned int id) {
    for (size_t i(0); i < coralVect.size(); ++i){
        if (coralVect[i]->getId() == id) {
           swap(coralVect[i], coralVect[coralVect.size() - 1]);
           coralVect.pop_back();
           break;
        } 
    }
}



void Simulation::saveSimulation(stringstream& data){
    saveAlgae(data);
    saveCorals(data);
    saveScavengers(data);
}

void Simulation::saveAlgae(stringstream& data){
    data << getNbAlg() << endl;
    for(auto &algae : algaeVect){
        data << algae->getPos().x << " " << algae->getPos().y << " "
             << algae->getAge() << endl << "\t";
    }
}


void Simulation::saveCorals(stringstream& data){
    data << getNbCor() << endl;
    for(auto &coral : coralVect){
        data << coral->getPos().x << " " << coral->getPos().y << " "
            << coral->getAge() << " " << coral->getId() << " " << coral->getStatusCor()
             << " " << coral->getDirRotCor() << " " << coral->getStatusDev() << " "
            << coral->getNbSeg() << endl;
        for(size_t i(0); i < coral->getCorSegments().size(); ++i){
            data << coral->getCorSegments()[i].getAngle() << " " 
                 << coral->getCorSegments()[i].getLength() << endl;
        }
    }
    data << "\t";
}



void Simulation::saveScavengers(stringstream& data){
    data << getNbSca() << endl;
    for(auto &scavenger : scavengerVect){
        data << scavenger->getPos().x << " " << scavenger->getPos().y << " "
             << scavenger->getAge() << " " << scavenger->getRadius() << " "
             << scavenger->getStatusSca() << " " << scavenger->getCorIdCib() << endl;

    }
    data << "\t";
}


void Simulation::clearSimulation(){
    algaeVect.clear();
    coralVect.clear();
    scavengerVect.clear(); 
}


void Simulation::lifeformChecks(bool& errors){
    checkPosAgeAlg(errors);
    checkPosAgeCor(errors);
    checkPosAgeRadiusSca(errors);
}


void Simulation::checkPosAgeAlg(bool& errors){
    for(auto& alg : algaeVect){
        if(alg->getNotAge() == 1 || alg->getNotCentered() == 1){
            errors = true;
        }
    }
}

void Simulation::checkPosAgeCor(bool& errors){
    for(auto& cor : coralVect){
        if(cor->getNotAge() == 1 || cor->getNotCentered() == 1){
            errors = true;
        }
    }
}




void Simulation::checkPosAgeRadiusSca(bool& errors){
    for(auto& sca : scavengerVect){
        if(sca->getNotAge() == 1 || sca->getNotCentered() == 1){
            errors = true;
        }
        if(sca->getNotRadius() == 1){
            errors = true;
        }
    }
}


void resetStatic( int& state,  unsigned& ageAlg,  unsigned& ageCor, 
                  unsigned& ageSca,  unsigned& id,  unsigned& corIdCib, 
                  double& angle,  double& length,  double& radius, 
                  int& countAlg,  int& countCor,  int& countSca, 
                  int& nbSeg, int& countSeg,  int& i, int& j, 
                  int& k,  vector<Segment>& corSegments, bool open ){
    if(open == true) {
        state = 0;
        ageAlg = 0;
        ageCor = 0;
        ageSca = 0;
        id = 0;
        corIdCib = 0;
        angle = 0;
        length = 0;
        radius = 0;
        countAlg = 0;
        countCor = 0;
        countSca = 0;
        nbSeg = 0;
        countSeg = 0;
        i = 0;
        j = 0;
        k = 0;
        corSegments.clear();
    }   
}