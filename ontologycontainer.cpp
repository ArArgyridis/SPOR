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
 
#include "ontologycontainer.h"

using namespace std;
using namespace pqxx;

void OntologyContainer::classify() {

    //creating maps containing the maximum membership value and the classification label for each segment. As index the name of the level is used
    //ontoData->classificationLabel  = new string [ *ontoData->segNumber ];
    ontoData->maxMembership = new double [ *ontoData->segNumber ];
    memset( ontoData->maxMembership, 0, *ontoData->segNumber );

    OntologyClass::setOntologyData(ontoData);

    ClassMapType::iterator classMapIt;
    for ( classMapIt  = ontoData->classMap->begin(); classMapIt   != ontoData->classMap->end(); classMapIt++ ) {
        classifyClass( classMapIt->second );
    }

}

void OntologyContainer::classifyClass( OntologyClassPtr curClass ) {
    if (curClass->getComputed() == false) {
        for (register int i = 0; i <(int) curClass->getClassVector( (string) "SuperClass")->size() ; i++ ) {
            if (  !( *ontoData->classMap )[ curClass->getClassVector( (string) "SuperClass")->at(i) ] -> getComputed() ) {

                classifyClass( ( *ontoData->classMap ) [ curClass->getClassVector( (string) "SuperClass" )->at(i) ] );
            }
        }

        for (register int i = 0; i < (int) curClass->getClassVector( (string) "ComplementClass")->size(); i++ ) {
            if (  !( *ontoData->classMap )[ curClass->getClassVector( (string) "ComplementClass")->at(i) ] -> getComputed() ) {
                if (  (*curClass->getClassName() ) != ( * (( *ontoData->classMap )[ curClass->getClassVector( (string) "ComplementClass")->at(i) ] ->getClassName()) ) )
                   classifyClass( ( *ontoData->classMap ) [ curClass->getClassVector( (string) "ComplementClass")->at(i) ] );
                else{
                    //connecting to the database
                    //crete new work
                    work Xaction(*ontoData->Conn, "topology connection");
                    Xaction.exec("drop table if exists " + *curClass->getClassName() + " ;" );
                    Xaction.commit();
                    curClass->estimateMembership();
                }
            }
        }

        if ( (curClass->getStatus()) && (!curClass->getComputed()) )     { //if the class is enabled for classification
            curClass->estimateMembership();
        }
    }
}

void OntologyContainer::computeClassification(bool computeTopology, bool recomputeTopology) {
    establishConnection( computeTopology, recomputeTopology );
    //OntologyClass::setClassificationParameters(param);
    classify();
    labelSegment();
}

void OntologyContainer::dropCRTables() {

    work Xaction(*ontoData->Conn,"topology ontoData->Connection");
    for (register int i = 0; i <(int) ontoData->classRelatedDBTable->size(); i++) {
        string query = "drop table if exists "+ontoData->classRelatedDBTable->at(i);
        Xaction.exec(query);
    }
    Xaction.commit();
}

void OntologyContainer::establishConnection(bool computeTopology, bool recomputeTopology) {
    //connecting to the database
    pqxx::connection Conn( *ontoData->connectionParameter );
    //creating a new work
    work Xaction(Conn,"retrieving segments and computing topology");
    //retrieving primary key column name
    string query = "SELECT  pg_attribute.attname FROM pg_index, pg_class, pg_attribute WHERE   pg_class.relname = '" +  *ontoData->tableName + "' AND  indrelid = pg_class.oid AND   pg_attribute.attrelid = pg_class.oid AND pg_attribute.attnum = any(pg_index.indkey)   AND indisprimary;";
    pqxx::result res = Xaction.exec(query);
    *ontoData->gidColumn = (string)to_string( res[0][0] );
    //retrieving geometry column name
    query = "select f_geometry_column from geometry_columns where type='POLYGON' and f_table_name='"  + *ontoData->tableName + "';";
    res = Xaction.exec(query);
    *ontoData->geomColumn = (string)to_string( res[0][0]);

    //retrieving segments for the respective
    query = "select " + *ontoData->gidColumn  +  *ontoData->selectStr +  " from "+  *ontoData->tableName +" order by " + *ontoData->gidColumn +  " ;";
    result tempSegments = Xaction.exec(query);
    TreeNode::addSegments( tempSegments );
    *ontoData->segNumber = ( int )tempSegments.size();
    Xaction.commit();

    cout << "Total number of elements in the database : \t" << *ontoData->segNumber << endl;

    generateTopology( computeTopology, recomputeTopology );
    //setting working path
    setPath();

}

