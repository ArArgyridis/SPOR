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
#include <math.h>
#include "RBM.hxx"
#include <stdio.h>
#include <stdlib.h>
#include <boost/ptr_container/nullable.hpp>

using namespace std;
using namespace boost::numeric::ublas;

void RBM::propup(matrix2dPtr inp, matrix2dPtr h1Mean) {

   matrix2dPtr preSigmoidActivation  = matrix2dPtr (new matrix2d  (inp->size1(), Weights->size2()));

    preSigmoidActivation->assign ( prod(*inp, *Weights) );
    //printMatrix("tmp",tmp);
    for (register unsigned int i = 0; i < preSigmoidActivation->size1(); i++ )
        for (register unsigned int j = 0; j <preSigmoidActivation->size2(); j++) {
            (*preSigmoidActivation)(i,j) += (*hBias)(j,0);
        }

    sigmoidArray(preSigmoidActivation, h1Mean);

}

void RBM::propdown(matrix2dPtr inp, matrix2dPtr v1Mean) {
    matrix2dPtr preSigmoidActivation;
    preSigmoidActivation  = matrix2dPtr (new matrix2d  (inp->size1(), Weights->size1()) );
    preSigmoidActivation->assign ( prod(*inp, trans(*Weights) ) ) ;

    for (register unsigned int i = 0; i < preSigmoidActivation->size1(); i++) {
        for (register unsigned int j = 0; j <preSigmoidActivation->size2(); j++) {
            (*preSigmoidActivation) (i, j ) += vBias->at_element(j,0);
        }
    }
    sigmoidArray(preSigmoidActivation, v1Mean);
}



RBM::RBM() {}

RBM::RBM(matrix2dPtr trainData, int visible, int hiddenLayers, matrix2dPtr w, matrix2dPtr hb, matrix2dPtr vb) {
    this->trainData = trainData;
    n_visible  = visible;
    n_hidden = hiddenLayers;
    n_inputs = trainData->size1();
    type = typeRBM;

    if ( w != NULL ) {
        Weights = w;
    }
    else {
        Weights = matrix2dPtr ( new matrix2d (n_visible,n_hidden )    );
        float a = -4.0*sqrt(6.0 / (n_visible + n_hidden)  );
        uniformArray(Weights, -a, a);

    }
    if (hb != NULL) {
        hBias = hb;
        //cout <<"hBias added\n";
    }
    else {
        hBias = matrix2dPtr ( new matrix2d (n_hidden,1)    );
        for(int i=0; i<n_hidden; i++)
            (*hBias)(i,0) = 0;
    }

    if (vb !=NULL)
        vBias = vb;
    else {
        vBias =  matrix2dPtr ( new matrix2d (n_visible, 1)    );
        for(int i=0; i<n_visible; i++)
            (*vBias)(i,0) = 0;
    }
}

RBM::~RBM() {}

void RBM::gibbsVhv( matrix2dPtr inp, matrix2dPtr nvm, matrix2dPtr nvs, matrix2dPtr nhm, matrix2dPtr nhs  )  {
    sample_h_given_v(inp, nhm, nhs);
    sample_v_given_h(nhs, nvm, nvs);
}

matrix2dPtr RBM::freeEnergy(matrix2dPtr vSample) {
    matrix2dPtr wxB = matrix2dPtr(new matrix2d(vSample->size1(), vSample->size2() )   );

    wxB->assign ( prod( *vSample, *Weights )  ) ;

    for (register unsigned  int i = 0; i <wxB->size1(); i++)
            for (register unsigned int j = 0; j < wxB->size2(); j++)
                wxB->at_element(i,j) += hBias->at_element(j, 0);
    matrix2dPtr vBiasTerm = matrix2dPtr(new matrix2d ( vSample->size1(), vBias->size2() ) );
    vBiasTerm->assign( prod(  *vSample, *vBias  )  );

    matrix2dPtr hiddenTerm = matrix2dPtr(new matrix2d (  1  , wxB->size2()) );

    for (register unsigned int j = 0; j < wxB->size2(); j++)
        for (register unsigned  int i = 0; i < wxB->size1(); i++)
            hiddenTerm->at_element(0,j) += log( 1 + exp( wxB->at_element(i,j) )    );
    //to be returned
    matrix2dPtr ret = matrix2dPtr(new matrix2d(hiddenTerm->size1(), hiddenTerm->size2() ) );
    //printMatrix("hiddenTerm", hiddenTerm);
    //printMatrix("vbiasterm", vBiasTerm);

    *ret = -(*hiddenTerm)- trans(*vBiasTerm);
    return ret;
}

