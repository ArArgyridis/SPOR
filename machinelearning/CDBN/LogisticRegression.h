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

#ifndef LOGISTICREGRESSION_H
#define LOGISTICREGRESSION_H


#include "functions.h"
#include <stdlib.h>
#include <boost/shared_array.hpp>

class LogisticRegression {
  matrix2dPtr Weights, bias, outData , valInputData, valOutData;
  int n_in;
  int n_out;
  int N;  // num of inputs
  void softmax(matrix2dPtr);
  float computeError();
public:

 matrix2dPtr inputData;


  matrix2dPtr getWeights();
  LogisticRegression(int, int, int);
  LogisticRegression(matrix2dPtr, matrix2dPtr, int, int);

  ~LogisticRegression();

  void train(int, float, float, float, matrix2dPtr);
  void predict(matrix2dPtr, matrix2dPtr);
  float negativeLogLikelihood();
};

void test_lr();

#endif
