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

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <stdlib.h>
#include <cmath>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/assignment.hpp>
#include <boost/random/binomial_distribution.hpp>

typedef boost::numeric::ublas::matrix <double> matrix2d;
typedef  std::shared_ptr<boost::numeric::ublas::matrix <double > > matrix2dPtr;
typedef boost::numeric::ublas::matrix<double >::iterator1 iterator1;
typedef boost::numeric::ublas::matrix<double >::iterator2 iterator2;

double computeAverage(matrix2dPtr);

double sigmoid (double);

void printMatrix(std::string, matrix2dPtr);

void uniformArray(matrix2dPtr , double , double );

void writeMatrix(std::string, matrix2dPtr );

void sigmoidArray (matrix2dPtr, matrix2dPtr);

void binomialArray(int, matrix2dPtr, matrix2dPtr);

void exponentialArray(matrix2dPtr, matrix2dPtr);

void hyperbolicTangentArray(matrix2dPtr, matrix2dPtr);

void logArray(matrix2dPtr, matrix2dPtr);

#endif // FUNCTIONS_H_INCLUDED
