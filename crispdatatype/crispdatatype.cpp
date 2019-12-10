#include "crispdatatype.hxx"
#include <iostream>
using namespace std;

CrispDatatype::CrispDatatype(string &type, double &val):Datatype(), type(type), value(val) {

}
double CrispDatatype::calculateValue(double v){
    if (type =="hasValue")
        return v == value;
    else if(type =="minInclusive")
        return v >= value;
    else if(type == "minExclusive")
        return v > value;
    else if (type == "maxInclusive") {
        return v <= value;
    }
    else if (type =="maxExclusive")
        return v < value;

    //return 0;
}
void CrispDatatype::setFunction(std::string func) {}
void CrispDatatype::setLimits(double*) {}
