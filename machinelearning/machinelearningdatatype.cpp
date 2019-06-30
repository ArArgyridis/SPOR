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


#include <string>
#include <fstream>
#include <iostream>

#include "machinelearningdatatype.hxx"
#include "CDBN/CDBN.hxx"
using namespace std;


void MachineLearningDatatype::addEmployedClass(string cName) {
    employedClass.push_back(cName);
}

string MachineLearningDatatype::getAttributeValue(string &name) {
    return attributeMap[name];
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

std::string MachineLearningDatatype::getMethod() {
    return method;
}

int MachineLearningDatatype::getNumberOfEmployedClasses() {
    return employedClass.size();
}

int MachineLearningDatatype::getNumberOfFeatures() {
    return features.size();
}

matrix2dPtr MachineLearningDatatype::getOutput() {
    return output;
}

MachineLearningDatatype::MachineLearningDatatype(){}

MachineLearningDatatype::MachineLearningDatatype (string methodName, string strNet, string fts, double lr, double lrCoef, int epchs, int k, double supLR, double supLRCoef, int supEp): computed(false), method(methodName),
leariningRate(lr), learningCoef(lrCoef), epochs(epchs), gibbsIterations(k), supervisedRate(supLR), supervisedRateCoef(supLRCoef), supervisedEpochs(supEp){
    if (fts.size() > 0) {
        stringstream ss(fts);
        string item;
        while (getline (ss, item, ',') ) {
            features.push_back(item);
        }
    }

    string item;
    stringstream ss(strNet);
    while (getline (ss, item, '-') ) {
        network.push_back(atoi(item.c_str()));
    }
}

MachineLearningDatatype::MachineLearningDatatype(string &methodName, std::map<string, string> &attributes): computed(false), method(methodName), attributeMap(attributes) {
    if (attributes["feature_space"].size() > 0) {
        stringstream ss(attributes["feature_space"]);
        string item;
        while (getline (ss, item, ',') ) {
            features.push_back(item);
        }
    }
}

void MachineLearningDatatype::setClassColumnMap(std::map<string, int> &cMap) {
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
        cdbn.pretrain(leariningRate, learningCoef, gibbsIterations, epochs);
        cdbn.finetuning( supervisedRate, supervisedRateCoef, supervisedEpochs );
        std :: cout <<"DBN training finished\n";
        samplesOutput = cdbn.predict(inputSamples);
        output = cdbn.predict( classifyData );
        writeMatrix("output", samplesOutput);
        //printMatrix("final output", output);
    }
}

void MachineLearningDatatype::mlComputation(SampleVector &samples, LabelTypeVector &labels, SampleVector &classifyData, std::vector<int> &labelVector) {
    classificationObjectID = labelVector;
    if(method == "SVM") {
        svm = SVM(samples, labels);
        LabelTypeVector outLabels;
        svm.predict(classifyData, outLabels);
        int numberOfClasses = getNumberOfEmployedClasses();
        output = matrix2dPtr(new matrix2d(outLabels.size(), numberOfClasses));
        for (int i = 0; i < outLabels.size(); i++) {
            for (int j = 0; j < numberOfClasses; j++)
                if(outLabels[i] == j)
                    (*output)(i, j) = 1;
                else
                    (*output)(i, j) = 0;
        }
    }
}
