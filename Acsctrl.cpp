#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include "Acsctrl.h"
#include "DataStorage.h"

using namespace std;

//string file_path = "di3.csv";

const int Acsctrl::RECORDF[11]={100,102,104,105,106,107,109,110,111,113,114};
const int Acsctrl::PERSONF[4]={101,102,103,104};
list<Record*> Acsctrl::newrecords=list<Record*>{};
list<Record*> Acsctrl::updatedrecords=list<Record*>{};
unsigned long Acsctrl::UID;
bool Acsctrl::STATEMENT_FAULT;
const map<string,int> Acsctrl::cmdset = {
    // Basic instruction structure
    {"FROM",0},{"SELECT",1},{"WHERE",2},{"ORDER",3},
    // Logic operations and symbols
    {"AND",10},{"OR",11},{"NOT",12},{"ALL",13},{"==",14},{"<=",15},{">=",16},{"<",17},{">",18},
    // Types of database
    {"RECORD",20},{"PERSON",21},
    // Fields
    {"UID",100},{"NAME",101},{"ID",102},{"BIRTH",103},{"PROF",104},{"STATUS",105},
    {"RISKGP",106},{"AGEGP",107},{"REGAT",108},{"REGTIME",109},{"QUETIME",110},
    {"TREATTP",111},{"TREATAT",112},{"ASSTIME",113},{"TRETIME",114},
    {"EMERGENCY",115},{"DEADLINE",116}
};
const map<int,string> Acsctrl::item_code = {
	{100,"UID"},{101,"NAME"},{102,"ID"},{103,"BIRTH"},{104,"PROF"},{105,"STATUS"},
    {106,"RISKGP"},{107,"AGEGP"},{108,"REGAT"},{109,"REGTIME"},{110,"QUETIME"},
    {111,"TREATTP"},{112,"TREATAT"},{113,"ASSTIME"},{114,"TRETIME"},
    {115,"EMERGENCY"},{116,"DEADLINE"}
};
/*
*   Find_Command:
*   INPUT: User's input
*   RETURN: An integer
*/
int Acsctrl::Find_Command(char* k) {
    map<string,int>::const_iterator cm;
    cm = cmdset.find(string(k));
    if (cm == cmdset.end()) return -1;
    return cm->second;
}

/*
*    d_to_s -- Changing a DateTime D to standard "yyyy/mm/dd/hh/mm/ss"
*    INPUT -- DateTime type D
*    RETURN -- The standard string "yyyy/mm/dd/hh/mm/ss"
*/
string Acsctrl::d_to_s(DateTime D1){
	string str="";
	int Y,M,D,H,MI,S;
	Y=D1.st.tm_year+1900;
	str+=to_string(Y);
	str+="/";
	M=D1.st.tm_mon+1;
	if(M<10) str+="0";  // Put a zero if the value is less than 10.
	str+=to_string(M);
	str+="/";
	D=D1.st.tm_mday;
	if(D<10) str+="0";
	str+=to_string(D);
	str+="-";
	H=D1.st.tm_hour;
	if(H<10)str+="0";
	str+=to_string(H);
	str+=":";
	MI=D1.st.tm_min;
	if(MI<10)str+="0";
	str+=to_string(MI);
	str+=":";
	S=D1.st.tm_sec;
	if(S<10)str+="0";
	str+=to_string(S);
	return str;
}

