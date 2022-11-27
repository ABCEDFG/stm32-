#include <rtc.h>
#include <sysTick.h>
#include <usart.h>


_calendar calendar;  //ʱ��ṹ��

//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�
	 
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};


static void RTC_NVIC_Config(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


//���� 1����ʼ��ʧ��  0����ʼ���ɹ�
u8 RTC_Init()
{
	u16 i=0;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  // ����PWRʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);  // ����BKPʱ��
	
	PWR_BackupAccessCmd(ENABLE);  // ʹ�ܺ󱸼Ĵ�������
	
	// �鿴д�����ȡֵ�Ƿ�һ�£������ж��Ƿ��ʼ��
	if(BKP_ReadBackupRegister(BKP_DR1) != 0Xa0a1)
	{
		BKP_DeInit();  // ��λ��������(���)
		RCC_LSEConfig(RCC_LSE_ON);  // ���� LSE ʱ�� 32.768kHz
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==0 && i<250)  // �ȴ�LSE�ȶ�
		{
			i++;
			delay_ms(10);
		}
		if(i>=250)  // ��ʼ��ʱ��ʧ�ܣ�����������
		{
			return 1;
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  // ѡ��LSEΪRTCʱ��
		RCC_RTCCLKCmd(ENABLE);  // ʹ��RTCʱ��
		RTC_WaitForLastTask();  // �ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_WaitForSynchro();   // �ȴ�RTC�Ĵ���ͬ��
		RTC_ITConfig(RTC_IT_SEC,ENABLE);  // ����RTC���ж�
		RTC_WaitForLastTask();  // �ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_EnterConfigMode();  // ��������
		RTC_SetPrescaler(32767);  // ����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();  // �ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_Set(2021,2,9,9,55,0);  // ����ʱ��
		
		RTC_ExitConfigMode(); // �˳�����ģʽ  
		
		BKP_WriteBackupRegister(BKP_DR1,0xa0a1);  // ��Ĵ���д��ָ��ֵ�����Ա���ѳ�ʼ��
	}
	else
	{
		RTC_WaitForSynchro();   //�ȴ�RTC�Ĵ���ͬ��
		RTC_ITConfig(RTC_IT_SEC,ENABLE);  //����RTC���ж�
		RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}
	RTC_NVIC_Config();
	RTC_Get();  // ��õ�ǰʱ��
	return 0;
}

//��ý���ʱ���ڼ�
//���� �� �� ��
u8 RTC_Get_Week(u16 year,u8 month,u8 data)  //û��
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

//�õ���ǰ��ʱ��  �����ڽṹ�� calendar ��
//����ֵ��  0���ɹ�  ����������
u8 RTC_Get()
{
	static u16 data=0;  //���ڼ�¼����
	u32 s;
	u32 time=0;  //��ȡʱ��
	u16 temp=0;  //��ʱ����
	s=RTC_GetCounter();  //�õ�����
	time=s/86400;  //�õ�����
	if(data!=time)  //�Ƿ����һ�죨��24.00Ϊ׼��
	{
		data=time;
		temp=1970;  //�˺����Ա�ʾ��ݣ���1970�꿪ʼ
		while(time>=365)  //����һ�꣬��ѭ���������
		{
			if(Leap_year(temp) == 1)  //�Ƿ�Ϊ����
			{
				if(time>=366)
				{
					time-=366;  //����
				}
				else
				{
					temp++;  break;
				}
			}
			else
			{
				time-=365;  //ƽ��
			}
			temp++;
		}
		calendar.year=temp;  //�õ����
		
		temp=0;  //�˺����Ա�ʾ�·�-1
		
		while(time>=28)  //��ѭ�������·ݺ�����
		{
			if(Leap_year(calendar.year)==1 && temp==1)  //�Ƿ�Ϊ����2��
			{
				if(time>=29)  //����һ����
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
				if(time>=mon_table[temp])  //�Ƿ����һ����
				{
					time-=mon_table[temp];
				}
				else
				{
					break;
				}
				temp++;  //�·�+1
			}
		}
		calendar.month=temp+1;  //�õ��·�
		calendar.date=time+1;  //�õ�����
	}
	time=s%86400;  //�õ�����
	calendar.hour=time/3600;  //Сʱ
	calendar.min=(time%3600)/60;  //����
	calendar.sec=(time%3600)%60;  //����
	calendar.week=RTC_Get_Week(calendar.year,calendar.month,calendar.date);  //��ȡ���ڼ�
	return 0;
}

//�ж��Ƿ�Ϊ���꣬ 1����  0������
u8 Leap_year(u16 year)
{
	if(year%4==0)  //�����ܱ�4����
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



//RTC����ʱ��(1970��1��1�յ�����ʱ�������)�����2099��
//�� �� �� ʱ �� ��
//����  0���ɹ�   1��ʧ��
u8 RTC_Set(u16 year,u8 month,u8 data,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 s=0;
	if(year<1970 || year>2099)  //�ж��Ƿ�Ϊ�Ϸ����
	{
		return 1;
	}
	for(t=1970;t<year;t++)  //������ݵ�����
	{
		if(Leap_year(t)==1)  //�Ƿ�Ϊ����
		{
			s+=31622400;  //��������
		}
		else
		{
			s+=31536000;  //ƽ������
		}
	}
	month--;
	for(t=0;t<month;t++)  //�µ�����
	{
		s+=(u32)mon_table[t]*86400;  //ÿ���������
		if(Leap_year(year)==1 && t==1)  //����2��29�յ�����
		{
			s+=86400;
		}
	}
	s+=(u32)(data-1)*86400;  //�յ��������
	s+=(u32)hour*3600;  //Сʱ���������
	s+=(u32)min*60;  //���ӵ��������
	s+=sec;  //�����������
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  //����PWRʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);  //����BKPʱ��
	
	PWR_BackupAccessCmd(ENABLE);  //ʹ�ܺ󱸼Ĵ�������
	
	RTC_SetCounter(s);	//����RTC��������ֵ
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 0;	
}


//��������
//���� �� �� �� ʱ �� ��
//���� 0���ɹ�   1��ʧ��
u8 RTC_Alarm_Set(u16 year,u8 month,u8 data,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 s;
	if(year<1970 || year>2099)  //�Ƿ�Ϊ�Ϸ����
	{
		return 1;
	}
	for(t=1970;t<year;t++)  //������ݵ�����
	{
		if(Leap_year(year)==1)  //�Ƿ�Ϊ����
		{
			s+=31622400;  //��������
		}
		else
		{
			s+=31536000;  //ƽ������
		}
	}
	month--;
	for(t=0;t<month;t++)  //�µ�����
	{
		s+=(u32)mon_table[t]*86400;  //ÿ���������
		if(Leap_year(year)==1 && t==1)  //����2��29�յ�����
		{
			s+=86400;
		}
	}
	s+=(u32)(data-1)*86400;  //�յ��������
	s+=(u32)(hour-1)*3600;  //Сʱ���������
	s+=(u32)(min-1)*60;  //���ӵ��������
	s+=sec;  //�����������
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  //����PWRʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);  //����BKPʱ��
	PWR_BackupAccessCmd(ENABLE);  //ʹ�ܺ󱸼Ĵ������� 
	//���������Ǳ����!
	
	RTC_SetAlarm(s);
	RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д������� 
	return 0;
}

//RTCʱ���ж�
//ÿ�봥��һ��  
//extern u16 tcnt; 
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
	{							
		RTC_Get();//����ʱ��  
		printf("RTC Time:%d-%d-%d %d:%d:%d\r\n",calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);//�������ʱ��	
				
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	  	
		RTC_Get();				//����ʱ��   
		printf("Alarm Time:%d-%d-%d %d:%d:%d\r\n",calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);//�������ʱ��	
		
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//�������ж�
	RTC_WaitForLastTask();	  	    						 	   	 
}


