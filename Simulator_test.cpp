#include "Simulator.h"
#include <list>
#include <vector>
#include "Acsctrl.h"
using namespace std;
typedef list<Record*>::iterator lit;
int main() {
    Sim_Condition cond;
    cond.now = DateTime(1970,1,1,0,0,0);
    cond.step = 3600;
    Simulator smain(cond);

    //Add Files!!
    int errcd = Acsctrl::Put("di3.CSV");

    switch (errcd)
    {
    case 1:
        std::cout<<"File Not Found!"<<std::endl;
        return -1;
        break;
    case 2:
        std::cout<<"Failed to load the file"<<std::endl;
        return -1;
        break;
    default:
        break;
    }
    for (int i = 0; i < 30; i++) {
        Sim_Status u = smain.Tick();
        cout << u.now<<endl;
        for (int j = 0; j < u.status_add->size(); j++) {
            std::cout << u.status_add->at(j)<<std::endl;
        }
        delete u.status_add;
    }
    // list<Record*>* nr = new list<Record*>(Acsctrl::newrecords);
    // Acsctrl::newrecords.clear();
    // for (lit i = nr->begin(); i != nr->end(); i++){
    //     (*i)->med->status = 0;
    //     DataStorage::recstore_status.Insert(0,*i);
    // }
    // for (lit i = nr->begin(); i != nr->end(); i++){
    //     (*i)->med->status = 1;
    //     DataStorage::recstore_status.Insert(1,*i);
    // }
    // for (lit i = nr->begin(); i != nr->end(); i++){
    //     (*i)->med->status = 2;
    //     DataStorage::recstore_status.Insert(2,*i);
    // }
    // vector<Record*>* fc = DataStorage::recstore_status.Find(2);
    


    // for (int i = 0; i < 30; i++) {
    //     Sim_Status u = smain.Tick();
    //     std::cout <<"----------\n";
    //     std::cout << u.now <<std::endl;
    //     std::cout <<"----------\n";
    //     for (int j = 0; j < u.status_add->size(); i++) {
    //         std::cout << u.status_add->at(j)<<std::endl;
    //     }
    //     std::cout <<"__________\n";
    //     delete u.status_add;
    // }
}