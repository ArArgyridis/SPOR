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


#include "ontologycontainer/ontologycontainer.hxx"
#include "ontologyclass/ontologyclass.hxx"
#include "ontologyparser/ontologyparser.hxx"
#include "fuzzyevaluator/fuzzyevaluator.hxx"
#include <ctime>

#include <iostream>
using namespace std;

void help();

int main ( int argc, const char* argv[] ) {
    //test_lr();

    //data required for the connection
    string cn, fileName, dbName="dbname=", hostName, userName, password, thingTable;
    //container to store information regarding the ontology
    OntologyParserPtr parser;
    OntologyDataPtr ontoData;
    OntologyContainerPtr ontoClassifier;
    FuzzyEvaluatorPtr eval;
    //time_t  tm0 = time(0);

    if (argc < 7) {
        cout <<"not enough parameters\n";
        help();
        return  1;
    }

    fileName = argv[1];
    dbName    += argv[2];
    hostName = argv[3];
    userName = argv[4];
    password  = argv[5];
    thingTable = argv[6];
    //estimateStability = true;
    cn = dbName +" host="+ hostName + " user="+ userName + " password="+ password;
    parser = OntologyParserPtr ( new OntologyParser( fileName, cn, thingTable ) );

    ontoData = parser->getOntologyData();
    ontoClassifier = OntologyContainerPtr ( new OntologyContainer ( ontoData, true, false ) );
    eval = FuzzyEvaluatorPtr(new FuzzyEvaluator ( ontoData ));
    eval->bestClassificationResult();
    eval->classificationStability();
    xmlCleanupMemory();

    //test_crbm();
    //test_cdbn();
    return 0;


}


void help() {
    cout<<"Parameter list: ontology_file database_name host_name user_name password thing_table\n";
}




