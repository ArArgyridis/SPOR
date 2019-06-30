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

#include "ontologyparser.hxx"

using namespace std;
using xmlUniquePtr = std::unique_ptr<xmlChar, decltype(xmlFree)>;


const string fuzzyOwlDatatypeProperties[] = {"type","a", "b", "c", "d"};

void OntologyParser::addOntologyHierarchy (xmlNodePtr ontoNode) {

    //cout <<ontoNode->name <<endl;
    if  ( xmlStrEqual (ontoNode->name, (xmlChar*)"Ontology") ) {
        ontoData->ontologyIRI = string( (char*)xmlGetProp(ontoNode,(xmlChar*)"about") );
        ontoData->ontologyIRI.append("#");
    }

    //Getting Fuzzy/ML Datatypes
    if (xmlStrEqual (ontoNode->name, (xmlChar*)"Datatype") ) {
        if( xmlHasProp(ontoNode,(xmlChar*)"about") ) {
            //cout <<xmlGetProp(ontoNode,(xmlChar*)"about") << endl;
            getOntologyFuzzyDatatype(ontoNode);
        }
    }

    //getting Class related features
    if (xmlStrEqual (ontoNode->name, (xmlChar*)"ObjectProperty") ) {
        if( xmlHasProp(ontoNode,(xmlChar*)"about") ) {
            getOntologyFuzzyObjectProperty(ontoNode);
        }
    }
    //Getting Class Properties
    if (xmlStrEqual (ontoNode->name, (xmlChar*)"Class") ) {
        if( xmlHasProp(ontoNode,(xmlChar*)"about") ) {
            AndNodePtr lNode;
            lNode = AndNodePtr(new AndNode());
            getOntologyClass(ontoNode, lNode);
        }
    }

    if (xmlStrEqual (ontoNode->name, (xmlChar*)"NamedIndividual") ) {
        getOntologyIndividual(ontoNode);
    }


    if ( (xmlNextElementSibling(ontoNode) !=nullptr) ) {
        addOntologyHierarchy(xmlNextElementSibling(ontoNode));
    }
}

string OntologyParser::changeTags(string *fileName) {
    // ifstream inFile("ontology_change_detection_v2.owl");
    ifstream inFile( fileName->c_str() );
    if ( inFile.is_open() ) {
        ofstream outFile;
        string outFileName =  "ontotemp.owl" ;
        outFile.open(outFileName.c_str());
        string fileLine;
        string changeElements[] = {"&lt;", "&quot;", "&gt;"};
        string replaceElements [] = { "<", "\"", ">"};
        size_t found;
        while (!inFile.eof() ) {
            inFile >> fileLine;
            for (register int i = 0; i < 3; i++) {
                found = fileLine.find(changeElements[i]);
                while ( found != string::npos) {
                    fileLine.replace(fileLine.find(changeElements[i]),changeElements[i].length(),replaceElements[i]);
                    found = fileLine.find(changeElements[i]);
                }
            }
            outFile << fileLine;
            outFile <<"\n";
            fileLine ="";
        }
        inFile.close();
        return outFileName;
    }
    else {
        cout << "Ontology file not found\n";
        exit(1);
    }
}

void OntologyParser::computeSelectStr() {
    for (register int i = 0; i < (int)ontoData->selectVec.size(); i++)
        selectStr += "," + ontoData->selectVec[i] ;

    //*selectStr = selectStr->substr(0, selectStr->size()-1);
    ontoData->selectStr = selectStr;
}

string cName, fName, pName, elCondition;
string dName="",  oProp="", funcName="", featureType="";
//bool classFeature = false;
map<string, ClassRelatedFeatureNodePtr>::iterator CRFeatureIt;
map<string, FuzzyObjectTypePtr >::iterator fuzzyObjectTypeMapIt;


map<string, OntologyClassPtr>::iterator classMapIt;
map<string, FuzzyDatatypePtr>::iterator fuzzyDatatypeMapIt;

map<string, MachineLearningDatatypePtr>::iterator mlDatatypeMapIt;

