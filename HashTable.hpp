#ifndef HASH_TABLE
#define HASH_TABLE

#include "Record.hpp"
#include "DeList.hpp"
#include "Utilities.hpp"
#include "Region.hpp"
#include "SubList.hpp"
//#include "RestRecord.hpp"
//#define hashSize 20

class HashTable
{

    public :
        HashTable();
        HashTable(int hashSize);
        ~HashTable();
        int HashFunction(char* key);
        void PutRecord(char* key, char* restKey, RestRecord* newRecord, int bucketSize);
        void Remove(char* key, char* id, int bucketSize);
        SubList* Find(char* key, char* time1, char* year1, char* time2, char* year2, int bucketSize, int appear);
        SubList* LookUp(char* key, char* time1, char* year1, char* time2, char* year2, int bucketSize, int appear);
        void Print(int bucketSize);
        int Search(char* key, int bucketSize);
        Region* TopDestination(char* key, int bucketSize);
        void setList(HashList* table);
        void setArray(HashList** hashArray);
      	HashList* getList();
        HashList** getArray();

    private :
        int hashSize;
        HashList** hashArray;
};

#endif
