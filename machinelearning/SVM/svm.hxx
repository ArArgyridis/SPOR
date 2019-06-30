#ifndef SVM_H
#define SVM_H
#include <dlib/svm.h>
#include <dlib/svm_threaded.h>

typedef dlib::matrix<double> SampleType;
typedef std::vector<SampleType> SampleVector;
typedef double LabelType;
typedef std::vector<LabelType> LabelTypeVector;

class SVM {
    typedef dlib::radial_basis_kernel<SampleType> KernelType;
    typedef dlib::one_vs_all_trainer<dlib::any_trainer<SampleType> > OvaTrainer;
    typedef dlib::radial_basis_kernel<SampleType> rbfKernel;
    typedef dlib::polynomial_kernel<SampleType> polyKernel;
    dlib::one_vs_all_decision_function<OvaTrainer> decisionFunction;

public:
    SVM();
    SVM(SampleVector& samples, LabelTypeVector & labels);
    ~SVM();
    void predict(SampleVector& data, LabelTypeVector& labels);
    void validateTrain(SampleVector& data, LabelTypeVector& labels);
};

#endif // SVM_H
