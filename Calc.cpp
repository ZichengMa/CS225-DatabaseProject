#include <iostream>
#include <cstdio>
#include <stack>
#include <vector>
#include "Acsctrl.h"
#include "DataStorage.h"

using namespace std;

/*
*    priority -- return the priority of the operators
*    INPUT -- an opertor
*	 RETURN -- its priority, measured by a integer number
*	 SIDE EFFECT -- None
*/
int Acsctrl::priority(string oper) {
	if (oper == "(") return 0;
	if (oper == "OR") return 1;
	if (oper == "AND") return 2;
	if (oper == "NOT") return 3;
	return 5;
}

/*
*    Compare -- Using uid in Record to sort the vector in order.
*/
bool compare(Record* x, Record* y){
	return x->uid < y->uid;
}
/*
*    Merge -- Deal with the relationship of two sets of strings
*    INPUT -- the pointer of the operator stack and the vector stack
*	 RETURN -- its priority, measured by a integer number
*	 SIDE EFFECT -- None
*/
void Acsctrl::Merge(stack<string>& Ope, stack<vector<Record*>*>& Num) {
	vector<Record*> *a, *b;
	a = Num.top(); Num.pop();
	int size_a = 0;
	if (a != NULL){
		size_a = a->size();
		sort(a->begin(),a->end(),compare);
	}
    vector<Record*>* c = new vector<Record*>;
	c->clear();
    unsigned long w = 0;
	// Sort a and b by the ascending order of uid first.
	int i, j;
	if (Ope.top() == "NOT") {
		// "NOT" condition, scan the B+ tree once again to find the complement elements.
		// Firstly, we will retrieve all the elements in the B+tree.
		vector<Record*>* all_set = DataStorage::recstore_uid.Range(DataStorage::minULong,DataStorage::maxULong,0,0);
		Record *x, *z;
		int size_c = 0;
		if (c != NULL){
			size_c = all_set->size();
			sort(all_set->begin(),all_set->end(),compare);
		}
		int k;
		i = j = 0;
		for(k = 0; k < size_c; k++){
			if(i==size_a){
				c->push_back((*all_set)[k]);
				continue;
			}
			x = (*a)[i];
			z = (*all_set)[k];
			if (z->uid != x->uid){ // If the element is not in a or b, add it to the NOT set.
				c->push_back(z);
			}
			while(i<size_a&&(*a)[i]->uid <= z->uid){ // Ensure that each element in a and b is considered only once.
				i++;
			}
		}
		Num.push(c);
	}
	else if (Ope.top() == "AND"){
		// "AND" condition, select the intersection of the two vectors of string.
		b = Num.top(); Num.pop();
		int size_b = 0;
		if (b != NULL){
			size_b = b->size();
			sort(b->begin(),b->end(),compare);
		}
		i = j = 0;
		Record *x, *y;
		while(i<size_a||j<size_b){
			if(i==size_a || j==size_b) break;
			x = (*a)[i];
			y = (*b)[j];
			if(x->uid <= y->uid){
				if (x->uid == y->uid){ 	// If they are equal, add them to the intersection set.
					c->push_back(x);
					//cout << x->uid << endl; 
				}
				i++;
		    } else{
		    	j++;
			}
		}
		Num.push(c);
	} 
	else if (Ope.top() == "OR"){
		// "OR" condition, merge the two vectors of string.
		b = Num.top(); Num.pop();
		int size_b = 0;
		if (b != NULL){
			size_b = b->size();
			sort(b->begin(),b->end(),compare);
		}
		i = j = 0;
		Record *x, *y;
		while(i<size_a||j<size_b){
			if(i == size_a){
				while(j<size_b){
					if (w != (*b)[j]->uid){
						c->push_back((*b)[j]);
						w = (*b)[j]->uid;
						//cout << (*b)[j]->uid << endl;
					}
					j++;
				}
				break;
			}
			if(j == size_b){
				while(i<size_a){
					if (w != (*a)[i]->uid){
					    c->push_back((*a)[i]);
					    w = (*a)[i]->uid;
					    //cout << (*a)[i]->uid << endl;
				    }
					i++;
				}
				break;
			}
			x = (*a)[i];
			y = (*b)[j];
			if(x->uid <= y->uid){
				if (w != x->uid){
					c->push_back(x); 			// Add them to the union set.
					//cout << x->uid << endl;
				}
				w = x->uid;
				i++;
			} else {
				if (w != y->uid){
					c->push_back(y);			// Add them to the union set.
					//cout << y->uid << endl;
				}
				w = y->uid;
				j++;
			}
		}
		Num.push(c);
	}
	Ope.pop();
}

