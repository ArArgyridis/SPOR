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
 
#ifndef HIEARCHYNODE_H_INCLUDED
#define HIEARCHYNODE_H_INCLUDED
#include <vector>
#include <libpq-fe.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "fuzzyfunctions.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <pqxx/pqxx>

extern const double AND_NODE_NULL;
extern const double OR_NODE_NULL;
extern const double FEATURE_NODE_NULL;
extern const double SUPERCLASS_NODE_NULL;
extern const double COMPLEMENT_NODE_NULL;

enum node_type {TREENODE = 0, FEATURE, CRFEATURE, LOGIC, AND, OR, SUPERCLASS, COMPLEMENT};



class TreeNode;
typedef boost::shared_ptr<TreeNode> TreeNodePtr;

class LogicNode;
typedef boost::shared_ptr<LogicNode> LogicNodePtr;

class TreeNode {
    double membershipValue; //node's membership value


protected:

    /*Holder to identify if for a curent segment the membership for the curent node is computed*/
    bool isEstimated;

    /*Node type identifier*/
    int nodeType;

    /*pointer to the super node of the current element*/
    LogicNodePtr parentNode;

    /*name of the current examined class*/
    static std::string curClass;

public:
    int parentNodesSumType;


    double getMembershipValue();
    int getNodeType();
    void setNodeType(int);
    void setMembershipValue(double);

    TreeNode();
    TreeNode(double, bool,  int );
    ~TreeNode();


    void setParentNode(LogicNodePtr);
    int getNodeSumType();
    virtual void valueEstimation(int) = 0;
    virtual void resetMembershipValue() = 0;
    virtual void forceNULLMembershipValue() = 0;

    static void addSegments(pqxx::result);
    static pqxx::result getSegments();
    static void emptyNodes();
    static pqxx::result segmentsResult;
    static void setCurClass(std::string);
};








#endif // HIEARCHYNODE_H_INCLUDED

