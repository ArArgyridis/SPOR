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
 
#include "ontologyclass.h"
#include<cmath>
#include <omp.h>

using namespace std;
using namespace pqxx;


OntologyDataPtr OntologyClass::ontoData;

void OntologyClass::addIndividual(string id) {
    ontoIndividuals.push_back(id);

}

double OntologyClass::getClassMembershipValueForElement(int i) {
    return classMembershipValue[i];
}

int OntologyClass::getIndividualID(int pos) {
    return ontoData->curIDMap[pos];
}



void OntologyClass::estimateMembership() {
    cout <<"Classifying "+this->className << " ";
    TreeNode::setCurClass(this->className);

    vector< vector<int> > keysForClassification;
    if (MotherClass.size() > 0) {
        for (register unsigned int i = 0; i < MotherClass.size(); i++) {
            //checking if the class depth is greater than any of the mother classes
            if(classTreeDepth < (*ontoData->classMap)[MotherClass.at(i)]->classTreeDepth  ) {
                classTreeDepth = (*ontoData->classMap)[MotherClass.at(i)]->classTreeDepth;
            }
            int cKeyPos = 0;
            vector<int> classKey ( (*ontoData->classMap)[MotherClass.at(i)]->classMembershipValue.size()  );
            for (map<int, double>::iterator it =  (*ontoData->classMap)[ MotherClass.at(i)]->classMembershipValue.begin();  it !=  (*ontoData->classMap)[MotherClass.at(i)]->classMembershipValue.end(); it++  ) {
                classKey[cKeyPos] = (it->first);
                cKeyPos++;
            }
            sort(classKey.begin(), classKey.end());
            keysForClassification.push_back(classKey);
        }
        toClassify = keysForClassification.at(0);
        for (register unsigned int i = 1; i <keysForClassification.size() -1;  i++ ) {
            toClassify =  getCommonElements( toClassify, keysForClassification.at(i+1) );
        }
    }
    else {
        for ( int i = 0; i < *ontoData->segNumber; i++   ) {
            ontoData->curIDMap[ TreeNode::getSegments()[i][0].as<int>() ] = i;
            toClassify.push_back( TreeNode::getSegments()[i][0].as<int>()  );
        }
    }

    cout << "Depth = " << classTreeDepth << endl;
    //Connecting to the database
    pqxx::connection Conn( *ontoData->connectionParameter );
    //crete new work
    work Xaction(Conn,"classification");
    Xaction.exec("CREATE TABLE IF NOT EXISTS "+this->className+" ("+*ontoData->gidColumn + " int,"+this->className+"_membership double precision, CONSTRAINT " + this->className + "_pk PRIMARY KEY ("+*ontoData->gidColumn + "));");



    Xaction.commit();

    //computing cr features.
    computeCRFeatures( this->rootElement );

    bool membershipValueChange = false;
    vector <string> columnNames;
    columnNames.push_back( *ontoData->gidColumn );
    columnNames.push_back(this->className+"_membership");
    work update( Conn, "Update" );
    update.exec("DROP TABLE IF EXISTS "+this->className+" ;");
    vector<string> classData (2);
    //re-creating the table
    update.exec("CREATE TABLE IF NOT EXISTS "+this->className+" ("+*ontoData->gidColumn + " int,"+this->className+"_membership double precision DEFAULT 0, CONSTRAINT " + this->className + "_pk PRIMARY KEY ("+*ontoData->gidColumn + "));");

    unsigned int reg;
    vector<int>::iterator it;
    tablewriter write(update, this->className, columnNames.begin(), columnNames.end() );
    double membershipValue = 1;


    for (  it = toClassify.begin(); it != toClassify.end(); it++ ) {
        rootElement->valueEstimation( ontoData->curIDMap[*it] );
        for ( reg = 0; reg < MotherClass.size(); reg++ ) {
            membershipValue = min ( (*ontoData->classMap)[MotherClass.at(reg)]->getClassMembershipValueForElement( *it), membershipValue);
        }

        if ( ( membershipValue >= 0.5 )  && ( getStatus() ) ) {
            membershipValue = min (rootElement->getMembershipValue(), membershipValue);

            if (membershipValue >= 0.1) {
                classData.at(0) =  ( to_string(*it)  );
                classData.at(1) = ( to_string(pqxx::to_string(membershipValue) ) );
                write << classData;

            }
            if (membershipValue >= 0.5) {
                ontoData->classificationLabel[ *it ] = this->className;
                ontoData->maxMembership[ ontoData->curIDMap[*it]  ] = membershipValue;
            }

            classMembershipValue[*it] = membershipValue;
        }

        if  ( ( getCycleDepedency() ) && abs ( oldClassMembershipValue[*it] - membershipValue) > 0.00001 )
            membershipValueChange = true;
        //re-initializing the membership value
        membershipValue = 1;
    }

    write.complete();
    update.commit();
    this->setComputed();

    if ((classificationIterration < 20) && (membershipValueChange==true) ) {
        oldClassMembershipValue.clear();
        oldClassMembershipValue.insert( classMembershipValue.begin(), classMembershipValue.end() );
        classificationIterration++;
        estimateMembership();
    }
    cout <<"Classification of "+this->className+" finished!!\n\n";
}




