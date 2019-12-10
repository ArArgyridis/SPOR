#ifndef DATATYPE_H
#define DATATYPE_H
#include <memory>

class Datatype {
public:
    Datatype();
    virtual double calculateValue(double v) = 0;
    virtual void setFunction(std::string) = 0;
    virtual void setLimits(double*) = 0;
};
typedef std::shared_ptr<Datatype> FuzzyDatatypePtr;

#endif // DATATYPE_H
