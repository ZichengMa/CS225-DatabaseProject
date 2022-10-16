#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "DataStorage.h"
#include "Acsctrl.h"
using namespace std;
/*
*    readFile: build a vector containing all strings in a csv file
*    INPUT: None
*    RETURN: a pointer to vector for strings
*/
vector<string>* Acsctrl::readFile(const char* path){
    ifstream ifs;
    ifs.open(path, ios::in);
    vector<string> *v = new vector<string>; // Build a vector for strings
    if (ifs.is_open()) // successfully opened
    {
        string buf; // a buffer
        getline(ifs,buf);
        while (getline(ifs, buf)){ // successfully get a line
            v->push_back(buf); // add current string to the vector
        }
    }
    return v;
}
/*
*	Put(string) -- Adding an original CSV table to insert to the B+tree.
*	INPUT -- filename, the CSV file to read.
*	RETURN -- 0 for success, 1 for file not found, 2 for other errors.
*	SIDE EFFECT -- None
*/
int Acsctrl::Put(string filename){
    newrecords.clear();
	fstream _file;
	_file.open(filename,ios::in);
	if(!_file){
		return 1;
	}
	_file.close();
	ifstream line_in(filename);
    string date, time, name, id, address, cord, phone, WeChat, email, prof, Birthday, Risk, TreatType;
    string line;
    int temp[6], xy[2];
    int i, j;
	// In the following lines, I will read the item seperated by the commas with the order stated in "Implement_std.md".
	vector<string>* content = readFile(filename.c_str());
	int size = (*content).size();
    int STATUS = 5; // All the people in this table have not been registered.
    Person *PS, *PS1;
    for(j = 0; j < size; j++){
        line = (*content)[j];
        istringstream line_in(line);
        getline(line_in, date, ',');
        getline(line_in, time, ',');
        getline(line_in, name, ',');
        getline(line_in, id, ',');
        getline(line_in, address, ',');
        getline(line_in, cord, ',');
        getline(line_in, phone, ',');
        getline(line_in, WeChat, ',');
        getline(line_in, email, ',');
        getline(line_in, prof, ',');
        getline(line_in, Birthday, ',');
        getline(line_in, Risk, ',');
        getline(line_in, TreatType);
        // Extract "yyyy/mm/dd" in date
        string p1;
        istringstream date_in(date); // seperate date to year, month, date.
        for(i = 0; i < 3; i++){
            getline(date_in, p1, '/');
            const char* t1 = p1.data();
            temp[i] = atoi(t1);  // Turn string into integer.
        }
        // Extract "hh/mm/ss" in time
        istringstream time_in(time); // seperate time to hour, minute, second.
        for(i = 3; i < 6; i++){
            getline(time_in, p1, ':');
            const char* t1 = p1.data();
            temp[i] = atoi(t1);
        }
        DateTime dt1(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]);
        // Extract "x|y" in cord
        istringstream str_in(cord); // seperate coordinate "x|y" to x and y.
        for(i = 0; i < 2; i++){
            getline(str_in, p1, '|');
            const char* t1 = p1.data();
            xy[i] = atoi(t1);
        }
        Location addr(xy[0], xy[1]);
        // Extract "yyyy/mm/dd" in Birthday
        istringstream bir_in(Birthday);
        for(i = 0; i < 3; i++){
            getline(bir_in, p1, '/');
            const char* t1 = p1.data();
            temp[i] = atoi(t1);  // Turn string into integer.
        }
        UID++;
        ContactDetail ct(address, phone, WeChat, email, addr);
        DateTime birth(temp[0], temp[1], temp[2]);
        PS1 = DataStorage::personstore_ID.Find(id);
        if (PS1 != nullptr) { // If he or she has been recorded previously
            PS1 -> records->push_back(UID);
            PS = PS1;
        } else {
            Person* PS2 = new Person;
            PS2->records = new vector<unsigned long>;
            PS2 -> id = id;
            PS2 -> name = name;
            PS2 -> ct = ct;
            PS2 -> birth = birth;
            PS2 -> prof = atoi(prof.c_str());
            PS2 -> records->push_back(UID);
            DataStorage::personstore_ID.Insert(PS2->id,PS2);
            DataStorage::personstore_birth.Insert(PS2->birth,PS2);
            DataStorage::personstore_prof.Insert(PS2->prof,PS2);
            PS = PS2;
        }
        Medical_State* MED = new Medical_State;
        Registration* REG = new Registration;
        Treatment* TMT = new Treatment;
        Hospital* HSP = new Hospital;
        MED -> uid = UID;
        MED -> status = STATUS;
        MED -> risk_gp = atoi(Risk.c_str());
        MED -> age_gp = Age_Group_get(dt1.YearLater(birth));
        REG -> uid = UID;
        REG -> regtime = dt1;
        TMT -> uid = UID;
        TMT -> type = atoi(TreatType.c_str());
        // Initialize some values which have not been initialized.
        REG -> queuetime = DateTime(1970,1,1,0,0,0);
        REG -> deadline = DateTime(1970,1,1,0,0,0);
        REG -> locreg = 0;
        HSP -> name = "NONE";
        TMT -> hospital = HSP;
        TMT -> assigned_time = DateTime(1970,1,1,0,0,0);
        TMT -> treated_time = DateTime(1970,1,1,0,0,0);
        Record* RC = new Record(UID, PS, MED, REG, TMT);
        newrecords.push_back(RC);
        DataStorage::recstore_uid.Insert(UID, RC);//
        DataStorage::recstore_id.Insert(RC->person->id, RC);//
        DataStorage::recstore_prof.Insert(RC->person->prof, RC);//
        DataStorage::recstore_status.Insert(RC->med->status, RC);//
        DataStorage::recstore_riskgp.Insert(RC->med->risk_gp, RC);//
        DataStorage::recstore_agegp.Insert(RC->med->age_gp, RC);//
        DataStorage::recstore_regtime.Insert(RC->reg->regtime, RC);//
        DataStorage::recstore_quetime.Insert(RC->reg->queuetime, RC);
        DataStorage::recstore_treattp.Insert(RC->treatment->type, RC);//
        DataStorage::recstore_asstime.Insert(RC->treatment->assigned_time, RC);
        DataStorage::recstore_tretime.Insert(RC->treatment->treated_time, RC);
    }
	return 0;
}