OntologyClass::OntologyClass(string name):classificationIterration(0), className(name), classTreeDepth(0), hasCycleDepedency(false), isEnabled(true), isEstimated(false) {
    rootElement = AndNodePtr(new AndNode());
}

void OntologyClass::setEnable() {
    isEnabled = true;
}

void OntologyClass::setCycleDepedency() {
    hasCycleDepedency = true;
}

bool OntologyClass::getCycleDepedency() {
    return hasCycleDepedency;
}

bool OntologyClass::getStatus() {
    return isEnabled;
}

bool OntologyClass::getComputed() {
    return isEstimated;
}

void OntologyClass::setComputed() {
    isEstimated = true;
}

void OntologyClass::computeCRFeatures( LogicNodePtr element ) {

    for (register int i = 0; i  <element->getNodeCount(); i++ ) {
        TreeNodePtr tempElement = element->getSubNode(i);
        if ( tempElement->getNodeType()  == CRFEATURE ) {
            /*
            if ( (boost::dynamic_pointer_cast<ClassRelatedFeatureNode>(tempElement)->getRelativeClassName() == this->className ) and (classificationIterration > 0) ) {
                crFeatureCalculation( boost::static_pointer_cast<ClassRelatedFeatureNode>( tempElement ) );
            }
            else
                       */
                crFeatureCalculation( boost::static_pointer_cast<ClassRelatedFeatureNode>(tempElement) );


        }
        else{
            if ( ( tempElement->getNodeType() == AND ) || ( tempElement->getNodeType() == OR ) ||  ( tempElement->getNodeType() == COMPLEMENT )   )
                computeCRFeatures( boost::static_pointer_cast<LogicNode> ( tempElement ) );
        }
    }
}

