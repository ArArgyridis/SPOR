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

#include "HiddenLayer.hxx"

using namespace std;
using namespace boost::numeric::ublas;


HiddenLayer::HiddenLayer(matrix2dPtr  inp, int in, int out, matrix2dPtr W, matrix2dPtr b): inputData(inp), nInputs(in), nOutputs(out)  {

    if (W == nullptr) {
        Weights = matrix2dPtr( new matrix2d( nInputs, nOutputs) );
        //float a = 1.0 /nInputs;
        float a = -4.0*sqrt(6.0 / (nInputs + nOutputs)  );
        uniformArray(Weights, -a , a  );
        //printMatrix("Weights", Weights);
    }
    else
        Weights = W;

    if (b == nullptr) {
        this->bias = matrix2dPtr( new matrix2d(nOutputs, 1 ) );
        for (iterator1 it1 = bias->begin1(); it1 != bias->end1(); it1++)
            for (iterator2 it2 = it1.begin(); it2 != it1.end();  it2++) {
                *it2 = 0;
            }
    }
    else {

        bias = b;
    }
    //printMatrix("bias",bias);

}

matrix2dPtr HiddenLayer::getBias() {
    return bias;
}

matrix2dPtr HiddenLayer::getSampleHGivenV(matrix2dPtr inp) {
    if (inp != nullptr) inputData = inp;
    matrix2dPtr ret = matrix2dPtr (new matrix2d ( inputData->size1(), Weights->size2()  ) );
    sample_h_given_v(inputData, ret);
    return ret;
}

int HiddenLayer::getOuts() {
    return nOutputs;
}

matrix2dPtr HiddenLayer::getWeights() {
    return Weights;
}


void HiddenLayer::output(matrix2dPtr input, matrix2dPtr output) {
    if (input != nullptr) inputData = input;
    *output = prod( *inputData, *Weights  )  ;

    for (iterator1 it1 = output->begin1(); it1 != output->end1(); it1++  )
        for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++ )
            *it2 += (*bias) ( it2.index2() , 0  );

    sigmoidArray(output, output);
}

void HiddenLayer::sample_h_given_v (matrix2dPtr in, matrix2dPtr out) {

    if(in != nullptr) inputData = in;
    //printMatrix("hidden layer sampling input", inputData);
    matrix2dPtr tmp = matrix2dPtr (new matrix2d ( out->size1(), out->size2() ) );
    output(inputData, tmp);
    //printMatrix("temp inside sampling", tmp);
    binomialArray(1, tmp, out);

}


void testHl() {

    int nrows = 6;
    int ncols = 6;
    int outs = 2;
    matrix2dPtr train_X = matrix2dPtr ( new matrix2d(nrows, ncols) );
    matrix2dPtr W , out;

    (*train_X) <<= 1,1,1, 0, 0, 0,1, 0, 1, 0, 0, 0,1, 1, 1, 0, 0, 0,0, 0, 1, 1, 1, 0,0, 0, 1, 1, 0, 0,0, 0, 1, 1, 1, 0;

    HiddenLayer k = HiddenLayer( train_X, ncols, outs, nullptr, nullptr   );

    out =  matrix2dPtr ( new matrix2d(nrows, outs) );
    k.sample_h_given_v(nullptr, out);
    k.output(nullptr, out);
    printMatrix("out", out);
}