/* 
*     Search -- Extract the instruction, and find the data we are interested in.
*				More explanations will be claimed in the in-text comments. Please see below.
*     INPUT -- the instruction
*     OUTPUT -- error report if there is any
*     RETURN -- A vector of pointers to the string vector
*     SIDE EFFECT -- None
*/
vector<vector<string>*>* Acsctrl::Search(string instr){
	transform(instr.begin(),instr.end(),instr.begin(),::toupper);
	int l=instr.length();
	if (l == 0) return nullptr;
	bool fin = 0; // Judge whether the instruction is finished extracting.
    int i = 0, j, k = 0;
	int db, cm, cm1;
	int part = -1;
	bool isstate = 0, isall = 0;
	char internbuff[1024]; // An buffer used to store.
	int select_field[30],fields;
	int top;
	bool have_order = 0; // If the statement doesn't contain "ORDER", we should set a default way (UID,0) to sort the result.
	int od = 0; // Default is ascending.
	bool where_appear = 0;
	vector<Record*>* rst; // If the command is valid, we will return rst(pst) for the query result.
	vector<Person*>* pst;
	vector<string>* header = new vector<string>; // The header of the table.
	vector<vector<string>*>* str_rst = new vector<vector<string>*>;
    int ORDER_POS = instr.find("ORDER");
	int WHERE_POS = instr.find("WHERE");
	while(!fin){
		if (!isstate){
			k = 0;
			while(instr[i] == ' ')i++;
			if(i == l){
				fin = 1;
				break;
			}
			while(instr[i] != ' '){
				internbuff[k++] = (instr[i]>='a' && instr[i]<='z') ? instr[i] - 32 : instr[i];
				i++;
			}
			internbuff[k++] = 0; // Extract the next key word
			cm = Find_Command(internbuff); // Find the command code
			if (cm <= part || cm == -1 || cm >= 4){
				return nullptr; // Invalid Command.
			}
			part = cm;
			isstate = 1;
		} else{
			switch (part){
				case 0:
					/*
					    Here we process "FROM", and extract "RECORD" or "PERSON".
					*/
					k = 0;
					while(instr[i] == ' ')i++;
					while(instr[i] != ' '){
						internbuff[k++] = (instr[i]>='a' && instr[i]<='z') ? instr[i] - 32 : instr[i];
						i++;
					}
					internbuff[k++] = 0; // Extract the next key word
					cm = Find_Command(internbuff); // Find the command code
					if (cm == 20){
						db = 0; // Select RECORD database
					} else
					if (cm == 21){
						db = 1; // Select PERSON database
					} else{
						return nullptr; // Invalid Command.
					}
					isstate = 0;
					break;
				case 1:
					/*
					    Here we process "SELECT". The select object will have the form like, (UID, PROF)
						seperated by ",". 
					*/
					while (instr[i] == ' ') i++;
				    while (instr[i] == '(') i++;
					fields = 0;
					isall = 0;
					while(instr[i] != ')'){
						k = 0;
						while(instr[i] != ',' && instr[i] != ')'){
							internbuff[k++] = (instr[i]>='a' && instr[i]<='z') ? instr[i] - 32 : instr[i];
							i++;
						}
						internbuff[k++] = 0; // Extract the field seperated by commas
						cm = Find_Command(internbuff); // Find the command code
						if (cm != 13){
							if (db == 0){
								if (cm < 100 || cm > 116) return nullptr; // Invalid Command.
							}
							if (db == 1){
								if (cm < 101 || cm > 104) return nullptr; // Invalid Command.
							}
						}
						select_field[fields++] = cm;
						if (cm == 13) isall = 1;    // If "ALL" appears, we ignore all other keys.
						while (instr[i] == ',') i++;
						while (instr[i] == ' ') i++;
					}
					i++;
					if (isall){
						if (db == 0){ // Mark all the keys in RECORD
							fields = 0;
							for(j = 100; j <= 116; j++){
								select_field[fields++] = j;
							}
						} else{  // Mark all the keys in PERSON
							fields = 0;
							for(j = 101; j <= 104 ; j++){
								select_field[fields++] = j;
							}
						}
					}
					for(j = 0; j < fields; j++){
						header -> push_back(item_code.find(select_field[j])->second);
					}
					if (db == 1 && fields > 1 && select_field[1] == 102){
						swap((*header)[0], (*header)[1]);
					}
					str_rst -> push_back(header);
					isstate = 0;
					break;
				case 2:
					where_appear = 1;
					/*
					    Here we process "WHERE". It is a bit more complicated.
						It has a form of (CONDITION) or ((CONDITION) OPERATION (CONDITION)), or more complicated, 
						as well as an arbitrary logic expression in C++.
						We are going to use stack structures to implement the process.
					*/
					while (instr[i] == ' ') i++;
					top = 0;
					k = 0;
					while (i < l){ // We extract the statement after "WHERE" here.
						if (ORDER_POS != -1 && i >= ORDER_POS) return nullptr;
						internbuff[k++] = (instr[i]>='a' && instr[i]<='z') ? instr[i] - 32 : instr[i];
						if(instr[i] == '('){
							top++;
						}
						if(instr[i] == ')'){
							top--;
						}
						i++;
						if(!top) break;
					}
					if (ORDER_POS != -1){
						while (i < ORDER_POS){
							if (instr[i] == ')') return nullptr; // Invalid Command.
							i++;
						}
					}
					if (db == 0){
						STATEMENT_FAULT = 0;
						rst = Processwhere(db, k, internbuff);
						if (STATEMENT_FAULT) return nullptr;
					} else{
						STATEMENT_FAULT = 0;
						pst = Processwherep(db, k, internbuff);
						if (STATEMENT_FAULT) return nullptr;
					}
					isstate = 0;
					break;
				case 3:
					/*
					    Here we process "ORDER". The select object will have the form like, (KEY, 0)
						seperated by ",".
					*/
					while (instr[i] == ' ') i++;
					while (instr[i] == '(') i++;
					k = 0;
					while(instr[i] != ',' && instr[i] != ' '){
						internbuff[k++] = (instr[i]>='a' && instr[i]<='z') ? instr[i] - 32 : instr[i]; // Extract the field
						i++;
					}
					internbuff[k++] = 0;
					cm1 = Find_Command(internbuff);
					while (instr[i] == ' ') i++;
					while (instr[i] == ',') i++;
					while (instr[i] == ' ') i++;
					if(instr[i]>='0'&&instr[i]<='1'){
						od = instr[i] - '0';
					}
					i = l;
					isstate = 0;
					have_order = 1;
					fin = 1;
					break;
				default:
					break;
			}
		}
	}
	if (UID == 0) return str_rst; // If we didn't find anything, just return the header.
	// Here we are going to sort out the by the keyword extracted above in ascending(0,default) or descending(1) order.
	if (part == 1){
		if (db == 0){
			rst = DataStorage::recstore_uid.Range(DataStorage::minULong,DataStorage::maxULong,0,0);
			cm1 = 100;
		} else{
			pst = DataStorage::personstore_ID.Range(DataStorage::minstring,DataStorage::maxstring,0,0);
			cm1 = 102;
		}
		od = 0;
	} else
	if (part == 2){
		if (!have_order){
			cm1 = 100;
			od = 0;
		} else{
			cm1 = Find_Command(internbuff);
		}
	} else
	if (part == 3){
		if (where_appear == 0){
			if (db == 0){
				rst = DataStorage::recstore_uid.Range(DataStorage::minULong,DataStorage::maxULong,0,0);
			} else{
				pst = DataStorage::personstore_ID.Range(DataStorage::minstring,DataStorage::maxstring,0,0);
			}
		}
	}
	int sz;
	if (db == 0){
		if (rst != nullptr){
			sz = rst->size();
		} else{
			return str_rst; // If we didn't find anything, just return the header.
		}
	} else{
		if (pst != nullptr){
			sz = pst->size();
		} else{
			return str_rst; // If we didn't find anything, just return the header.
		}
	}
	// We consider each case of KEY
	switch (cm1){ // Discuss each key that can be used to search for data.
		case 100: // UID
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_uid);
					else sort(rst->begin(),rst->end(),cmpr_uid1);
			break;
		case 101: // NAME
			if (db == 0){
				if (od == 0) sort(rst->begin(),rst->end(),cmpr_name);
						else sort(rst->begin(),rst->end(),cmpr_name1);
			} else{
				if (od == 0) sort(pst->begin(),pst->end(),cmpp_name);
						else sort(pst->begin(),pst->end(),cmpp_name1);
			}
		    break;
		case 102: // ID
			if (db == 0){
				if (od == 0) sort(rst->begin(),rst->end(),cmpr_id);
						else sort(rst->begin(),rst->end(),cmpr_id1);
			} else{
				if (od == 0) sort(pst->begin(),pst->end(),cmpp_id);
						else sort(pst->begin(),pst->end(),cmpp_id1);
			}
			break;
		case 103:
			if (db == 0) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(pst->begin(),pst->end(),cmpp_birth);
					else sort(pst->begin(),pst->end(),cmpp_birth1);
			break;
		case 104: // PROF
			if (db == 0){
				if (od == 0) sort(rst->begin(),rst->end(),cmpr_prof);
						else sort(rst->begin(),rst->end(),cmpr_prof1);
			} else{
				if (od == 0) sort(pst->begin(),pst->end(),cmpp_prof);
						else sort(pst->begin(),pst->end(),cmpp_prof1);
			}
			break;
		case 105: // STATUS
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_status);
					else sort(rst->begin(),rst->end(),cmpr_status1);
			break;
		case 106: // RISKGP
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_riskgp);
					else sort(rst->begin(),rst->end(),cmpr_riskgp1);
			break;
		case 107: // AGEGP
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_agegp);
					else sort(rst->begin(),rst->end(),cmpr_agegp1);
			break;
		case 108: // REGAT
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_regat);
					else sort(rst->begin(),rst->end(),cmpr_regat1);
			break;
		case 109: // REGTIME
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_regtime);
					else sort(rst->begin(),rst->end(),cmpr_regtime1);
			break;
		case 110: // QUETIME
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_quetime);
					else sort(rst->begin(),rst->end(),cmpr_quetime1);
			break;
		case 111: // TREATTP
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_treattp);
					else sort(rst->begin(),rst->end(),cmpr_treattp1);
			break;
		case 113: // ASSTIME
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_asstime);
					else sort(rst->begin(),rst->end(),cmpr_asstime1);
			break;
		case 114: // TRETIME
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_tretime);
					else sort(rst->begin(),rst->end(),cmpr_tretime1);	
			break;
		case 116: // DEADLINE
			if (db == 1) return nullptr; // The item is not in such dataset.
			if (od == 0) sort(rst->begin(),rst->end(),cmpr_ddl);
					else sort(rst->begin(),rst->end(),cmpr_ddl1);
			break;
		default:
			break;
	}
	// Now, we are going to joint the strings to form the return vector.
	string item_str, temp_date;
	bool Q0;
	for(j = 0; j < sz; j++){
		vector<string>* item_str = new vector<string>;
		if(db == 0){
			for(i = 0; i < fields; i++){
				switch (select_field[i]){ // Discuss each key that can be used to search for data.
					case 100: // UID
						item_str->push_back(to_string((*rst)[j]->uid));
						break;
					case 101: // NAME
						item_str->push_back((*rst)[j]->person->name);
						break;
					case 102: // ID
						item_str->push_back((*rst)[j]->person->id);
						break;
					case 103: // BIRTH
						temp_date=d_to_s((*rst)[j]->person->birth);
						item_str->push_back(temp_date);
						break;
					case 104: // PROF
						item_str->push_back(to_string((*rst)[j]->person->prof));
						break;
					case 105: // STATUS
						item_str->push_back(to_string((*rst)[j]->med->status));
						break;
					case 106: // RISKGP
						item_str->push_back(to_string((*rst)[j]->med->risk_gp));
						break;
					case 107: // AGEGP
						item_str->push_back(to_string((*rst)[j]->med->age_gp));
						break;
					case 108: // REGAT
						item_str->push_back(to_string((*rst)[j]->reg->locreg));
						break;
					case 109: // REGTIME
						temp_date=d_to_s((*rst)[j]->reg->regtime);
						item_str->push_back(temp_date);
						break;
					case 110: // QUETIME
						temp_date=d_to_s((*rst)[j]->reg->queuetime);
						item_str->push_back(temp_date);
						break;
					case 111: // TREATTP
						item_str->push_back(to_string((*rst)[j]->treatment->type));
						break;
					case 112: // TREATAT
						item_str->push_back((*rst)[j]->treatment->hospital->name);
						break;
					case 113: // ASSTIME
						temp_date=d_to_s((*rst)[j]->treatment->assigned_time);
						item_str->push_back(temp_date);
						break;
					case 114: // TRETIME
						temp_date=d_to_s((*rst)[j]->treatment->treated_time);
						item_str->push_back(temp_date);
						break;
					case 115: // EMERGENCY
						Q0 = (*rst)[j]->reg->isEmergency;
						if (Q0) item_str->push_back("true");
						   else item_str->push_back("false");
					    break;
					case 116: // DEADLINE
						temp_date=d_to_s((*rst)[j]->reg->deadline);
						item_str->push_back(temp_date);
						break;
					default:
						break;
				}
			}
			str_rst -> push_back(item_str);
		} else{
			if(fields > 1 && select_field[1] == 102){
				int temp_num;
				temp_num = select_field[0];
				select_field[0] = 102;
				select_field[1] = temp_num;
			}
			for(i = 0; i < fields; i++){
				switch (select_field[i]){ // Discuss each key that can be used to search for data.
					case 101: // NAME
						item_str->push_back((*pst)[j]->name);
						break;
					case 102: // ID
						item_str->push_back((*pst)[j]->id);
						break;
					case 103: // BIRTH
						temp_date=d_to_s((*pst)[j]->birth);
						item_str->push_back(temp_date);
						break;
					case 104: // PROF
						item_str->push_back(to_string((*pst)[j]->prof));
						break;
					default:
						break;
				}
			}
			str_rst -> push_back(item_str);
		}
    }
	return str_rst;
}