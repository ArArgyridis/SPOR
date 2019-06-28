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

#ifndef CLASSRELATEDFEATURENODE_H
#define CLASSRELATEDFEATURENODE_H

#include "../fuzzyobjectproperty/fuzzyobjectproperty.hxx"
#include "../treenode/treenode.hxx"
#include <string>

class ClassRelatedFeatureNode: public TreeNode {
    pqxx::result cResult; //result of the feature
    FuzzyObjectTypePtr fuzzyNominal; //container of the fuzzy function and the feature name
    std::string relativeClassName; //the class where the feature points
    int position;
    int maxPosition;

public:

    ClassRelatedFeatureNode();

    ClassRelatedFeatureNode(std::string, FuzzyObjectTypePtr);

    virtual ~ClassRelatedFeatureNode();

    void forceNULLMembershipValue();

    std::string getFeatureName();

    std::string getFeatureType();

    std::string getRelativeClassName();

    double returnFeatureValue(int); //returning the value of the feature for a certain segment. The segment is identified by the int - gid

    void resetMembershipValue();

    void resetPosition();

    void setCResult(pqxx::connection*, std::string*, std::string*);

    void setFeatureType(std::string);

    void valueEstimation(int);
};

typedef std::shared_ptr<ClassRelatedFeatureNode> ClassRelatedFeatureNodePtr;

#endif // CLASSRELATEDFEATURENODE_H
