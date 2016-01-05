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

#include "machinelearningdatatype.h"
#include "CDBN/CDBN.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;


void MachineLearningDatatype::addEmployedClass(string cName) {
    employedClass.push_back(cName);
}

int MachineLearningDatatype::getClassPosition(string cName) {
    std::map<string, int>::iterator it;
    it = classColumnMap.find( cName );
    if ( it != classColumnMap.end() )
        return (*it).second;
    else
        return -1;
}

int MachineLearningDatatype::getClassificationObjectID(int pos) {
    return classificationObjectID[pos];
}

bool MachineLearningDatatype::getComputed() {
    return computed;
}


string* MachineLearningDatatype::getEmployedClass(int pos) {
    return &employedClass[pos];
}

std::string* MachineLearningDatatype::getFeature(int pos) {
    return &features[pos];
}

int MachineLearningDatatype::getNumberOfEmployedClasses() {
    return employedClass.size();
}

int MachineLearningDatatype::getNumberOfFeatures() {
    return features.size();
}

matrix2dPtr MachineLearningDatatype::getOutput() {
    return this->output;
}

MachineLearningDatatype::MachineLearningDatatype(){}


MachineLearningDatatype::MachineLearningDatatype (string mtd, string strNet, string fts, double lr, double lrCoef, int epchs, int k, double supLR, double supLRCoef, int supEp): computed(false), method(mtd),
leariningRate(lr), learningCoef(lrCoef), epochs(epchs), gibbsIterations(k), supervisedRate(supLR), supervisedRateCoef(supLRCoef), supervisedEpochs(supEp){
    if (fts.size() > 0) {
        stringstream ss(fts);
        string item;
        while (getline (ss, item, ',') )
            features.push_back(item);
    }

    string item;
    stringstream ss(strNet);
    while (getline (ss, item, '-') ) {
        network.push_back(atoi(item.c_str()));
    }
}

void MachineLearningDatatype::setClassColumnMap(std::map<string, int> cMap) {
    classColumnMap = cMap;
}

void MachineLearningDatatype::setComputed() {
    computed = true;
}


void MachineLearningDatatype::mlComputation(matrix2dPtr inputSamples, matrix2dPtr outputSamples, matrix2dPtr classifyData, vector<int> objID ) {

    classificationObjectID = objID;

    if (method == "DBN") {
        CDBN cdbn(inputSamples, outputSamples, inputSamples->size2(), network, outputSamples->size2() );
        std :: cout << "starting DBN training\n";
        //cdbn.pretrain(0.0001, 3, 1000);
        cdbn.pretrain(leariningRate, learningCoef, gibbsIterations, epochs);
        double finetuneLR = 0.5, finetuneCoef = 0.95;
        int f_epochs = 2000;
        //cdbn.finetuning(  finetuneLR, finetuneCoef, f_epochs  );
        cdbn.finetuning( supervisedRate, supervisedRateCoef, supervisedEpochs );
        std :: cout <<"DBN training finished\n";
        samplesOutput = cdbn.predict(inputSamples);
        output = cdbn.predict( classifyData );
        writeMatrix("output", samplesOutput);
        //printMatrix("final output", output);
    }
}
