#ifndef SupClass
#define SupClass
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <string>
#include <queue>
#include <cstring>
using namespace std;

class Location {
    public:
    int x;
    int y;
    Location();
    Location(int x, int y);
    double DistanceTo(Location* that);  //Return the distance between two points
};
class Hospital {
    public:
    string name;
    Location loc;
    int capacity;
    int max_capacity;
    int cd;
    Hospital();
    Hospital(string name, int x,int y, int capacity);
};
class ContactDetail{
    public:
    string address;
    string phone;
    string WeChat;
    string email;
    Location addr;
    ContactDetail();
    ContactDetail(string address, string phone, string WeChat, string email, Location &addr);
};

class DateTime {
    public:
    time_t cct;                     //Time stamp in long long
    DateTime();
    DateTime(int year,int month, int day);
    DateTime(int year,int month, int day, int hour, int minu, int sec);
    DateTime(string time_str);
    bool LaterThan(const DateTime &that) const;
    time_t SecLater(const DateTime &that) const;
    int YearLater(const DateTime &that) const;  //Time difference in Year
    int DayLater(const DateTime &that) const;   //Time differnce in Days
    int HourLater(const DateTime &that) const;  //Time Difference in Hour
    void AddSecond(long long sec);
    string ToString() const;
    string ToShortString() const;
    struct tm st;
};

class Person {
    public:
    string id;
    string name;
    ContactDetail ct;
    DateTime birth;
    int prof;
    vector<unsigned long>* records;
};

class Medical_State {
    public:
    unsigned long uid;
    int status;         //Status: 0:Registed, 1:Queuing, 2:Assigned, 3:Treated, 4:Withdrawn, 5:Not Registed
    int risk_gp;        //Risk Level, from 0 to 3
    int age_gp;         //Age Group, from 0 to 6;
};

class Registration {
    public:
    unsigned long uid;
    DateTime regtime;
    DateTime considertime;
    DateTime queuetime;
    int locreg;
    bool isEmergency = false;
    DateTime deadline;
};

class Treatment {
    public:
    unsigned long uid;
    DateTime assigned_time;
    DateTime treated_time;
    int type;           //treatment type
    Hospital* hospital;
};

class Record{
    public:
    unsigned long uid;
    Person* person;
    Medical_State* med;
    Registration* reg;
    Treatment* treatment;
    Record();
    Record(unsigned long uid, Person* person, Medical_State* med, Registration* reg, Treatment* treatment);
};
class Reg_FBH_Node {   // This is node for FBH
    public:
    Reg_FBH_Node();
    Reg_FBH_Node(Record* p);
    Record *p;
    Reg_FBH_Node *child;
    Reg_FBH_Node *parent;
    Reg_FBH_Node *left;
    Reg_FBH_Node *right;
    bool marked = false;
    bool deleted = false;
    int degree;
};

class FBH {
    public:
    FBH(bool (*pr) (const Record *, const Record *));
    Record* Max() const;
    Reg_FBH_Node* Enqueue(Record* toeq);
    Record* Dequeue();
    Record* Delete(Reg_FBH_Node* todel);
    bool isEmpty();
    void UpdateNode(Reg_FBH_Node* handle);
    private:
    Reg_FBH_Node* max;
    void Consolidate();
    void Conscut(Reg_FBH_Node* p);
    void Cut(Reg_FBH_Node* p);
    int count;
    bool PriorThan (const Reg_FBH_Node* a, const Reg_FBH_Node *b) const;
    bool (*Priority_Rule) (const Record* , const Record* );
    const double logp = 0.4812118250596;
};

class RecordPriority {
    public:
    static bool Priority_Rule1(const Record *a,const Record *b);
    static bool Priority_Rule2(const Record *a,const Record *b);
    static bool Priority_Rule3(const Record *a,const Record *b);
    const static long treattime[3];
    const static long waittime = 14400;
    const static long penaltytime = 1209600; //Twoweek month penal time;
    const static long riskdelay = 2592000; //Highrisk Delay;
};

struct Letter {
    unsigned long uid;
    DateTime deadline;
};
extern Hospital* BestHospital(Hospital* hospitals,Location* loc,int size);
extern vector<Registration*>* Give_reg_vector(const char* path);
extern int Age_Group_get( int age );
extern vector<struct Letter>* Give_letter_info(const char* path);
extern vector<Registration*>* Give_uidreg_vector(const char* path);
#endif