void OntologyParser::getOntologyClass(xmlNodePtr ontoNode, LogicNodePtr tempRoot) {
    //cout <<ontoNode->name << endl;
    if  (xmlStrEqual (ontoNode->name, (xmlChar*) "Class" )  )  {
        if (xmlHasProp(ontoNode,(xmlChar*)"about")) {
            cName = ontoPropValue(ontoNode,"about");
            classMapIt = ontoData->classMap->find(cName);
            //if the class has not been readed until now
            if (classMapIt == ontoData->classMap->end() ) {
                (*ontoData->classMap)[cName] = OntologyClassPtr(new OntologyClass(cName));
            }
            //adding current class to class map
            cout <<"class "+cName+" added!\n";
            tempRoot =(*ontoData->classMap)[cName]->getRootElement();
            getOntologyClass( xmlFirstElementChild(ontoNode), tempRoot );
        }
        else {

            //it will enter here on the <owl:Class> definitions within a class
            if (xmlFirstElementChild(ontoNode) != nullptr)
                getOntologyClass( xmlFirstElementChild(ontoNode), tempRoot );
            //advancing to the next sibling
            if (xmlNextElementSibling(ontoNode) !=nullptr)
                getOntologyClass( xmlNextElementSibling(ontoNode), tempRoot );

        }
    }
    else if  (xmlStrEqual (ontoNode->name, (xmlChar*) "equivalentClass" )  )  {
        getOntologyClass( xmlFirstElementChild(ontoNode), tempRoot );
    }
    else if (xmlStrEqual (ontoNode->name, (xmlChar*) "intersectionOf" )  ) {
        AndNodePtr tempNode = AndNodePtr( new AndNode() );
        (*ontoData->classMap)[cName] ->increaseClassTreeDepth();
        tempRoot->addSubNode(tempNode);
        getOntologyClass( xmlFirstElementChild(ontoNode), tempNode);

        if (xmlNextElementSibling(ontoNode) !=nullptr)
            getOntologyClass(xmlNextElementSibling(ontoNode) , tempRoot);

    }
    else if (xmlStrEqual (ontoNode->name, (xmlChar*) "Description" ) ) {
        dName = (char*)xmlGetProp(ontoNode,(xmlChar*)"about");
        //if the name of the superclass is not Thing
        if ( dName.substr( dName.size()-5, dName.size()) !="Thing" ) {
            dName.erase( dName.begin(), dName.begin() + ontoData->ontologyIRI.size());
            classMapIt = ontoData->classMap->find(dName);
            //if the class has not been met until now
            if (classMapIt == ontoData->classMap->end() )
                (*ontoData->classMap)[dName] = OntologyClassPtr(new OntologyClass(dName));


            (*ontoData->classMap)[cName] ->addToClassVector("SuperClass", dName);

            if  ( tempRoot->getAllParentAndNode() == 0) { //this is  a parent class
                (*ontoData->classMap)[cName] ->addToClassVector("MotherClass", dName);
                (*ontoData->classMap)[dName]->addToClassVector((string)"SubClass", cName);
            }

            if ( ontoData->superClassNodeMap->find(dName) == ontoData->superClassNodeMap->end() )
                (*ontoData->superClassNodeMap)[dName]  = SuperClassNodePtr ( new SuperClassNode (   (*ontoData->classMap)[dName]  ) );

            tempRoot->addSubNode( (*ontoData->superClassNodeMap)[dName] );
            /*
            else {
                //check if the superclass node exists
                if ( ontoData->superClassNodeMap->find(dName) == ontoData->superClassNodeMap->end() ) {
                    (*ontoData->superClassNodeMap)[dName]  = SuperClassNodePtr ( new SuperClassNode (   (*ontoData->classMap)[dName]  ) );
                }
                tempRoot->addSubNode( (*ontoData->superClassNodeMap)[dName] );
            }
            */

        }

        if (xmlNextElementSibling(ontoNode) != nullptr )
            getOntologyClass(xmlNextElementSibling(ontoNode), tempRoot);
    }
    else if (xmlStrEqual (ontoNode->name, (xmlChar*) "unionOf" ) ) {
        OrNodePtr tempNode = OrNodePtr(new OrNode);
        (*ontoData->classMap)[cName] ->increaseClassTreeDepth();
        tempRoot->addSubNode(tempNode);
        getOntologyClass(xmlFirstElementChild(ontoNode), tempNode);

        if (xmlNextElementSibling(ontoNode) !=nullptr){
            //cout <<  ontoPropValue(xmlNextElementSibling(ontoNode),"resource")<< endl;
            getOntologyClass(xmlNextElementSibling(ontoNode) , tempRoot);
        }

    }
    else if (xmlStrEqual (ontoNode->name, (xmlChar*) "Restriction" )) {

        getOntologyClass(xmlFirstElementChild(ontoNode), tempRoot);

        if (xmlNextElementSibling(ontoNode) != nullptr)
            getOntologyClass(xmlNextElementSibling(ontoNode), tempRoot) ;

    }
    else if (xmlStrEqual (ontoNode->name, (xmlChar*) "onProperty" )) {
        dName = ontoPropValue(ontoNode,"resource");
        //checking here if the current feature is a class-related one
        getOntologyClass(xmlNextElementSibling(ontoNode), tempRoot) ;
    }
    else if (xmlStrEqual (ontoNode->name, (xmlChar*) "someValuesFrom" ))   { //this is a restriction
        if (xmlHasProp(ontoNode,(xmlChar*)"resource") ) {
            fName = ontoPropValue(ontoNode,"resource");
            //std :: cout << "dName \t" <<dName <<"\n"; //checkings below must be changed. dName should be considered -- TODO
            if(    (*ontoData->fuzzyDatatypeMap).find( fName )  != (*ontoData->fuzzyDatatypeMap).end()  ) { // if it is a datatype property
                if( std::find(ontoData->selectVec.begin(), ontoData->selectVec.end(), dName) ==  ontoData->selectVec.end() )
                    ontoData->selectVec.push_back(dName);

                FeatureNodePtr tFtNode =  FeatureNodePtr (new FeatureNode( dName, cName, (*ontoData->fuzzyDatatypeMap)[fName] ) );
                tempRoot->addSubNode(tFtNode);
            }
            else if (   ( *ontoData->machineLearningDatatypeMap).find(fName ) !=  ( *ontoData->machineLearningDatatypeMap).end()   ) { // if it is a machine learning algorithm
                MachineLearningNodePtr mlNode = MachineLearningNodePtr (new MachineLearningNode (dName, ( *ontoData->machineLearningDatatypeMap)[fName]    )    );
                (*ontoData->machineLearningDatatypeMap)[fName]->addEmployedClass(cName);
                tempRoot->addSubNode(mlNode);
            }
            else {// it is an object property
                //checking if the class has been defined
                classMapIt = ontoData->classMap->find(fName);

                if (classMapIt == ontoData->classMap->end() )
                    (*ontoData->classMap)[fName] = OntologyClassPtr(new OntologyClass(fName));

                ClassRelatedFeatureNodePtr crNode = ClassRelatedFeatureNodePtr(new ClassRelatedFeatureNode(fName, (*ontoData->fuzzyObjectTypeMap)[dName])); //create the new feature
                tempRoot->addSubNode(crNode); //add it as subnode to the tempRoot ;)
                (*ontoData->classMap)[cName]->addToClassVector("ComplementClass", fName);
                if (cName == fName)
                    (*ontoData->classMap)[cName]->setCycleDepedency();
                fName = "";

            }
        }
        else {
            //if there is no resource on the SomeValuesFrom node, then a complex expression follows
            string tmpCName = genRandomString(10);
            (*ontoData->classMap)[tmpCName] = OntologyClassPtr(new OntologyClass(tmpCName));
            LogicNodePtr tmpRt = (*ontoData->classMap)[tmpCName]->getRootElement();
            string objectFeature = dName;

            getOntologyClass( xmlFirstElementChild(ontoNode), tmpRt );

            ClassRelatedFeatureNodePtr crNode = ClassRelatedFeatureNodePtr(new ClassRelatedFeatureNode(tmpCName, (*ontoData->fuzzyObjectTypeMap)[objectFeature])); //create the new feature
            tempRoot->addSubNode(crNode); //add it as subnode to the tempRoot ;)
            cout <<"object feature name: " <<objectFeature << endl;
            (*ontoData->classMap)[cName]->addToClassVector("ComplementClass", tmpCName);
        }
    }
    else if (xmlStrEqual (ontoNode->name, (xmlChar*) "complementOf" ) ) {
        ComplementNodePtr comNode = ComplementNodePtr (new ComplementNode() );
        if ( xmlHasProp(ontoNode,(xmlChar*)"resource")  ) {
            dName = ontoPropValue(ontoNode, "resource");
            classMapIt = ontoData->classMap->find(dName);

            /*
             if (classMapIt ==ontoData->classMap->end()) {
                 (*ontoData->classMap)[dName] = OntologyClassPtr( new OntologyClass(dName) );
                 SuperClassNodePtr sNode = SuperClassNodePtr ( new SuperClassNode (   (*ontoData->classMap)[dName]  ) );
                 (*ontoData->superClassNodeMap)[dName]  = sNode;
             } else if ( ontoData->superClassNodeMap->find(dName) == ontoData->superClassNodeMap->end() ) {
                 SuperClassNodePtr sNode = SuperClassNodePtr ( new SuperClassNode (   (*ontoData->classMap)[dName]  ) );
                 (*ontoData->superClassNodeMap)[dName]  = sNode;
             }
             */

            //if the class does not exist, create it
            if (classMapIt ==ontoData->classMap->end())
                (*ontoData->classMap)[dName] = OntologyClassPtr( new OntologyClass(dName) );
            //if the superclassnode does not exist, also create it
            if ( ontoData->superClassNodeMap->find(dName) == ontoData->superClassNodeMap->end() )
                (*ontoData->superClassNodeMap)[dName] = SuperClassNodePtr ( new SuperClassNode (   (*ontoData->classMap)[dName]  ) );


            comNode->addSubNode( (*ontoData->superClassNodeMap)[dName] );
            tempRoot->addSubNode( comNode );
            (*ontoData->classMap)[cName]->addToClassVector ("ComplementClass", dName);
        }
        else {
            tempRoot->addSubNode(comNode);
            getOntologyClass( xmlFirstElementChild(ontoNode), comNode );

        }
    }
}

