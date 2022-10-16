#include "SupClass.h"
DateTime::DateTime(){
    
}

DateTime::DateTime(int year,int month, int day){
    this -> st.tm_year = year - 1900;
    this -> st.tm_mon = month - 1;
    this -> st.tm_mday = day;
    this -> st.tm_hour = 0;
    this -> st.tm_min = 0;
    this -> st.tm_sec = 0;    
    this -> st.tm_isdst = 0;
    this -> cct = mktime(&(this->st));
}
DateTime::DateTime(int year,int month, int day,int hour,int minu,int sec){
    this -> st.tm_year = year - 1900;
    this -> st.tm_mon = month - 1;
    this -> st.tm_mday = day;
    this -> st.tm_hour = hour;
    this -> st.tm_min = minu;
    this -> st.tm_sec = sec;
    this -> st.tm_isdst = 0;
    this -> cct = mktime(&(this->st));
}

/*
    Construction function of DateTime
    use the string to get the DateTime
    the string MUST have the style:
    1."yyyy/mm/dd/hh/mm/ss"
    2."yyyy/mm/dd"
    otherwise error will occur

*/
DateTime::DateTime(string time_str){
    //define two string to test the length
    string long_time_t = "1999/12/31/11/01/01";
    string short_time_t = "1999/12/31";
    if( time_str.length() == long_time_t.length() ){
        // if this is a long string
        string get;
        int set_num;
        // this is for get the part of the string
        // year:
        get = time_str.substr(0,4);
        set_num = atoi(get.c_str());
        this -> st.tm_year = set_num - 1900;
        // month:
        get = time_str.substr(5,2);
        set_num = atoi(get.c_str());
        this -> st.tm_mon = set_num - 1;
        // day:
        get = time_str.substr(8,2);
        set_num = atoi(get.c_str());
        this -> st.tm_mday = set_num;
        // hour:
        get = time_str.substr(11,2);
        set_num = atoi(get.c_str());
        this -> st.tm_hour = set_num;
        // minute:
        get = time_str.substr(14,2);
        set_num = atoi(get.c_str());
        this -> st.tm_min = set_num;
        // second:
        get = time_str.substr(17,2);
        set_num = atoi(get.c_str());
        this -> st.tm_sec = set_num;

    }
    else if( time_str.length() == short_time_t.length() ){
        // if this is a short string
        string get;
        int set_num;
        // this is for get the part of the string
        // year:
        get = time_str.substr(0,4);
        set_num = atoi(get.c_str());
        this -> st.tm_year = set_num - 1900;
        // month:
        get = time_str.substr(5,2);
        set_num = atoi(get.c_str());
        this -> st.tm_mon = set_num - 1;
        // day:
        get = time_str.substr(8,2);
        set_num = atoi(get.c_str());
        this -> st.tm_mday = set_num;
        // hour:
        this -> st.tm_hour = 0;
        // minute:
        this -> st.tm_min = 0;
        // second:
        this -> st.tm_sec = 0;
    }
    else{
        // it seems that this is a wrong style
        this -> st.tm_year = 0;
        this -> st.tm_mon = 0;
        this -> st.tm_mday = 0;
        this -> st.tm_hour = 0;
        this -> st.tm_min = 0;
        this -> st.tm_sec = -1;  
    }
    ///// this part is completely same to other 2 ones.
    this -> st.tm_isdst = 0;
    this -> cct = mktime(&(this->st));
}

void DateTime::AddSecond(long long sec){
    this -> cct += sec;
    this -> st = *(localtime(&(this->cct)));
}
/*
Show whether current time is larger than input time.
*/
bool DateTime::LaterThan(const DateTime &that) const{
    if (difftime(this -> cct, that.cct)>0) return 1;
    return 0;
}
/*
Two time's second difference
*/
time_t DateTime::SecLater(const DateTime &that) const{
    return this->cct - that.cct;
}
int DateTime::YearLater(const DateTime &that) const {
    if (this -> st.tm_year < that.st.tm_year) return -that.YearLater(*this);
    int r = this->st.tm_year - that.st.tm_year;
    if (this -> st.tm_mon > that.st.tm_mon) 
        return r;
    else 
        if (this -> st.tm_mon == that.st.tm_mon) {
            if (this -> st.tm_mday > that.st.tm_mday) 
                return r;
            else 
                return r - 1;
        } else return r-1;
}
int DateTime::DayLater(const DateTime &that) const{
    return (this->cct - that.cct)/86400;
}
int DateTime::HourLater(const DateTime &that) const{
    return (this->cct - that.cct)/3600;
}

