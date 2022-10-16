#include "SupClass.h"
/*
    Registration -- import the register's personal information
            For more details, the input string "line" stands for one line in the CSV file.
            It uses for extracting the string for the fields in "Registration".
            One turn for one registration.
    Input: fname -- csv file name
    Output: timestamp -- the registation timestamp
            name -- the patient's name
            id -- the patient's id
            ct -- the patient's address, phone, WeChat, email, coordinate
            prof -- the patient's profession
            birth -- the patient's birthday
            agegp -- the patient's age group
            risk -- the patient's risk status
    Return: construction function, return Registration
    Side effect: None
*/
Registration::Registration(){
    
}
Registration::Registration(string line){
    int i;
    int temp[6], xy[2];
    string field;
    string address, cd, phone, WeChat, email;
    istringstream line_in(line); // read "line" into istringstream.
    string date, time;
    getline(line_in, date, ','); // Read the registration date.
    getline(line_in, time, ','); // Read the registration time.
    string p1;
    istringstream date_in(date); // seperate date to year, month, date.
    for(i = 0; i < 3; i++){
       	getline(date_in, p1, '/');
       	const char* t1 = p1.data();
       	temp[i] = atoi(t1);  // Turn string into integer.
	}
    istringstream time_in(time); // seperate time to hour, minute, second.
    for(i = 3; i < 6; i++){
       	getline(time_in, p1, ':');
       	const char* t1 = p1.data();
       	temp[i] = atoi(t1);
	}
    DateTime dt1(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]);
    this -> timestamp = dt1;
    getline(line_in, field, ','); // Read the name of the register.
    this -> name = field;
    getline(line_in, field, ','); // Read the id of the register.   
    this -> id = field;
    getline(line_in, address, ','); // Read the contact detail of the register. 
    getline(line_in, cd, ',');  // Address, Coordinate, Phone, WeChat, and email.
    getline(line_in, phone, ','); 
    getline(line_in, WeChat, ','); 
    getline(line_in, email, ','); 
    istringstream str_in(cd); // seperate coordinate "x|y" to x and y.
    for(i = 0; i < 2; i++){
       	getline(str_in, p1, '|');
       	const char* t1 = p1.data();
       	xy[i] = atoi(t1);
	}
    Location addr(xy[0], xy[1]);
    ContactDetail ct(address, phone, WeChat, email, addr);
    this -> ct = ct;
    getline(line_in, field, ','); // Read the profession.
    const char* t1 = field.data();
    temp[0] = atoi(t1);
    this -> prof = temp[0];
    getline(line_in, field, ','); // Read the birthday.
    istringstream bir_in(field); // seperate birthday to year, month, date.
    for(i = 0; i < 3; i++){
        getline(bir_in, p1, '/');
        const char* t1 = p1.data();
        temp[i] = atoi(t1);  // Turn string into integer.
	}
	this -> birth = DateTime(temp[0], temp[1], temp[2]);
	getline(line_in, field, ','); // Read the risk.
    const char* t2 = field.data();
    temp[0] = atoi(t2);
    this -> risk = temp[0];
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