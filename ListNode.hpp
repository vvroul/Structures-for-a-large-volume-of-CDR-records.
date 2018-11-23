#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "RestRecord.hpp"
#define restCdrSize 128

class ListNode
{
    public:
        ListNode();
        //ListNode(int size);
        ~ListNode();
        int getTimes();
        ListNode* getNext();
        RestRecord* getRecordArray();
        void setTimes(int times);
        void setNext(ListNode* next);
        void setRecordArray(RestRecord* recordArray);
        void PrintData();

    private:
        RestRecord* recordArray;
        ListNode* next;
        int times;

};


#endif //!LIST_NODE_H
