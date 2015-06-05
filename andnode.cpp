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
 
 
#include "andnode.h"


///:~ AndNode Methods//////////////////////////////////////////////////////////////

AndNode::~AndNode() {}

AndNode::AndNode() {
    setMembershipValue(AND_NODE_NULL);
    setNodeType(AND);
    parentNodesSumType= AND;
}
void AndNode::forceNULLMembershipValue(){
    setMembershipValue(AND_NODE_NULL);
    for (register int i = 0; i <subNodeCount; i ++)
        subNodeVector.at(i)->forceNULLMembershipValue();
}

void AndNode::resetMembershipValue() {
    setMembershipValue(AND_NODE_NULL);
    for (register int i = 0; i <(int)subNodeVector.size(); i++) {
        subNodeVector.at(i)->resetMembershipValue();
    }
}


void AndNode::valueEstimation(int curSegment) {
    this->setMembershipValue(AND_NODE_NULL);
    for (int i = 0; i< (int)subNodeVector.size(); i++) {
        subNodeVector.at(i)->valueEstimation(curSegment);
        if (this->getMembershipValue() > subNodeVector.at(i)->getMembershipValue()) {
                this->setMembershipValue(subNodeVector.at(i)->getMembershipValue() );
        }
    }
}

