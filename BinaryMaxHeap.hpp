#ifndef BINARY_MAX_HEAP
#define BINARY_MAX_HEAP

class BinaryMaxNode
{
    public:
        BinaryMaxNode();
        ~BinaryMaxNode();
        char* getTelNum();
        float getCost();
        BinaryMaxNode* getRightChild();
        BinaryMaxNode* getLeftChild();
        BinaryMaxNode* getParent();
        void setTelNum(char* telnum);
        void setCost(float cost);
        void setLeftChild(BinaryMaxNode* leftChild);
        void setRightChild(BinaryMaxNode* rightChild);
        void setParent(BinaryMaxNode* parent);

    private:
        char telnum[15];
        float cost;
        BinaryMaxNode* leftChild;
        BinaryMaxNode* rightChild;
        BinaryMaxNode* parent;
};


class BinaryMaxHeap
{
    public:
        BinaryMaxHeap();
        ~BinaryMaxHeap();
        BinaryMaxNode* getRoot();
        int getNodeCount();
        float Traverse(BinaryMaxNode* node);
        void setRoot(BinaryMaxNode* root);
        void setNodeCount(int nodeCount);
        void Insert(BinaryMaxNode* newNode);
        void DestroyHeap(BinaryMaxNode* node);
        void TopSubs(BinaryMaxNode* node, BinaryMaxNode* altNode, float rpercentage);
        BinaryMaxNode* Exists(char* key, BinaryMaxNode* node);
        
    private:
        BinaryMaxNode* root;
        int nodeCount;
};

#endif
