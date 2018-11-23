#include "Headers.hpp"
#include "DeList.hpp"


DeList::DeList(int size)
{
	pointerRecordArray = new RestRecord*[size];
	//cout << "bfr seg" << endl;
	for (int i = 0; i < size; ++i)
	{
		pointerRecordArray[i] = NULL;
	}
	//cout << "------->  DeList initialized successfully!" << endl << endl;
}


DeList::~DeList()
{
	//cout << "------->  Finished processing the DeList." << endl;
}


RestRecord** DeList::getArray()
{
	return pointerRecordArray;
}


void DeList::setArray(RestRecord** pointerRecordArray)
{
	this->pointerRecordArray = pointerRecordArray;
}