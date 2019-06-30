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

#include "DBN.hxx"
#include "CRBM.hxx"
#include <memory>

using namespace std;


DBN::DBN(matrix2dPtr tX, matrix2dPtr tY, std::vector<int> hl): inputData(tX), outData(tY), hiddenLayerSizes(hl) {
    nInputs = inputData->size2();
    nOuts = outData->size2();
    nSamples = inputData->size1();

    nLayers = hiddenLayerSizes.size();
    if (nLayers < 2) {
        cout <<"Network must have at least 2 hidden layers!\n";
        return;
    }

    //creating multi-layer DBN
    for (register int i = 0; i < nLayers; i++ ) {
        int inputSize;
        matrix2dPtr layerInput;
        if (i == 0) {
            inputSize = nInputs;
            layerInput = inputData;
        }
        else {
            inputSize = hiddenLayerSizes.back();
                    std :: cout <<"first output"<<"\n";
            layerInput = sigmoidLayers.back().getSampleHGivenV(nullptr);
            std :: cout << "end of fist output\n";
        }

        HiddenLayer sigmoidLayer = HiddenLayer(layerInput, inputSize, hiddenLayerSizes[i], nullptr, nullptr);
        sigmoidLayers.push_back(sigmoidLayer);

        RBM *rbmLayer = new RBM( layerInput, inputSize, hiddenLayerSizes[i],  sigmoidLayer.getWeights(), sigmoidLayer.getBias() , nullptr) ;
        rbmLayers.push_back(rbmLayer);
    }

    //layer for logistic regression
    logisticLayer = new LogisticRegression (sigmoidLayers.back().getSampleHGivenV(nullptr), outData, hiddenLayerSizes.back(), nOuts);
    finetuneCost = logisticLayer->negativeLogLikelihood();
}


DBN::~DBN() {
    delete logisticLayer;
}

void DBN::pretrain(float learningRate, float lrCoef, int  k, int epochs) {
    matrix2dPtr layerInput = inputData;
    for (register unsigned int i = 0; i < hiddenLayerSizes.size(); i++) {

        if (i == 0) {
            layerInput = inputData;
        }
        else {            
            layerInput = sigmoidLayers[i-1].getSampleHGivenV(layerInput);
        }

        RBM *rbm;
        CRBM *crbm;
        if ( rbmLayers[i]->type == typeRBM) {
            rbm = new RBM();
            rbm = rbmLayers[i];
            rbm->contrastive_divergence(k, epochs, learningRate, lrCoef, layerInput, true);
        }
        else if (rbmLayers[i]->type == typeCRBM) {
            crbm = dynamic_cast< CRBM* > (rbmLayers[i]);
            crbm->contrastive_divergence(k, epochs, learningRate, lrCoef, layerInput, true);
        }
    }
}


void DBN::finetuning (float learningRate, float lrCoef, int epochs) {
    /*
    matrix2dPtr layerInput = sigmoidLayers.back().getSampleHGivenV(nullptr);
    printMatrix("layer input of fine tuning", layerInput);

    */
    matrix2dPtr layerInput = this->inputData;
    for (register unsigned  int i = 0; i < hiddenLayerSizes.size(); i++) {

        matrix2dPtr tmp = matrix2dPtr( new matrix2d(inputData->size1(), sigmoidLayers[i].getOuts() ) );
       /*
        printMatrix("layerInput", layerInput);
        printMatrix("layer weights", sigmoidLayers[i].getWeights() );
        */
        sigmoidLayers[i].output(layerInput, tmp ) ;

        layerInput = tmp;
    }
        logisticLayer->train(epochs, learningRate, lrCoef, 0.5, layerInput);
}

matrix2dPtr DBN::predict(matrix2dPtr input) {
    matrix2dPtr layerInput = input;
    for (register unsigned  int i = 0; i < hiddenLayerSizes.size(); i++) {
        matrix2dPtr tmp = matrix2dPtr( new matrix2d(input->size1(), sigmoidLayers[i].getOuts() ) );
        writeMatrix("predictRBM",layerInput);
        writeMatrix("weights", sigmoidLayers[i].getWeights() );

        sigmoidLayers[i].output(layerInput, tmp ) ;
        layerInput = tmp;
    }


    matrix2dPtr output = matrix2dPtr( new matrix2d(input->size1(), nOuts));
    logisticLayer->predict(layerInput, output );
    return output;

}

DBN::DBN(){}


void test_dbn() {
    float learning_rate = 0.1, lrcoef = 0.95;
    float n_epochs =1000;
    vector<int> hl;
    int nrows = 6;
    int ncols = 6;
    int outs = 2;
    matrix2dPtr train_X = matrix2dPtr ( new matrix2d(nrows, ncols) );
    matrix2dPtr train_Y = matrix2dPtr( new matrix2d(nrows, outs) );

    (*train_X) <<= 1,1,1, 0, 0, 0,1, 0, 1, 0, 0, 0,1, 1, 1, 0, 0, 0,0, 0, 1, 1, 1, 0,0, 0, 1, 1, 0, 0,0, 0, 1, 1, 1, 0;
    (*train_Y ) <<=1,0,1,0,1,0,0,1,0,1,0,1;
    hl.push_back(55);
    hl.push_back(55);


   matrix2dPtr  test_X =  matrix2dPtr ( new matrix2d(2, ncols) );

   (*test_X)        <<= 1,1, 0, 0, 0, 0,
                                    0, 0, 0, 1, 1, 0;
    DBN dbn = DBN(train_X, train_Y, hl);
    dbn.pretrain(learning_rate, lrcoef, 15, n_epochs);

    int f_epochs = 200;
    float finetuneLR = 0.1 , finetuneCoef = 0.95;
    dbn.finetuning( finetuneLR, finetuneCoef, f_epochs  );
    matrix2dPtr output ;//= matrix2dPtr (new matrix2d (nrows, outs));
    output = dbn.predict( test_X );
    printMatrix("final output", output);
}
