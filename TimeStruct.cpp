/**************************************************
Copyright (C), 2011-2012, RIST OF UESTC.
* FileName     :  TimeStruct.cpp
* Author       :  YLF(cxwcylf@126.com)
* Version      :  v1.1
* Date         :  15/08/2012
* Description  :  本地时间转换为GPS时间
* Function List:  TimeSet::TimeSet()
*                 void TimeSet::SetLocalTime()
*                 void TimeSet::LocalTime2UTCTime()
*                 double TimeSet::UTCTime2MJDTime()
*                 void TimeSet::UTCTime2GPSTime()
*                 void TimeSet::ShowTime()
* History      :  
**************************************************/
#include "TimeStruct.h"
#include <assert.h>
#include <iostream>
#include <iomanip>
using namespace std;

/**************************************************
* Function   :  TimeSet
* Description:  TimeSet类的构造函数,初始时间为本地时间(北京时间)
* Calls      :  SetLocalTime
**************************************************/
TimeSet::TimeSet() //默认构造函数，初始时间为北京时间1980年1月6日8时整，即GPS时间0周0秒
{
    SetLocalTime(1980, 1, 6, 8, 0, 0);
    time.weekID = 0;
    time.SecInWeek = 0;
}


TimeSet::TimeSet(Time t)
{
    SetLocalTime(t.year, t.mon, t.day, t.hour, t.min, t.sec);
    time.weekID = 0;
    time.SecInWeek = 0;
}


TimeSet::TimeSet(int Y, int M, int D, int h, int m, int s)
{
    //设置本地时间
    SetLocalTime(Y, M, D, h, m,s);
    time.weekID = 0;
    time.SecInWeek = 0;
}


/**************************************************
 * Function   :  SetLocalTime()
 * Description:  设置本地时间(如北京时间)
 **************************************************/
void TimeSet::SetLocalTime(int Y = 1900, int M = 1, int D = 1, int h = 0, int m = 0, int s = 0)
{
    time.year = Y; 
    time.mon = M;
    time.day = D;
    time.hour = h;
    time.min = m;
    time.sec = s;
}


/**************************************************
 * Function   :  LocalTime2UTCTime()
 * Description:  将本地时间转换为UTC时间
 **************************************************/
void TimeSet::LocalTime2UTCTime()
{
    if (time.hour >= 8)
    {
        time.hour = time.hour - 8;
    }
    else if (time.day != 1)
    {
        time.hour += 16;
        time.day -= 1;
    }
    else 
    {
        time.hour += 16;
        switch (time.mon)
        {

            case 2:
            case 4:
            case 6:
            case 8:
            case 9:
            case 11:
                time.day = 31;
                time.mon -= 1;
                break;
            case 5:
            case 7:
            case 10:
            case 12:
                time.day = 30;
                time.mon -= 1;
                break;
            case 3:
                if ((time.year % 4 == 0 && time.year % 100 != 0) 
                        || (time.year % 400 == 0))
                {
                    time.day = 29;
                    time.mon -= 1;
                }
                else
                {
                    time.day = 28;
                    time.mon -= 1;
                }
                break;
            case 1:
                time.day = 31;
                time.year -= 1;
                time.mon = 12;
                break;
            default:
                break;
        }
    }
}


/**************************************************
 * Function   :  double UTCTime2MJDTime()
 * Description:  将公历年月日时分秒转换为儒略日
 * Called By  :  void TimeSet::UTCTime2GPSTime()
 * Return     :  儒略日mjd
 **************************************************/
double TimeSet::UTCTime2MJDTime()
{
    double mjd;
    int year = time.year;
    int month = time.mon;
    double day = time.day + (time.hour*3600 + time.min*60 + time.sec)/86400.0;
    int y = time.year + 4800;

    if (year < 0)
        y += 1;

    //为方便计算将将month转化
    int m = month;
    if (m <= 2)
    {
        m += 12;
        y -= 1; 
    }

    int e = (int)(30.6 * (m +1));
    int a = (int)(y / 100);

    // 教皇格雷戈里十三世于1582年2月24日以教皇训令颁布，将1582年10月5日至14日抹掉
    //1582年10月4日过完后第二天是10月15日
    int b;
    if ((year < 1582) ||(year == 1582 &&month < 10) || (year == 1582 &&month == 10 && day < 15))
    {
        b = -38;
    }
    else
    {
        b = (int)(a / 4 - a);
    }

    //计算儒略日
    int c = (int)(365.25 * y);
    double jd = b + c + e + day - 32167.5;
    mjd = jd - 2400000.5;
    return mjd;
}  


/**************************************************
 * Function   :  void UTCTime2GPSTime()
 * Description:  将UTC时间转换位GPS和周内的秒
 **************************************************/
void TimeSet::UTCTime2GPSTime()
{
    double mjd = UTCTime2MJDTime();
    //1980年1月6日0时0分0秒 == MJD- 44244
    double temp = mjd -44244;

    //将增加闰秒的点转换为年份，如1988年6月30日23点59分60秒,
    //那么在在6月之前闰秒不变，6月之后闰秒加一，
    //故以月份计算分割点x，即6/12<x<7/12，不考虑时分秒,
    //在12月31日增加的闰秒方法类似
    double TransitYear[] = {1981.58, 1982.58, 1983.58, 1985.58, 1988.08, 1990.08, 1991.08, 1992.58, 1993.58, 
        1994.58, 1996.08, 1997.58, 1999.08, 2006.08, 2009.08, 2012.58};
    int LeapSecond = 0; //闰秒
    double TempYear = time.year + time.mon/12.0;  //utc时间年份，将月份包含进去
    int length = sizeof(TransitYear)/sizeof(double);
    for(int i = 0; i < length; i++)
    {
        LeapSecond += TempYear/TransitYear[i];
    }


    time.weekID = (int)(temp /7); //周数
    temp = temp - time.weekID*7;       //周内天数
    time.SecInWeek = temp*86400 + LeapSecond;
    //if (time.weekID >= 1024)
    //{
        //time.weekID %= 1024;
    //}
}


/**************************************************
* Function   :  void TimeSet::LocalTime2GPSTime()
* Description:  本地时间转换为GPS时间
* Calls      :  TimeSet::LocalTime2UTCTime();
                TimeSet::UTCTime2GPSTime();
**************************************************/
void TimeSet::LocalTime2GPSTime()
{
     TimeSet::LocalTime2UTCTime();
     TimeSet::UTCTime2GPSTime();
}


/**************************************************
* Function   :   void TimeSet::ShowTime()
* Description:  打印UTC时间和GPS时间
**************************************************/
void TimeSet::ShowTime()
{
    cout << "UTC Time: " << endl;
    cout <<"year: " << time.year;
    cout << "   month: " << time.mon;
    cout << "   day: " << time.day;
    cout << "   hour: " << time.hour;
    cout << "   min: " << time.min;
    cout << "   sec: " << time.sec << endl;
    cout << "The time weekID: " << time.weekID << endl;
    cout << "The SecInWeek: " << time.SecInWeek << endl;
}


/**************************************************
* Function   :  TimeSet::~TimeSet()
* Description:  TimeSet类的析构函数
**************************************************/
TimeSet::~TimeSet()
{
     //do nothing
}
