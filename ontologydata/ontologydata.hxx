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

#ifndef ONTOLOGYDATA_H
#define ONTOLOGYDATA_H
#include <vector>
#include <pqxx/pqxx>
#include <boost/shared_array.hpp>
#include "../fuzzydatatype/fuzzydatatype.hxx"
#include "../machinelearning/machinelearningdatatype.hxx"
#include "../fuzzyobjectproperty/fuzzyobjectproperty.hxx"

class OntologyClass;

typedef std::shared_ptr<OntologyClass> OntologyClassPtr;

typedef std::map<std::string, OntologyClassPtr> ClassMapType;

typedef std::shared_ptr<ClassMapType> ClassMapTypePtr;


class SuperClassNode;

typedef std::shared_ptr<SuperClassNode> SuperClassNodePtr;

typedef  std::map <std::string, SuperClassNodePtr> SuperClassNodeMapType;

typedef  std::shared_ptr<SuperClassNodeMapType> SuperClassNodeMapTypePtr;

class OntologyParser;
class OntologyContainer;
class FuzzyEvaluator;

class OntologyData {

    DataTypeMapTypePtr fuzzyDatatypeMap; //maping fuzzy datatypes with their name
    ObjectTypeMapTypePtr fuzzyObjectTypeMap; //maping object types with their name
    SuperClassNodeMapTypePtr superClassNodeMap; //maping superclass nodes based on superclass name
    MachineLearningDatatypeMapTypePtr machineLearningDatatypeMap; //maping machine learning nodes based on their name

    std::vector<std::string> selectVec;
    std::vector<std::string> objectPropertyVector; //the names of all the class related properties used in the ontology
    std::string ontologyFile;     //ontology file
    ClassMapTypePtr classMap;
    int *segNumber;
    std::string  connectionParameter,  gidColumn, geomColumn, neighborTblName, neighborTblid, neighborTblidNeigh, ontologyIRI, selectStr, schema,  tableName;
    pqxx::connection *Conn;
    double *maxMembership;
    std::map<int, int> curIDMap;
    std::vector<std::string> *classRelatedDBTable;
    std::map<int, std::string> classificationLabel;
public:
    OntologyData();
    ~OntologyData();
    friend OntologyClass;
    friend OntologyParser;
    friend OntologyContainer;
    friend FuzzyEvaluator;
};

typedef std::shared_ptr<OntologyData> OntologyDataPtr;

#endif // ONTOLOGYDATA_H
