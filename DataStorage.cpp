#include "DataStorage.h"
BPlusTree<unsigned long, Record> DataStorage::recstore_uid = BPlusTree<unsigned long, Record>(DataStorage::degree,DataStorage::CompareTo_ULong);
BPlus_multi<string, Record> DataStorage::recstore_id = BPlus_multi<string,Record>(DataStorage::degree,DataStorage::CompareTo_string,DataStorage::IsValid_id);
BPlus_multi<int, Record> DataStorage::recstore_prof = BPlus_multi<int,Record>(DataStorage::degree,DataStorage::CompareTo_int,DataStorage::IsValid_prof);
BPlus_multi<int, Record> DataStorage::recstore_status = BPlus_multi<int,Record>(DataStorage::degree,DataStorage::CompareTo_int,DataStorage::IsValid_status);
BPlus_multi<int, Record> DataStorage::recstore_riskgp = BPlus_multi<int,Record>(DataStorage::degree,DataStorage::CompareTo_int,DataStorage::IsValid_riskgp);;
BPlus_multi<int, Record> DataStorage::recstore_agegp = BPlus_multi<int,Record>(DataStorage::degree,DataStorage::CompareTo_int,DataStorage::IsValid_agegp);
BPlus_multi<DateTime, Record> DataStorage::recstore_regtime = BPlus_multi<DateTime,Record>(DataStorage::degree,DataStorage::CompareTo_DateTime,DataStorage::IsValid_regtime);
BPlus_multi<DateTime, Record> DataStorage::recstore_quetime = BPlus_multi<DateTime,Record>(DataStorage::degree,DataStorage::CompareTo_DateTime,DataStorage::IsValid_quetime);
BPlus_multi<int, Record> DataStorage::DataStorage::recstore_treattp = BPlus_multi<int,Record>(DataStorage::degree,DataStorage::CompareTo_int,DataStorage::IsValid_treattp);;
BPlus_multi<DateTime, Record> DataStorage::recstore_asstime =  BPlus_multi<DateTime,Record>(DataStorage::degree,DataStorage::CompareTo_DateTime,DataStorage::IsValid_asstime);
BPlus_multi<DateTime, Record> DataStorage::recstore_tretime = BPlus_multi<DateTime,Record>(DataStorage::degree,DataStorage::CompareTo_DateTime,DataStorage::IsValid_tretime);


BPlusTree<string, Person> DataStorage::personstore_ID = BPlusTree<string, Person>(DataStorage::degree,DataStorage::CompareTo_string);
BPlus_multi<DateTime, Person> DataStorage::personstore_birth = BPlus_multi<DateTime, Person>(DataStorage::degree,DataStorage::CompareTo_DateTime,DataStorage::IsValid_birth);
BPlus_multi<int, Person> DataStorage::personstore_prof = BPlus_multi<int, Person>(DataStorage::degree,DataStorage::CompareTo_int,DataStorage::IsValid_p_prof);

const string DataStorage::maxstring = "$$MAXSTR$$";
const string DataStorage::minstring = "$$MINSTR$$";
const DateTime DataStorage::maxDateTime = DateTime(2035,1,1);
const DateTime DataStorage::minDateTime = DateTime(1850,1,1);
int DataStorage::CompareTo_ULong(const unsigned long & a, const unsigned long & b){
    if (a < b) return -1;
    if (a == b) return 0;
    return 1;
}
int DataStorage::CompareTo_int(const int & a, const int & b){
    if (a < b) return -1;
    if (a == b) return 0;
    return 1;
}
int DataStorage::CompareTo_string(const string & a, const string & b){
    if (a == b) return 0;
    if (a == maxstring) return 1;
    if (b == maxstring) return -1;
    if (a == minstring) return -1;
    if (b == minstring) return 1;
    if (a < b) return -1;
    return 1;
}
int DataStorage::CompareTo_DateTime(const DateTime & a, const DateTime & b){
    if (a.cct < b.cct) return -1;
    if (a.cct == b.cct) return 0;
    return 1;
}
bool DataStorage::IsValid_id(const string & a, const Record* b){
    return (a == b->person->id);
}
bool DataStorage::IsValid_prof(const int& r, const Record* a) {
    return (a->person->prof == r);
}
bool DataStorage::IsValid_status(const int & a, const Record* b) {
    return (b->med->status == a);
}
bool DataStorage::IsValid_riskgp(const int & a, const Record* b) {
    return (a == b->med->risk_gp);
}
bool DataStorage::IsValid_agegp(const int & a, const Record* b){
    return (a == b->med->age_gp);
}
bool DataStorage::IsValid_treattp(const int & a, const Record* b){
    return (a == b->treatment->type);
}
bool DataStorage::IsValid_regtime(const DateTime & a, const Record* b){
    return (a.cct == b->reg->regtime.cct);
}
bool DataStorage::IsValid_quetime(const DateTime & a, const Record* b){
    return (a.cct == b->reg->queuetime.cct);
}
bool DataStorage::IsValid_asstime(const DateTime & a, const Record* b){
    return (a.cct == b->treatment->assigned_time.cct);
}
bool DataStorage::IsValid_tretime(const DateTime & a, const Record* b){
    return (a.cct == b->treatment->treated_time.cct);
}
bool DataStorage::IsValid_birth(const DateTime & a, const Person* b){
    return (a.cct == b->birth.cct);
}
bool DataStorage::IsValid_p_prof(const int& r, const Person* a) {
    return (a->prof == r);
}