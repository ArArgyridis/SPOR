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

#ifndef LOGICNODE_H
#define LOGICNODE_H

#include "treenode.h"
#include <boost/enable_shared_from_this.hpp>

class LogicNode: public boost::enable_shared_from_this<LogicNode>, public TreeNode {
protected:
    std::vector <TreeNodePtr> subNodeVector;    //vector containing pointers to other sub-nodes
    int subNodeCount; //number of sub-nodes
public:
    LogicNode();
    ~LogicNode();
    void setNotEstimated();
    int getNodeCount();
    int getAllParentAndNode();
    TreeNodePtr getSubNode(int);
    void addSubNode(TreeNodePtr);
   //virtual  void resetMembershipValue() = 0;
    virtual void valueEstimation(int) = 0;
    virtual void forceNULLMembershipValue() = 0;


};




#endif // LOGICNODE_H
