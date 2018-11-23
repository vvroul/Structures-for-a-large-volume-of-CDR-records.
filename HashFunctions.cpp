#include "Headers.hpp"
#include "HashTable.hpp"


HashTable::HashTable()
{
	hashArray = new HashList*[hashSize];
	for (int i = 0; i < hashSize; ++i)
	{
	    hashArray[i] = NULL;
	}

	cout << "Hash table initialized successfully!" << endl;
}


HashTable::HashTable(int hashSize)
{
    this->hashSize = hashSize;
    hashArray = new HashList*[hashSize]();
    for (int i = 0; i < hashSize; ++i)
    {
        hashArray[i] = NULL;
    }

    cout << "Hash table initialized successfully, with size : " << hashSize << endl;
}


HashTable::~HashTable()				
{
    for (int i = 0; i < hashSize; ++i) 
    {
	    HashList *entry = hashArray[i];
	    while (entry != NULL) 
	    {
	        HashList *prev = entry;
	        entry = entry->getNext();
	        delete prev;
	    }
	    hashArray[i] = NULL;
    }
    
    delete [] hashArray;
    //std::cout << "Processing hashArray completed." << std::endl;
}


int HashTable::HashFunction(char* key)
{
	unsigned int hash = 0;
    for (int i = 0 ; key[i] != '\0' ; i++)
    {
        hash = 31*hash + key[i];
    }
    return hash % hashSize;
}


HashList** HashTable::getArray()
{
    return hashArray;
}


void HashTable::setArray(HashList** hashArray)
{
    this->hashArray = hashArray;
}


HashList* HashTable::getList()
{
    return *hashArray;
}


void HashTable::setList(HashList* table)
{
    *hashArray = table;
}



void HashTable::PutRecord(char* key, char* restKey, RestRecord* newRecord, int bucketSize)		
{
	int hashValue = HashFunction(key);
	int sameKey = 0;
	int emptySpace = 0;
	int emptyCdrSpace = 0;
	int inserted = 0;
	int pos;
	int cdrPos;
	HashList* prev = NULL;
	HashList* list = hashArray[hashValue];
	ListNode* cdrListNode;
	strcpy(newRecord->num, restKey);

	while (list != NULL)
	{
		for (int i = 0; i < bucketSize; i++)
		{
			if (strcmp(list->getBucketArray()[i].getKey(), key) == 0)
			{
				sameKey = 1;
				pos = i;
			}
		}

		if (sameKey == 0)
		{
			prev = list;
			list = list->getNext();
		}
		else
		{
			break;
		}
	}

	if (list == NULL)
	{
		//If list pointer equals with NULL, we're making a new bucket list node
		list = new HashList(bucketSize);
		//And attaching to it the appropriate info
		list->getBucketArray()[0].setKey(key);
		cdrListNode = list->getBucketArray()[0].getCdrPointer();
		cdrListNode = new ListNode();
		cdrListNode->getRecordArray()[0] = *newRecord;  
		list->getBucketArray()[0].setCdrPointer(cdrListNode);
		inserted = 1;
		cout << "	OK, with new hash value : " << hashValue <<endl;
		if (prev == NULL)
		{
			hashArray[hashValue] = list;
		}
		else
		{
			prev->setNext(list);
		}
	}
	else
	{
		//Assign new value
		for (int i = 0; i < restCdrSize; i++)
		{
		 	if (strcmp(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id, "NULL") == 0)
		 	{
		 		cdrPos = i;
		 		emptyCdrSpace = 1;
		 	}
		}
		if (emptyCdrSpace == 1)
		{
			//There is an empty position, so update the values in record array
			list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[cdrPos] = *newRecord;
			inserted = 1;
			cout << "   	OK, with hash value, in empty space : " << hashValue <<endl;	
		}
		else
		{
			//Create a new lis node and assign the value to the first empty place
			ListNode* newCdrListNode = list->getBucketArray()[pos].getCdrPointer();
			newCdrListNode = new ListNode();
			newCdrListNode->getRecordArray()[0] = *newRecord;  
			list->getBucketArray()[pos].setCdrPointer(newCdrListNode);
			inserted = 1;
			cout << "	OK, with hash value (no space left) : " << hashValue <<endl;
		}
	}
	if (inserted == 0)
	{
		cout << "   IError" << endl;
	}
}



