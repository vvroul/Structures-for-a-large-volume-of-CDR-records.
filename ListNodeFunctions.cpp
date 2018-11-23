#include "Headers.hpp"
#include "ListNode.hpp"


ListNode::ListNode()
{
	recordArray = new RestRecord[restCdrSize];
	for (int i = 0; i < restCdrSize; ++i)
	{
		recordArray[i].cdr_uniq_id = new char[20]();
		strcpy(recordArray[i].cdr_uniq_id, "NULL");
		strcpy(recordArray[i].init_time, "NULL");
		strcpy(recordArray[i].date, "NULL");
	}
	next = NULL;
	//cout << "------->  ListNode initialized successfully!" << endl << endl;
}


ListNode::~ListNode()
{
	if (next != NULL)
	{
		next = NULL;
		delete next;		
		//cout << "------->  Finished processing the ListNode." << endl;
	}
	else
	{
		//cout << "------->  Finished processing the ListNode." << endl;
	}
}


int ListNode::getTimes()
{
    return times;
}


ListNode* ListNode::getNext()
{
	return next;
}


RestRecord* ListNode::getRecordArray()
{
	return recordArray;
}


void ListNode::setTimes(int times)
{
    this->times = times;
}


void ListNode::setNext(ListNode* next)
{
    this->next = next;
}


void ListNode::setRecordArray(RestRecord* recordArray)
{
    this->recordArray = recordArray;
}


void ListNode::PrintData()
{
}