/*
    function: DateTime::ToString()
    this function return the string style of the DateTime,
    for this function, the return style is:"yyyy/mm/dd/hh/mm/ss"
    Example: "2022/04/01/02/03/12"
    the order is: year, month, date, hour, minute and second
    input: none, because it is in the class of DateTime
    output: the string that we need

*/
string DateTime::ToString() const{
    //char* u = new char[32]; no need to use this char
    // use this u_str to return
    string u_str = "";
    
    //u_str = u_str + to_string(st.tm_year+1900) + string("y ");
    // use the new style
    u_str = u_str + to_string(st.tm_year+1900) + string("/");

    //if( st.tm_mon+1 < 10 ){u_str = u_str + string("0") + to_string( st.tm_mon+1 ) + string("m ");}
    //else{u_str = u_str + to_string( st.tm_mon+1 ) + string("m ");}
    // in order to make sure that this function is correct, we straightly get the string length
    // month part
    if( (to_string( st.tm_mon+1 )).length() == 0 ){ u_str = u_str + string("00") + to_string( st.tm_mon+1 ) + string("/"); }
    else if( (to_string( st.tm_mon+1 )).length() == 1 ){ u_str = u_str + string("0") + to_string( st.tm_mon+1 ) + string("/"); }
    else{ u_str = u_str + to_string( st.tm_mon+1 ) + string("/"); }

    // date part
    //if( st.tm_mday < 10  ){u_str = u_str + string("0") + to_string( st.tm_mday ) + string("d  ");}
    //else{u_str = u_str + to_string( st.tm_mday ) + string("d ");}
    if( (to_string( st.tm_mday )).length() == 0 ){ u_str = u_str + string("00") + to_string( st.tm_mday ) + string("/"); }
    else if( (to_string( st.tm_mday )).length() == 1 ){ u_str = u_str + string("0") + to_string( st.tm_mday ) + string("/"); }
    else{ u_str = u_str + to_string( st.tm_mday ) + string("/"); }

    
    // hour part
    //if( st.tm_hour < 10  ){u_str = u_str + string("0") + to_string( st.tm_hour ) + string(" : ");}
    //else{u_str = u_str + to_string( st.tm_hour ) + string(" : ");}
    if( (to_string( st.tm_hour)).length() == 0 ){ u_str = u_str + string("00") + to_string( st.tm_hour ) + string("/"); }
    else if( (to_string( st.tm_hour )).length() == 1 ){ u_str = u_str + string("0") + to_string( st.tm_hour ) + string("/"); }
    else{ u_str = u_str + to_string( st.tm_hour ) + string("/"); }

    // minute part
    //if( st.tm_min < 10  ){u_str = u_str + string("0") + to_string( st.tm_min ) + string(" : ");}
    //else{u_str = u_str + to_string( st.tm_min ) + string(" : ");}
    if( (to_string( st.tm_min )).length() == 0 ){ u_str = u_str + string("00") + to_string( st.tm_min ) + string("/"); }
    else if( (to_string( st.tm_min )).length() == 1 ){ u_str = u_str + string("0") + to_string( st.tm_min ) + string("/"); }
    else{ u_str = u_str + to_string( st.tm_min ) + string("/"); }
    
    // second part
    if( (to_string( st.tm_sec )).length() == 0 ){ u_str = u_str + string("00") + to_string( st.tm_sec ); }
    else if( (to_string( st.tm_sec )).length() == 1 ){ u_str = u_str + string("0") + to_string( st.tm_sec ); }
    else{ u_str = u_str + to_string( st.tm_sec ); }

    return u_str;
}

/*
    function: DateTime::ToShortString()
    this function return the string style of the DateTime, similar to the ToString
    for this function, the return style is:"yyyy/mm/dd"
    the order is: year, month, date.
    Example: "2022/04/01"
    input: none, because it is in the class of DateTime
    output: the string that we need
*/
string DateTime::ToShortString() const{
    //char* u = new char[32]; no need to use this char
    // use this u_str to return
    string u_str = "";
    
    //u_str = u_str + to_string(st.tm_year+1900) + string("y ");
    // use the new style
    u_str = u_str + to_string(st.tm_year+1900) + string("/");

    //if( st.tm_mon+1 < 10 ){u_str = u_str + string("0") + to_string( st.tm_mon+1 ) + string("m ");}
    //else{u_str = u_str + to_string( st.tm_mon+1 ) + string("m ");}
    // in order to make sure that this function is correct, we straightly get the string length
    // month part
    if( (to_string( st.tm_mon+1 )).length() == 0 ){ u_str = u_str + string("00") + to_string( st.tm_mon+1 ) + string("/"); }
    else if( (to_string( st.tm_mon+1 )).length() == 1 ){ u_str = u_str + string("0") + to_string( st.tm_mon+1 ) + string("/"); }
    else{ u_str = u_str + to_string( st.tm_mon+1 ) + string("/"); }

    // date part
    //if( st.tm_mday < 10  ){u_str = u_str + string("0") + to_string( st.tm_mday ) + string("d  ");}
    //else{u_str = u_str + to_string( st.tm_mday ) + string("d ");}
    if( (to_string( st.tm_mday )).length() == 0 ){ u_str = u_str + string("00") + to_string( st.tm_mday ); }
    else if( (to_string( st.tm_mday )).length() == 1 ){ u_str = u_str + string("0") + to_string( st.tm_mday ); }
    else{ u_str = u_str + to_string( st.tm_mday ); }


    return u_str;
}


