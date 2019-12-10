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

#ifndef ONTOLOGYCONTAINER_H
#define ONTOLOGYCONTAINER_H

#include <boost/shared_array.hpp>
#include <map>
#include <pqxx/pqxx>
#include <sys/param.h>  // MAXPATHLEN definition
#include <unistd.h>  // getcwd()
#include <vector>

#include "../superclassnode/superclassnode.hxx"
#include "../fuzzydatatype/fuzzydatatype.hxx"
#include "../fuzzyobjectproperty/fuzzyobjectproperty.hxx"
#include "../ontologyclass/ontologyclass.hxx"
#include "../ontologydata/ontologydata.hxx"

class OntologyContainer {

    OntologyDataPtr ontoData;

    void classify();
    void classifyClass( OntologyClassPtr );
    void dropCRTables();
    void establishConnection( bool, bool );
    void generateTopology( bool, bool );
    void labelSegment();
    void setPath();

public:
    void computeClassification(bool, bool);
    OntologyClassPtr getClass(std::string);
    std::string getGidColumn();
    std::string* getOntologyIRI();
    OntologyContainer();
    OntologyContainer( OntologyDataPtr, bool, bool );
    ~OntologyContainer();
    void setConnectionParameters(std::string& con, std::string& tbl);

};

typedef std::shared_ptr<OntologyContainer> OntologyContainerPtr;
#endif // ONTOLOGYCONTAINER_H
