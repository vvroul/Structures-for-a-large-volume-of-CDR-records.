#include "Headers.hpp"
#include "HashTable.hpp"
#include "BinaryMaxHeap.hpp"


void UICommands(ifstream& costFile, int hashEntries_1, int hashEntries_2, int bucketSize)
{
    std::cout << "       ********************** In UICommands function **********************" << '\n';

    ifstream tempFile;
    tempFile.open("temp.txt");   //temporary file that holds input commands
    if (tempFile == NULL)
    {
        cout << "You've given a wrong temporary file." << endl;
        exit(1);
    }

    if (is_empty(tempFile))
    {
        cout << "        No UI input commands given. Returning..." << endl;
        return;
    }

    //Structures Creation
    HashTable* callerHashTable = new HashTable(hashEntries_1);
    HashTable* calleeHashTable = new HashTable(hashEntries_2);
    BinaryMaxHeap* binaryMaxHeap = new BinaryMaxHeap();
    Region* regionList = new Region();

    int k;
    int uiChoice;
    int afterDelete = 0;
    char instr[150];
    char instr2[150];
    char* token = new char[500]();
    
    while (!tempFile.eof())
    {
        tempFile >> instr;
        while (strcmp(instr, "insert") == 0)
        {
            cout << "insert  >>>>" << endl;
            Record* newRecord = new Record();
            RestRecord* newRestRecord = new RestRecord();
            BinaryMaxNode* maxNode = new BinaryMaxNode();
            float theCost;
            
            tempFile >> instr;
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
                maxNode->setCost(theCost);
                binaryMaxHeap->Insert(maxNode);                                                                                    //insert to max heap
            }
            tempFile >> instr;
            newRecord = NULL;
            newRestRecord = NULL;
            maxNode = NULL;
            //cout << "********************************** " << endl << endl;
        }

        while (strcmp(instr, "delete") == 0)
        {
            cout << "delete  >>>>" << endl;
            Record* newRecord = new Record();
            tempFile >> instr;
            strcpy(newRecord->originator_num, instr);
            tempFile >> instr;
            newRecord->cdr_uniq_id = instr;
            callerHashTable->Remove(newRecord->originator_num, newRecord->cdr_uniq_id, bucketSize);
            tempFile >> instr;
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
            tempFile >> instr;
            strcpy(newRecord->originator_num, instr);
            tempFile >> instr;
            if (strchr(instr, ':'))                    //time_1 is defined
            {
                strcpy(newRecord->init_time, instr);
                strcpy(tempTime, instr);
                tempFile >> instr;
                if (strchr(instr, ':'))                //time_2 is defined, we don't have years
                {
                    strcpy(newRecord->init_time, instr);
                    callerHashTable->Find(newRecord->originator_num, tempTime, "NULL", newRecord->init_time, "NULL", bucketSize, 0);      //calling find with times only
                    tempFile >> instr;
                }
                else                                   //year_1 is defined
                {
                    strcpy(newRecord->date, instr);
                    strcpy(tempDate, newRecord->date);
                    tempFile >> instr;                 //time_2 is next
                    strcpy(newRecord->init_time, instr);
                    strcpy(tempTime1, newRecord->init_time);
                    tempFile >> instr;                 //year_2 is next
                    strcpy(newRecord->date, instr);
                    callerHashTable->Find(newRecord->originator_num, tempTime, tempDate, tempTime1, newRecord->date, bucketSize, 0);      //calling find with times and years
                    tempFile >> instr;
                }
            }
            else if (isdigit(instr[0]))
            {
                strcpy(newRecord->date, instr);
                strcpy(tempDate, newRecord->date);
                tempFile >> instr;
                strcpy(newRecord->date, instr);
                callerHashTable->Find(newRecord->originator_num, "NULL", tempDate, "NULL", newRecord->date, bucketSize, 0);               //calling find with years only
                tempFile >> instr;
            }
            else
            {
                callerHashTable->Find(newRecord->originator_num, "NULL", "NULL", "NULL", "NULL", bucketSize, 0);                          //calling find with no restrictions
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
           tempFile >> instr;
           strcpy(newRecord->destination_num, instr);
           tempFile >> instr;
           if (strchr(instr, ':'))                     //time_1 is defined
           {
               strcpy(newRecord->init_time, instr);
               strcpy(tempTime, instr);
               tempFile >> instr;
               if (strchr(instr, ':'))                 //time_2 is defined, we don't have years
               {
                   strcpy(newRecord->init_time, instr);
                   calleeHashTable->LookUp(newRecord->destination_num, tempTime, "NULL", newRecord->init_time, "NULL", bucketSize, 0);    //calling lookup with times only
                   tempFile >> instr;
               }
               else                                    //year_1 is defined
               {
                   strcpy(newRecord->date, instr);
                   strcpy(tempDate, newRecord->date);
                   tempFile >> instr;                  //time_2 is next
                   strcpy(newRecord->init_time, instr);
                   strcpy(tempTime1, newRecord->init_time);
                   tempFile >> instr;                  //year_2 is next
                   strcpy(newRecord->date, instr);
                   calleeHashTable->LookUp(newRecord->destination_num, tempTime, tempDate, tempTime1, newRecord->date, bucketSize, 0);    //calling lookup with times and years
                   tempFile >> instr;
               }
           }
           else if (isdigit(instr[0]))
           {
               strcpy(newRecord->date, instr);
               strcpy(tempDate, newRecord->date);
               tempFile >> instr;
               strcpy(newRecord->date, instr);
               calleeHashTable->LookUp(newRecord->destination_num, "NULL", tempDate, "NULL", newRecord->date, bucketSize, 0);             //calling lookup with years only
               tempFile >> instr;
           }
           else
           {
               calleeHashTable->LookUp(newRecord->destination_num, "NULL", "NULL", "NULL", "NULL", bucketSize, 0);                        //calling lookup no restrictions
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
            tempFile >> instr;
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
            tempFile >> instr;
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
            tempFile >> instr;
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
            char theCode[3];
            int topCount = 0;
            Record* newRecord = new Record();
            tempFile >> instr;
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
            tempFile >> instr;
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
            tempFile >> instr;
            k = atoi(instr);
            total_income = binaryMaxHeap->Traverse(binaryMaxHeap->getRoot());
            percentage = (total_income * k ) / 100;
            rpercentage = total_income * percentage;
            binaryMaxHeap->TopSubs(binaryMaxHeap->getRoot(), altNode, rpercentage);
            tempFile >> instr;
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
            tempFile >> instr;
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
            tempFile >> instr;
            //cout << "********************************** " << endl << endl;
        }

        while (strcmp(instr, "dump") == 0)
        {
            tempFile >> instr;
            tempFile >> instr;
            tempFile >> instr;
        }
    }
}



int TimeSearch(char* time1, char* time2, HashList* list, int pos)
{

    char timeToCheck[5];
    char* token1 = new char[20]();
    char* token2 = new char[20]();
    char* tokenCheck = new char[20]();
    char* tookie1 = new char[20]();
    char* tookie2 = new char[20]();
    char* tookieCheck = new char[20]();
    int cdrsFound = 0;

    token1 = strtok(time1, ":");
    tookie1 = strtok(NULL, ":");
    token2 = strtok(time2, ":");
    tookie2 = strtok(NULL, ":");

    for (int i = 0; i < restCdrSize; ++i)
    {
        if (list->getBucketArray()[pos].getCdrPointer() != NULL)
        {
            if(strcmp(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time, "NULL") != 0)                  //Time exists 
            {
                ++cdrsFound;
                strcpy(timeToCheck, list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time);
                tokenCheck = strtok(timeToCheck, ":");
                tookieCheck = strtok(NULL, ":");

                if ((atoi(token1) < atoi(token2)))                                                                               //Time 1 < Time 2
                {
                    if ((atoi(tokenCheck) > atoi(token1)) && (atoi(tokenCheck) < atoi(token2)))                                  //Valid time between
                    {
                        cout << "        ---Valid time" << endl;
                        cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                        continue;
                    }
                    else if ((atoi(tokenCheck) == atoi(token1)) && (atoi(tokenCheck) < atoi(token2)))                            //Valid time equal to time1 and less or equal in minutes
                    {
                        if (tookieCheck[0]-'0' > tookie1[0]-'0')
                        {
                            cout << "        ---Valid time (for minutes)" << endl;
                            cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                            //return 0;
                            continue;
                        }
                        else if (tookieCheck[0]-'0' == tookie1[0]-'0')
                        {
                            if (tookieCheck[1]-'0' >= tookie1[1]-'0')
                            {
                                cout << "        ---Valid time (for seconds)" << endl;
                                cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                                //return 0;
                                continue;
                            }
                            else
                            {
                                cout << "        ---Invalid time (for seconds)." << endl;
                                //return -1;
                                continue;
                            }
                        }
                        else
                        {
                            cout << "        ---Invalid time (for minutes)." << endl;
                            //return -1;
                            continue;
                        }
                    }
                    else if ((atoi(tokenCheck) > atoi(token1)) && (atoi(tokenCheck) == atoi(token2)))                            //Valid time equal to time2 and less or equal in minutes
                    {
                        if (tookieCheck[0]-'0' < tookie2[0]-'0')
                        {
                            cout << "        ---Valid time (for minutes)" << endl;
                            cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                            //return 0;
                            continue;
                        }
                        else if (tookieCheck[0]-'0' == tookie2[0]-'0')
                        {
                            if (tookieCheck[1]-'0' <= tookie2[1]-'0')
                            {
                                cout << "        ---Valid time (for seconds)" << endl;
                                cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                                //return 0;
                                continue;
                            }
                            else
                            {
                                cout << "        ---Invalid time (for seconds)." << endl;
                                //return -1;
                                continue;
                            }
                        }
                        else
                        {
                            cout << "        ---Invalid time (for minutes)." << endl;
                            //return -1;
                            continue;
                        }
                    }
                    else if ((atoi(tokenCheck) == atoi(token1)) && (atoi(tokenCheck) == atoi(token2)))  
                    {
                        if ((atoi(tookieCheck) >= atoi(tookie1)) && (atoi(tookieCheck) <= atoi(tookie2)))
                        {
                            cout << "        ---Valid time (for minutes and secs)" << endl;
                            cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                            //return 0;
                            continue;
                        }
                        else
                        {
                            cout << "        ---Invalid time." << endl;
                            //return -1;
                            continue;
                        }
                    }
                    else
                    {
                        cout << "        ---Invalid time." << endl;
                        //return -1;
                        continue;
                    }
                }
                else if ((atoi(token1) > atoi(token2)))                                                                          //time1 > time2                                                               //check only minutes
                {
                    if ((atoi(tokenCheck) > atoi(token1)) || (atoi(tokenCheck) < atoi(token2)))                                  //Valid time between
                    {
                        cout << "        ---Valid time" << endl;
                        cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                        //return 0;
                        continue;
                    }
                    else if ((atoi(tokenCheck) == atoi(token1)) || (atoi(tokenCheck) < atoi(token2)))                            //Valid time equal to time1 and less or equal in minutes
                    {
                        if (tookieCheck[0]-'0' > tookie1[0]-'0')
                        {
                            cout << "        ---Valid time (for minutes)" << endl;
                            cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                            //return 0;
                            continue;
                        }
                        else if (tookieCheck[0]-'0' == tookie1[0]-'0')
                        {
                            if (tookieCheck[1]-'0' >= tookie1[1]-'0')
                            {
                                cout << "        ---Valid time (for seconds)" << endl;
                                cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                                //return 0;
                                continue;
                            }
                            else
                            {
                                cout << "        ---Invalid time (for seconds)." << endl;
                                //return -1;
                                continue;
                            }
                        }
                        else
                        {
                            cout << "        ---Invalid time (for minutes)." << endl;
                            //return -1;
                            continue;
                        }
                    }
                    else if ((atoi(tokenCheck) > atoi(token1)) || (atoi(tokenCheck) == atoi(token2)))                            //Valid time equal to time2 and less or equal in minutes
                    {
                        if (tookieCheck[0]-'0' < tookie2[0]-'0')
                        {
                            cout << "        ---Valid time (for minutes)" << endl;
                            cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                            //return 0;
                            continue;
                        }
                        else if (tookieCheck[0]-'0' == tookie2[0]-'0')
                        {
                            if (tookieCheck[1]-'0' <= tookie2[1]-'0')
                            {
                                cout << "        ---Valid time (for seconds)" << endl;
                                cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                                cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl; 
                                //return 0;
                                continue;
                            }
                            else
                            {
                                cout << "        ---Invalid time (for seconds)." << endl;
                                //return -1;
                                continue;
                            }
                        }
                        else
                        {
                            cout << "        ---Invalid time (for minutes)." << endl;
                            //return -1;
                            continue;
                        }
                    }
                    else
                    {
                        cout << "        ---Invalid time." << endl;
                        //return -1;
                        continue;
                    }
                }
                else
                {
                    if ((atoi(tookieCheck) >= atoi(tookie1)) && (atoi(tookieCheck) <= atoi(tookie2)))
                    {
                        cout << "        ---Valid time (for minutes and secs)" << endl;
                        cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                        //return 0;
                        continue;
                    }
                    else
                    {
                        cout << "        ---Invalid time." << endl;
                        //return -1;
                        continue;
                    }
                }
            }
        }
    }
    if (cdrsFound == 0)
    {
        return -2;
    }
    return 0;
}



int YearSearch(char* year1, char* year2, HashList* list, int pos)
{

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
    int cdrsFound = 0;

    strcpy(tok1, year1);
    strcpy(tok2, year2);
    strcpy(token1, &year1[strlen(year1) - 4]); 
    strcpy(token2, &year2[strlen(year2) - 4]); 
    strcpy(tookie1, &year1[strlen(year1) - 6]); 
    strcpy(tookie2, &year2[strlen(year2) - 6]); 
    
    tookie1[strlen(tookie1) - 4] = '\0';
    tookie2[strlen(tookie2) - 4] = '\0';
    tok1[strlen(tok1) - 6] = '\0';
    tok2[strlen(tok2) - 6] = '\0';
    
    for (int i = 0; i < restCdrSize; ++i)                       
    {
        if(strcmp(list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date, "NULL") != 0)            //Date exists //compare years, compare months, compare days 
        {
            ++cdrsFound;
            strcpy(yearToCheck, list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date);
            yearToCheck[strlen(yearToCheck) - 5] = '\0';
            strcpy(tokC, yearToCheck);
            strcpy(tokenCheck, &yearToCheck[strlen(yearToCheck) - 4]);
            strcpy(tookieCheck, &yearToCheck[strlen(yearToCheck) - 6]); 
            tookieCheck[strlen(tookieCheck) - 4] = '\0';
            tokC[strlen(tokC) - 6] = '\0';

            if (atoi(token1) < atoi(token2))
            {
                if ((atoi(tokenCheck) > atoi(token1)) && (atoi(tokenCheck) < atoi(token2)))
                {
                    cout << "        ---Valid date" << endl;
                    cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                    //return 0;
                }
                else if ((atoi(tokenCheck) == atoi(token1)) && (atoi(tokenCheck) < atoi(token2)))
                {
                    //Check month
                    if ((atoi(tookieCheck) > atoi(tookie1)))
                    {
                        cout << "        ---Valid date (month)" << endl;
                        cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                        //return 0;
                    }
                    else if ((atoi(tookieCheck) == atoi(tookie1)))
                    {
                        //Check day
                        if ((atoi(tokC) >= atoi(tok1)))
                        {
                            cout << "        ---Valid date (day)" << endl;
                            cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                            //return 0;
                        }
                        else
                        {
                            cout << "        ---Invalid date (day)" << endl;
                            //return -1;
                        }
                    }
                    else
                    {
                        cout << "        ---Invalid date (month)" << endl;
                        //return -1;
                    }
                }
                else if ((atoi(tokenCheck) > atoi(token1)) && (atoi(tokenCheck) == atoi(token2)))
                {
                    //Check month
                    if ((atoi(tookieCheck) < atoi(tookie2)))
                    {
                        cout << "        ---Valid date (month)" << endl;
                        cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                        //return 0;
                    }
                    else if ((atoi(tookieCheck) == atoi(tookie2)))
                    {
                        //Check day
                        if ((atoi(tokC) <= atoi(tok2)))
                        {
                            cout << "        ---Valid date (day)" << endl;
                            cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                            cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                            //return 0;
                        }
                        else
                        {
                            cout << "        ---Invalid date (day)" << endl;
                            //return -1;
                        }
                    }
                    else
                    {
                        cout << "        ---Invalid date (month)" << endl;
                        //return -1;
                    }
                }
                else 
                {
                    cout << "        ---Invalid date." << endl;
                    //return -1;
                }
            }
            else if (atoi(token1) == atoi(token2))
            {
                //cout << "Same year. Search in month, day." << endl; 
                //Check month
                if ((atoi(tookieCheck) > atoi(tookie1)) && (atoi(tookieCheck) < atoi(tookie2)))
                {
                    cout << "        ---Valid date (month)" << endl;
                    cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                    cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                    //return 0;
                }
                else if ((atoi(tookieCheck) == atoi(tookie1)) && (atoi(tookieCheck) == atoi(tookie2))) 
                {
                    //Check day
                    if  (((atoi(tokC) >= atoi(tok1))) && ((atoi(tokC) <= atoi(tok2))))
                    {
                        cout << "        ---Valid date (day)" << endl;
                        cout << "        " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].cdr_uniq_id;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].num;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].date;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].init_time;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].duration;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].type;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].tarrif;
                        cout << " " << list->getBucketArray()[pos].getCdrPointer()->getRecordArray()[i].fault_condition << endl;
                        //return 0;
                    }
                    else
                    {
                        cout << "        ---Invalid date (day)" << endl;
                        //return -1;
                    }
                }
                else
                {
                    cout << "        ---Invalid date (month)" << endl;
                    //return -1;
                }
            }
            else 
            {
                cout << "        ---Invalid dates, nothing to search here" << endl;
                //return -1;
            }
        }
    }
    if (cdrsFound == 0)
    {
        return -2;
    }
    return 0;
}