void OntologyClass::crFeatureCalculation( ClassRelatedFeatureNodePtr crNode ) {
    string query;
    crNode->resetPosition();
    string tempstring = DBTABLE;
    transform(tempstring.begin(), tempstring.end(), tempstring.begin(), ::tolower);
    //Connecting to the database
    //crete new work
    connection Conn( *ontoData->connectionParameter );
    work Xaction(Conn );

    //checking if there is a cycle depedency and drops the relative table

    if ( ( crNode->getRelativeClassName() ==this->className ) ) {
        Xaction.exec("drop table if exists " + tempstring);
    }

    query = "select  tablename from pg_tables where schemaname='public' and  tablename ='" + tempstring + "' ;";
    // check if table exists in the database

    transform(tempstring.begin(), tempstring.end(), tempstring.begin(), ::tolower);

    result res = Xaction.exec(query);
    if ( res.empty() ) {
       /*
        //checking if the relative class has been disabled from classification. If yes, then classification of this class must occur

        if (!(*ontoData->classMap)[crNode->getRelativeClassName()]->getComputed() ) {
            (*ontoData->classMap)[crNode->getRelativeClassName()]->estimateMembership();
        }
        */

        //creating an "in" statement for the spatial query
        stringstream ss;
        ss<< "(";
        for (vector<int>::iterator it = toClassify.begin(); it != toClassify.end(); it++) {
            ss << *it+1<< ",";
        }

        string inSt = ss.str().erase(ss.str().size()-1);

        inSt += ")";
        string fromStr=",", whereStr=" and ";
        for (register int i = 0; i < (int) MotherClass.size(); i++ ) {
            fromStr += MotherClass[i] + ",";
        }
        fromStr = fromStr.substr(0, fromStr.size()-1  );

        //checking the type of the feature and setting the proper query for the database
        if ( crNode->getFeatureType() =="relative_border" ) {
            for (register int i = 0; i < (int) MotherClass.size(); i++ )
                whereStr += MotherClass[i] + "." + *ontoData->gidColumn +  " = k1." +*ontoData->gidColumn +  " and " + MotherClass[i] + "_membership >= 0.5 and " ;
            whereStr = whereStr.substr(4, whereStr.size()-8);
            query = "with tmp1 as ( "
                    " select  k1." + *ontoData->gidColumn + ", st_length(st_boundary(k1." + *ontoData->geomColumn + ") ) length "
                    " from  "  + *ontoData->tableName + " k1 " + fromStr +
                    " where " + whereStr +
                    ") select a." +  *ontoData->gidColumn + ", sum (a.length/ tmp1.length) relative_border into " + tempstring +
                    " from " + *ontoData->neighborTblName + " a,  tmp1, " + crNode->getRelativeClassName() +
                    " where a." + *ontoData->gidColumn + " = tmp1." + *ontoData->gidColumn + " and a." + *ontoData->gidColumn +  "neigh=" + crNode->getRelativeClassName() +".id and " + crNode->getRelativeClassName() +"_membership >=0.5 "
                    " group by a." +  *ontoData->gidColumn ;
        }else if ( crNode->getFeatureType() =="border" ) {
            query = "select "+*ontoData->gidColumn + ", border  into "+tempstring+" from (select * from (select "+*ontoData->gidColumn + " from " +  *ontoData->tableName + ") as a left join (select n1."+*ontoData->gidColumn + " "+*ontoData->gidColumn + "d, sum(st_length((st_intersection(k1." + *ontoData->geomColumn + ",k2." + *ontoData->geomColumn + "))) )) as border from neighbors" +  *ontoData->tableName + " n1, " +  *ontoData->tableName + " k1, " +  *ontoData->tableName + " k2, "+crNode->getRelativeClassName()+" s1 where k1."+*ontoData->gidColumn + " = n1."+*ontoData->gidColumn + " and k2."+*ontoData->gidColumn + " = n1."+*ontoData->gidColumn + "neigh and s1."+crNode->getRelativeClassName()+"_membership>=0.5 and s1."+*ontoData->gidColumn + " = n1."+*ontoData->gidColumn + "neigh group by n1."+*ontoData->gidColumn + ") as b on a."+*ontoData->gidColumn + " = b."+*ontoData->gidColumn + "d  order by a."+*ontoData->gidColumn + ") as foo;";
        } else if ( crNode->getFeatureType() =="relative_area" ) {
            for (register int i = 0; i < (int) MotherClass.size(); i++ )
                whereStr += MotherClass[i] + "." + *ontoData->gidColumn +  " = a." +*ontoData->gidColumn +  " and ";
            whereStr = whereStr.substr(0, whereStr.size()-4);

            query = "select a."+*ontoData->gidColumn + ", sum(relative_area_"+*ontoData->gidColumn + "contained_"+*ontoData->gidColumn + ") as relative_area into " + tempstring + " from "+  *ontoData->tableName + " o1,  " + *ontoData->tableName + " o2,  objecthierarchy" +  *ontoData->tableName +  " a, "+crNode->getRelativeClassName()+"  c1 " + fromStr +
                    " where a."+*ontoData->gidColumn + "contained = c1."+*ontoData->gidColumn + " and "+crNode->getRelativeClassName()+"_membership >=0.5 and a."+*ontoData->gidColumn + "contained = o2."+*ontoData->gidColumn + " and a."+*ontoData->gidColumn + " = o1."+*ontoData->gidColumn + " " + whereStr + " group by a."+*ontoData->gidColumn + " order by a."+*ontoData->gidColumn;
        } else if (crNode->getFeatureType() =="not") {
            query = "select b."+*ontoData->gidColumn + ", " + crNode->getRelativeClassName()+"_membership as nott into " + tempstring +
                    " from "+crNode->getRelativeClassName()+ " a left join "  + *ontoData->tableName + " b on a." + *ontoData->gidColumn + " =b." + *ontoData->gidColumn + " order by " + *ontoData->gidColumn;
        } else if (crNode->getFeatureType() =="is_covered") {
            //query = "select distinct  c."+*ontoData->gidColumn + ", c.contained is_covered into "+tempstring+"  from (select * from (select "+*ontoData->gidColumn + " from " +  *ontoData->tableName + ") as a left join  (select o1."+*ontoData->gidColumn + "contained, 1 as contained  from objecthierarchy" + *ontoData->tableName + " o1, " + crNode->getRelativeClassName() + " c1 where c1."+*ontoData->gidColumn + " = o1."+*ontoData->gidColumn + " and c1."+crNode->getRelativeClassName()+"_membership >= 0.5) as b on   a."+*ontoData->gidColumn + " = b."+*ontoData->gidColumn + "contained) as c order by "+*ontoData->gidColumn + "";
            cout << fromStr << endl;
            for (register int i = 0; i < (int) MotherClass.size(); i++ )
                whereStr += MotherClass[i] + "." + *ontoData->gidColumn +  " = a." +*ontoData->gidColumn +  "contained and ";
            whereStr = whereStr.substr(0, whereStr.size()-4);

            query = "with tmp1 as ( "
                    " select " + *ontoData->gidColumn + " from " + crNode->getRelativeClassName() + " where " + crNode->getRelativeClassName() +  "_membership >= 0.5 ) " +
                    " select  a." + *ontoData->gidColumn + "contained " + *ontoData->gidColumn + ", 1 is_covered into " + tempstring +
                    " from objecthierarchy" + *ontoData->tableName +  "  a, tmp1  " + fromStr +
                    " where "  +
                    " tmp1.id = a.id " + whereStr;

        } else if (crNode->getFeatureType() =="distance_to") {
            query = "select a."+*ontoData->gidColumn + ", b.distance_to into  "+tempstring+" from (select "+*ontoData->gidColumn + " from "+  *ontoData->tableName +") as a left join (select l2."+*ontoData->gidColumn + ", min( st_distance(k1.centroid,k2.centroid)) as distance_to from " +  *ontoData->tableName + " l1, " +  *ontoData->tableName + " l2,  "+ crNode->getRelativeClassName()+" c1, "+ crNode->getRelativeClassName()+" c2, centroid" +  *ontoData->tableName + " k1, centroid" +  *ontoData->tableName + " k2 where l1."+*ontoData->gidColumn + " != l2."+*ontoData->gidColumn + " and c1."+ crNode->getRelativeClassName()+"_membership>=0.5 and l1."+*ontoData->gidColumn + " = c1."+*ontoData->gidColumn + " and l2."+*ontoData->gidColumn + " = c2."+*ontoData->gidColumn + " and c2."+ crNode->getRelativeClassName()+"_membership <0.5 and k1."+*ontoData->gidColumn + " = c1."+*ontoData->gidColumn + " and k2."+*ontoData->gidColumn + "  = c2."+*ontoData->gidColumn + " and l1.level = l2.level group by l2."+*ontoData->gidColumn + " order by l2."+*ontoData->gidColumn + ") as b on a."+*ontoData->gidColumn + " = b."+*ontoData->gidColumn + " order by a."+*ontoData->gidColumn + "";
        } else if ((crNode->getFeatureType() == "subobject_distance_to_exterior")) {
            query = " with tmp1 as ("
                    "select  a." + *ontoData->gidColumn + ", b.exteriorring "
                    "from " + crNode->getRelativeClassName() + " a  join centroid"+ *ontoData->tableName + " b "
                    "on a."+ *ontoData->gidColumn + " = b." + *ontoData->gidColumn + " "
                    "   and a." + crNode->getRelativeClassName() + "_membership >= 0.5 order by " + *ontoData->gidColumn + "), "
                    "tmp2 as("
                    "select " + *ontoData->gidColumn +     "contained, st_distance(centroid,tmp1.exteriorring) subobject_distance_to_exterior "
                    "from objecthierarchy" + *ontoData->tableName +  "  d join tmp1 "
                    "on d." + *ontoData->gidColumn + "  = tmp1." + *ontoData->gidColumn + " "
                    "join centroid" + *ontoData->tableName + " c on c." + *ontoData->gidColumn + " = d." + *ontoData->gidColumn + "contained ) "
                    "select id, subobject_distance_to_exterior into " + tempstring + " from " + *ontoData->tableName + " a left join tmp2 b "
                    "on a." + *ontoData->gidColumn + " = b." + *ontoData->gidColumn +  "contained and a." + *ontoData->gidColumn + "in " + inSt + "order by a."  + *ontoData->gidColumn  ;

        } else if ( crNode->getFeatureType() == "neighbor_distance_to_exterior" ) {
            for (register int i = 0; i < (int) MotherClass.size(); i++ )
                whereStr += MotherClass[i] + "." + *ontoData->gidColumn +  " = a." +*ontoData->gidColumn +  " and ";
            whereStr = whereStr.substr(0, whereStr.size()-4);
            query = " with tmp1 as ("
                    " select a." + *ontoData->gidColumn + ", st_exteriorring(b.geom) geom from " + crNode->getRelativeClassName() +  "  a join " + *ontoData->tableName + " b "
                    " on " + crNode->getRelativeClassName() + "_membership >= 0.5 "
                    "  and a." + *ontoData->gidColumn + " = b." + *ontoData->gidColumn + " ) , "
                    " tmp2 as ( "
                    " select distinct a." + *ontoData->gidColumn + ", a.centroid "
                    " from  centroid" + *ontoData->tableName + " a " + fromStr +
                    " where  a." + *ontoData->gidColumn + "  not in (select " + *ontoData->gidColumn + " from tmp1) " +
                     whereStr + ")" +
                    " select b." + *ontoData->gidColumn + ", min(st_distance(a." + *ontoData->geomColumn + ", b.centroid)) neighbor_distance_to_exterior into " + tempstring +
                    " from tmp1 a join tmp2 b on a." + *ontoData->gidColumn + " <> b. " + *ontoData->gidColumn +
                    " group by b." + *ontoData->gidColumn ;

        }
        else if  (crNode->getFeatureType() ==  "overlap" ) {
            query = "with tmp1 as ( "
                    " select a." + *ontoData->gidColumn + ", " + *ontoData->geomColumn +
                    " from " + *ontoData->tableName + " a "
                    " inner join " +  crNode->getRelativeClassName()  + "   b "
                    " on " + crNode->getRelativeClassName() +  "_membership >= 0.5 and a." + *ontoData->gidColumn + " = b." + *ontoData->gidColumn + "  )"
                    " , tmp2 as ( "
                    " select distinct a." + *ontoData->gidColumn + ", a." + *ontoData->geomColumn +
                    " from " + *ontoData->tableName + " a "
                    " left join " +  crNode->getRelativeClassName()  + "   b "
                    " on a." + *ontoData->gidColumn + " = b." + *ontoData->gidColumn + " where b. " + *ontoData->gidColumn +  " is null)"
                    ", tmp3 as ("
                    " select distinct t2." + *ontoData->gidColumn +  ", 1 overlap "
                    " from tmp1 t1 join tmp2 t2 "
                    " on t1." + *ontoData->geomColumn + " && t2." + *ontoData->geomColumn + " and st_overlaps(t1." + *ontoData->geomColumn + ", t2." + *ontoData->geomColumn + ") order by t2." + *ontoData->gidColumn +
                    ") select a." + *ontoData->gidColumn + ", b.overlap into " + tempstring  +
                    " from tmp2 a left join tmp3 b on a."+*ontoData->gidColumn + "=b." + *ontoData->gidColumn +
                    " order by a." + *ontoData->gidColumn;
        }

        Xaction.exec(query);
    }



    if (crNode->getFeatureType() !="not") {
        query = "update " + tempstring + " set " +  crNode->getFeatureType() + " = 0 WHERE " +  crNode->getFeatureType() + " IS NULL";
        Xaction.exec(query);
    }
    Xaction.commit();

    vector<string>::iterator it;
    it = find( ontoData->classRelatedDBTable->begin(), ontoData->classRelatedDBTable->end(), tempstring );
    if(it == ontoData->classRelatedDBTable->end()) {
        ontoData->classRelatedDBTable->push_back(tempstring); //pushing back the name of the table, so it can later be removed from the db
    }

    crNode->setCResult( ontoData->Conn, &tempstring, ontoData->gidColumn );

}



