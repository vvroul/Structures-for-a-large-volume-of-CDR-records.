#ifndef BUCKET_NODE_H
#define BUCKET_NODE_H

#include "ListNode.hpp"

class BucketNode
{
    public:
        BucketNode();
        BucketNode(char* key);
        ~BucketNode();
        ListNode* getCdrPointer();
        char* getKey();
        void setCdrPointer(ListNode* cdrPointer);
        void setKey(char* key);

    private:
        char key[15];
        ListNode* cdrPointer;
};


#endif //!BUCKET_NODE_H