void HashTable::Remove(char* key, char* id, int bucketSize)
{
	int hashValue = HashFunction(key);
	int sameKey = 0;
	int emptySpace = 0;
	int emptyCdrSpace = 0;
	int pos, recpos;
	int cdrPos;
	HashList* list = hashArray[hashValue];
	ListNode* cdrListNode;
	//DeList* deList = new DeList(restCdrSize);	

	while (list != NULL)
	{
		for (int i = 0; i < bucketSize; i++)
		{
			if (strcmp(list->getBucketArray()[i].getKey(), "NULL") != 0)
			{
				for (int k = 0; k < restCdrSize; ++k)
				{
					if (strcmp(list->getBucketArray()[i].getCdrPointer()->getRecordArray()[k].cdr_uniq_id, id) == 0)
					{
						//cout << "Key in recodArray : " << list->getBucketArray()[i].getCdrPointer()->getRecordArray()[k].cdr_uniq_id << " ID : " << id << endl;
						cdrListNode = list->getBucketArray()[i].getCdrPointer();
						sameKey = 1;
						pos = i;
						recpos = k;
						//deList->getArray()[recpos] = list->getBucketArray()[i].getCdrPointer()->getRecordArray();	
					}
				}
			}
		}

		if (sameKey == 0)
		{
			list = list->getNext();
		}
		else
		{
			break;
		}
	}

	if (list == NULL)
	{
		cout << "	DError." << endl;
		return;
	}
	else
	{
		//RestRecord* recordTobeDeleted = new RestRecord();
		//recordTobeDeleted = &(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[recpos]);
		//delete recordTobeDeleted;
		//delete list->getBucketArray()[pos].getCdrPointer()->getRecordArray();
		//delete deList->getArray()[recpos];
		//cout << "Record for key : " << key << " of id : " << id << " deleted, in position : " << recpos << endl;
		delete cdrListNode;
		list->getBucketArray()[pos].setCdrPointer(NULL);
		cout << "	Deleted " << id << endl;
	}
}




SubList* HashTable::Find(char* key, char* time1, char* year1, char* time2, char* year2, int bucketSize, int appear)
{
	int hashValue = HashFunction(key);
	int sameKey = 0;
	int emptySpace = 0;
	int emptyCdrSpace = 0;
	int pos;
	int cdrPos;
	int timerr = 0, yearer = 0;
	char timeToCheck[5];
	char yearToCheck[8];
	char realYear[8];
	char* token1 = new char[20]();
	char* token2 = new char[20]();
	char* tokenCheck = new char[20]();
	char* tookie1 = new char[20]();
	char* tookie2 = new char[20]();
	char* tookieCheck = new char[20]();
	char* tok1 = new char[20]();
	char* tok2 = new char[20]();
	char* tokC = new char[20]();
	HashList* prev = NULL;
	HashList* list = hashArray[hashValue];
	ListNode* cdrListNode;
	SubList* subList = new SubList();

	if (list == NULL)
	{
		cout << "Hash list is empty, caller not found." << endl;
	}
	else
	{
		//Bucket found. Going to search for the caller.
		for (int i = 0; i < bucketSize; i++)
		{
			if (strcmp(list->getBucketArray()[i].getKey(), key) == 0)
			{
				//cout << "Caller in list : " << list->getBucketArray()[i].getKey() << " and given key : " << key << endl;
				pos = i;
			}
		}
		//cout << "Caller found in position : " << pos << " going to search for the records." << endl;
		if (strcmp(time1, "NULL") != 0)
		{
			if (strcmp(year1, "NULL") != 0)									//search between time and year
			{
				timerr = TimeSearch(time1, time2, list, pos);
				yearer = YearSearch(year1, year2, list, pos);
				if ((timerr == -2) && (yearer == -2))
				{
					cout << "  		No CDRs found." << endl;
				}
				else
				{
					cout << "        Find operation completed." << endl;
				}
			}
			else															//search between time 
			{
				timerr = TimeSearch(time1, time2, list, pos);
				if (timerr == -2)
				{
					cout << "  		No CDRs found." << endl;
				}
				else
				{
					cout << "        Find operation completed." << endl;
				}
			}
		}
		else
		{
			if (strcmp(year1, "NULL") != 0)									//search between year
			{
				yearer = YearSearch(year1, year2, list, pos);
				if (yearer == -2)
				{
					cout << "  		No CDRs found." << endl;
				}
				else
				{
					cout << "        Find operation completed." << endl;
				}
			}
			else
			{
				for (int i = 0; i < restCdrSize; ++i)
				{
					if(strcmp(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id, "NULL") != 0)     //Time exists 
					{
						if (appear == 0)
						{
							cout << "  	No restrictions. Going to print all records of caller." << endl;
							cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
						}
						subList->Insert(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num);
					}
				}
			}
		}
	}

	return subList;
}



