#include "Acsctrl.h"

/*
*    cmpr -- comparison based on database RECORD
*    cmpp -- comparison based on database PERSON
*    The word after cmpr(p) is the comparing field.
*    If there is a "0" attached to the end of the word, that means an ascending order.
*    If there is a "1" attached to the end of the word, that means a descending order.
*/
bool Acsctrl::cmpr_uid(Record* x, Record* y){return x->uid < y->uid;}
bool Acsctrl::cmpr_name(Record* x, Record* y){return x->person->name < y->person->name;}
bool Acsctrl::cmpr_id(Record* x, Record* y){return x->person->id < y->person->id;}
bool Acsctrl::cmpr_prof(Record* x, Record* y){return x->person->prof < y->person->prof;}
bool Acsctrl::cmpr_status(Record* x, Record* y){return x->med->status < y->med->status;}
bool Acsctrl::cmpr_riskgp(Record* x, Record* y){return x->med->risk_gp < y->med->risk_gp;}
bool Acsctrl::cmpr_agegp(Record* x, Record* y){return x->med->age_gp < y->med->age_gp;}
bool Acsctrl::cmpr_regat(Record* x, Record* y){return x->reg->locreg < y->reg->locreg;}
bool Acsctrl::cmpr_regtime(Record* x, Record* y){return difftime(x->reg->regtime.cct, y->reg->regtime.cct) < 0;}
bool Acsctrl::cmpr_quetime(Record* x, Record* y){return difftime(x->reg->queuetime.cct, y->reg->queuetime.cct) < 0;}
bool Acsctrl::cmpr_treattp(Record* x, Record* y){return x->treatment->type < y->treatment->type;}
bool Acsctrl::cmpr_asstime(Record* x, Record* y){return difftime(x->treatment->assigned_time.cct, y->treatment->assigned_time.cct) < 0;}
bool Acsctrl::cmpr_tretime(Record* x, Record* y){return difftime(x->treatment->treated_time.cct, y->treatment->treated_time.cct) < 0;}
bool Acsctrl::cmpr_ddl(Record* x, Record* y){return difftime(x->reg->deadline.cct, y->reg->deadline.cct) < 0;}

bool Acsctrl::cmpr_uid1(Record* x, Record* y){return x->uid > y->uid;}
bool Acsctrl::cmpr_name1(Record* x, Record* y){return x->person->name > y->person->name;}
bool Acsctrl::cmpr_id1(Record* x, Record* y){return x->person->id > y->person->id;}
bool Acsctrl::cmpr_prof1(Record* x, Record* y){return x->person->prof > y->person->prof;}
bool Acsctrl::cmpr_status1(Record* x, Record* y){return x->med->status > y->med->status;}
bool Acsctrl::cmpr_riskgp1(Record* x, Record* y){return x->med->risk_gp > y->med->risk_gp;}
bool Acsctrl::cmpr_agegp1(Record* x, Record* y){return x->med->age_gp > y->med->age_gp;}
bool Acsctrl::cmpr_regat1(Record* x, Record* y){return x->reg->locreg > y->reg->locreg;}
bool Acsctrl::cmpr_regtime1(Record* x, Record* y){return difftime(x->reg->regtime.cct, y->reg->regtime.cct) > 0;}
bool Acsctrl::cmpr_quetime1(Record* x, Record* y){return difftime(x->reg->queuetime.cct, y->reg->queuetime.cct) > 0;}
bool Acsctrl::cmpr_treattp1(Record* x, Record* y){return x->treatment->type > y->treatment->type;}
bool Acsctrl::cmpr_asstime1(Record* x, Record* y){return difftime(x->treatment->assigned_time.cct, y->treatment->assigned_time.cct) > 0;}
bool Acsctrl::cmpr_tretime1(Record* x, Record* y){return difftime(x->treatment->treated_time.cct, y->treatment->treated_time.cct) > 0;}
bool Acsctrl::cmpr_ddl1(Record* x, Record* y){return difftime(x->reg->deadline.cct, y->reg->deadline.cct) > 0;}

bool Acsctrl::cmpp_name(Person* x, Person* y){return x->name < y->name;}
bool Acsctrl::cmpp_name1(Person* x, Person* y){return x->name > y->name;}
bool Acsctrl::cmpp_id(Person* x, Person* y){return x->id < y->id;}
bool Acsctrl::cmpp_id1(Person* x, Person* y){return x->id > y->id;}
bool Acsctrl::cmpp_birth(Person* x, Person* y){return x->prof < y->prof;}
bool Acsctrl::cmpp_birth1(Person* x, Person* y){return x->prof > y->prof;}
bool Acsctrl::cmpp_prof(Person* x, Person* y){return x->prof < y->prof;}
bool Acsctrl::cmpp_prof1(Person* x, Person* y){return x->prof > y->prof;}