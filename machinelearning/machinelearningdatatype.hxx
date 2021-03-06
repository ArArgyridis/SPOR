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

#ifndef MACHINELEARNINGNODE_H
#define MACHINELEARNINGNODE_H

#include "CDBN/CDBN.hxx"
#include "SVM/svm.hxx"
#include <dlib/svm_threaded.h>
#include <iostream>
#include <map>


typedef std::shared_ptr<CDBN> CDBNPtr;

class MachineLearningDatatype {
protected:
    CDBNPtr DBN;
    SVM svm;
    std::string method;
    int epochs, gibbsIterations, supervisedEpochs;
    double leariningRate, learningCoef, supervisedRate, supervisedRateCoef;
    std::vector<std::string> features;
    std::vector<std::string> employedClass;
    bool computed;
    matrix2dPtr output;
    std::map<std::string, int> classColumnMap;
    std::map<std::string, std::string> attributeMap;
    std::vector<int> classificationObjectID, network;

public:

    void addEmployedClass(std::string);
    std::string getAttributeValue(std::string& name);
    int getClassificationObjectID(int);
    int getClassPosition(std::string);
    bool getComputed();
    std::string* getEmployedClass(int);
    std::string* getFeature(int);
    std::string getMethod();
    int getNumberOfEmployedClasses();
    int getNumberOfFeatures();
    MachineLearningDatatype();
    MachineLearningDatatype(std::string, std::string, std::string, double, double, int, int , double, double, int );
    MachineLearningDatatype(std::string& methodName, std::map<std::string, std::string>& attributes );
    void mlComputation(matrix2dPtr, matrix2dPtr, matrix2dPtr, std::vector<int>);
    void mlComputation(SampleVector& samples, LabelTypeVector& labels,  SampleVector& classifyData, std::vector<int>& labelVector);
    matrix2dPtr getOutput();
    void setComputed();
    void setClassColumnMap(std::map<std::string, int>&);
    matrix2dPtr samplesOutput;
};

typedef std::shared_ptr<MachineLearningDatatype> MachineLearningDatatypePtr;
typedef std::map <std::string, MachineLearningDatatypePtr> MachineLearningDatatypeMapType;
typedef std::shared_ptr<MachineLearningDatatypeMapType> MachineLearningDatatypeMapTypePtr;

#endif // MACHINELEARNINGNODE_H
