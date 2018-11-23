#include "Headers.hpp"
#include "HashList.hpp"


HashList::HashList()
{
    //std::cout << "Bucket List initialized successfully!" << '\n';
    next = NULL;
}


HashList::HashList(int bucketListSize)
{
    bucketArray = new BucketNode[bucketListSize];
    for (int i = 0; i < bucketListSize; ++i)
    {
        strcpy(bucketArray[i].getKey(), "NULL"); 
    }
	// for (int i = 0; i < bucketListSize; ++i)
	// {
	//     bucketList[i] = NULL;
	// }

    next = NULL;
	//cout << "Bucket List initialized successfully!" << endl;
}


HashList::~HashList()
{
    //~
}


HashList* HashList::getNext()
{
    return next;
}


BucketNode* HashList::getBucketArray()
{
    return bucketArray;
}


void HashList::setNext(HashList* next)
{
    this->next = next;
}


void HashList::setBucketArray(BucketNode* bucketArray)
{
    this->bucketArray = bucketArray;
}
