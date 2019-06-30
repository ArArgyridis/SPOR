#include "svm.hxx"
#include <cmath>

using namespace  dlib;
using namespace  std;

SVM::SVM(SampleVector &samples, LabelTypeVector &labels) {
    OvaTrainer trainer;
    svm_nu_trainer<rbfKernel> rbfTainer;

    trainer.set_trainer(rbfTainer);
    trainer.set_num_threads(8);
    decisionFunction = trainer.train(samples, labels);

    int sum = 0;
    for (size_t i = 0; i < samples.size(); i++)
        if (decisionFunction(samples[i]) != labels[i] )
            sum += 1;

    cout << "correct classification percentage: " << (labels.size()*1.0 - sum)/labels.size()*100 << ", samples : " << labels.size() << endl;
}

SVM::SVM() {}

SVM::~SVM(){}

void SVM::predict(SampleVector &data, LabelTypeVector &labels) {
    for (size_t i = 0; i < data.size(); i++)
        labels.push_back(decisionFunction(data[i]));
}
void SVM::validateTrain(SampleVector &data, LabelTypeVector &labels) {
    int sum = 0, tp = 0, sumTrueTotal = 0, fn = 0, fp = 0;

    for (size_t i = 0; i < data.size(); i++){
        if (decisionFunction(data[i]) !=  labels[i]  )
            sum ++;
         if (decisionFunction(data[i]) ==  labels[i] && labels[i] == 1 )
             tp++;

         if (decisionFunction(data[i]) ==  0 && labels[i] == 1 )
            fn++;

         if (decisionFunction(data[i]) ==  1 && labels[i] == 0 )
            fp++;

         if (labels[i] == 1)
             sumTrueTotal++;
        cout << decisionFunction(data[i]) << "\t" << labels[i] << endl;
        }
     cout << "correct test classification percentage: " << (labels.size()*1.0 - sum)/labels.size()*100 << ", samples : " << labels.size() <<"\t true event: " << sumTrueTotal << "\tTP: " <<tp*100.0/sumTrueTotal  <<"\tFP: " << fp*100.0/sumTrueTotal  << "\tFN:"<< fn*100.0/sumTrueTotal  << "\t success rate: " << tp*100.0/(tp+fp)<< endl;

}
