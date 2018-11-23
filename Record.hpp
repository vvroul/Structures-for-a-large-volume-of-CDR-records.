#ifndef RECORD
#define RECORD

class Record
{
    public:
        Record() {}
        ~Record() {}

        char* cdr_uniq_id;          //string
        char originator_num[15];    //actually 14-digit
        char destination_num[15];   //actually 14-digit
        char date[8];
        char init_time[5];
        float duration;
        int type;
        int tarrif;
        int fault_condition;
};

#endif //!RECORD
