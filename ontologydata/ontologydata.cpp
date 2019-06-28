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

#include "ontologydata.hxx"
#include <iostream>

using namespace std;

OntologyData::OntologyData() {
    Conn = nullptr;

    classMap =  ClassMapTypePtr ( new ClassMapType );
    classRelatedDBTable = new vector<string>;
    //classificationLabel = new string;
    connectionParameter = new string;
    fuzzyDatatypeMap = DataTypeMapTypePtr( new DatatypeMapType ); //maping datatypes with their name
    fuzzyObjectTypeMap = ObjectTypeMapTypePtr  ( new ObjectTypeMapType ); //maping object types with their name
    machineLearningDatatypeMap = MachineLearningDatatypeMapTypePtr (new MachineLearningDatatypeMapType);
     gidColumn = new string;
     geomColumn  = new string;
     neighborTblName  = new string;
     neighborTblid  = new string;
     neighborTblidNeigh = new string;

     objectPropertyVector = new vector<string> ;
     segNumber = new int;
     selectStr = new string;
     superClassNodeMap = SuperClassNodeMapTypePtr ( new SuperClassNodeMapType ); //maping superclass nodes based on superclass name
     tableName  = new string;


}

OntologyData::~OntologyData() {
    //delete[] classificationLabel;
    delete gidColumn;
    delete geomColumn;
    delete neighborTblid;
    delete neighborTblName;
    delete neighborTblidNeigh;
    delete[] maxMembership;
    delete segNumber;
    delete selectStr;
}
