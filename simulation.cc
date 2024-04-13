/*
 * simulation.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
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

using namespace std;

void Simulation::readFile(char *file_name) {
    ifstream file(file_name);
    if (!file.fail()) {
        string line;
        while (getline(file >> ws, line)) {
            if (line[0] == '#')
                continue;
            lineDecoding(line);
        }
        checkId();
        checkScaId();
        checkSup();
        checkIntersect();
    } else {
        cout << "failed" << endl;
        exit(EXIT_FAILURE);
    }
}

void Simulation::lineDecoding(string line) {
    enum ReadingState { COUNTALG, ALG, COUNTCOR, COR, SEG, COUNTSCA, SCA };
    static int state(COUNTALG);
    static S2d pAlg, pCor, pSca;
    static unsigned ageAlg, ageCor, ageSca, id, corIdCib;
    static double angle, length, radius;
    static vector<Segment> corSegments;
    static bool statusSca, statusCor, dirRotCor, statusDev;
    static int counterAlg, counterCor, counterSca, nbSeg, countSeg, i, j, k;
    S2d previous;
    istringstream data(line);
    switch (state) {
    case COUNTALG:
        if (!(data >> counterAlg)) break;
        state = ALG;
        break;
    case ALG:
        while (i < counterAlg) {
            createAlgae(data, pAlg, ageAlg);
            ++i;
            break;
        }
        if (i == counterAlg) state = COUNTCOR;
        break;
    case COUNTCOR:
        if (!(data >> counterCor)) break;
        state = COR;
        break;
    case COR:
        if (j < counterCor) {
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
                if (countSeg == 0) {
                    addSegments(corSegments, angle, length, pCor, seg);
                } else {
                    previous = corSegments.back().getEnd();
                    addSegments(corSegments, angle, length, previous, seg);
                }
                checkLength(seg, id);
                checkAngle(seg, id);
                checkExtremity(seg.getEnd(), id, 0);
            }
            ++countSeg;
            if (countSeg == nbSeg) {
                createCoral(pCor,ageCor, id, statusCor, dirRotCor, statusDev, nbSeg,
                            corSegments);
                state = COR;
                if (j == counterCor)
                    state = COUNTSCA;
            }
            break;
        }
    case COUNTSCA:
        if (!(data >> counterSca)) break;
        state = SCA;
        break;
    case SCA:
        while (k < counterSca) {
            if (data >> pSca.x >> pSca.y >> ageSca >> radius >> statusSca) {
                if (data >> corIdCib) {
                    createScavenger(data,pSca,ageSca,radius,statusSca,corIdCib);
                } else {
                    createScavenger(data,pSca,ageSca,radius,statusSca);
                }
            }
            ++k;
            break;
        }
        break;
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
                        bool dirRotCor, bool statusDev, int nbSeg, vector<Segment> corSegments){
    unique_ptr<Coral> ptr(new Coral({pCor.x, pCor.y}, ageCor, id, statusCor, dirRotCor,
                                    statusDev, nbSeg, corSegments));
    setCoral(move(ptr));
}




void Simulation::checkId() {
    for (auto &coral1 : coralVect) {
        for (auto &coral2 : coralVect) {
            if (coral1 != coral2 && coral1->getId() == coral2->getId()) {
                cout << message::lifeform_duplicated_id(coral2->getId()) << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}

void Simulation::checkScaId() {
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
        exit(EXIT_FAILURE);
    }
}

void Simulation::checkIntersect() {
    bool precision = false;
    for (size_t i = 0; i < coralVect.size(); ++i) {
        for (size_t j = 0; j < coralVect.size(); ++j) {
            int coraliId = coralVect[i]->getId();
            int coraljId = coralVect[j]->getId();
            int counterSeg1 = 0;
            for (auto &seg1 : coralVect[i]->getCorSegments()) {
                int counterSeg2 = 0;
                for (auto &seg2 : coralVect[j]->getCorSegments()) {
                    if (counterSeg1 == counterSeg2 && i == j) {
                        ++counterSeg2;
                        continue;
                    }
                    if (i == j && counterSeg2 == counterSeg1 + 1) {
                        ++counterSeg2;
                        continue;
                    }
                    if (doIntersect(seg1.getBase(), seg1.getEnd(), seg2.getBase(),
                                    seg2.getEnd(), precision) == 1) {
                        ;
                        cout << message::segment_collision(coraliId, counterSeg1,
                                                           coraljId, counterSeg2)
                             << endl;
                        exit(EXIT_FAILURE);
                    }
                    ++counterSeg2;
                }
                ++counterSeg1;
            }
        }
    }
}

void Simulation::checkSup(){
    // get coral vector
     for (size_t i = 0; i < coralVect.size(); ++i) {
        // get segmentsvector
        for (size_t j = 0; j < coralVect[i]->getCorSegments().size()-1; ++j){
            bool superposed(false);
            int coraliId = coralVect[i]->getId();
            superposed = supSegment(coralVect[i]->getCorSegments()[j], coralVect[i]->getCorSegments()[j+1]);

            if(superposed){
                cout << message::segment_superposition(coraliId, j, j+1);
                exit(EXIT_FAILURE);
            }
        }
     }
}

void Simulation::checkLength(const Segment &segmentInp, const int &idInp) {
    if ((segmentInp.getLength() >= l_repro) or
        (segmentInp.getLength() < l_repro - l_seg_interne)) {
        cout << message::segment_length_outside(idInp, segmentInp.getLength());
        exit(EXIT_FAILURE); 
    }
}

void Simulation::checkAngle(const Segment &segmentInp, const int &idInp) {
    if ((segmentInp.getAngle() > M_PI) or (segmentInp.getAngle() < -M_PI)) {
        cout << message::segment_angle_outside(idInp, segmentInp.getAngle());
        exit(EXIT_FAILURE);
    }
}

void Simulation::checkExtremity(const S2d &posInp, const int &idInp, bool approx) {
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
        exit(EXIT_FAILURE); 
    }
}