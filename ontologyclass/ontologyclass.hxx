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

#ifndef ONTOLOGYCLASS_H_INCLUDED
#define ONTOLOGYCLASS_H_INCLUDED
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>
#include <algorithm>
#include "../fuzzyobjectproperty/fuzzyobjectproperty.hxx"
#include "../classrelatedfeaturenode/classrelatedfeaturenode.hxx"
#include <boost/shared_array.hpp>
#include "../superclassnode/superclassnode.hxx"
#include "../treenode/treenode.hxx"
#include "../andnode/andnode.hxx"
#include "../ornode/ornode.hxx"
#include "../featurenode/featurenode.hxx"
#include "../helperfunctions/helperfunctions.hxx"
#include "../ontologydata/ontologydata.hxx"
#include "../machinelearningnode/machinelearningnode.hxx"

#define  DBTABLE className + "_" + crNode->getFeatureType() + "_to_" + crNode->getRelativeClassName()

class OntologyClass;

typedef std::shared_ptr<OntologyClass> OntologyClassPtr;

typedef std::map<std::string, OntologyClassPtr> ClassMapType;

typedef std::shared_ptr<ClassMapType> ClassMapTypePtr;

class OntologyData;

typedef std::shared_ptr<OntologyData> OntologyDataPtr;

class OntologyClass {

    int classificationIterration; //number of classification loop
    std::string className;     //name of the class
    int classTreeDepth;
    bool hasCycleDepedency;
    bool isEnabled;            //true if the class is going to be classified
    bool isEstimated;          //variable containing information about the estimation status of the class
    LogicNodePtr rootElement; // node where Class logical root element relies
    std::vector <std::string> ComplementClass; //contains the names of the related classes (either inverse or from a class-related feature)
    std::vector <std::string> SubClass; //contains the names of the Subclasses of a class
    std::vector <std::string> SuperClass; //contains the names of the superclasses
    std::vector <std::string> MotherClass; //contains the names of the mother classes of the class (the current class has part-hole hierarchy with these classes)
    std::vector <std::string> UnionClass; //contains the names of the union classes of the class (classes that are connected with logical or)
    std::vector <std::string> ontoIndividuals; //contains the individuals belonging to the class, as it is defined by the ontology
    std::map <int, double>  classMembershipValue, oldClassMembershipValue; //the membership values for all segments for the current class
    std::vector <int> toClassify;
    //static private members
    static OntologyDataPtr ontoData;

    //private functions
    void computeMachineLearningClassification(LogicNodePtr); //computes a table containing the results of the machine learning classification
    void setComputed(); //turning off further computations for this class
    void setDisable(); //disables the class from the classification. used mainly if the class has subclasses
    void setEnable();
    void computeCRFeatures( LogicNodePtr ); //checks the root element for any cr-features and computes the relative tables
    bool getCycleDepedency();

    void crFeatureCalculation( ClassRelatedFeatureNodePtr );  //calculates the spatial features
    void classifyClass(ClassMapTypePtr);  //fires up which class must be classified



    /*
    //static private functions
    static void dropCRTables();
    static void setPath(); //setting working path
    */

public:

    void addIndividual(std::string);
    void estimateMembership();

    OntologyClass();
    OntologyClass(std::string);
    ~OntologyClass();
    void increaseClassTreeDepth();
    void setLogicNodeType(int);
    void setCycleDepedency();
    bool getComputed(); //returns TRUE if class is estimated
    double getClassMembershipValueForElement(int);
    int getClassDepth();
    LogicNodePtr getRootElement();
    bool getStatus();
    void addToClassVector(std::string, std::string);
    std::vector <std::string>* getClassVector( std::string );
    std::string * getClassName();
    int getIndividualID(int);

    static  void setOntologyData(OntologyDataPtr);


    //static public functions

   static OntologyClassPtr getClass(std::string*);
};



#endif // OntologyClass_H_INCLUDED