SubList* HashTable::LookUp(char* key, char* time1, char* year1, char* time2, char* year2, int bucketSize, int appear)
{
	int hashValue = HashFunction(key);
	int sameKey = 0;
	int emptySpace = 0;
	int emptyCdrSpace = 0;
	int pos;
	int cdrPos;
	int timerr = 0, yearer = 0;
	char timeToCheck[5];
	char yearToCheck[8];
	char realYear[8];
	char* token1 = new char[20]();
	char* token2 = new char[20]();
	char* tokenCheck = new char[20]();
	char* tookie1 = new char[20]();
	char* tookie2 = new char[20]();
	char* tookieCheck = new char[20]();
	char* tok1 = new char[20]();
	char* tok2 = new char[20]();
	char* tokC = new char[20]();
	HashList* prev = NULL;
	HashList* list = hashArray[hashValue];
	ListNode* cdrListNode;
	SubList* subList = new SubList();

	if (list == NULL)
	{
		cout << "Hash list is empty, caller not found." << endl;
	}
	else
	{
		//Bucket found. Going to search for the caller.
		for (int i = 0; i < bucketSize; i++)
		{
			if (strcmp(list->getBucketArray()[i].getKey(), key) == 0)
			{
				//cout << "Caller in list : " << list->getBucketArray()[i].getKey() << " and given key : " << key << endl;
				pos = i;
			}
		}
		if (strcmp(time1, "NULL") != 0)
		{
			if (strcmp(year1, "NULL") != 0)									//search between time and year
			{
				timerr = TimeSearch(time1, time2, list, pos);
				yearer = YearSearch(year1, year2, list, pos);
				if ((timerr == -2) && (yearer == -2))
				{
					cout << "  		No CDRs found." << endl;
				}
				else
				{
					cout << "        LookUp operation completed." << endl;
				}
			}
			else															//search between time 
			{
				timerr = TimeSearch(time1, time2, list, pos);
				if (timerr == -2)
				{
					cout << "  		No CDRs found." << endl;
				}
				else
				{
					cout << "        LookUp operation completed." << endl;
				}
			}
		}
		else
		{
			if (strcmp(year1, "NULL") != 0)									//search between year
			{
				yearer = YearSearch(year1, year2, list, pos);
				if (yearer == -2)
				{
					cout << "  		No CDRs found." << endl;
				}
				else
				{
					cout << "        LookUp operation completed." << endl;
				}
			}
			else
			{
				for (int i = 0; i < restCdrSize; ++i)
				{
					if(strcmp(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id, "NULL") != 0)     //Time exists 
					{
						if (appear == 0)
						{
							cout << "  	No restrictions. Going to print all records of callee." << endl;
							cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
							cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
						}
						subList->Insert(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num);
					}
				}
			}
		}
	}

	return subList;
}



void HashTable::Print(int bucketSize)
{
	HashList* list;
	char yearToCheck[8];

	for (int i = 0; i < hashSize; ++i)
	{
		list = hashArray[i];
		//cout << "		 Pos " << i << " in hashtable, has bucket with elements : " << endl;
		if (list != NULL)
		{
			for (int k = 0; k < bucketSize; ++k)
			{
				if (strcmp(list->getBucketArray()[k].getKey(), "NULL") != 0)
				{
					cout << "      ----------------------------------------------- " << endl;
					cout << "      In pos " << i << " key : " << list->getBucketArray()[k].getKey() << " and CDRs : " << endl;
					for (int j = 0; j < restCdrSize; ++j)
					{
						if (list->getBucketArray()[k].getCdrPointer() != NULL)
						{
							if (strcmp(list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].cdr_uniq_id, "NULL") != 0)
							{
								cout << "      " << list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].cdr_uniq_id;
								cout << " " << list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].num;
								list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].date[strlen(list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].date) - 5] = '\0';
								cout << " " << list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].date;
								cout << " " << list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].init_time;
								cout << " " << list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].duration;
								cout << " " << list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].type;
								cout << " " << list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].tarrif;
								cout << " " << list->getBucketArray()[k].getCdrPointer()->getRecordArray()[j].fault_condition << endl;
							}
						}
					}
				}
			}
		}
		else
		{
			continue;
		}
	}
	cout << endl;
}



Region* HashTable::TopDestination(char* key, int bucketSize)
{
	int hashValue = HashFunction(key);
	HashList* list = hashArray[hashValue];
	Region* regionList = new Region();
	ListNode* cdrListNode;
	int pos;
	char* numToken = new char[25]();

	if (list == NULL)
	{
		cout << "Hash list is empty, caller not found." << endl;
	}
	else
	{
		//Bucket found. Going to search for the caller.
		for (int i = 0; i < bucketSize; i++)
		{
			if (strcmp(list->getBucketArray()[i].getKey(), key) == 0)
			{
				//cout << "Caller in list : " << list->getBucketArray()[i].getKey() << " and given key : " << key << endl;
				pos = i;
			}
		}

		for (int j = 0; j < restCdrSize; ++j)
		{
			if (list->getBucketArray()[pos].getCdrPointer() != NULL)
			{
				if (strcmp(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[j].cdr_uniq_id, "NULL") != 0)
				{
					strcpy(numToken, list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[j].num);
					numToken[strlen(numToken) - 11] = '\0';
					regionList->Insert(numToken);
				}
			}
		}
	}

	return regionList;
}



int HashTable::Search(char* key, int bucketSize)
{
	int hashValue = HashFunction(key);
	int sameKey = 0;
	int pos;
	HashList* list = hashArray[hashValue];
	ListNode* cdrListNode;

	if (list == NULL)
	{
		cout << "		 Hash list is empty, caller not found." << endl;
	}
	else
	{
		//Bucket found. Going to search for the caller.
		for (int i = 0; i < bucketSize; i++)
		{
			if (strcmp(list->getBucketArray()[i].getKey(), key) == 0)
			{
				//cout << "		 Caller in list : " << list->getBucketArray()[i].getKey() << " and given key : " << key << endl;
				pos = i;
				return 1;
			}
		}
	}

	return 0;
}