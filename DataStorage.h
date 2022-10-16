#ifndef DataST
#define DataST
#include "SupClass.h"
#include "BPlus.h"
#include "BPlus_multi.h"
class DataStorage {
    public:
    static BPlusTree<unsigned long, Record> recstore_uid;
    static BPlus_multi<string, Record> recstore_id;
    static BPlus_multi<int, Record> recstore_prof;
    static BPlus_multi<int, Record> recstore_status;
    static BPlus_multi<int, Record> recstore_riskgp;
    static BPlus_multi<int, Record> recstore_agegp;
    static BPlus_multi<DateTime, Record> recstore_regtime;
    static BPlus_multi<DateTime, Record> recstore_quetime;
    static BPlus_multi<int, Record> recstore_treattp;
    static BPlus_multi<DateTime, Record> recstore_asstime;
    static BPlus_multi<DateTime, Record> recstore_tretime;
    static BPlusTree<string, Person> personstore_ID;
    static BPlus_multi<DateTime, Person> personstore_birth;
    static BPlus_multi<int, Person> personstore_prof;
    const static string maxstring;
    const static string minstring;
    const static DateTime maxDateTime;
    const static DateTime minDateTime;
    const static unsigned long maxULong = 4294967295;
    const static unsigned long minULong = 0; 
    const static int maxInt = 2147483647;
    const static int minInt = -2147483648;
    const static int degree = 8;
    static int CompareTo_ULong(const unsigned long & a, const unsigned long & b);
    static int CompareTo_int(const int & a, const int & b);
    static int CompareTo_string(const string & a, const string & b);
    static int CompareTo_DateTime(const DateTime & a, const DateTime & b);
    static bool IsValid_id(const string & a, const Record* b);
    static bool IsValid_prof(const int & a, const Record* b);
    static bool IsValid_status(const int & a, const Record* b);
    static bool IsValid_riskgp(const int & a, const Record* b);
    static bool IsValid_agegp(const int & a, const Record* b);
    static bool IsValid_treattp(const int & a, const Record* b);
    static bool IsValid_regtime(const DateTime & a, const Record* b);
    static bool IsValid_quetime(const DateTime & a, const Record* b);
    static bool IsValid_asstime(const DateTime & a, const Record* b);
    static bool IsValid_tretime(const DateTime & a, const Record* b);
    static bool IsValid_birth(const DateTime & a, const Person* b);
    static bool IsValid_p_prof(const int & a, const Person* b);
};
#endif