void RBM::getCostUpdates( double lr, matrix2dPtr per, int k ) {

    matrix2dPtr phMean     = matrix2dPtr(new matrix2d ( n_inputs, n_hidden ) );
    matrix2dPtr phSample  = matrix2dPtr(new matrix2d ( n_inputs, n_hidden ) );
    matrix2dPtr nvMeans    = matrix2dPtr(new matrix2d ( n_inputs, n_visible ) );
    matrix2dPtr nvSamples =  matrix2dPtr(new matrix2d ( n_inputs, n_visible ) );
    matrix2dPtr nhMeans    =  matrix2dPtr(new matrix2d ( n_inputs, n_hidden ) );
    matrix2dPtr nhSamples = matrix2dPtr(new matrix2d ( n_inputs, n_hidden ) );

    sample_h_given_v( trainData, phMean, phSample );

    matrix2dPtr chainStart  = phSample;
    for (register int step = 0; step < k; step++) {
        //cout <<"k = " << step << endl;
        if (step == 0)
            gibbsHvh ( chainStart, nvMeans, nvSamples, nhMeans, nhSamples);
        else
            gibbsHvh ( nhSamples, nvMeans, nvSamples, nhMeans, nhSamples);
    }
    matrix2dPtr inputEnergy, nvEnergy;

    inputEnergy = freeEnergy(trainData);
    nvEnergy  = freeEnergy(nvSamples);
    //printMatrix("inputEnergy", inputEnergy);
    //printMatrix("nvsamples energy", nvEnergy);

    double cost = computeAverage(inputEnergy) - computeAverage(nvEnergy);
    cout <<"cost = " << cost << endl;

}


void RBM::contrastive_divergence(int k, int epochs, double lr, double lrCoef,  matrix2dPtr inp, bool persistent) {
    if (inp != NULL) trainData = inp;

    //printMatrix("initial RBM Weights", Weights);
    matrix2dPtr phMean     = matrix2dPtr(new matrix2d ( n_inputs, n_hidden ) );
    matrix2dPtr phSample  = matrix2dPtr(new matrix2d ( n_inputs, n_hidden ) );
    matrix2dPtr nvMeans    = matrix2dPtr(new matrix2d ( n_inputs, n_visible ) );
    matrix2dPtr nvSamples =  matrix2dPtr(new matrix2d ( n_inputs, n_visible ) );
    matrix2dPtr nhMeans    =  matrix2dPtr(new matrix2d ( n_inputs, n_hidden ) );
    matrix2dPtr nhSamples = matrix2dPtr(new matrix2d ( n_inputs, n_hidden ) );

    matrix2dPtr perSamples;
    if (persistent)
        perSamples = matrix2dPtr( new matrix2d ( n_inputs, n_hidden ) );

    double val = 0.0;
     if(this->type == typeCRBM)
        val = 0.1;

    for(iterator1 it1 = perSamples->begin1(); it1 != perSamples->end1(); it1++)
        for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++)
            *it2 = val;

    //printMatrix("persistent", perSamples);
    for (register int i = 0; i < epochs; i++ ) {

        // "CD-k"
        sample_h_given_v(trainData, phMean, phSample);
        matrix2dPtr chainStart;

        if ( persistent )
            chainStart = perSamples;
        else
            chainStart = phSample;
       // printMatrix("chainstart", chainStart);
        for (register int step = 0; step < k; step++) {
            //printMatrix("initial weights", nvSamples);
            //cout <<"k = " << step << endl;
            if (step == 0)
                gibbsHvh ( chainStart, nvMeans, nvSamples, nhMeans, nhSamples);
            else
                gibbsHvh ( nhSamples, nvMeans, nvSamples, nhMeans, nhSamples);
        }

        *Weights += lr * (prod( trans(*trainData), *phMean ) - prod(trans(*nvSamples) , *nhMeans) ) ;
        //printMatrix("weights", Weights);
        matrix2dPtr dif, tmp1, dif2, tmp2;
        dif = matrix2dPtr(new matrix2d ( n_inputs, n_visible) );

        tmp1 = matrix2dPtr (new matrix2d (vBias->size1(), vBias->size2()));
        for (register unsigned int i = 0; i < vBias->size1(); i++ )
             for (register unsigned int j = 0; j < vBias->size2(); j++ )
                 (*tmp1)(i,j) = 0;

        *dif = *trainData - *nvSamples;
        for (register unsigned int j = 0; j < dif->size2(); j++)
            for (register unsigned int i = 0; i < dif->size1(); i++)
                (*tmp1)(j, 0) += (*dif)(i,j)/dif->size1();

        *vBias += lr* (*tmp1);

        dif2 = matrix2dPtr(new matrix2d  ( n_inputs, n_hidden ) );
        *dif2 = (*phMean) - (*nhMeans);
        tmp2 = matrix2dPtr (new matrix2d (hBias->size1(), hBias->size2()));
        for (register unsigned int i = 0; i < hBias->size1(); i++ )
             for (register unsigned int j = 0; j < hBias->size2(); j++ )
                 (*tmp2)(i,j) = 0;

        for (register unsigned int j = 0; j < dif2->size2(); j++)
            for (register unsigned int i = 0; i < dif2->size1(); i++) {
                (*tmp2)(j, 0) += (*dif2)(i,j);
            }


        *hBias += lr* (*tmp2)/dif2->size1();

        if (persistent)
            perSamples = nhSamples;
        lr *= lrCoef;
    }


}

