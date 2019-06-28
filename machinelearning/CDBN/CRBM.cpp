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

#include "CRBM.hxx"

using namespace std;
using namespace boost::numeric::ublas;

CRBM::CRBM(matrix2dPtr trainData,int visibleLayers,  int hiddenLayers, matrix2dPtr w, matrix2dPtr hb, matrix2dPtr vb):RBM( trainData,  visibleLayers, hiddenLayers,  w,  hb,  vb) {
    type = typeCRBM;
}

CRBM::CRBM() {}


void CRBM::propdown(matrix2dPtr inp, matrix2dPtr preActivation) {
    preActivation->assign(prod(*inp, trans(*Weights) ) ) ;

    for (register unsigned int i = 0; i < preActivation->size1(); i++) {
        for (register unsigned int j = 0; j <preActivation->size2(); j++) {
            (*preActivation) (i, j ) += (*vBias)(j,0);
        }
    }


}


void CRBM::sample_v_given_h(matrix2dPtr inp, matrix2dPtr v1m, matrix2dPtr v1s) {
    matrix2dPtr aH;
    aH  = matrix2dPtr (new matrix2d  (inp->size1(), Weights->size1() ) );

    propdown(inp, aH);
    //printMatrix("Weights", Weights);
    matrix2dPtr expNeg, expPos, tmpNeg, uniform  ;
    expNeg = matrix2dPtr( new matrix2d(aH->size1(), aH->size2())  );
    expPos = matrix2dPtr( new matrix2d(aH->size1(), aH->size2())  );
    tmpNeg  = matrix2dPtr( new matrix2d(aH->size1(), aH->size2())  );
    uniform = matrix2dPtr( new matrix2d(v1m->size1(), v1m->size2()  ) );

    tmpNeg->assign (- (*aH) );

     exponentialArray(tmpNeg, expNeg);
     exponentialArray(aH, expPos);



    for (iterator1 it1 = v1m->begin1(); it1 != v1m->end1(); it1++    )
        for(iterator2 it2 = it1.begin(); it2 != it1.end(); it2++) {
              *it2 = 1/( 1 - (*expNeg)(it2.index1(), it2.index2() ) ) - 1/(*aH)(it2.index1(), it2.index2() );
        }
    uniformArray(uniform, 0, 1);

    for (iterator1 it1 = v1s->begin1(); it1 != v1s->end1(); it1++    )
        for(iterator2 it2 = it1.begin(); it2 != it1.end(); it2++) {
            *it2 = log(1 -            (*uniform)(it2.index1(), it2.index2() ) * (1 -  (*expPos)(it2.index1(), it2.index2())   )  ) / (*aH) (it2.index1(), it2.index2());
        }
}

void test_crbm() {

    int nrows = 6;
    int ncols = 6;
    int outs = 2;
    int nHidden = 5;
    int training_epochs = 1000;
    int k = 15;
    float learning_rate = 0.01;
    float coef = 0.99;
    matrix2dPtr train_X = matrix2dPtr ( new matrix2d(nrows, ncols) );
    matrix2dPtr train_Y = matrix2dPtr( new matrix2d(nrows, outs) );


    (*train_X) <<= 0.4,0.5,0.5, 0, 0, 0,
                               0.5, 0.3, 0.5, 0, 0, 0,
                                0.4, 0.5, 0.5, 0, 0, 0,
                                0, 0, 0.5, 0.3, 0.5, 0,
                                0, 0, 0.5, 0.4, 0.5, 0,
                                0, 0, 0.5, 0.5, 0.5, 0;

    (*train_Y ) <<=1,0,1,0,1,0,0,1,0,1,0,1;

    int test_out = 2;
    matrix2dPtr test_X = matrix2dPtr (new matrix2d(test_out, ncols) );

    //(*test_X)<<= 1,1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0;
    (*test_X)<<= 0.5,0.5, 0, 0, 0,0,
                           0, 0, 0, 0.5, 0.5, 0;
    matrix2dPtr asd;
    //create RBM
    CRBM rbm (train_X, ncols, nHidden, asd, asd, asd);
    //train RBM
    rbm.contrastive_divergence(k, training_epochs, learning_rate, coef, NULL, true );
    //reconstruct
    matrix2dPtr out;

    out = rbm.reconstruct( test_X );

    printMatrix("out", out);

}
