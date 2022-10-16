#include "SupClass.h"

Record::Record(){
    
}

Record::Record(unsigned long uid, Person* person, Medical_State* med, Registration* reg, Treatment* treatment) {
    this->uid = uid;
    this->person = person;
    this->med = med;
    this->reg = reg;
    this->treatment = treatment;
}