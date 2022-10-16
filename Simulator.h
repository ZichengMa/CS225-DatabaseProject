#ifndef SIM_HEAD
#define SIM_HEAD
#include "SupClass.h"
#include "DataStorage.h"
#include <list>
#include <queue>
#include <set>
#include <map>
struct Sim_Status {
    string now;
    vector<string>* status_add;
};

struct Sim_Condition {
    DateTime now;
    unsigned long step;
};

class Simulator {
    public:
    Simulator(Sim_Condition cond);
    ~Simulator();
    Sim_Status Tick();
    void UpdateCond(Sim_Condition cond);
    private:
    Sim_Condition cond;
    FBH* center[3];
    Hospital hosset[3];
    std::queue<Record*> locset[3];
    std::map<Record*,Reg_FBH_Node*> heapset;
    std::set<Person*> blk_list;
    int lq_counter = 0;
    int fbh_counter = 0;
    const int lq_tt = 3;
    const int hos_tt = 3;
    const long dump_interval = 43200;
    void Withdraw(Record * k);
    DateTime lastdump;
};

#endif
