#include "SupClass.h"

bool RecordPriority::Priority_Rule1(const Record* a,const Record* b) {
    if ((a->reg->isEmergency)&&(!b->reg->isEmergency)) return true;
    if ((!a->reg->isEmergency)&&(b->reg->isEmergency)) return false;    //If the registration has a priority letter
    if ((a->reg->isEmergency)&&(b->reg->isEmergency)) {
        if (b->reg->deadline.LaterThan(a->reg->deadline)) return true; else return false;   //Decide the priority between two prioritized registration
    }
    if ((a->med->risk_gp != 3)&&(b->med->risk_gp == 3)) return true;
    if ((a->med->risk_gp == 3)&&(b->med->risk_gp != 3)) return false;     //Decide the priority between High Risk and other risk groups
    if (a->person->prof > b->person->prof) return true;                 
    if (a->person->prof < b->person->prof) return false;                 //Decide the priority with different professions
    if (a->med->age_gp > b->med->age_gp) return true;
    if (a->med->age_gp < b ->med->age_gp) return false;             //Decide the priority with different age group
    if (b->reg->considertime.LaterThan(a->reg->considertime)) return true;       //Decide the priority by considered time
    return false;   
}

bool RecordPriority::Priority_Rule2(const Record* a,const Record* b) {
    if ((a->reg->isEmergency)&&(!b->reg->isEmergency)) return true;
    if ((!a->reg->isEmergency)&&(b->reg->isEmergency)) return false;    //If the registration has a priority letter
    if ((a->reg->isEmergency)&&(b->reg->isEmergency)) {
        if (b->reg->deadline.LaterThan(a->reg->deadline)) return true; else return false;   //Decide the priority between two prioritized registration
    }
    if ((a->med->risk_gp) < (b->med->risk_gp)) return true;
    if ((a->med->risk_gp) > (b->med->risk_gp)) return false;     //Decide the priority between High Risk and other risk groups
    if (a->person->prof < b->person->prof) return true;                 
    if (a->person->prof > b->person->prof) return false;                 //Decide the priority with different professions
    if (a->med->age_gp < b->med->age_gp) return true;
    if (a->med->age_gp > b ->med->age_gp) return false;             //Decide the priority with different age group
    if (b->reg->considertime.LaterThan(a->reg->considertime)) return true;       //Decide the priority by considered time
    return false;
}

bool RecordPriority::Priority_Rule3(const Record* a,const Record* b) {
    if (a->med->age_gp > b->med->age_gp) return true;
    if (a->med->age_gp < b ->med->age_gp) return false;             //Decide the priority with different age group
    if ((a->med->risk_gp) < (b->med->risk_gp)) return true;
    if ((a->med->risk_gp) > (b->med->risk_gp)) return false;     //Decide the priority between High Risk and other risk groups
    if (b->reg->considertime.LaterThan(a->reg->considertime)) return true;       //Decide the priority by considered time
    if (a->reg->considertime.LaterThan(b->reg->considertime)) return false;
    if ((a->reg->isEmergency)&&(!b->reg->isEmergency)) return true;
    if ((!a->reg->isEmergency)&&(b->reg->isEmergency)) return false;    //If the registration has a priority letter
    if ((a->reg->isEmergency)&&(b->reg->isEmergency)) {
        if (b->reg->deadline.LaterThan(a->reg->deadline)) return true; else return false;   //Decide the priority between two prioritized registration
    }
    if (a->person->prof > b->person->prof) return true;     //Decide the priority with different professions
    return false;
}

const long RecordPriority::treattime[3] =  {3600,10800,43200};

int Age_Group_get( int age ){
    if( age <= 12 ){
        return 0;
    }
    else if( age <= 18 ){
        return 1;
    }
    else if( age <= 35 ){
        return 2;
    }
    else if( age <= 50 ){
        return 3;
    }
    else if( age <= 65 ){
        return 4;
    }
    else if( age <= 75 ){
        return 5;
    }
    else if( age > 75 ){
        return 6;
    }
    return 0;
}
