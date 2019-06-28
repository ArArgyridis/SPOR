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

#include "machinelearningnode.hxx"
#include "../machinelearning/CDBN/functions.hxx"
using namespace std;

//const double MACHINE_LEARNING_NODE_NULL = 2;

void MachineLearningNode::computeClassification(matrix2dPtr inputSamples, matrix2dPtr out, matrix2dPtr data, map<string, int> clColMap, vector<int> objID) {

    datatype->mlComputation(inputSamples, out, data, objID);
    datatype->setClassColumnMap(clColMap);
}

void MachineLearningNode::forceNULLMembershipValue( ) {}

bool MachineLearningNode::getComputed() {
    return datatype->getComputed();
}

string* MachineLearningNode::getEmployedClass(int pos) {
    return datatype->getEmployedClass(pos);
}


string* MachineLearningNode::getFeature(int pos) {
    return datatype->getFeature(pos);
}


int MachineLearningNode::getNumberOfEmployedClasses () {
    return datatype->getNumberOfEmployedClasses();
}

int MachineLearningNode::getNumberOfFeatures() {
    return datatype->getNumberOfFeatures();
}

MachineLearningNode::MachineLearningNode(string propName, MachineLearningDatatypePtr dtype): TreeNode(MACHINE_LEARNING_NODE_NULL,  false, MACHINELEARNING),
    curMLObject(0), propertyName(propName), datatype(dtype) {}

void MachineLearningNode::resetMembershipValue() {
    setMembershipValue(MACHINE_LEARNING_NODE_NULL);
}

void MachineLearningNode::setComputed() {
    datatype->setComputed();
}

void MachineLearningNode::valueEstimation(int curSegment) {
    if(  getSegments()[curSegment][0].as<int>() == datatype->getClassificationObjectID(curMLObject)  ) {
    /*
    if (  datatype->getOutput()->at_element(curMLObject,  datatype->getClassPosition(curClass) ) >= 0.5  )
        setMembershipValue(1);
     else
         setMembershipValue(0);

    */
     setMembershipValue(  datatype->getOutput()->at_element(curMLObject,  datatype->getClassPosition(curClass) )    );
     curMLObject++;
    }
    else
        setMembershipValue(0);
}
