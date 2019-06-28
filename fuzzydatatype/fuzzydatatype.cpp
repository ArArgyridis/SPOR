/*
   Part of the code of the SPatial Ontology Reasoner designed to reason over multi-scale GEOBIA Ontologies, as described in the following paper:
   Argyridis A., Argialas, D., 2015. A Fuzzy Spatial Reasoner for Multi-Scale GEOBIA Ontologies, Photogrammetric Engineering and Remote Sesing, 41-48
   Copyright (C) 2015  Argyros Argyridis
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fuzzydatatype.hxx"
#include "../fuzzyfunctions/fuzzyfunctions.hxx"
#include <iostream>
using namespace std;

///:~ FuzzyDatatype Methods//////////////////////////////////////////////////////////////

FuzzyDatatype::FuzzyDatatype():parNum(0), fuzzyInput(NULL){}

FuzzyDatatype::FuzzyDatatype(string funCode, double *finput):parNum(0) {
    setFunction(funCode);
    setLimits(finput);
}

FuzzyDatatype::~FuzzyDatatype() {
        delete[] fuzzyInput;
}



double FuzzyDatatype::calculateValue(double v) {
    return fuzzyFunction(v, fuzzyInput);
}

void FuzzyDatatype::setLimits(double *finput) {
    for (register int i = 0; i <parNum; i ++) {
        fuzzyInput[i] = finput[i];
    }
}

void FuzzyDatatype::setFunction(string funCode) {
        if (funCode == "triangular") {
            parNum = 3;
            fuzzyFunction = &triangular;

        } else if (funCode == "leftshoulder") {
            parNum = 2;
            fuzzyFunction = &leftShoulder;

        } else if (funCode == "rightshoulder") {
            parNum = 2;
            fuzzyFunction = &rightShoulder;

        } else if(funCode == "trapezoidal") {
            parNum = 4;
            fuzzyFunction = &trapezoidal;
        } else {
            cout <<"Invalid Membership function, please check defined hierarchy for possible errors\n";
        }
        if (parNum != 0)
            fuzzyInput = new double[parNum];

}