matrix2dPtr RBM::getWeights() {
    return Weights;
}

matrix2dPtr RBM::gethBias() {
    return hBias;
}


void RBM::gibbsHvh( matrix2dPtr inp, matrix2dPtr nvm, matrix2dPtr nvs, matrix2dPtr nhm, matrix2dPtr nhs  ) {
    sample_v_given_h(inp, nvm, nvs);

    sample_h_given_v(nvs, nhm, nhs);
}

void RBM::sample_v_given_h(matrix2dPtr inp, matrix2dPtr v1m, matrix2dPtr v1s) {
    propdown(inp, v1m);
    binomialArray(1, v1m, v1s);

}

matrix2dPtr RBM::reconstruct(matrix2dPtr testData  ) {

    matrix2dPtr tmp, tmp2, outData;
    tmp = matrix2dPtr (new matrix2d (testData->size1(), Weights->size2() )  );


    for (register unsigned int i = 0; i < testData->size1(); i++) {
        for (register unsigned int j = 0; j < Weights->size2(); j++) {
            (*tmp)(i,j) = 0;
        }
    }


    tmp->assign ( prod(*testData, *Weights) );
    for (register unsigned int i = 0; i < tmp->size1(); i++ )
        for (register unsigned int j = 0; j <tmp->size2(); j++) {
            (*tmp)(i,j) += (*hBias)(j,0);
        }

     tmp2 = matrix2dPtr (new matrix2d (tmp->size1(), Weights->size1() )  );

     sigmoidArray(tmp, tmp);
    tmp2 ->assign ( prod(*tmp,  trans( *Weights)) ) ;
     for (register unsigned int i = 0; i < tmp2->size1(); i++ )
         for (register unsigned int j = 0; j <tmp2->size2(); j++)
             (*tmp2)(i,j) += (*vBias)(j,0);

     outData =  matrix2dPtr (new matrix2d (tmp->size1(), Weights->size1() )  );
     sigmoidArray(tmp2, outData);

    return outData;

}

void RBM::sample_h_given_v(matrix2dPtr inp, matrix2dPtr h1Mean, matrix2dPtr h1Sample) {

    propup(inp,  h1Mean);
    binomialArray(1,h1Mean, h1Sample  );
}


void test_rbm() {

    int nrows = 6;
    int ncols = 6;
    int outs = 2;
    int nHidden = 6;
    int training_epochs = 1000;
    int k = 15;
    float learning_rate = 0.01;
    float lr_coef = 0.95;
    matrix2dPtr train_X = matrix2dPtr ( new matrix2d(nrows, ncols) );
    matrix2dPtr train_Y = matrix2dPtr( new matrix2d(nrows, outs) );

    (*train_X) <<= 1,1,1, 0, 0, 0,1, 0, 1, 0, 0, 0,1, 1, 1, 0, 0, 0,0, 0, 1, 1, 1, 0,0, 0, 1, 1, 0, 0,0, 0, 1, 1, 1, 0;

    (*train_Y ) <<=1,0,1,0,1,0,0,1,0,1,0,1;

    int test_out = 2;
    matrix2dPtr test_X = matrix2dPtr (new matrix2d(test_out, ncols) );

    (*test_X)<<= 1,1, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0;
    //create RBM
    RBM rbm (train_X, ncols,  nHidden, NULL, NULL, NULL);
    rbm.contrastive_divergence(k, training_epochs, learning_rate, lr_coef, NULL, true );
    //train RBM

    //reconstruct
    matrix2dPtr out;

    out = rbm.reconstruct( test_X );

    printMatrix("out", out);

}