/*
*   UpdateData -- To update a certain field specified on the first line.
*   INPUT -- a csv file named filename
*   RETURN -- 0 for success, 1 for failure.
*/
int Acsctrl::UpdateData(string filename){
    updatedrecords.clear();
	fstream _file;
	_file.open(filename,ios::in);
	if(!_file){
		return 1;
	}
	_file.close();
	ifstream line_in(filename);
    string uid_s, ITEM, val;
    getline(line_in, uid_s, ',');
	getline(line_in, ITEM);
    ITEM.pop_back();
    transform(ITEM.begin(),ITEM.end(),ITEM.begin(),::toupper);
    char *p11 = (char*)ITEM.c_str();
    int cm = Acsctrl::Find_Command(p11); // Identify which ITEM we will modify
    string line;
    int i, j;
	vector<string>* content = readFile(filename.c_str());
	int size = (*content).size();
    unsigned long UID1;
    int temp[6];
    Record* RC;
    DateTime dttm;
    int sz, NB;
    unsigned long uid_2;
    for(i = 0; i < size; i++){
        /*
        *    After getting uid, we look for the record by the uid, and modify it, put it into the B+tree again.
        */
        line = (*content)[i];
        istringstream line_in(line);
        getline(line_in, uid_s, ',');
        getline(line_in, val);
        UID1 = 0;
        int l = uid_s.length();
        for(j = 0; j < l; j++){  // Turn string into unsigned long
            UID1 = UID1 * 10 + uid_s[j] - '0';
        }
        RC = DataStorage::recstore_uid.Find(UID1); // Fetch the record
        if (RC == NULL){
            cout << "Can't Find The Person!" << endl;
            continue;
        }
        updatedrecords.push_back(RC);
        if(cm == 103 || cm == 109 || cm == 110 || cm == 113 || cm == 114 || cm == 116){
            istringstream time_in(val); // seperate time to hour, minute, second.
            string p1;
            for(j = 0; j < 6; j++){
                if (j == 0 || j == 1){
                    getline(time_in, p1, '/');
                } else
                if (j == 2){
                    getline(time_in, p1, '-');
                } else{
                    getline(time_in, p1, ':');
                }
                while(p1[p1.length()-1]<'0' || p1[p1.length()-1]>'9') p1.pop_back();
       	        const char* t1 = p1.data();
            	temp[j] = atoi(t1);
            }
            if (cm == 103){
                temp[3] = temp[4] = temp[5] = 0;
            }
            DateTime dttm1(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]);
            dttm = dttm1;
        }
        if(cm == 115){
            int len_val = val.length();
            if (len_val == 1){
                if (val == "1") val = "TRUE";
                           else val = "FALSE";
            } else
            if (len_val >= 5){
                if ((val[0]=='T' || val[0]=='t') && (val[1]=='R' || val[1]=='r') &&
                    (val[2]=='U' || val[2]=='u') && (val[3]=='E' || val[3]=='e')) val = "TRUE";
                if ((val[0]=='F' || val[0]=='f') && (val[1]=='A' || val[1]=='a') &&
                    (val[2]=='L' || val[2]=='l') && (val[3]=='S' || val[3]=='s') && (val[4]=='E' || val[4]=='e')) val = "FALSE";
            } else{
                return 2; // Invalid Data Cell.
            }
        }
        // For an update operation, we should delete the old record from the B+trees of all the keys.
        switch (cm){ // Discuss each key that can be used to search for record.
            case 100:  // UID
                return 2; // UID cannot be changed.
                break;
            case 101: // NAME
                sz = RC->person->records->size();
                for (j = 0; j < sz; j++){
                    uid_2 = (*RC->person->records)[j];
                    Record* RC1 = DataStorage::recstore_uid.Find(uid_2);
                    if (RC1->person->name == val) break; // No need to update.
                    RC1->person->id=val;
                    DataStorage::recstore_id.Insert(val, RC1);
                }
                break;
            case 102: // ID
                sz = RC->person->records->size();
                for (j = 0; j < sz; j++){
                    uid_2 = (*RC->person->records)[j];
                    Record* RC1 = DataStorage::recstore_uid.Find(uid_2);
                    if (RC1->person->id == val) break;
                    RC1->person->id=val;
                    DataStorage::recstore_id.Insert(val, RC1);
                }
                // Update the person's id in database PERSON.
                DataStorage::personstore_ID.Insert(val, RC->person);
                break;
            case 103: // BIRTH, (PERSON)
                sz = RC->person->records->size();
                for (j = 0; j < sz; j++){
                    uid_2 = (*RC->person->records)[j];
                    Record* RC1 = DataStorage::recstore_uid.Find(uid_2);
                    if (RC1->person->birth.cct == dttm.cct) break;
                    RC1->person->birth = dttm;
                    DataStorage::personstore_birth.Insert(dttm, RC->person);
                    int agp = Age_Group_get(RC1->reg->regtime.YearLater(dttm));
                    if (RC1->med->age_gp == agp) continue;
                    RC1->med->age_gp = agp;
                    DataStorage::recstore_agegp.Insert(agp, RC1);
                }
                break;
            case 104: // PROF, (RECORD/PERSON)
                NB = atoi(val.c_str());
                sz = RC->person->records->size();
                for (j = 0; j < sz; j++){
                    uid_2 = (*RC->person->records)[j];
                    Record* RC1 = DataStorage::recstore_uid.Find(uid_2);
                    if (RC1->person->prof == NB) break;
                    RC1->person->prof=NB;
                    DataStorage::recstore_prof.Insert(NB, RC1);
                }
                break;
            case 105: // STATUS
                NB = atoi(val.c_str());
                if (RC->med->status == NB) break;
                RC->med->status = NB;
                DataStorage::recstore_status.Insert(RC->med->status, RC);
                break;
            case 106: // RISKGP
                NB = atoi(val.c_str());
                if (RC->med->risk_gp == NB) break;
                RC->med->risk_gp = NB;
                DataStorage::recstore_riskgp.Insert(RC->med->risk_gp, RC);
                break;
            case 107: // AGEGP
                return 2; // Cannot modify one's age group directly.
                break;
            case 109: // REGTIME
                if (RC->reg->regtime.cct == dttm.cct) break;
                RC->reg->regtime = dttm;
                DataStorage::recstore_regtime.Insert(dttm, RC);
                break;
            case 110: // QUETIME
                if (RC->reg->queuetime.cct == dttm.cct) break;
                RC->reg->queuetime = dttm;
                DataStorage::recstore_quetime.Insert(dttm, RC);
                break;
            case 111: // TREATTP
                NB = atoi(val.c_str());
                if (RC->treatment->type == NB) break;
                RC->treatment->type = NB;
                DataStorage::recstore_treattp.Insert(RC->treatment->type, RC);
                break;
            case 113: // ASSTIME
                if (RC->treatment->assigned_time.cct == dttm.cct) break;
                RC->treatment->assigned_time = dttm;
                DataStorage::recstore_asstime.Insert(dttm, RC);
                break;
            case 114: // TRETIME
                if (RC->treatment->treated_time.cct == dttm.cct) break;
                RC->treatment->treated_time = dttm;
                DataStorage::recstore_tretime.Insert(dttm, RC);
                break;
            case 115: // EMERGENCY
                if (val == "TRUE") RC->reg->isEmergency = true;
                if (val == "FALSE") RC->reg->isEmergency = false;
                break;
            case 116: // DEADLINE
                if (RC->reg->deadline.cct == dttm.cct) break;
                RC->reg->deadline = dttm;
                RC->treatment->treated_time = dttm;
                break;
            default:
                break;
        }
    }
	return 0;
}