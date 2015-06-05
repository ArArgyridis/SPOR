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
 
#include <iostream>
#include <cmath>
#include "fuzzyfunctions.h"
#include "ontologyclass.h"
using namespace std;

double sigmoidal (double x, double *fuzzyInput) {
    double sig;
    sig = (x-fuzzyInput[0])/(fuzzyInput[0]-fuzzyInput[1]);
    return sig;
}

double inverse_sigmoidal (double x, double *fuzzyInput) {
    double inv_sig =1- sigmoidal(x,fuzzyInput);
    return inv_sig;
}

double triangular (double x, double *fuzzyInput) {
    double trg = max(min ((x - fuzzyInput[0])/(fuzzyInput[1] - fuzzyInput[0]), ( fuzzyInput[2] - x)/(fuzzyInput[2] - fuzzyInput[1] )) ,0.0);
    return trg;
}

double trapezoidal(double x, double *fuzzyInput)  {

    double trpz =  min( (x-fuzzyInput[0])/(fuzzyInput[1]-fuzzyInput[0]), 1.0 );
    trpz = min ( trpz, (fuzzyInput[3] - x)/(fuzzyInput[3] - fuzzyInput[2]) );
    return  max (trpz, 0.0);
}

double rightShoulder(double x, double *fuzzyInput) {
    double lftShd = min( (x - fuzzyInput[0])/(fuzzyInput[1] - fuzzyInput[0]) , 1.0);
    return lftShd = max (lftShd, 0.0);
}

double leftShoulder(double x, double *fuzzyInput)  {
    double lftShd = min(( fuzzyInput[1] - x)/(fuzzyInput[1] - fuzzyInput[0]) , 1.0);
    return lftShd = max (lftShd, 0.0);
}