void OntologyClass::setDisable() {
    isEnabled = false;
}


OntologyClass::~OntologyClass() {

}

void OntologyClass::setLogicNodeType(int node) {
    if (node == AND)
        this->rootElement = AndNodePtr(new AndNode);
    else if (node ==OR)
        this->rootElement = OrNodePtr (new OrNode());
}


LogicNodePtr OntologyClass::getRootElement() {
    return this->rootElement;
}

int OntologyClass::getClassDepth() {
    return classTreeDepth;
}

void OntologyClass::increaseClassTreeDepth() {
    classTreeDepth++;
}

void OntologyClass::addToClassVector(string classType,  string className) {
    if (className != this->className) {
        if ( classType == "MotherClass" ) {
            std::vector<string>::iterator it = std::find( MotherClass.begin(), MotherClass.end(), className );
            if ( it == MotherClass.end() )
                MotherClass.push_back( className );
        }
        else if ( classType == "SuperClass" ) {
             std::vector<string>::iterator it = std::find( SuperClass.begin(), SuperClass.end(), className );
             if ( it == SuperClass.end() )
                 SuperClass.push_back( className );
        }
        else if ( classType == "ComplementClass" ) {
            std::vector<string>::iterator it = std::find( ComplementClass.begin(), ComplementClass.end(), className );
            if ( it == ComplementClass.end() )
                ComplementClass.push_back( className );
        }
        else if ( classType == "SubClass" ) {
            std::vector<string>::iterator it = std::find( SubClass.begin(), SubClass.end(), className );
            if ( it == SubClass.end() )
                SubClass.push_back( className );
        }
        else {
            cout << "ERROR: This class vector is not defined\n";
            exit(1);
        }
    }
}

vector<string>* OntologyClass::getClassVector( string className) {
    if ( className == "MotherClass" )
        return &MotherClass;
    else if ( className == "SuperClass" )
        return &SuperClass;
    else if ( className == "ComplementClass" )
        return &ComplementClass;
    else if ( className == "SubClass" )
        return &SubClass;
    else {
        cout << "ERROR: This class vector is not defined\n";
        exit(1);
    }
}

std::string * OntologyClass::getClassName() {
    return &className;
}

void OntologyClass::setOntologyData(OntologyDataPtr p) {
    ontoData = p;
}

OntologyClassPtr OntologyClass::getClass(std::string *cName) {
    return (*ontoData->classMap)[*cName];
}