OntologyDataPtr OntologyParser::getOntologyData() {
    return ontoData;
}


bool OntologyParser::getOntologyFuzzyDatatype(xmlNodePtr ontoNode) {
    bool added = false;

    if( xmlHasProp( ontoNode, ( xmlChar* )"about" ) ) {
        dName =(char*) xmlGetProp(ontoNode,(xmlChar*)"about");
        dName.erase(dName.begin(),dName.begin()+ontoData->ontologyIRI.size());
    }

    if( xmlStrEqual( ontoNode->name ,  (xmlChar*)"fuzzyLabel") ) {
        ontoNode = xmlFirstElementChild( xmlFirstElementChild( ( ontoNode) ) ) ;
        double *fInput = new double[4];
        if (xmlHasProp(ontoNode,(xmlChar*)"type" )) {
            for (register int i = 0; i < 5; i++) {
                if ( i == 0) {
                    funcName = (char*)xmlGetProp(ontoNode,(xmlChar*)fuzzyOwlDatatypeProperties[i].c_str());
                } else {
                    if (xmlHasProp(ontoNode,(xmlChar*)fuzzyOwlDatatypeProperties[i].c_str())) {
                        fInput[i-1] = atof( (char*) (xmlGetProp(ontoNode,(xmlChar*)fuzzyOwlDatatypeProperties[i].c_str())) );
                    }
                }
            }
        }
        (* ontoData->fuzzyDatatypeMap)[dName] = FuzzyDatatypePtr( new FuzzyDatatype (funcName, fInput) );
        dName = funcName ="";
        delete fInput;
        return true;
    }

    if( xmlStrEqual( ontoNode->name ,  (xmlChar*)"machineLearningLabel") ) {
        ontoNode = xmlFirstElementChild(ontoNode);
        xmlUniquePtr mhd (xmlGetProp(ontoNode, (xmlChar*)"type"), xmlFree);
        string method = reinterpret_cast<char*>(mhd.get());

        ontoNode = xmlFirstElementChild(ontoNode);
        map<string, string> attributes;
        for (xmlAttrPtr attribute = ontoNode->properties; attribute != nullptr; attribute = attribute->next) {
            xmlUniquePtr ft (xmlGetProp(ontoNode, attribute->name), xmlFree);
            attributes [reinterpret_cast<char*>(const_cast<unsigned char*>(attribute->name))] = reinterpret_cast<char*>(ft.get());
        }
        (*ontoData->machineLearningDatatypeMap ) [dName] = MachineLearningDatatypePtr ( new MachineLearningDatatype (method, attributes ) );
        return true;
    }

    if ( (xmlFirstElementChild(ontoNode) != nullptr) and (added ==false) )
        added = getOntologyFuzzyDatatype(xmlFirstElementChild(ontoNode));
    if ( (xmlNextElementSibling(ontoNode) != nullptr) and (added ==false) )
        added = getOntologyFuzzyDatatype(xmlNextElementSibling(ontoNode));
    if (added ==true)
        return true;

    return false;
}