void OntologyContainer::generateTopology(bool computeTopology, bool recomputeTopology) {

    cout <<"\nGenerating topology (this could take some time)...\n";

    //establishing multi level topology here.
    if (computeTopology) {
        cout <<"Generating per level topology\n";

        //connecting to the database
        pqxx::connection Conn(*ontoData->connectionParameter);
        //crete new work
        work Xaction(Conn,"topology connection");

        cout<<"Computing topology for "<< *ontoData->tableName << endl;
        *ontoData->neighborTblName = "neighbors" +  (*ontoData->tableName);
        //checking if the neighbors table exists in the database
        string query = "select  tablename from pg_tables where schemaname='public' and  tablename= '" + *ontoData->neighborTblName  + "';";

        result res = Xaction.exec(query);

        if (res.empty()) { // if not topology computation takes place
            string *neighborTblid = ontoData->gidColumn;
            string *neighborTblidNeigh = new string( *ontoData->gidColumn + "neigh" );
            query = "select k1." + *ontoData->gidColumn + " as " + *neighborTblid +  ", k2. " + *ontoData->gidColumn + " as " + *neighborTblidNeigh + ", st_length(st_intersection( k1. "+ *ontoData->geomColumn +", k2." + *ontoData->geomColumn +") ) length "
                    "into  " + *ontoData->neighborTblName + " from " + *ontoData->tableName  + " k1 inner join " + *ontoData->tableName  + " k2 on k1.level=k2.level  and k1.geom && k2.geom and k1.id !=k2.id order by k1.id; ";
            Xaction.exec(query);
            query = "CREATE INDEX neighbors"+ *ontoData->tableName+"_neighbor_idx ON " + *ontoData->neighborTblName +"("+*ontoData->gidColumn + ")";
            Xaction.exec(query);
        }

        //checking if the centroid table exists in the database
        query = "select  tablename from pg_tables where schemaname='public' and  tablename='centroid" +  *ontoData->tableName + "';";
        res = Xaction.exec(query);

        if (res.empty()) { // if not centroid computation takes place
            query = "select "+*ontoData->gidColumn + ", st_centroid(" + *ontoData->geomColumn+ ") as centroid into centroid" +  *ontoData->tableName + " from " + *ontoData->tableName + ";";
            Xaction.exec(query);
        }

        cout <<"\nComputing Object-Subobject hierarchical structure\n";
        query = "select  tablename from pg_tables where schemaname='public' and  tablename='objecthierarchy" +  *ontoData->tableName + "';";
        res = Xaction.exec(query);
        if(res.empty()){
            query = " select o1."+ *ontoData->gidColumn + ", o2."+ *ontoData->gidColumn + " as "+ *ontoData->gidColumn + "contained, o1.level-o2.level as distance, st_area(o2."+ *ontoData->geomColumn +")/st_area(o1."+ *ontoData->geomColumn +") as relative_area_" + *ontoData->gidColumn + "contained_"+ *ontoData->gidColumn + "  into objecthierarchy" + *ontoData->tableName +  " from "+ *ontoData->tableName + " o1, " + *ontoData->tableName +  " o2 where  o1.level>o2.level and o1."+ *ontoData->geomColumn + " && o2."+ *ontoData->geomColumn + " and st_contains (o1."+ *ontoData->geomColumn + ",o2."+ *ontoData->geomColumn + ")=true;";
            Xaction.exec(query);
        }

        else { //otherwise topology computation occurs only if the topology is meant to be recomputed
            if (recomputeTopology == true) { //NEEEDS TO BE REPLACEEEED!!!!!!
                query = "drop table if exists neighbors"+ *ontoData->neighborTblName;
                Xaction.exec(query);

                query = "select k1."+*ontoData->gidColumn + ", k2."+*ontoData->gidColumn + " as "+*ontoData->gidColumn + "neigh, st_length((st_intersection(k1."+ *ontoData->geomColumn + ", k2." + *ontoData->geomColumn + ")))/st_length(st_boundary(k1." + *ontoData->geomColumn + ") ) as relative_border_" + *ontoData->gidColumn + "_" + *ontoData->gidColumn + "neigh  into  neighbors" + *ontoData->tableName +" from "+ *ontoData->tableName+" k1, "+ *ontoData->tableName+" k2 where k1."+ *ontoData->geomColumn + " && k2." + *ontoData->geomColumn+ " and k1 !=k2 and k1.level=k2.level order by k1." + *ontoData->gidColumn + ";";
                Xaction.exec(query);
                query = "CREATE INDEX neighbors"+ *ontoData->tableName+"_neighbor_idx ON neighbors"+ *ontoData->tableName+"("+*ontoData->gidColumn + ")";
                Xaction.exec(query);
                query = "drop table if exists centroid"+ *ontoData->tableName;
                Xaction.exec(query);
                query = "select "+*ontoData->gidColumn + ", st_centroid(" + *ontoData->geomColumn+ ") as centroid into centroid" + *ontoData->tableName + " from " +  *ontoData->tableName + ";";
                Xaction.exec(query);
            }
        }

        cout <<"\n";

        Xaction.commit();
    }
}

