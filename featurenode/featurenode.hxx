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

#ifndef FEATURENODE_H
#define FEATURENODE_H
#include "../treenode/treenode.hxx"
#include "../fuzzydatatype/fuzzydatatype.hxx"
#include "../crispdatatype/crispdatatype.hxx"

class FeatureNode: public TreeNode {
    std::string className; //name of the class the current feature is belonging to
    std::string featureName; //the name of the feature, e.g. ndvi, relative_border, relative_area etc
    FuzzyDatatypePtr fuzzyNominal; //variable containing the fuzzy function and her limits
    int nodeFeatureCode; //number of column inside the database where requested feature relies. If it's a CR-Feature it's equal to -1

public:

    FeatureNode();
    FeatureNode (std::string,std::string, FuzzyDatatypePtr);//Property name and fuzzyNominal
    void forceNULLMembershipValue();
    virtual ~FeatureNode();
    int getFeatureCode();
    void resetMembershipValue();
    void setNotEstimated();
    virtual void valueEstimation(int);

    //static public members
    static void setTableName(std::string);
};

typedef std::shared_ptr<FeatureNode> FeatureNodePtr;

#endif // FEATURENODE_H
