/**************************************************
Copyright (C), 2011-2012, RIST OF UESTC.
* FileName     :  TimeStruct.h
* Author       :  YLF(cxwcylf@126.com)
* Version      :  v1.0
* Date         :  15/08/2012
* Description  :  GPS中时间转换的头文件
**************************************************/

#ifndef _TIMESTRUCT_H_
#define _TIMESTRUCT_H_
struct Time
{
    //int nsec;  /*纳秒 - 取值区间为[0,59] */
    //int usec;  /*微妙 - 取值区间为[0,59] */
    //int msec;  /*毫秒 - 取值区间为[0,59] */
    int sec;     /*秒 - 取值区间为[0,59] */
    int min;     /*分 - 取值区间为[0,59] */
    int hour;    /*时 - 取值区间为[0,23] */
    int day;     /*天 - 取值区间为[1,31] */
    int mon;     /*月 - 取值区间为[1,12] */
    int year;    /*年 - 取值区间>1980 年*/
    int weekID;
    int SecInWeek;
};

class TimeSet
{
    private:
        Time time; //本地时间
    public:
        ~TimeSet(); 
        TimeSet();
        TimeSet(Time);
        TimeSet(int, int, int, int, int, int);
        void SetLocalTime(int, int, int, int, int, int); //设置本地时间，如北京时间
        void LocalTime2UTCTime(); //UTC时间
        void UTCTime2GPSTime();
        void LocalTime2GPSTime();
        double UTCTime2MJDTime();
        void ShowTime();
};

#endif
