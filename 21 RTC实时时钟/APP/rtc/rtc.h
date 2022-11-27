#ifndef _rtc_H
#define _rtc_H

#include <system.h>

//ʱ��ṹ��
typedef struct
{
	u8 hour;  //ʱ
	u8 min;   //��
	u8 sec;   //��
	
	u16 year;  //��
	u8 month;  //��
	u8 date;   //��
	u8 week;   //��
}_calendar;

extern _calendar calendar;

u8 RTC_Set(u16 year,u8 month,u8 data,u8 hour,u8 min,u8 sec);  // ����ʱ��
u8 Leap_year(u16 year);  // �ж��Ƿ�Ϊ����
u8 RTC_Get(void);  // ��õ�ǰʱ��
u8 RTC_Get_Week(u16 year,u8 month,u8 data);  // ��ü������ڼ�
u8 RTC_Init(void);
static void RTC_NVIC_Config(void);
u8 RTC_Alarm_Set(u16 year,u8 month,u8 data,u8 hour,u8 min,u8 sec);  // ��������

#endif
