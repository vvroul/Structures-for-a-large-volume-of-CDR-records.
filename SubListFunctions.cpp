#include "Headers.hpp"
#include "SubList.hpp"


// ************ Sub Node ************


SubNode::SubNode()
{
	//cout << "SubNode created successfully!" << endl;
}


SubNode::~SubNode()
{
	//cout << "SubNode deleted." << endl;
}


SubNode* SubNode::getNext()
{
	return next;
}


void SubNode::setNext(SubNode* next)
{
	this->next = next;
}


char* SubNode::getKey()
{
	return key;
}


void SubNode::setKey(char* key)
{
	strcpy(this->key, key);
}


int SubNode::getTimes()
{
	return times;
}


void SubNode::setTimes(int times)
{
	this->times = times;
}	


// ************ SubList  ************


SubList::SubList()		
{
	header = NULL;
	//cout << "SubList initialized successfully!" << endl << endl;		
}



SubList::~SubList()
{
	//cout << "Processing general list completed." << endl;
}



SubNode* SubList::getNode() 
{
	return header;
}



int SubList::getCall()
{
	return call;
}



void SubList::setCall(int call)
{
	this->call = call;
}



void SubList::PrintData()
{
	SubNode* node = new SubNode();
	node = header;
	int counter = 1;
	while (node->getNext() != NULL)
	{
		cout << "	Item " << counter << " in list : " << node->getKey() << " with times : " << node->getTimes() << endl;
		node = node->getNext();
		++counter;
	}

	cout << "	Item " << counter << " in list : " << node->getKey() << " with times : " << node->getTimes() << endl;
}



int SubList::Exists(char* item)
{
    SubNode* node = header;
    
    while (node != NULL) 
    {
        if(strcmp(node->getKey(), item) == 0)
        {
            return 1;
        }
        node = node->getNext();
    }
    return 0;
}



SubNode* SubList::Search(char* item)
{
    SubNode* node = header;
    
    while (node != NULL) 
    {
        if(strcmp(node->getKey(), item) == 0)
        {
            return node;
        }
        node = node->getNext();
    }
    return NULL;
}



void SubList::Insert(char* newItem)
{

	SubNode* node = new SubNode();

	if (Exists(newItem) == 1)
	{
		node = Search(newItem);
		int theTimes = node->getTimes();
		node->setTimes(++theTimes);
		return;
	}
	else
	{
		node->setKey(newItem);
		node->setTimes(1);
		node->setNext(NULL);

		if (header == NULL)
	    {
			header = node;
	    }
		else
		{
			SubNode* tail = header;
			while (tail->getNext() != NULL)
			{
				tail = tail->getNext();
			}
			tail->setNext(node);
		}
		//cout << "Item inserted : " << newItem << endl;
	}
}