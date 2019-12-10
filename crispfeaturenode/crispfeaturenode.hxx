#ifndef CRISPFEATURENODE_HXX
#define CRISPFEATURENODE_HXX

#include "../treenode/treenode.hxx"
#include "../crispdatatype/crispdatatype.hxx"

class CrispFeatureNode: public TreeNode {
    std::string featureName;
    CrispDatatypePtr crispDatatype;
public:
    CrispFeatureNode(std::string& name, CrispDatatypePtr datatype);
    ~CrispFeatureNode();

    void forceNULLMembershipValue();
    int getFeatureCode();
    void resetMembershipValue();
    void setNotEstimated();
    void valueEstimation(int);
};
typedef std::shared_ptr<CrispFeatureNode> CrispFeatureNodePtr;
#endif // CRISPFEATURENODE_HXX




