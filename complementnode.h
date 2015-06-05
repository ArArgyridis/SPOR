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
 
#ifndef COMPLEMENTNODE_H
#define COMPLEMENTNODE_H
#include "logicnode.h"

class ComplementNode;

typedef boost::shared_ptr<ComplementNode> ComplementNodePtr;


class ComplementNode: public LogicNode {

public:
    void forceNULLMembershipValue();
    ComplementNode();
    virtual ~ComplementNode();
    void resetMembershipValue();
    void valueEstimation(int);
};

#endif // COMPLEMENTNODE_H
