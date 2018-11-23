#include "Headers.hpp"
#include "BucketNode.hpp"


BucketNode::BucketNode()
{
	cdrPointer = NULL;
    //cout << "------->  BucketNode [1] initialized successfully!" << endl << endl;
}


BucketNode::BucketNode(char* key)
{
    strcpy(this->key, key);
    cdrPointer = NULL;
    //cout << "------->  BucketNode [2] initialized successfully!" << endl << endl;
}


BucketNode::~BucketNode()
{
    //cout << "------->  Finished processing the BucketNode." << endl;
}


ListNode* BucketNode::getCdrPointer()
{
    return cdrPointer;
}


char* BucketNode::getKey()
{
    return key;
}


void BucketNode::setCdrPointer(ListNode* cdrPointer)
{
    this->cdrPointer = cdrPointer;
}


void BucketNode::setKey(char* key)
{
    strcpy(this->key, key);
}
