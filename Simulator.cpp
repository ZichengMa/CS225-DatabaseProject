#include "Simulator.h"
#include "DataStorage.h"
#include "Acsctrl.h"
using namespace std;
typedef list<Record*>::iterator lit;
typedef map<Record*,Reg_FBH_Node*>::iterator mapit;

void Simulator::Withdraw(Record * k){
    blk_list.insert(k->person);
    k->treatment->treated_time = cond.now;
    DataStorage::recstore_tretime.Insert(cond.now,k); //For withdrawn people, treatment time is the time they withdrew.
}


Simulator::Simulator(Sim_Condition cond){
    this->cond = cond;
    center[0] = new FBH(RecordPriority::Priority_Rule1);
    center[1] = new FBH(RecordPriority::Priority_Rule2);
    center[2] = new FBH(RecordPriority::Priority_Rule3);
    hosset[0] = Hospital("McKenly Health Center",12,26,10);
    hosset[1] = Hospital("St. John Hospital",31,11,10);
    hosset[2] = Hospital("St. George Hospital",25,8,10);
    lastdump = cond.now;
}

Simulator::~Simulator(){
    for (mapit i = heapset.begin(); i != heapset.end(); i++){
        delete i->second;
    }
    delete center[0];
    delete center[1];
    delete center[2];
}

Sim_Status Simulator::Tick() {
    list<Record*>* newrec = &Acsctrl::newrecords;
    list<Record*>* updrec = &Acsctrl::updatedrecords;

    Sim_Status rtv;
    vector<string>* returnst = new vector<string>;
    
    //Load new records;
    for (lit i = newrec->begin(); i != newrec->end();) {
        Record* temp = *i;
        if (!temp->reg->regtime.LaterThan(cond.now)) {
            temp->med->status = 0;
            temp->reg->locreg = lq_counter;
            DataStorage::recstore_status.Insert(temp->med->status,temp); //Insert an idex!
            locset[lq_counter].emplace(temp);
            temp->reg->considertime = temp->reg->regtime;
            if (blk_list.find(temp->person) != blk_list.end()) {
                if ((temp->med->risk_gp == 0)||(temp->med->risk_gp == 1))  {
                    temp->reg->considertime.AddSecond(RecordPriority::penaltytime);
                }
                blk_list.erase(temp->person);
            }
            if (temp->med->risk_gp == 2) temp->reg->considertime.AddSecond(RecordPriority::riskdelay);      //Define considered time here
            lq_counter = (lq_counter + 1) % lq_tt;      //increment the local reg counter
            lit t = i;
            i++;
            newrec->erase(t);                           //remove the record from the list
        }else{
            i++;
        }
    }

    //Update records;
    for (lit i = updrec->begin(); i != updrec->end(); i++) {
        Record* temp = *i;
        mapit tpfind = heapset.find(temp);
        if (tpfind !=heapset.end()) {
                //In the heap
            FBH* opheap = center[temp->treatment->type];
            if (temp->med->status == 4) {
                                //Withdrawn
                opheap->Delete(tpfind->second);
                heapset.erase(tpfind);      //Erase it from the FBH and the helper mapping.
                Withdraw(temp);             //Withdraw
            }else{
                //Not Withdraw;
                opheap->UpdateNode(tpfind->second);
            }
        } else {
            //Not in the heap
            if (temp->med->status == 4) Withdraw(temp);     //Withdraw
        }
    }
    updrec->clear();
    //Check If to dump the queue.
    if (cond.now.cct - lastdump.cct >= dump_interval) {
        for (int i = 0; i < lq_tt; i++) {
            while (!locset[i].empty()) {
                Record* temp = locset[i].front();
                if (temp->med->status != 0) {
                    locset[i].pop();
                    continue;
                }
                heapset.emplace(pair<Record*,Reg_FBH_Node*>(temp,center[temp->treatment->type]->Enqueue(temp)));    //Put the record into the queue;
                temp->reg->queuetime = cond.now;
                DataStorage::recstore_quetime.Insert(cond.now,temp);        //Update the timestamp of entering the queue.
                temp->med->status = 1;
                DataStorage::recstore_status.Insert(temp->med->status,temp);  //Update Status
                locset[i].pop();
            }
        }
        lastdump = cond.now;
    }

    //Assign the Patient
    while(!heapset.empty()) {
        while (center[fbh_counter]->isEmpty()) fbh_counter = (fbh_counter + 1) % 3;
        Record* maxp = center[fbh_counter]->Max();
        Hospital* bhosp = BestHospital(hosset,&maxp->person->ct.addr,hos_tt);
        if (bhosp == nullptr) break;    //No hospital available, break!
        bhosp->capacity = bhosp->capacity - 1;
        maxp->med->status = 2;
        DataStorage::recstore_status.Insert(2, maxp);       //New status!
        maxp->treatment->assigned_time = cond.now;
        maxp->treatment->hospital = bhosp;
        heapset.erase(maxp);
        center[fbh_counter]->Dequeue();     //Remove that from the center queue
        DateTime assdue = DateTime(cond.now);
        assdue.AddSecond(RecordPriority::waittime);
        returnst->push_back("Paitent with ID " + (maxp->person->id) + " shall go to " + bhosp->name + " by " + assdue.ToString());
        fbh_counter = (fbh_counter + 1) % 3;
    }

    //For Assigned records
    vector<Record*>* assigned = DataStorage::recstore_status.Find(2);
    if (assigned != nullptr){
        for (unsigned long i = 0; i < assigned->size(); i++) {
            Record* temp = assigned->at(i);
            if (cond.now.cct > temp->treatment->assigned_time.cct + RecordPriority::treattime[temp->treatment->type]+RecordPriority::waittime) {
                temp->treatment->treated_time = cond.now;
                DataStorage::recstore_tretime.Insert(cond.now,temp);
                temp->med->status = 3;
                DataStorage::recstore_status.Insert(3,temp);
                temp->treatment->hospital->capacity ++;
            }
        }
        delete assigned;
    }

    cond.now.AddSecond(cond.step);
    rtv.now = cond.now.ToString();
    rtv.status_add = returnst;
    return rtv;
}

void Simulator::UpdateCond(Sim_Condition cond) {
    this->cond = cond;
}
//from record select (all) where ((status == 5) and (1<= agegp <= 2))
