#ifndef REGION_H
#define REGION_H


class RegionNode
{
    public:
        RegionNode();
        ~RegionNode();
        char* getKey();
        int getTimes();
        void setNext(RegionNode* next);
        void setKey(char* key);
        void setTimes(int times);
        RegionNode* getNext();
        
    private:
        char key[3];
        int times;
        RegionNode* next;
};


class Region
{
    public:
        Region();
        ~Region();
        int Exists(char* item);
        void PrintData();
        void Insert(char* item);
        RegionNode* Search(char* item);
        RegionNode* getNode();
    
    private:
        RegionNode* header;
};



#endif //!REGION_H
