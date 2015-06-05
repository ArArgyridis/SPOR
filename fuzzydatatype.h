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
 
#ifndef FUZZYDATATYPE_H
#define FUZZYDATATYPE_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

class FuzzyDatatype {
    int parNum; //counter that stores the number of the fuzzy function parameters
    double *fuzzyInput; //
    double (*fuzzyFunction)(double, double*);
public:
    FuzzyDatatype();
    FuzzyDatatype(std::string,double*);// for normal features
   ~FuzzyDatatype();
    void setFunction(std::string);
    void setLimits(double*);
    double calculateValue(double);
};

typedef boost::shared_ptr<FuzzyDatatype> FuzzyDatatypePtr;

typedef std::map <std::string, FuzzyDatatypePtr> DatatypeMapType;

typedef boost::shared_ptr<DatatypeMapType> DataTypeMapTypePtr;

#endif // FUZZYDATATYPE_H
