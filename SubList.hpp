#ifndef SUBLIST_H
#define SUBLIST_H


class SubNode
{
    public:
        SubNode();
        ~SubNode();
        char* getKey();
        int getTimes();
        void setNext(SubNode* next);
        void setKey(char* key);
        void setTimes(int times);

        SubNode* getNext();
        
    private:
        char key[15];
        int times;
        SubNode* next;
};


class SubList
{
    public:
        SubList();
        ~SubList();
        int Exists(char* item);
        int getCall();
        void PrintData();
        void setCall(int call);
        void Insert(char* item);
        SubNode* Search(char* item);
        SubNode* getNode();
    
    private:
        SubNode* header;
        int call;
};


#endif //!SUBLIST_H