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

#include "logicnode.hxx"

void LogicNode::addSubNode(TreeNodePtr nodePtr) {
    subNodeVector.push_back(nodePtr);
    nodePtr->setParentNode(shared_from_this() );
    nodePtr->parentNodesSumType += this->nodeType;
    //subNodeCount++;
}

int LogicNode::getAllParentAndNode() {
    if ( parentNode != NULL   ) {
        int type = parentNode->getAllParentAndNode();
        if ( ( type == 0 ) && ( nodeType == AND ) )
            return 0;
        else
            return 1;
    }
    else {
        if ( nodeType == AND )
            return 0;
        else
            return 1;
    }
    //return parentNodesSumType % AND;
}

LogicNode::LogicNode():TreeNode(0.0, false, LOGIC), subNodeCount(0) {}



int LogicNode::getNodeCount() {
    return (int)subNodeVector.size();
}

TreeNodePtr LogicNode::getSubNode(int position) {
    return subNodeVector.at(position);
}

LogicNode::~LogicNode() {}

void TreeNode::emptyNodes() {}


