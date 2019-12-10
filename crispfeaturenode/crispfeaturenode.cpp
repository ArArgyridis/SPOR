#include "crispfeaturenode.hxx"

using namespace  std;

CrispFeatureNode:: CrispFeatureNode(std::string& fName, CrispDatatypePtr datatype):TreeNode(FEATURE_NODE_NULL, false, CRISPFEATURENODE), featureName(fName), crispDatatype(datatype) {}
    CrispFeatureNode::~CrispFeatureNode(){}

void CrispFeatureNode::forceNULLMembershipValue(){
    membershipValue = FEATURE_NODE_NULL;
}

/*
int CrispFeatureNode::getFeatureCode() {
    return nodeFeatureCode;
}
*/

void CrispFeatureNode::setNotEstimated() {
    isEstimated = false;
}

void CrispFeatureNode::resetMembershipValue() {
    membershipValue = FEATURE_NODE_NULL;
}

void CrispFeatureNode::valueEstimation(int curSegment) {
    //setMembershipValue(fuzzyNominal->calculateValue((double) atof(to_string(TreeNode::getSegments()[curSegment][featureName]).c_str())));
    membershipValue = crispDatatype->calculateValue(atof(to_string(TreeNode::getSegments()[curSegment][featureName]).c_str()));
}
