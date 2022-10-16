#include "SupClass.h"
#include <cmath>
Location::Location(){

}
Location::Location(int x, int y){
    this -> x = x;
    this -> y = y;
}
double Location::DistanceTo(Location* that){
    return sqrt(pow(this->x - that->x,2)+pow(this->y - that->y,2));
}

ContactDetail::ContactDetail(){

}

ContactDetail::ContactDetail(string address, string phone, string WeChat, string email, Location &addr) {
    this->address = address;
    this -> email = email;
    this -> phone  = phone;
    this -> WeChat = WeChat;
    this -> addr.x = addr.x;
    this -> addr.y = addr.y;
}
