#ifndef HASH_LIST
#define HASH_LIST

#include "BucketNode.hpp"

class HashList
{
    public:
        HashList();
        HashList(int bucketListSize);
        ~HashList();
        HashList* getNext();
        BucketNode* getBucketArray();
        void setNext(HashList* next);
        void setBucketArray(BucketNode* bucketArray);

    private:
        BucketNode* bucketArray;
        HashList* next;
};


#endif
