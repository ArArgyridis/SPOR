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

#include "CDBN.hxx"

using namespace std;

CDBN::CDBN(matrix2dPtr inp, matrix2dPtr out, int ins, vector<int> hls, int n_outs)  {
    inputData = inp;
    outData = out;
    nInputs = ins;
    hiddenLayerSizes = hls;
    nOuts = n_outs;
    nLayers = hiddenLayerSizes.size();

    for (register int i = 0; i <nLayers; i++ ) {
        matrix2dPtr layerInput;
        int inputSize;
        if (i == 0) {
            layerInput = inputData;
            inputSize = ins;
        }
        else {
            layerInput = sigmoidLayers.back().getSampleHGivenV(layerInput);
            inputSize = hiddenLayerSizes[i - 1];
        }

        //add hidden layer
        HiddenLayer sigmoidLayer = HiddenLayer (layerInput, inputSize, hiddenLayerSizes[i], NULL,NULL);
        sigmoidLayers.push_back(sigmoidLayer);
        //add rbm layer
        if (i == 0) {
            CRBM *rbmLayer = new CRBM( layerInput, inputSize, hiddenLayerSizes[i],  sigmoidLayer.getWeights(), sigmoidLayer.getBias() , NULL) ;
            rbmLayers.push_back(rbmLayer);
        }
        else {
            RBM *rbmLayer = new RBM( layerInput, inputSize, hiddenLayerSizes[i],  sigmoidLayer.getWeights(), sigmoidLayer.getBias() , NULL) ;
            rbmLayers.push_back(rbmLayer);

        }
    }

    logisticLayer = new LogisticRegression (sigmoidLayers.back().getSampleHGivenV(NULL), outData, hiddenLayerSizes.back(), nOuts);
    finetuneCost = logisticLayer->negativeLogLikelihood();
}


void test_cdbn() {
    float learning_rate = 0.01, lrCoef = 0.95;
    float n_epochs =5000;
    vector<int> hl;
    int nrows = 6;
    int ncols = 6;
    int outs = 2;
    matrix2dPtr train_X = matrix2dPtr ( new matrix2d(nrows, ncols) );
    matrix2dPtr train_Y = matrix2dPtr( new matrix2d(nrows, outs) );

    (*train_X) <<= 0.4, 0.5, 0.5, 0.,  0.,  0.
            ,0.5, 0.3,  0.5, 0.,  0.,  0.,
            0.4, 0.5, 0.5, 0.,  0.,  0.,
            0.,  0.,  0.5, 0.3, 0.5, 0.,
            0.,  0.,  0.5, 0.4, 0.5, 0.,
            0.,  0.,  0.5, 0.5, 0.5, 0.;

    (*train_Y ) <<=1,0,
            1,0,
            1,0,
            0,1,
            0,1,
            0,1;

    hl.push_back(80);
    hl.push_back(100);
    //hl.push_back(60);
    //hl.push_back(80);
    matrix2dPtr test_X = matrix2dPtr ( new matrix2d(3, ncols) );
    (*test_X) <<= 0.5, 0.5, 0., 0., 0., 0.,
    0., 0., 0., 0.5, 0.5, 0.,
     0.5,0.3,0.5,0.5,0.5,0;

    CDBN dbn = CDBN(train_X, train_Y,ncols,  hl, outs);
    dbn.pretrain(learning_rate, lrCoef, 1, n_epochs);

    int f_epochs = 5000;
    float finetuneLR = 0.1 , finetuneCoef = 0.95;
    dbn.finetuning( finetuneLR, finetuneCoef, f_epochs  );
    matrix2dPtr output ;//= matrix2dPtr (new matrix2d (nrows, outs));
    output = dbn.predict( test_X );
    printMatrix("final output", output);
}



