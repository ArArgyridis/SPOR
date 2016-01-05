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

#ifndef HIDDENLAYER_H
#define HIDDENLAYER_H

#include "functions.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/assignment.hpp>
#include <boost/random/binomial_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include "boost/random/variate_generator.hpp"

typedef boost::numeric::ublas::matrix <double> matrix2d;
typedef  boost::shared_ptr<boost::numeric::ublas::matrix <double > > matrix2dPtr;


class HiddenLayer {
    matrix2dPtr inputData,  Weights, bias;
    int nInputs, nOutputs;

public:

    HiddenLayer(matrix2dPtr, int, int, matrix2dPtr, matrix2dPtr);
    void output(matrix2dPtr, matrix2dPtr);
    void sample_h_given_v(matrix2dPtr, matrix2dPtr);
    matrix2dPtr getWeights();
    matrix2dPtr getBias();
    int getOuts();

    matrix2dPtr getSampleHGivenV(matrix2dPtr);
};

void testHl();

#endif
