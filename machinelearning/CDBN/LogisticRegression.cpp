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
#include <string>
#include <math.h>
#include "LogisticRegression.h"

#include <stdio.h>
#include <algorithm>

using namespace std;
using namespace boost::numeric::ublas;


float LogisticRegression::computeError() {
    valInputData = inputData;
    valOutData    = outData;
    matrix2dPtr comOut = matrix2dPtr (new matrix2d (valOutData->size1(), valOutData->size2())  );

    predict( valInputData,  comOut );

    //printMatrix("comOut", comOut);
    float errors = 0.0;
    for (register unsigned int i = 0; i < comOut->size1(); i++) {
        float sum = 0.0;
        for  (register unsigned int j = 0; j < comOut->size2(); j++) {
            //cout <<comOut->at_element(i,j ) <<"\t" << outData->at_element(i,j) << endl;
            sum += abs(  comOut->at_element(i,j) - outData->at_element(i,j)    );
        }
        sum /= comOut->size2();
        //cout <<"sum = " << sum << endl;
        if (sum >= 0.5)
            errors +=1;


    }
  cout <<"number of sample missclasifications: " << errors << endl;
  return errors;
}

LogisticRegression::LogisticRegression(matrix2dPtr inp, matrix2dPtr out, int in, int outs) {
    N = inp->size1();
    n_in = in;
    n_out = outs;
    inputData = inp;
    outData = out;

    Weights = matrix2dPtr( new matrix2d (n_in,n_out) );
    bias = matrix2dPtr( new matrix2d(1, n_out) );

    for (register int i = 0; i < n_in; i++)
        for (register int j = 0; j < n_out; j++) {
            (*Weights)(i,j) = 0;
            (*bias)(0,j) = 0;
        }
}

LogisticRegression::~LogisticRegression() {}

matrix2dPtr LogisticRegression::getWeights() {
    return Weights;
}

float LogisticRegression::negativeLogLikelihood() {

    matrix2dPtr tmp1 = matrix2dPtr (  new matrix2d (inputData->size1(),  Weights->size2() )   );
    tmp1->assign( prod( *inputData, *Weights) ) ;

    for (iterator1 it1  = tmp1->begin1()  ; it1 != tmp1->end1(); it1++ )
        for (iterator2 it2 = it1.begin();  it2 !=it1.end() ; it2++)
            *it2 += (*bias)(0, it2.index2());
    softmax(tmp1);

    float res = 0.0;
    for (register unsigned int i = 0; i < tmp1->size1(); i++) {
        float sum = 0.0;
        for(register unsigned int j = 0; j < tmp1->size2(); j++) {
            sum  +=  (*outData)(i, j) * log(  (*tmp1)(i,j) ) + ( 1 - (*outData)(i,j) ) * log (  1 - (*tmp1)(i,j)   );
        }
        res += sum;
    }

    return -res/tmp1->size1();
}




void LogisticRegression::train(int epochs, float lr, float lr_coef, float l2Reg, matrix2dPtr inp) {
    if (inp != NULL) inputData = inp;

    for (register int i = 0; i < epochs; i++) {

        matrix2dPtr p_y_given_x = matrix2dPtr (new matrix2d(inputData->size1(), Weights->size2()) );

        p_y_given_x->assign( prod( *inputData, *Weights) ) ;

        for (register unsigned int i = 0; i < inputData->size1(); i++)
            for (register  int j = 0; j <n_out; j++) {
                (*p_y_given_x)(i,j) += (*bias)(0,j);
            }
        softmax(p_y_given_x);


        matrix2dPtr dy = matrix2dPtr ( new matrix2d (outData->size1(), outData->size2()));
        dy->assign(*outData - *p_y_given_x);
        *Weights +=  lr*prod(trans(*inputData) , *dy) - lr *l2Reg *(*Weights) ;
        matrix2d tmp(bias->size1(), bias->size2());

        for (register unsigned int j = 0; j < dy->size2(); j++) {
            for (register unsigned int i = 0;  i < dy->size1(); i++)
                tmp(0,j) += (*dy)(i,j)/dy->size1();
        }
        *bias +=  lr*tmp;


        lr *= lr_coef;
        //cout <<"cost of fine tuning : " << negativeLogLikelihood()*100 << endl;

        //cout <<"=================\n";
    }
    computeError();
}



void LogisticRegression::softmax(matrix2dPtr mat) {


    for (register unsigned int i = 0; i < mat->size1(); i++) {
        float max = 0.0;
        float sum = 0.0;
        for (register unsigned  int j = 0; j < mat->size2(); j++) {

            if  (  max < (*mat)( i,j ) )
                max = (*mat)( i,j );
        }

        for (register unsigned int j = 0; j < mat->size2(); j++) {
            (*mat)(i,j) = exp ( (*mat)(i,j) - max  );
            sum += (*mat)(i,j);
        }
        for (register unsigned int j = 0; j < mat->size2(); j++) {
            (*mat)(i,j) /= sum;
        }
    }
}

void LogisticRegression::predict(matrix2dPtr in, matrix2dPtr out) {
    (*out) = prod( *in, *Weights) ;

    for (register unsigned int i = 0; i < out->size1(); i++)
        for (register unsigned int j = 0; j <out->size2(); j++)
            (*out)(i,j) += (*bias)(0, j);
    softmax(out);
}

void test_lr() {
    srand(0);

    float learning_rate = 0.05;
    float n_epochs =50000;

    int nrows = 12;
    int ncols = 3;
    int outs = 3;
    matrix2dPtr train_X = matrix2dPtr ( new matrix2d(nrows, ncols) );
    matrix2dPtr train_Y = matrix2dPtr( new matrix2d(nrows, outs) );
    /*
    (*train_X) <<= 1,1,1, 0, 0, 0,1, 0, 1, 0, 0, 0,1, 1, 1, 0, 0, 0,0, 0, 1, 1, 1, 0,0, 0, 1, 1, 0, 0,0, 0, 1, 1, 1, 0;
    //(*train_X)  <<= 0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1;
    (*train_Y ) <<=1,0,1,0,1,0,0,1,0,1,0,1;
    */
    (*train_X) <<= 1,1,0,
            2,1,0,
            3,1,0,
            4,1,0,
            2,2,1,
            3,4,1,
            4,5,1,
            8,6,1,
            3,3,2,
            4,8,2,
            8,6,2,
            10,5,2;
    (*train_Y)<<= 1,0,0,1,0,0,1,0,0,1,0,0,
            0,1,0,0,1,0,0,1,0,0,1,0,
            0,0,1,0,0,1,0,0,1,0,0,1;
    // construct LogisticRegression
    LogisticRegression classifier(train_X, train_Y, ncols, outs);

    // train online
    classifier.train(n_epochs, learning_rate,1, 0, NULL);

    int test_out = 12;
    matrix2dPtr test_X = matrix2dPtr (new matrix2d(test_out, ncols) );

    //(*test_X)<<= 1,1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,1, 1, 1, 1, 1, 0;

    matrix2dPtr test_Y = matrix2dPtr (new matrix2d(test_out, outs) );

    classifier.predict(train_X, test_Y);

    cout <<"cross entropy: " <<   classifier.negativeLogLikelihood()*100 << endl;

    printMatrix("Result", test_Y);
    //printMatrix("finalWeights", classifier.getWeights());

}



