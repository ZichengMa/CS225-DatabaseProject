#ifndef Acs
#define Acs
#include <stack>
#include <map>
#include <algorithm>
#include <list>
#include "SupClass.h"
using namespace std;
class Acsctrl {
    public:
    const static map<int,string> item_code; // Finding the item by knowing its code
    const static map<string,int> cmdset; // A command set for finding the key word
    const static int RECORDF[11];        // For the field selection, RECORD has at most 11 fields to select.
    const static int PERSONF[4];         // For the field selection, PERSON has at most 11 fields to select.
    static unsigned long UID;            // Total UID
    static bool STATEMENT_FAULT;         // Judge whether statement in WHERE is wrong.
    static int Find_Command(char* k);    // find the command code
    static int priority(string oper);    // Priority for the 
    static list<Record*> newrecords;       // List the pointers of new records
    static list<Record*> updatedrecords;   // List the pointers of updated records
    static vector<vector<string>*>* Search(string instr); // Searching for the instructions.
    static vector<Record*>* Processwhere(int db, int l, char* internbuff); // Processing the statement which follows "WHERE".
    static vector<Person*>* Processwherep(int db, int l, char* internbuff); // Processing the statement which follows "WHERE".
    static void Merge(stack<string>& Ope, stack<vector<Record*>*>& Num); // Merge two sets.
    static void Merge(stack<string>& Ope, stack<vector<Person*>*>& Num); // Merge two sets.
    static vector<string>* readFile(const char* path);  // Read csv file.
    static int Put(string filename);
    static int UpdateData(string filename);
    static bool cmpr_uid(Record* x, Record *y);  // Comparison functions. Details are described in "Comparison.cpp"
    static bool cmpr_name(Record* x, Record* y);
    static bool cmpr_id(Record* x, Record *y);
    static bool cmpr_prof(Record* x, Record* y);
    static bool cmpr_status(Record* x, Record* y);
    static bool cmpr_riskgp(Record* x, Record* y);
    static bool cmpr_agegp(Record* x, Record* y);
    static bool cmpr_regat(Record* x, Record* y);
    static bool cmpr_regtime(Record* x, Record* y);
    static bool cmpr_quetime(Record* x, Record* y);
    static bool cmpr_treattp(Record* x, Record* y);
    static bool cmpr_asstime(Record* x, Record* y);
    static bool cmpr_tretime(Record* x, Record* y);
    static bool cmpr_ddl(Record* x, Record* y);
    static bool cmpr_uid1(Record* x, Record *y);
    static bool cmpr_name1(Record* x, Record* y);
    static bool cmpr_id1(Record* x, Record *y);
    static bool cmpr_prof1(Record* x, Record* y);
    static bool cmpr_status1(Record* x, Record* y);
    static bool cmpr_riskgp1(Record* x, Record* y);
    static bool cmpr_agegp1(Record* x, Record* y);
    static bool cmpr_regat1(Record* x, Record* y);
    static bool cmpr_regtime1(Record* x, Record* y);
    static bool cmpr_quetime1(Record* x, Record* y);
    static bool cmpr_treattp1(Record* x, Record* y);
    static bool cmpr_asstime1(Record* x, Record* y);
    static bool cmpr_tretime1(Record* x, Record* y);
    static bool cmpr_ddl1(Record* x, Record* y);

    static bool cmpp_name(Person* x, Person* y);
    static bool cmpp_id(Person* x, Person *y);
    static bool cmpp_birth(Person* x, Person *y);
    static bool cmpp_prof(Person* x, Person *y);
    static bool cmpp_name1(Person* x, Person* y);
    static bool cmpp_id1(Person* x, Person *y);
    static bool cmpp_birth1(Person* x, Person *y);
    static bool cmpp_prof1(Person* x, Person *y);
    static string d_to_s(DateTime D);
};
#endif