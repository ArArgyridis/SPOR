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

#include "treenode.hxx"


using namespace std;
using namespace pqxx;

const double TREE_NODE_NULL = 2;
const double AND_NODE_NULL = 2;
const double OR_NODE_NULL = -2;
const double FEATURE_NODE_NULL = 3;
const double SUPERCLASS_NODE_NULL = 4;
const double COMPLEMENT_NODE_NULL = 4;
const double MACHINE_LEARNING_NODE_NULL = 2;

result TreeNode::segmentsResult ;
string TreeNode::curClass;

///:~ TreeNode Methods /////////////////////////////////////////////////////
TreeNode::TreeNode(): membershipValue(TREE_NODE_NULL),  isEstimated(false) {}

TreeNode::TreeNode(double mv, bool est, int type): membershipValue(mv), isEstimated(est), nodeType(type), parentNode(nullptr){}

TreeNode::~TreeNode() {}

void TreeNode::setCurClass(string cName) {
    curClass = cName;
}

void TreeNode::setNodeType(int ntype) {
    nodeType = ntype;
}

void TreeNode::setParentNode(LogicNodePtr node) {
    parentNode = node;
}

void TreeNode::addSegments(result res) {
        segmentsResult = res;
}

double TreeNode::getMembershipValue() {
    return membershipValue;
}

void TreeNode::setMembershipValue(double m) {
        membershipValue = m;
}

int TreeNode::getNodeType() {
    return nodeType;
}

result TreeNode::getSegments() {
    return segmentsResult;
}
