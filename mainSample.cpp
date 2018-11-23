#include "Headers.hpp"
#include "HashTable.hpp"
#include "BinaryMaxHeap.hpp"

int main(int argc, char const *argv[])
{
    cout << "Initializing firmware........Firmware functional." << endl;
    cout << "System check............Passed." << endl;
    cout << "Initiating plain language interface...Done." << endl;
    cout << "Loading library session...Done." << endl;
    cout << "Loading \'werhauz\' executable program....Done." << endl;
    cout << "Ready." << endl << endl << endl;

    ifstream operationsFile, costFile;
    fstream tempFileForUser;
    tempFileForUser.open("temp.txt");   //temporary file that holds input commands
    if (tempFileForUser == NULL)
    {
        cout << "You've given a wrong temporary file." << endl;
        exit(1);
    }
    int hashEntries_1, hashEntries_2;
    int bucketSize;
    int inputFileUsed = 0;

    if (argc > 8)
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-o") == 0)
			{
				operationsFile.open(argv[i+1]);   //input file is next on argv
				if (operationsFile == NULL)
				{
					cout << "You've given a wrong input file." << endl;
					exit(1);
				}
				else
				{
					cout << "File : " << argv[i+1] << " opened successfully!" << endl;
                    inputFileUsed = 1;
				}
				++i;
			}
            else if (strcmp(argv[i], "-c") == 0)
            {
                costFile.open(argv[i+1]);         //cost file is next on argv
                if (costFile == NULL)
                {
                    cout << "You've given a wrong cost file." << endl;
                    exit(1);
                }
                else
                {
                    cout << "File : " << argv[i+1] << " opened successfully!" << endl;
                }
                ++i;
            }
			else if (strcmp(argv[i], "-h1") == 0)
			{
				hashEntries_1 = atoi(argv[i+1]);
				cout << "Number of entries for hash table 1 : " << hashEntries_1 << endl;
				++i;
			}
            else if (strcmp(argv[i], "-h2") == 0)
			{
				hashEntries_2 = atoi(argv[i+1]);
				cout << "Number of entries for hash table 2 : " << hashEntries_2 << endl;
				++i;
			}
            else if (strcmp(argv[i], "-s") == 0)
			{
				bucketSize = atoi(argv[i+1]);
				cout << "Size of buckets in bytes : " << bucketSize << endl;
				++i;
			}
			else
			{
				cout << "You've given wrong input" << endl;
				exit(6);
			}
		}
	}
	else
	{
		cout << "------>  Suggested input: $./werhauz –o <input_file> –h1 <hash_entries_1> –h2 <hash_entries_2> -s <bucket_size> -c <cost file>" << endl; //+-f <configuration file>
		exit(1);
	}

    cout << "*********** Welcome to CDR Management! ***********" << endl;

    //srand(time(NULL));
	std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
	std::cout.precision(2);

    //Structures Creation
    HashTable* callerHashTable = new HashTable(hashEntries_1);
    HashTable* calleeHashTable = new HashTable(hashEntries_2);
    BinaryMaxHeap* binaryMaxHeap = new BinaryMaxHeap();
    Region* regionList = new Region();

    char instr[150];
    char instr2[150];
    char* token = new char[500]();
    int k;
    int uiChoice;
    int afterDelete = 0;

    cout << "------>  Entering the UI Manager before executing any available commands in input file" << endl;

    do
    {
        cout << "Select one of the available options : " << endl;   //UI Management
        cout << "   1. Enter a command." << endl;
        cout << "   2. Exit the UI manager." << endl;
        cin >> uiChoice;
        if (uiChoice == 1)
        {
            cout << "------>  Please enter a command you want to execute : " << endl;
            cin.ignore();
            gets(instr2);
            cout << "Command added to temporary file." << endl;
            //wrtite to temporary file
            tempFileForUser << instr2 << endl;
        }
        else
        {
            cout << "Please select again..." << endl;
        }
    }while (uiChoice != 2);

    tempFileForUser.close();

    if (inputFileUsed == 1)     //Input file Management
    {
        while (!operationsFile.eof())
        {
            operationsFile >> instr;
            while (strcmp(instr, "insert") == 0)
            {
                cout << "insert  >>>>" << endl;
                Record* newRecord = new Record();
                RestRecord* newRestRecord = new RestRecord();
                BinaryMaxNode* maxNode = new BinaryMaxNode();
                float theCost;
                
                operationsFile >> instr;
                /*
                    Extract the appropriate information from insert command
                    and pass it to the record's fields
                */
                token = strtok(instr, ";");
                while (token)
                {
                    newRecord->cdr_uniq_id = new char[sizeof(token)]();
                    newRestRecord->cdr_uniq_id = new char[sizeof(token)]();
                    strcpy(newRecord->cdr_uniq_id, token);
                    strcpy(newRestRecord->cdr_uniq_id, token);
                    token = strtok(NULL, ";");
                    strcpy(newRecord->originator_num, token);
                    maxNode->setTelNum(newRecord->originator_num);
                    token = strtok(NULL, ";");
                    strcpy(newRecord->destination_num, token);
                    token = strtok(NULL, ";");
                    strcpy(newRestRecord->date, token);
                    token = strtok(NULL, ";");
                    strcpy(newRestRecord->init_time, token);
                    token = strtok(NULL, ";");
                    newRestRecord->duration = atoi(token);
                    token = strtok(NULL, ";");
                    newRestRecord->type = atoi(token);
                    token = strtok(NULL, ";");
                    newRestRecord->tarrif = atoi(token);
                    token = strtok(NULL, ";");
                    newRestRecord->fault_condition = atoi(token);
                    token = strtok(NULL, ";");
                    callerHashTable->PutRecord(newRecord->originator_num, newRecord->destination_num, newRestRecord, bucketSize);      //insert to caller struct
                    calleeHashTable->PutRecord(newRecord->destination_num, newRecord->originator_num, newRestRecord, bucketSize);      //insert to callee struct
                    theCost = CalCost(newRestRecord, costFile);
                    //cout << "Cost of this call : " << theCost << endl;
                    if (theCost != 0)
                    {
                        maxNode->setCost(theCost);
                    }
                    binaryMaxHeap->Insert(maxNode);                                                                                    //insert to max heap
                }
                operationsFile >> instr;
                newRecord = NULL;
                newRestRecord = NULL;
                maxNode = NULL;
                //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "delete") == 0)
            {
                cout << "delete  >>>>" << endl;
                Record* newRecord = new Record();
                operationsFile >> instr;
                strcpy(newRecord->originator_num, instr);
                operationsFile >> instr;
                newRecord->cdr_uniq_id = instr;
                callerHashTable->Remove(newRecord->originator_num, newRecord->cdr_uniq_id, bucketSize);
                operationsFile >> instr;
                newRecord = NULL;
                //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "find") == 0)      
            {
                cout << "find  >>>>" << endl;
                Record* newRecord = new Record();
                char* tempTime = new char[5]();
                char* tempDate = new char[8]();
                char* tempTime1 = new char[5]();
                operationsFile >> instr;
                strcpy(newRecord->originator_num, instr);
                operationsFile >> instr;
                if (strchr(instr, ':'))                    //time_1 is defined
                {
                    strcpy(newRecord->init_time, instr);
                    strcpy(tempTime, instr);
                    operationsFile >> instr;
                    if (strchr(instr, ':'))                //time_2 is defined, we don't have years
                    {
                        strcpy(newRecord->init_time, instr);
                        callerHashTable->Find(newRecord->originator_num, tempTime, "NULL", newRecord->init_time, "NULL", bucketSize, 0);      //calling find with times only
                        operationsFile >> instr;
                    }
                    else                                   //year_1 is defined
                    {
                        strcpy(newRecord->date, instr);
                        strcpy(tempDate, newRecord->date);
                        operationsFile >> instr;           //time_2 is next
                        strcpy(newRecord->init_time, instr);
                        strcpy(tempTime1, newRecord->init_time);
                        operationsFile >> instr;           //year_2 is next
                        strcpy(newRecord->date, instr);
                        callerHashTable->Find(newRecord->originator_num, tempTime, tempDate, tempTime1, newRecord->date, bucketSize, 0);      //calling find with times and years
                        operationsFile >> instr;
                    }
                }
                else if (isdigit(instr[0]))
                {
                    strcpy(newRecord->date, instr);
                    strcpy(tempDate, newRecord->date);
                    operationsFile >> instr;
                    strcpy(newRecord->date, instr);
                    callerHashTable->Find(newRecord->originator_num, "NULL", tempDate, "NULL", newRecord->date, bucketSize, 0);               //calling find with years only
                    operationsFile >> instr;
                }
                else
                {
                    callerHashTable->Find(newRecord->originator_num, "NULL", "NULL", "NULL", "NULL", bucketSize, 0);                          //calling find without restrictions
                }
                newRecord = NULL;
                //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "lookup") == 0)    
            {
               cout << "lookup  >>>>" << endl;
               Record* newRecord = new Record();
               char* tempTime = new char[5]();
               char* tempDate = new char[8]();
               char* tempTime1 = new char[5]();
               operationsFile >> instr;
               strcpy(newRecord->destination_num, instr);
               operationsFile >> instr;
               if (strchr(instr, ':'))                     //time_1 is defined
               {
                   strcpy(newRecord->init_time, instr);
                   strcpy(tempTime, instr);
                   operationsFile >> instr;
                   if (strchr(instr, ':'))                 //time_2 is defined, we don't have years
                   {
                       strcpy(newRecord->init_time, instr);
                       calleeHashTable->LookUp(newRecord->destination_num, tempTime, "NULL", newRecord->init_time, "NULL", bucketSize, 0);    //calling lookup with times only
                       operationsFile >> instr;
                   }
                   else                                    //year_1 is defined
                   {
                       strcpy(newRecord->date, instr);
                       strcpy(tempDate, newRecord->date);
                       operationsFile >> instr;            //time_2 is next
                       strcpy(newRecord->init_time, instr);
                       strcpy(tempTime1, newRecord->init_time);
                       operationsFile >> instr;            //year_2 is next
                       strcpy(newRecord->date, instr);
                       calleeHashTable->LookUp(newRecord->destination_num, tempTime, tempDate, tempTime1, newRecord->date, bucketSize, 0);    //calling lookup with times and years
                       operationsFile >> instr;
                   }
               }
               else if (isdigit(instr[0]))
               {
                   strcpy(newRecord->date, instr);
                   strcpy(tempDate, newRecord->date);
                   operationsFile >> instr;
                   strcpy(newRecord->date, instr);
                   calleeHashTable->LookUp(newRecord->destination_num, "NULL", tempDate, "NULL", newRecord->date, bucketSize, 0);             //calling lookup with years only
                   operationsFile >> instr;
               }
               else
               {
                   calleeHashTable->LookUp(newRecord->destination_num, "NULL", "NULL", "NULL", "NULL", bucketSize, 0);                        //calling lookup with years only
               }
               newRecord = NULL;
               //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "indist") == 0)
            {
                cout << "indist  >>>>" << endl;
                Record* newRecord = new Record();
                char* tempNum = new char[15]();
                SubList* subListOne = new SubList();
                SubList* subListTwo = new SubList();
                SubList* subListBoth = new SubList();
                int is_subcaller = 0, is_subcallee = 0;
                //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
                operationsFile >> instr;
                strcpy(newRecord->originator_num, instr);
                int is_caller = callerHashTable->Search(newRecord->originator_num, bucketSize);
                if (is_caller == 1)
                {
                    subListOne = callerHashTable->Find(newRecord->originator_num, "NULL", "NULL", "NULL", "NULL", bucketSize, 1);               //if in caller table, call Find
                    subListOne->setCall(0);         //for caller
                }
                else
                {
                    subListOne = calleeHashTable->LookUp(newRecord->originator_num, "NULL", "NULL", "NULL", "NULL", bucketSize, 1);            //if in callee table, call LookUp
                    subListOne->setCall(1);         //for callee
                }
                //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
                operationsFile >> instr;
                strcpy(newRecord->destination_num, instr);
                int is_callee = calleeHashTable->Search(newRecord->destination_num, bucketSize);
                if (is_callee == 1)
                {    
                    subListTwo = calleeHashTable->LookUp(newRecord->destination_num, "NULL", "NULL", "NULL", "NULL", bucketSize, 1);            //if in callee table, call LookUp
                    subListTwo->setCall(1);         //for callee
                }
                else
                {
                    subListTwo = callerHashTable->Find(newRecord->destination_num, "NULL", "NULL", "NULL", "NULL", bucketSize, 1);               //if in caller table, call Find
                    subListTwo->setCall(0);         //for caller
                }
                //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
                SubNode* node = new SubNode();
                node = subListOne->getNode();
                while (node != NULL)
                {
                    if (subListOne->getCall() == 0)     //search in callee table, because listOne was the caller
                    {
                        SubList* theSubList = new SubList();
                        theSubList = calleeHashTable->LookUp(node->getKey(), "NULL", "NULL", "NULL", "NULL", bucketSize, 1);
                        if (theSubList->Exists(newRecord->destination_num))     //has communicated with both callers, insert that key in the list for both
                        {
                            subListBoth->Insert(node->getKey());
                        }
                        theSubList = NULL;
                    }
                    else                                //search in caller table, because listOne was the callee
                    {
                        SubList* theSubList = new SubList();
                        theSubList = callerHashTable->Find(node->getKey(), "NULL", "NULL", "NULL", "NULL", bucketSize, 1); 
                        if (theSubList->Exists(newRecord->destination_num))     //has communicated with both callers, insert that key in the list for both
                        {
                            subListBoth->Insert(node->getKey());
                        }
                        theSubList = NULL;
                    }

                    node = node->getNext();
                }
                //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
                SubNode* node2 = new SubNode();
                node2 = subListTwo->getNode();
                while (node2 != NULL)
                {
                    if (subListTwo->getCall() == 0)     //search in callee table, because listTwo was the caller
                    {
                        SubList* theSubList = new SubList();
                        theSubList = calleeHashTable->LookUp(node2->getKey(), "NULL", "NULL", "NULL", "NULL", bucketSize, 1);
                        if (theSubList->Exists(newRecord->originator_num))     //has communicated with both callers, insert that key in the list for both
                        {
                            subListBoth->Insert(node2->getKey());
                        }
                        theSubList = NULL;
                    }
                    else                                //search in caller table, because listTwo was the callee
                    {
                        SubList* theSubList = new SubList();
                        theSubList = callerHashTable->Find(node2->getKey(), "NULL", "NULL", "NULL", "NULL", bucketSize, 1); 
                        if (theSubList->Exists(newRecord->originator_num))     //has communicated with both callers, insert that key in the list for both
                        {
                            subListBoth->Insert(node2->getKey());
                        }
                        theSubList = NULL;
                    }

                    node2 = node2->getNext();
                }
                //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
                cout << "        Have communicated with both of them : " << endl;
                subListBoth->PrintData();
                //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
                operationsFile >> instr;
                subListOne = NULL;
                subListTwo = NULL;
                subListBoth = NULL;
                newRecord = NULL;
                //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "topdest") == 0)
            {
                cout << "topdest  >>>>" << endl;
                int maximum = 0;
                int topCount = 0;
                char theCode[3];
                Record* newRecord = new Record();
                operationsFile >> instr;
                strcpy(newRecord->originator_num, instr);
                regionList = callerHashTable->TopDestination(newRecord->originator_num, bucketSize);
                RegionNode* node = new RegionNode();
                node = regionList->getNode();
                RegionNode* nodeAfter = new RegionNode();
                nodeAfter = regionList->getNode();
                while (node != NULL)
                {
                    if (node->getTimes() > maximum)
                    {
                        maximum = node->getTimes();
                        strcpy(theCode, node->getKey());
                    }
                    else
                    {
                        node = node->getNext();
                    }
                }
                cout << "        Top destination : " << theCode << " with times : " << maximum << endl;
                ++topCount;
                //In case of same times
                while (nodeAfter != NULL)
                {
                    if ((nodeAfter->getTimes() == maximum) && (strcmp(nodeAfter->getKey(), theCode) != 0))
                    {
                        cout << "        Top destination (also) : " << nodeAfter->getKey() << " with times : " << maximum << endl;
                        ++topCount;
                        if (topCount == 3)
                        {
                            break;
                        }
                    }
                    else
                    {
                        nodeAfter = nodeAfter->getNext();
                    }
                }
                operationsFile >> instr;
                newRecord = NULL;
                //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "top") == 0)
            {
                cout << "top  >>>>" << endl;
                Record* newRecord = new Record();
                BinaryMaxNode* altNode = new BinaryMaxNode();
                float total_income;
                float percentage, rpercentage;
                operationsFile >> instr;
                k = atoi(instr);
                total_income = binaryMaxHeap->Traverse(binaryMaxHeap->getRoot());
                percentage = (total_income * k )/ 100;
                rpercentage = total_income * percentage;
                binaryMaxHeap->TopSubs(binaryMaxHeap->getRoot(), altNode, rpercentage);
                operationsFile >> instr;
                newRecord = NULL;
                altNode = NULL;
                //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "bye") == 0)
            {
                cout << "bye  >>>>" << endl;
                delete callerHashTable;
                cout << "        Caller hash table deleted successfully!" << endl;
                delete calleeHashTable;
                cout << "        Callee hash table deleted successfully!" << endl;
                delete binaryMaxHeap;
                cout << "        Binary max heap deleted successfully!" << endl;
                afterDelete = 1;
                break;
                //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "print") == 0)
            {
                cout << "print  >>>>" << endl << endl;
                operationsFile >> instr;
                if (instr[strlen(instr) - 1]-'0' == 1)
                {
                    cout << "        Printing caller HashTable : " << endl;
                    callerHashTable->Print(bucketSize);
                }
                else
                {
                    cout << "        Printing callee HashTable : " << endl;
                    calleeHashTable->Print(bucketSize);
                }
                operationsFile >> instr;
                //cout << "********************************** " << endl << endl;
            }

            while (strcmp(instr, "dump") == 0)
            {
                operationsFile >> instr;
                operationsFile >> instr;
                operationsFile >> instr;
            }
        }
    }


    cout << "------>  Going to execute input commands " << endl;
    //UI management function in utilities
    UICommands(costFile, hashEntries_1, hashEntries_2, bucketSize);

    return 0;
}
