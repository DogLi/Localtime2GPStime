/**************************************************
Copyright (C), 2011-2012, RIST OF UESTC.
* FileName     :  TimeStruct.cpp
* Author       :  YLF(cxwcylf@126.com)
* Version      :  v1.1
* Date         :  15/08/2012
* Description  :  ����ʱ��ת��ΪGPSʱ��
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
* Description:  TimeSet��Ĺ��캯��,��ʼʱ��Ϊ����ʱ��(����ʱ��)
* Calls      :  SetLocalTime
**************************************************/
TimeSet::TimeSet() //Ĭ�Ϲ��캯������ʼʱ��Ϊ����ʱ��1980��1��6��8ʱ������GPSʱ��0��0��
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
    //���ñ���ʱ��
    SetLocalTime(Y, M, D, h, m,s);
    time.weekID = 0;
    time.SecInWeek = 0;
}


/**************************************************
 * Function   :  SetLocalTime()
 * Description:  ���ñ���ʱ��(�籱��ʱ��)
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
 * Description:  ������ʱ��ת��ΪUTCʱ��
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
 * Description:  ������������ʱ����ת��Ϊ������
 * Called By  :  void TimeSet::UTCTime2GPSTime()
 * Return     :  ������mjd
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

    //Ϊ������㽫��monthת��
    int m = month;
    if (m <= 2)
    {
        m += 12;
        y -= 1; 
    }

    int e = (int)(30.6 * (m +1));
    int a = (int)(y / 100);

    // �̻ʸ��׸���ʮ������1582��2��24���Խ̻�ѵ��䲼����1582��10��5����14��Ĩ��
    //1582��10��4�չ����ڶ�����10��15��
    int b;
    if ((year < 1582) ||(year == 1582 &&month < 10) || (year == 1582 &&month == 10 && day < 15))
    {
        b = -38;
    }
    else
    {
        b = (int)(a / 4 - a);
    }

    //����������
    int c = (int)(365.25 * y);
    double jd = b + c + e + day - 32167.5;
    mjd = jd - 2400000.5;
    return mjd;
}  


/**************************************************
 * Function   :  void UTCTime2GPSTime()
 * Description:  ��UTCʱ��ת��λGPS�����ڵ���
 **************************************************/
void TimeSet::UTCTime2GPSTime()
{
    double mjd = UTCTime2MJDTime();
    //1980��1��6��0ʱ0��0�� == MJD- 44244
    double temp = mjd -44244;

    //����������ĵ�ת��Ϊ��ݣ���1988��6��30��23��59��60��,
    //��ô����6��֮ǰ���벻�䣬6��֮�������һ��
    //�����·ݼ���ָ��x����6/12<x<7/12��������ʱ����,
    //��12��31�����ӵ����뷽������
    double TransitYear[] = {1981.58, 1982.58, 1983.58, 1985.58, 1988.08, 1990.08, 1991.08, 1992.58, 1993.58, 
        1994.58, 1996.08, 1997.58, 1999.08, 2006.08, 2009.08, 2012.58};
    int LeapSecond = 0; //����
    double TempYear = time.year + time.mon/12.0;  //utcʱ����ݣ����·ݰ�����ȥ
    int length = sizeof(TransitYear)/sizeof(double);
    for(int i = 0; i < length; i++)
    {
        LeapSecond += TempYear/TransitYear[i];
    }


    time.weekID = (int)(temp /7); //����
    temp = temp - time.weekID*7;       //��������
    time.SecInWeek = temp*86400 + LeapSecond;
    //if (time.weekID >= 1024)
    //{
        //time.weekID %= 1024;
    //}
}


/**************************************************
* Function   :  void TimeSet::LocalTime2GPSTime()
* Description:  ����ʱ��ת��ΪGPSʱ��
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
* Description:  ��ӡUTCʱ���GPSʱ��
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
* Description:  TimeSet�����������
**************************************************/
TimeSet::~TimeSet()
{
     //do nothing
}