/*
*    Processwhere -- Process the statement in the parenthesis after "WHERE".
*					 The details is described in the code.
*	 INPUT -- None
*	 OUTPUT -- Error report if there is any.
*	 RETURN -- A vector<Record*>* type pointer for the querying result.
*	 SIDE EFFECT -- Allocate memory in the dynamic memory heap.
*
*/
vector<Record*>* Acsctrl::Processwhere(int db, int l, char* internbuff) {
	stack<string> Ope;
	stack<string> Num;
	stack<vector<Record*>*> Stk_R; // Calculation Stack for RECORD
	string ope_;
	string num_;
	string temp;
	string a, b, c;
	char x;
	vector<Record*>* Vec;
    int len_, len_1;
	unsigned long VAL1, VAL2;
	string VAL1_S, VAL2_S;
	DateTime D1, D2;
	Record* RC;
    int i = 0, j, optr = 0;
	bool L, G, Leq, Geq, Eq;
	int cds;  // Condition ID
	bool start = 0;
	int cm1,cm2,cm3;
	bool b_min, b_max;
	DateTime ldatetime, rdatetime; // Used for storing the left and right bound of the query operations.
	unsigned long lval, rval;
	int lint,rint;
	string lstr, rstr;
	int valtype; // 0 for integer(or unsigned long); 1 for DateTime; 2 for string
	while ((!Ope.empty()) || start == 0) {
		while (internbuff[i] == ' ') i++;
		start = 1;
		temp = "";
		x = internbuff[i];// Identify AND, OR, NOT.
		if (internbuff[i] == 'A' && internbuff[i+1] == 'N' && internbuff[i+2] == 'D'){
			temp = "AND";
			i+=3;
		} else
		if (internbuff[i] == 'N' && internbuff[i+1] == 'O' && internbuff[i+2] == 'T'){
			temp = "NOT";
			i+=3;
		} else
		if (internbuff[i] == 'O' && internbuff[i+1] == 'R'){
			temp = "OR";
			i+=2;
		} else{
			temp.push_back(x);
			i++;
		}
		if (temp != "(" && temp != ")" && temp != "AND" && temp != "OR" && temp != "NOT") { // If it is a search condition?
			num_.clear();
			num_.push_back(x);
			while (internbuff[i] != ')'){
				if (internbuff[i] != ' ' && internbuff[i] != '\"') num_.push_back(internbuff[i]);
				i++;
			}
			// We consider creating a vector of string here to represent the selected string due to this elementary screen condition.
			// First consider the "KEY OPERATOR CONST" form. (Condition 1-5, optr=1)
			// Then consider the "CONST1 OPERATOR1 KEY OPERATOR2 CONST2" form. (Condition 6-9, optr=2)
			// If there are more than operators, the command must be invalid.
			// The order of the conditions maybe different from those stated in "Computing Assignment 2 Implement Standard", for the convenience to implement.
			len_ = num_.length();
			L = G = Leq = Geq = Eq = 0;
			optr = 0;
			a = b = c = "";
			cds = 0;
			for (j = 0 ; j < len_ ; j++){
				if (num_[j] == '=' && num_[j+1] == '='){
					// Condition 1 : KEY == CONST
					optr++;
					cds = 1;
					if (Eq){
						STATEMENT_FAULT = 1;
						return nullptr; // Invalid Command.
					}
					Eq = 1;
					j++;
				} else
				if (num_[j] == '<' && num_[j+1] == '='){
					// Condition 2 : KEY <= CONST
					// Condition 9 : CONST1 < KEY <= CONST2
					// Condition 7 : CONST1 <= KEY <= CONST2
					optr++;
					cds = 2;
					if (L) cds = 9; else
					if (Leq) cds = 7;
					Leq = 1;
					j++;
				} else
				if (num_[j] == '>' && num_[j+1] == '='){
					// Condition 3 : KEY >= CONST
					optr++;
					cds = 3;
					if (Geq){
						STATEMENT_FAULT = 1;
						return nullptr; // Invalid Command.
					}
					j++;
					Geq = 1;
				} else
				if (num_[j] == '<'){
					// Condition 4 : KEY < CONST
					// Condition 8 : CONST1 < KEY < CONST2
					// Condition 6 : CONST1 <= KEY < CONST2
					optr++;
					cds = 4;
					if (L) cds = 8; else
					if (Leq) cds = 6;
					L = 1;
				} else
				if (num_[j] == '>'){
					// Condition 5 : KEY > CONST
					optr++;
					cds = 5;
					if (G){
						STATEMENT_FAULT = 1;
						return nullptr; // Invalid Command.
					}
					G = 1;
				} else{
					if(optr==0){
						a.push_back(num_[j]); // Extract the three "KEY" or "CONST" in the expression.
					} else
					if(optr==1){
						b.push_back(num_[j]);
					} else{
						c.push_back(num_[j]);
					}
				}
				if(optr >= 3){
					STATEMENT_FAULT = 1;
					return nullptr; // Invalid Command.
				}
			}
			char *p = (char*)a.c_str();
			cm1 = Find_Command(p);
			char *p1 = (char*)b.c_str();
			cm2 = Find_Command(p1);
			int lmt = 10;			 // This part use to judge if the key is belongs to corresponding dataset.
			int target = (optr==1)? cm1 : cm2;	 // CONST1 OP1 KEY OP2 CONST2, only the second is a key.
			bool correct = 0;  					 //If it doesn't belong, the command is invalid.
			for(j = 0; j<=lmt; j++){
				if (RECORDF[j] == target){ 
					correct = 1; //break
					break;
				}
			}
			if(!correct){
				STATEMENT_FAULT = 1;
				return nullptr; // Invalid Command.
			}
			if(optr == 1){
				len_1 = b.length();
				VAL1 = 0;
				valtype = 0;
				for(j = 0; j < len_1 ; j++){								// If the type is DateTime, use D1 as the key value.
					if (!isdigit(b[j])){									// If the type is integer, use VAL1 as the key value.
						valtype = 1;
						break;
					} else{
						VAL1 = (VAL1 * 10) + (b[j] - '0');
					}
				}
				D1=DateTime(b);
				if(D1.st.tm_sec == -1) valtype = 2;
			} else
			if(optr == 2){
				len_1 = a.length();
				VAL1 = 0;
				valtype = 0;
				for(j = 0; j < len_1 ; j++){
					if (isdigit(a[j])) VAL1 = (VAL1 * 10) + (a[j] - '0'); 
					else {
						valtype = 1;
						break;
					}
				}
				D1=DateTime(a);                                             // If the type is DateTime, use D1, D2 as the key value.
				if(valtype == 1 && D1.st.tm_sec == -1) valtype = 2;
				len_1 = c.length();											// If the type is integer, use VAL1, VAL2 as the key value.
				VAL2 = 0;
				for(j = 0; j < len_1 ; j++){
					if (isdigit(c[j])) VAL2 = (VAL2 * 10) + (c[j] - '0'); 
					else {
						valtype = 1;
						break;
					}
				}
				D2=DateTime(c);
				if(valtype == 1 && D2.st.tm_sec == -1) valtype = 2;
			}
			if (cds == 7){
				if(valtype == 0){
					if(a == c){
						cds = 1;
						cm1 = cm2;
						optr = 1;
					}
				} else
				if(valtype == 1){
					if ( D1.cct == D2.cct ){
						cds = 1;
						cm1 = cm2;
						optr = 1;
					}
				} else
				if(valtype == 2){
					if(a == c){
						cds = 1;
						cm1 = cm2;
						a = b;
						b = c;
						optr = 1;
					}
				}
			}
			cm3 = optr==1 ? cm1 : cm2;
			switch (cds){
				case 1: // Condition 1 : KEY == CONST
					// We use Find of the B+ tree. For other cases, we use Range of the B+ tree.
					switch (cm1){ // Discuss each key that can be used to search for data.
						case 100:  // UID
							RC = DataStorage::recstore_uid.Find(VAL1);
							break;
						case 102: // ID
							Vec = DataStorage::recstore_id.Find(b);
							break;
						case 104: // PROF
							Vec = DataStorage::recstore_prof.Find(VAL1);
							break;
						case 105: // STATUS
							Vec = DataStorage::recstore_status.Find(VAL1);
							break;
						case 106: // RISKGP
							Vec = DataStorage::recstore_riskgp.Find(VAL1);
							break;
						case 107: // AGEGP
							Vec = DataStorage::recstore_agegp.Find(VAL1);
							break;
						case 109: // REGTIME
							Vec = DataStorage::recstore_regtime.Find(D1);
							break;
						case 110: // QUETIME
							Vec = DataStorage::recstore_quetime.Find(D1);
							break;
						case 111: // TREATTP
							Vec = DataStorage::recstore_treattp.Find(VAL1);
							break;
						case 113: // ASSTIME
							Vec = DataStorage::recstore_asstime.Find(D1);
							break;
						case 114: // TRETIME
							Vec = DataStorage::recstore_tretime.Find(D1);
							break;
						default:
							break;
					}
					break;
				case 2: // Condition 2 : KEY <= CONST
					b_min = 0;
					b_max = 1; // (-INF,CONST]
					if(valtype == 0){
						if (cm3 == 100){
							lval = DataStorage::minULong;
							rval = VAL1;
						} else{
							lint = DataStorage::minInt;
							rint = VAL1;
						}
					} else 
					if(valtype == 1){
						ldatetime = DataStorage::minDateTime;
						rdatetime = D1;
					} else{
						lstr = DataStorage::minstring;
						rstr = b;
					}
					break;
				case 3: // Condition 3 : KEY >= CONST
				    b_min = 1;
					b_max = 0; // [CONST,+INF)
					if(valtype == 0){
						if (cm3 == 100){
							lval = VAL1;
							rval = DataStorage::maxULong;
						} else{
							lint = VAL1;
							rint = DataStorage::maxInt;
						}
					} else 
					if(valtype == 1){
						ldatetime = D1;
						rdatetime = DataStorage::maxDateTime;
					} else{
						lstr = b;
						rstr = DataStorage::maxstring;
					}
					break;
				case 4: // Condition 4 : KEY < CONST
				    b_min = 0;
					b_max = 0; // (-INF,CONST)
					if(valtype == 0){
						if (cm3 == 100){
							lval = DataStorage::minULong;
							rval = VAL1;
						} else{
							lint = DataStorage::minInt;
							rint = VAL1;
						}
					} else 
					if(valtype == 1){
						ldatetime = DataStorage::minDateTime;
						rdatetime = D1;
					} else{
						lstr = DataStorage::minstring;
						rstr = b;
					}
					break;
				case 5: // Condition 5 : KEY > CONST
					b_min = 0;
					b_max = 0; // (CONST,+INF)
					if(valtype == 0){
						if (cm3 == 100){
							lval = VAL1;
							rval = DataStorage::maxULong;
						} else{
							lint = VAL1;
							rint = DataStorage::maxInt;
						}
					} else 
					if(valtype == 1){
						ldatetime = D1;
						rdatetime = DataStorage::maxDateTime;
					} else{
						lstr = b;
						rstr = DataStorage::maxstring;
					}
					break;
				case 6: // Condition 6 : CONST1 <= KEY < CONST2
					b_min = 1;
					b_max = 0; // [CONST1,CONST2)
					if(valtype == 0){
						if (cm3 == 100){
							lval = VAL1;
							rval = VAL2;
						} else{
							lint = VAL1;
							rint = VAL2;
						}				
					} else 
					if(valtype == 1){
						ldatetime = D1;
						rdatetime = D2;
					} else{
						lstr = a;
						rstr = c;
					}
					break;
				case 7: // Condition 7 : CONST1 <= KEY <= CONST2
					b_min = 1;
					b_max = 1; // [CONST1,CONST2]
					if(valtype == 0){
						if (cm3 == 100){
							lval = VAL1;
							rval = VAL2;
						} else{
							lint = VAL1;
							rint = VAL2;
						}
					} else 
					if(valtype == 1){
						ldatetime = D1;
						rdatetime = D2;
					} else{
						lstr = a;
						rstr = c;
					}
					break;
				case 8: // Condition 8 : CONST1 < KEY < CONST2
					b_min = 0;
					b_max = 0; // (CONST1,CONST2)
					if(valtype == 0){
						if (cm3 == 100){
							lval = VAL1;
							rval = VAL2;
						} else{
							lint = VAL1;
							rint = VAL2;
						}
					} else 
					if(valtype == 1){
						ldatetime = D1;
						rdatetime = D2;
					} else{
						lstr = a;
						rstr = c;
					}
					break;
				case 9: // Condition 9 : CONST1 < KEY <= CONST2
					b_min = 0;
					b_max = 1; // (CONST1,CONST2]
					if(valtype == 0){
						if (cm3 == 100){
							lval = VAL1;
							rval = VAL2;
						} else{
							lint = VAL1;
							rint = VAL2;
						}
					} else 
					if(valtype == 1){
						ldatetime = D1;
						rdatetime = D2;
					} else{
						lstr = a;
						rstr = c;
					}
					break;
				default:
					break;
			}
			/*****************************************
			*       HERE IS THE QUERY PART           *
			*   COLLECTING THE DATA IN THE RANGE     *
			******************************************/
			if(cds>=2 && cds<=9){
				switch (cm3){ // Discuss each key that can be used to search for data.
					case 100: // UID
						Vec = DataStorage::recstore_uid.Range(lval, rval, b_min, b_max);
						break;
					case 102: // ID // vector<Record*>* Vec; vector<Record*>* 
						Vec = DataStorage::recstore_id.Range(lstr, rstr, b_min, b_max);
						break;
					case 104: // PROF
						Vec = DataStorage::recstore_prof.Range(lint, rint, b_min, b_max);
						break;
					case 105: // STATUS
						Vec = DataStorage::recstore_status.Range(lint, rint, b_min, b_max);
						break;
					case 106: // RISKGP
						Vec = DataStorage::recstore_riskgp.Range(lint, rint, b_min, b_max);
						break;
					case 107: // AGEGP
						Vec = DataStorage::recstore_agegp.Range(lint, rint, b_min, b_max);
						break;
					case 109: // REGTIME
						Vec = DataStorage::recstore_regtime.Range(ldatetime, rdatetime, b_min, b_max);
						break;
					case 110: // QUETIME
						Vec = DataStorage::recstore_quetime.Range(ldatetime, rdatetime, b_min, b_max);
						break;
					case 111: // TREATTP
						Vec = DataStorage::recstore_treattp.Range(lint, rint, b_min, b_max);
						break;
					case 113: // ASSTIME
						Vec = DataStorage::recstore_asstime.Range(ldatetime, rdatetime, b_min, b_max);
						break;
					case 114: // TRETIME
						Vec = DataStorage::recstore_tretime.Range(ldatetime, rdatetime, b_min, b_max);
						break;
					default:
						break;
				}
			}
			Stk_R.push(Vec);   					 // Add the set to the top of the stack directly.
		}
		else { // Otherwise, it's an operator.
			ope_ = temp;
			if (ope_ == "(" || Ope.empty()){
					Ope.push(ope_);    //If the character is a "(", add it directly to the top of the stack.
				}
			else if (ope_ == ")") {  // If the character is a ")".
				while (Ope.top() != "("){
					Merge(Ope, Stk_R); // Calculate all the items in the parentheses.
				}
				Ope.pop();  // Pop "(".
			}
			else if (!Ope.empty() && priority(Ope.top()) >= priority(ope_)) {
				/*
				*    If the current operator has lower priority than the stack top's, 
				*    we consider processing the previous set first rather than the new one.
				*/
				Merge(Ope, Stk_R);
				Ope.push(ope_);  // After the procedure mentioned above, add the operator directly to the stack.
			}
			else Ope.push(ope_);  // Otherwise, add the operator directly to the stack.
		}
	}
	Vec = Stk_R.top();
	return Vec;
}