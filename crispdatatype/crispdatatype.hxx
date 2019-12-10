#ifndef CRISPDATATYPE_HXX
#define CRISPDATATYPE_HXX
#include "../datatype/datatype.hxx"

class CrispDatatype: public Datatype {
    std::string type;
    double value;
public:
    CrispDatatype(std::string& type, double& val);
    double calculateValue(double v);
    void setFunction(std::string);
    void setLimits(double*);
};

typedef std::shared_ptr<CrispDatatype> CrispDatatypePtr;

#endif // CRISPDATATYPE_HXX
