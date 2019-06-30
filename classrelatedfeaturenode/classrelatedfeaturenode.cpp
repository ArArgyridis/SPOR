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

#include "classrelatedfeaturenode.hxx"

using namespace pqxx;
using namespace std;


void ClassRelatedFeatureNode::resetPosition() {
    position = 0;
}

ClassRelatedFeatureNode::ClassRelatedFeatureNode() {
    relativeClassName = "Dummy";
    position = 0;
    //cResult = nullptr;
}

string ClassRelatedFeatureNode::getRelativeClassName() {
    return relativeClassName;
}

void ClassRelatedFeatureNode::setCResult(connection *conn, string &tableName, string &gidColumn){

    work Xaction2 (*conn);
    string query = "select * from " + tableName + " order by " + gidColumn + " asc" ;
    cResult = Xaction2.exec(query);
    maxPosition = (int) cResult.size();
    Xaction2.commit();
}

ClassRelatedFeatureNode::ClassRelatedFeatureNode( string cName,  FuzzyObjectTypePtr fOType):fuzzyNominal(fOType),relativeClassName(cName), position(0), maxPosition(0) {
    //relativeClassName = cName;
    //position = 0;
    setNodeType(CRFEATURE);
}

void ClassRelatedFeatureNode::valueEstimation(int curSegment) {
 //   cout << this->relativeClassName <<"\t"<< position << "\t"<< cResult[position] [0] << "\t" << TreeNode::getSegments()[curSegment][0] << "\t" << fuzzyNominal->calculateValue((double)atof(to_string( cResult[position] [1] ).c_str() ) ) <<endl;
        if ( (position < maxPosition ) && (cResult[position] [0] == TreeNode::getSegments()[curSegment][0] ) )  {
            setMembershipValue(fuzzyNominal->calculateValue((double)atof(to_string( cResult[position] [1] ).c_str() ) ));
            position++;
        }
        else {
            setMembershipValue(fuzzyNominal->calculateValue(0.0));
        }
}
ClassRelatedFeatureNode::~ClassRelatedFeatureNode() {

}

void ClassRelatedFeatureNode::forceNULLMembershipValue(){
    setMembershipValue(FEATURE_NODE_NULL);
}

void ClassRelatedFeatureNode::resetMembershipValue() {
    this->setMembershipValue(FEATURE_NODE_NULL);
}

string ClassRelatedFeatureNode::getFeatureType() {
    return fuzzyNominal->getFeatureType();
}
