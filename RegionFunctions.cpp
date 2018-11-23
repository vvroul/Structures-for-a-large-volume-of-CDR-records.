#include "Headers.hpp"
#include "Region.hpp"


// ************ Region Node ************


RegionNode::RegionNode()
{
	//cout << "RegionNode created successfully!" << endl;
}


RegionNode::~RegionNode()
{
	//cout << "RegionNode deleted." << endl;
}


RegionNode* RegionNode::getNext()
{
	return next;
}


void RegionNode::setNext(RegionNode* next)
{
	this->next = next;
}


char* RegionNode::getKey()
{
	return key;
}


void RegionNode::setKey(char* key)
{
	strcpy(this->key, key);
}


int RegionNode::getTimes()
{
	return times;
}


void RegionNode::setTimes(int times)
{
	this->times = times;
}	



// ************ Region  ************


Region::Region()		
{
	header = NULL;
	//cout << "Region initialized successfully!" << endl << endl;		
}



Region::~Region()
{
	//cout << "Processing general list completed." << endl;
}



RegionNode* Region::getNode() 
{
	return header;
}



void Region::PrintData()
{
	RegionNode* node = new RegionNode();
	node = header;
	int counter = 1;
	while (node->getNext() != NULL)
	{
		cout << "Item " << counter << " in list : " << node->getKey() << " with times : " << node->getTimes() << endl;
		node = node->getNext();
		++counter;
	}

	cout << "Item " << counter << " in list : " << node->getKey() << " with times : " << node->getTimes() << endl;
}



int Region::Exists(char* item)
{
    RegionNode* node = header;
    
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



RegionNode* Region::Search(char* item)
{
    RegionNode* node = header;
    
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



void Region::Insert(char* newItem)
{

	RegionNode* node = new RegionNode();

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
			RegionNode* tail = header;
			while (tail->getNext() != NULL)
			{
				tail = tail->getNext();
			}
			tail->setNext(node);
		}
		//cout << "Item inserted : " << newItem << endl;
	}
}