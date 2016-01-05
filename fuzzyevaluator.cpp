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

#include "fuzzyevaluator.h"

FuzzyEvaluator::FuzzyEvaluator(OntologyDataPtr data): ontoData(data){}

void FuzzyEvaluator::bestClassificationResult() {
    //connecting to the database
    pqxx::connection Conn( *ontoData->connectionParameter);
    //creating a new work
    pqxx::work Xaction(Conn,"Best Classification result");

    cout<<"Computing Best Classification Result\n\n";

    string query;
    string bClasResName;
    vector <string> classData;

    //droping best classification table
    bClasResName  = "bestclassificationresult_" + *ontoData->tableName;
    query = "drop table if exists " + bClasResName + "  ;";
    Xaction.exec(query);
    query = "create table IF NOT EXISTS " + bClasResName +  "(" + *ontoData->gidColumn + " int, bestclassificationresult double precision);";
    Xaction.exec(query);
    vector<string> columnNames;
    columnNames.push_back(*ontoData->gidColumn);
    columnNames.push_back("bestclassificationresult");

    pqxx::tablewriter write(Xaction, bClasResName, columnNames.begin(), columnNames.end() );
    for (int curSegment = 0; curSegment  < *ontoData->segNumber; curSegment ++ ) {
        classData.push_back( pqxx::to_string( TreeNode::getSegments()[curSegment][0]  ) );
        classData.push_back(pqxx::to_string(ontoData->maxMembership[ curSegment ] ) );
        write.push_back(classData);
        classData.clear();
    }
    write.complete();
    Xaction.commit();
}


void FuzzyEvaluator::classificationStability() {
    cout << "Computing Classification Stability\n";
    pqxx::connection Conn( *ontoData->connectionParameter );
    //crete new work
    pqxx::work Xaction(Conn,"Computing Classification Stability");
    string bClasStabName  = "classificationstability_" + *ontoData->tableName;
    string query = "drop table if exists " + bClasStabName + "  ;";
    Xaction.exec(query);
    query = "create table IF NOT EXISTS " + bClasStabName +  "(" + *ontoData->gidColumn + " int, classificationstability double precision);";
   Xaction.exec(query);

    vector<string> columnNames;
    columnNames.push_back(*ontoData->gidColumn);
    columnNames.push_back("classificationstability");

    pqxx::tablewriter write(Xaction, bClasStabName, columnNames.begin(), columnNames.end() );
    vector <string> classData(2);

    map<string, OntologyClassPtr>::iterator it;
    vector<string> topClass;
    for ( it = ontoData->classMap->begin(); it != ontoData->classMap->end(); it++  )
        if( it->second->getClassDepth() == 1  )
            topClass.push_back(it->first);

    for (map<int, int>::iterator itm = ontoData->curIDMap.begin(); itm!= ontoData->curIDMap.end(); itm++  ) {

        int curId  = itm->first;
        map<string, double> *objectMem;
        objectMem = new map<string, double>;

        for (register unsigned  int i = 0; i < topClass.size(); i++)
            addSubClassMembs(curId, topClass[i], objectMem);


        classData[0] = pqxx::to_string(curId);
        if ( objectMem->size() ==1 )  {
            map<string, double>::iterator it;
            it = objectMem->begin();
            classData[1] = pqxx::to_string(it->second);
        }
        else {
            vector<double> tmp;
            for ( map<string, double>::iterator it = objectMem->begin(); it != objectMem->end(); it++ )
                tmp.push_back(it->second);
             tmp.push_back(0.00001);
            std::sort(tmp.begin(), tmp.end());

            classData[1] = pqxx::to_string(tmp[tmp.size()-1] - tmp[tmp.size()-2]);
        }

        write.push_back(classData);

    }
   write.complete();
  Xaction.commit();
}

void FuzzyEvaluator::addSubClassMembs(int curId, string cName, map<string, double> *data) {

    if ( (*ontoData->classMap)[cName ]->getClassMembershipValueForElement(curId ) >=0.5  ) {
        //checking if the parent classes of the class already exists in the data
        vector<string> *parVec = (*ontoData->classMap)[ cName  ]->getClassVector( (string) "MotherClass");
        //erasing all mother classes from map
        for (register unsigned  int i = 0; i < parVec->size(); i++ ) {
            map <string, double>::iterator it = data->find(parVec->at(i) );
            //deleting from map
            if (it != data->end())
                data->erase(it);
        }
        //inserting the normal information
        data->insert( pair<string, double>(cName , (*ontoData->classMap)[cName ]->getClassMembershipValueForElement(curId ) ) );

        vector<string> *exVec = (*ontoData->classMap)[ cName  ]->getClassVector( (string) "SubClass");
        if (!exVec->empty() ) {
            for (register unsigned int j = 0; j < exVec->size(); j++)
                addSubClassMembs( curId, exVec->at(j), data );
        }
    }
}