OntologyContainer::OntologyContainer(){}

OntologyContainer::OntologyContainer( OntologyDataPtr data, bool computeTopology, bool recomputeTopology ): ontoData( data )   {
    establishConnection( computeTopology, recomputeTopology );
    classify();
    labelSegment();
}

OntologyContainer::~OntologyContainer() {
    dropCRTables();
}

void OntologyContainer::labelSegment() {
    //pqxx::ontoData->Connection ontoData->Conn(ontoData->ConnectionontoDataeter);
    work Xaction (*ontoData->Conn,"inserting labels into database");

    //droping classification table if exists
    string updateQuery="drop table if exists  final_classification_" + *ontoData->tableName + ";";
    Xaction.exec(updateQuery);

    updateQuery="create table IF NOT EXISTS final_classification_" + *ontoData->tableName + "("+*ontoData->gidColumn + " int, class_name text);";
    Xaction.exec(updateQuery);
    vector<string> columnNames;
    columnNames.push_back(""+*ontoData->gidColumn + "");
    columnNames.push_back("class_name");
    tablewriter write(Xaction, "final_classification_"  + *ontoData->tableName, columnNames.begin(), columnNames.end() );
    vector<string>classData;

    for (register int j = 0; j < *ontoData->segNumber; j++) {
        classData.push_back(to_string( TreeNode::getSegments() [j][0]) );
        classData.push_back(to_string(ontoData->classificationLabel[  TreeNode::getSegments() [j][0].as<int>()    ]  ));
        write.push_back(classData);
        classData.clear();
    }

    write.complete();
    Xaction.commit();
}


void OntologyContainer::setConnectionParameters(string *con, string *tbl){
    ontoData->connectionParameter = con;
    ontoData->tableName = tbl;
}

//getting currend working path
void OntologyContainer::setPath() {
    char *k = NULL;
    k = getcwd( k, MAXPATHLEN );
    string path = string( k );
}

