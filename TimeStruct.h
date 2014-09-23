/**************************************************
Copyright (C), 2011-2012, RIST OF UESTC.
* FileName     :  TimeStruct.h
* Author       :  YLF(cxwcylf@126.com)
* Version      :  v1.0
* Date         :  15/08/2012
* Description  :  GPS��ʱ��ת����ͷ�ļ�
**************************************************/

#ifndef _TIMESTRUCT_H_
#define _TIMESTRUCT_H_
struct Time
{
    //int nsec;  /*���� - ȡֵ����Ϊ[0,59] */
    //int usec;  /*΢�� - ȡֵ����Ϊ[0,59] */
    //int msec;  /*���� - ȡֵ����Ϊ[0,59] */
    int sec;     /*�� - ȡֵ����Ϊ[0,59] */
    int min;     /*�� - ȡֵ����Ϊ[0,59] */
    int hour;    /*ʱ - ȡֵ����Ϊ[0,23] */
    int day;     /*�� - ȡֵ����Ϊ[1,31] */
    int mon;     /*�� - ȡֵ����Ϊ[1,12] */
    int year;    /*�� - ȡֵ����>1980 ��*/
    int weekID;
    int SecInWeek;
};

class TimeSet
{
    private:
        Time time; //����ʱ��
    public:
        ~TimeSet(); 
        TimeSet();
        TimeSet(Time);
        TimeSet(int, int, int, int, int, int);
        void SetLocalTime(int, int, int, int, int, int); //���ñ���ʱ�䣬�籱��ʱ��
        void LocalTime2UTCTime(); //UTCʱ��
        void UTCTime2GPSTime();
        void LocalTime2GPSTime();
        double UTCTime2MJDTime();
        void ShowTime();
};

#endif