float CalCost(RestRecord* newRecord, ifstream& costFile)
{
    float cost;
    float total_cost;
    int firstDig;
    int ftype, ftarrif;
    char instr[20];

    firstDig = newRecord->fault_condition;

    while(firstDig >= 10)
    {
        firstDig = firstDig / 10;
    }

    if (firstDig == 2)
    {
        costFile >> instr;
        costFile >> instr; 
        costFile >> instr; 
        costFile >> instr; 
        while (!costFile.eof())
        {
            //cout << "Valid call." << endl;
            ftype =  instr[0] - '0';
            ftarrif =  instr[2] - '0';
            cost = strtof(&instr[strlen(instr) - 3], NULL);

            if ((newRecord->type == ftype) && (newRecord->tarrif == ftarrif))
            {
                if (ftype == 0)
                {
                    total_cost = cost;
                    //cout << "Total cost 0 : " << total_cost << endl;
                    costFile.clear();
                    costFile.seekg(0, ios::beg);
                    return total_cost;
                }
                else
                {
                    total_cost = cost * newRecord->duration;
                    //cout << "Total cost dur : " << total_cost << endl;
                    costFile.clear();
                    costFile.seekg(0, ios::beg);
                    return total_cost;
                }
            } 
            costFile >> instr;
        }
    }
    else
    {
        //cout << "Invalid call." << endl;
        cost = 0.0;
        return cost;
    }
}



bool is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}