void OntologyParser::getOntologyFuzzyObjectProperty(xmlNodePtr ontoNode) {
    if(xmlHasProp( ontoNode, (xmlChar*)"about" )) {
        dName = ontoPropValue( ontoNode, "about" );
        ontoData->objectPropertyVector.push_back(dName);
        getOntologyFuzzyObjectProperty( xmlFirstElementChild(ontoNode)  );
    }
    else if(!xmlStrEqual (ontoNode->name, (xmlChar*)"fuzzyLabel")   ) {
        getOntologyFuzzyObjectProperty( xmlNextElementSibling(ontoNode)  );
    } else {
        //now the node has the fuzzyLabel
        ontoNode = xmlFirstElementChild(xmlFirstElementChild(ontoNode)); //getting to the level where the properties lies
        oProp = (char*) xmlGetProp(ontoNode,(xmlChar*)"modifier");

        featureType = (char*) xmlGetProp( ontoNode,(xmlChar*)"feature_type" );
        (*ontoData->fuzzyObjectTypeMap)[dName] = FuzzyObjectTypePtr(new FuzzyObjectProperty(featureType, (*ontoData->fuzzyDatatypeMap)[oProp])   );
    }
}

vector<string> tmpVector;
bool OntologyParser::getOntologyIndividual(xmlNodePtr ontoNode) {
    bool added = false;

    if (xmlStrEqual (ontoNode->name, (xmlChar*)"type")   ) {
        string cName = ontoPropValue(ontoNode, "resource");
        tmpVector.push_back(cName);
    }

    if ( xmlStrEqual(ontoNode->name, (xmlChar*)"id") )  {
        string idCode = (char*) xmlNodeGetContent(ontoNode);

        for (register unsigned int i = 0; i < tmpVector.size(); i++) {
            //cout << "adding " << tmpVector.back()<<"\t" << idCode << endl;
            (*ontoData->classMap)[  tmpVector.back() ]->addIndividual(idCode);
            tmpVector.pop_back();
        }
        added = true;
        //return true;
    }

    if ( ( xmlFirstElementChild(ontoNode) != nullptr) && ( added == false ) )
        added = getOntologyIndividual( xmlFirstElementChild(ontoNode) );

    if ( ( xmlNextElementSibling(ontoNode) != nullptr) && ( added == false ) )
        added = getOntologyIndividual( xmlNextElementSibling(ontoNode) );

    return added;
}

string OntologyParser::ontoPropValue(xmlNodePtr node, string pName) {
    pName =(char*) xmlGetProp(node,(xmlChar*)pName.c_str());
    if ( pName.substr(pName.size()-5,pName.size()) !="Thing")
        pName.erase(pName.begin(),pName.begin()+ontoData->ontologyIRI.size());
    return pName;
}


OntologyParser::OntologyParser() {}

OntologyParser::OntologyParser(string &ontologyFile, string &cn, string &schema, string &tbl) {
    ontoData = OntologyDataPtr(new OntologyData() );
    ontoData->ontologyFile = ontologyFile;
    ontoData->connectionParameter = cn;
    ontoData->Conn =new  pqxx::connection( ontoData->connectionParameter );
    ontoData->schema = schema;
    ontoData->tableName = tbl;
    changeTags( &ontologyFile );
    xmlDocPtr ontoFile = xmlParseFile( "ontotemp.owl" );
    xmlNodePtr ontoNode = xmlFirstElementChild( xmlDocGetRootElement( ontoFile ) );
    addOntologyHierarchy( ontoNode );
    computeSelectStr();
}


OntologyParser::~OntologyParser() {}
