#include "TimeStruct.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    TimeSet a;
    a.LocalTime2GPSTime();
    a.ShowTime();
    cout << endl;

    TimeSet t(1982,7,1,7,59,59);
    t.LocalTime2GPSTime();
    t.ShowTime();
    cout << endl;

    t.SetLocalTime(1982,7,1,8,0,0);
    t.LocalTime2GPSTime();
    t.ShowTime();
    cout << endl;


    int year, month, day, hour, min, sec;
    cin >> year >> month >> day >> hour >> min >> sec;
    t.SetLocalTime(year, month, day, hour, min, sec);
    t.LocalTime2GPSTime();
    t.ShowTime();
    return 0;
}
