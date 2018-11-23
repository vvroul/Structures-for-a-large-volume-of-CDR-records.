#ifndef DELIST_H
#define DELIST_H

#include "RestRecord.hpp"

class DeList
{
    public:
        DeList(int size);
        ~DeList();
        RestRecord** getArray();
        void setArray(RestRecord** pointerRecordArray);

    private:
        RestRecord** pointerRecordArray;

};


#endif //!DELIST_H
