#ifndef REST_RECORD
#define REST_RECORD

class RestRecord
{
    public:
        RestRecord() {}
        ~RestRecord() {}

        char* cdr_uniq_id;      
        char num[15];           //actually 14-digit
        char date[8];
        char init_time[5];
        float duration;
        int type;
        int tarrif;
        int fault_condition;
};

#endif //!REST_RECORD
