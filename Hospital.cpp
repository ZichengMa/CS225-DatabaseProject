#include "SupClass.h"
#include <cmath>
#include <iostream>
using namespace std;
#define INF 100000.0

/*
Return the best hospital. If no hospital is valiable, return nullptr.
hospitals -- An array of hospitals. In our program, we define 3 hospitals
loc -- The patient's location
size -- The length of hospitals.
*/
Hospital* BestHospital(Hospital* hospitals,Location* loc,int size){
    int i;
    bool flag = true;
    for(i=0;i<size;i++){ // Run through hospitals and check whether they can recive a patient
        flag = flag && (hospitals[i].capacity==0);
    }
    // After the loop, if flag is still true, there are no beds for patient
    if(flag)
        return nullptr;
    int index=0; // This shows the best hospital's index
    double best_distance=INF; // This shows current best distance.
    for(i=0;i<size;i++){ // Run through hospitals and check which is the best
        if(hospitals[i].capacity!=0){
            // This hospital has beds, calculate distance.
            if(hospitals->loc.DistanceTo(loc)<best_distance){
                index = i;
                best_distance = hospitals->loc.DistanceTo(loc)<best_distance;
            }
        }else{
            // This hospital has no beds, continue to loop.
            continue;
        }
    }
    return (hospitals+index);
}


Hospital::Hospital(string name, int x,int y, int capacity){
    this->name=name;
    this->loc = Location(x,y);
    this->capacity = capacity;
}

Hospital::Hospital(){}