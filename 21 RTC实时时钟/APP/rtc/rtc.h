#ifndef _rtc_H
#define _rtc_H

#include <system.h>

//时间结构体
typedef struct
{
	u8 hour;  //时
	u8 min;   //分
	u8 sec;   //秒
	
	u16 year;  //年
	u8 month;  //月
	u8 date;   //日
	u8 week;   //周
}_calendar;

extern _calendar calendar;

u8 RTC_Set(u16 year,u8 month,u8 data,u8 hour,u8 min,u8 sec);  // 设置时间
u8 Leap_year(u16 year);  // 判断是否为润年
u8 RTC_Get(void);  // 获得当前时间
u8 RTC_Get_Week(u16 year,u8 month,u8 data);  // 获得几天星期几
u8 RTC_Init(void);
static void RTC_NVIC_Config(void);
u8 RTC_Alarm_Set(u16 year,u8 month,u8 data,u8 hour,u8 min,u8 sec);  // 设置闹钟

#endif
