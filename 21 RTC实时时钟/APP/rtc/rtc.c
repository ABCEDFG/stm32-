#include <rtc.h>
#include <sysTick.h>
#include <usart.h>


_calendar calendar;  //时间结构体

//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
	 
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};


static void RTC_NVIC_Config(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


//返回 1：初始化失败  0：初始化成功
u8 RTC_Init()
{
	u16 i=0;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  // 开启PWR时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);  // 开启BKP时钟
	
	PWR_BackupAccessCmd(ENABLE);  // 使能后备寄存器访问
	
	// 查看写入与读取值是否一致，用以判断是否初始化
	if(BKP_ReadBackupRegister(BKP_DR1) != 0Xa0a1)
	{
		BKP_DeInit();  // 复位备份区域(清空)
		RCC_LSEConfig(RCC_LSE_ON);  // 开启 LSE 时钟 32.768kHz
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==0 && i<250)  // 等待LSE稳定
		{
			i++;
			delay_ms(10);
		}
		if(i>=250)  // 初始化时钟失败，晶振有问题
		{
			return 1;
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  // 选择LSE为RTC时钟
		RCC_RTCCLKCmd(ENABLE);  // 使能RTC时钟
		RTC_WaitForLastTask();  // 等待最近一次对RTC寄存器的写操作完成
		
		RTC_WaitForSynchro();   // 等待RTC寄存器同步
		RTC_ITConfig(RTC_IT_SEC,ENABLE);  // 设置RTC秒中断
		RTC_WaitForLastTask();  // 等待最近一次对RTC寄存器的写操作完成
		
		RTC_EnterConfigMode();  // 允许配置
		RTC_SetPrescaler(32767);  // 设置RTC预分频的值
		RTC_WaitForLastTask();  // 等待最近一次对RTC寄存器的写操作完成
		
		RTC_Set(2021,2,9,9,55,0);  // 设置时间
		
		RTC_ExitConfigMode(); // 退出配置模式  
		
		BKP_WriteBackupRegister(BKP_DR1,0xa0a1);  // 向寄存器写入指定值，用以标记已初始化
	}
	else
	{
		RTC_WaitForSynchro();   //等待RTC寄存器同步
		RTC_ITConfig(RTC_IT_SEC,ENABLE);  //设置RTC秒中断
		RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
	}
	RTC_NVIC_Config();
	RTC_Get();  // 获得当前时间
	return 0;
}

//获得今天时星期几
//输入 年 月 日
u8 RTC_Get_Week(u16 year,u8 month,u8 data)  //没懂
{
	u16 temp;
	u8 yearH,yearL;
	
	yearH=year/100;
	yearL=year%100;
	if(yearH>19)
	{
		yearL+=100;
	}
	temp=yearL+yearL/4;
	temp=temp%7;
	temp=temp+data+table_week[month-1];
	
	if(yearL%4==0 && month<3)
	{
		temp--;
	}
	return (temp%7);
}

//得到当前的时间  保存在结构体 calendar 中
//返回值：  0：成功  其它：错误
u8 RTC_Get()
{
	static u16 data=0;  //用于记录天数
	u32 s;
	u32 time=0;  //获取时间
	u16 temp=0;  //临时变量
	s=RTC_GetCounter();  //得到秒数
	time=s/86400;  //得到天数
	if(data!=time)  //是否过了一天（以24.00为准）
	{
		data=time;
		temp=1970;  //此后用以表示年份，从1970年开始
		while(time>=365)  //超过一年，次循环计算年份
		{
			if(Leap_year(temp) == 1)  //是否为闰年
			{
				if(time>=366)
				{
					time-=366;  //闰年
				}
				else
				{
					temp++;  break;
				}
			}
			else
			{
				time-=365;  //平年
			}
			temp++;
		}
		calendar.year=temp;  //得到年份
		
		temp=0;  //此后用以表示月份-1
		
		while(time>=28)  //此循环计算月份和天数
		{
			if(Leap_year(calendar.year)==1 && temp==1)  //是否为闰年2月
			{
				if(time>=29)  //大于一个月
				{
					time-=29;
				}
				else
				{
					break;
				}
			}
			else
			{
				if(time>=mon_table[temp])  //是否大于一个月
				{
					time-=mon_table[temp];
				}
				else
				{
					break;
				}
				temp++;  //月份+1
			}
		}
		calendar.month=temp+1;  //得到月份
		calendar.date=time+1;  //得到天数
	}
	time=s%86400;  //得到秒数
	calendar.hour=time/3600;  //小时
	calendar.min=(time%3600)/60;  //分钟
	calendar.sec=(time%3600)%60;  //秒数
	calendar.week=RTC_Get_Week(calendar.year,calendar.month,calendar.date);  //获取星期几
	return 0;
}

//判断是否为闰年， 1：是  0：不是
u8 Leap_year(u16 year)
{
	if(year%4==0)  //必须能被4整除
	{
		if(year%100==0)  //
		{
			if(year%400==0)
				return 1;
			else
				return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}



//RTC设置时间(1970年1月1日到输入时间的秒数)，最大到2099年
//年 月 日 时 分 秒
//返回  0：成功   1：失败
u8 RTC_Set(u16 year,u8 month,u8 data,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 s=0;
	if(year<1970 || year>2099)  //判断是否为合法年份
	{
		return 1;
	}
	for(t=1970;t<year;t++)  //计算年份的秒数
	{
		if(Leap_year(t)==1)  //是否为闰年
		{
			s+=31622400;  //闰年秒数
		}
		else
		{
			s+=31536000;  //平年秒数
		}
	}
	month--;
	for(t=0;t<month;t++)  //月的秒数
	{
		s+=(u32)mon_table[t]*86400;  //每月秒数相加
		if(Leap_year(year)==1 && t==1)  //闰年2月29日的秒数
		{
			s+=86400;
		}
	}
	s+=(u32)(data-1)*86400;  //日的秒数相加
	s+=(u32)hour*3600;  //小时的秒数相加
	s+=(u32)min*60;  //分钟的描述相加
	s+=sec;  //最后的秒数相加
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  //开启PWR时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);  //开启BKP时钟
	
	PWR_BackupAccessCmd(ENABLE);  //使能后备寄存器访问
	
	RTC_SetCounter(s);	//设置RTC计数器的值
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	return 0;	
}


//设置闹钟
//输入 年 月 日 时 分 秒
//返回 0：成功   1：失败
u8 RTC_Alarm_Set(u16 year,u8 month,u8 data,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 s;
	if(year<1970 || year>2099)  //是否为合法年份
	{
		return 1;
	}
	for(t=1970;t<year;t++)  //计算年份的秒数
	{
		if(Leap_year(year)==1)  //是否为闰年
		{
			s+=31622400;  //闰年秒数
		}
		else
		{
			s+=31536000;  //平年秒数
		}
	}
	month--;
	for(t=0;t<month;t++)  //月的秒数
	{
		s+=(u32)mon_table[t]*86400;  //每月秒数相加
		if(Leap_year(year)==1 && t==1)  //闰年2月29日的秒数
		{
			s+=86400;
		}
	}
	s+=(u32)(data-1)*86400;  //日的秒数相加
	s+=(u32)(hour-1)*3600;  //小时的秒数相加
	s+=(u32)(min-1)*60;  //分钟的描述相加
	s+=sec;  //最后的秒数相加
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  //开启PWR时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);  //开启BKP时钟
	PWR_BackupAccessCmd(ENABLE);  //使能后备寄存器访问 
	//上面三步是必须的!
	
	RTC_SetAlarm(s);
	RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成 
	return 0;
}

//RTC时钟中断
//每秒触发一次  
//extern u16 tcnt; 
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{							
		RTC_Get();//更新时间  
		printf("RTC Time:%d-%d-%d %d:%d:%d\r\n",calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);//输出闹铃时间	
				
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	
		RTC_Get();				//更新时间   
		printf("Alarm Time:%d-%d-%d %d:%d:%d\r\n",calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);//输出闹铃时间	
		
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}


