#include <iwdg.h>


/*
*   preȡֵ�� 0�� 1�� 2�� 3�� 4�� 5�� 6
*	��Ƶ��С��4�� 8��16��32��64��128��256
*   rlr�� 0 ~ 4095
*   T = (4 * 2^pre * rlr) / 40000 
*   ��λ��S
*   ע�⣺���Ź�ʱ���ǲ�׼ȷ�ģ����ι����
*/

void Iwdg_Init(u8 pre,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//�����Ĵ�������
	IWDG_SetPrescaler(pre);	//����Ԥ��Ƶϵ��
	IWDG_SetReload(rlr);	//��������ֵ
	IWDG_ReloadCounter();	
	IWDG_Enable();	//ʹ�ܿ��Ź�
}


void Iwdg_FeedDog()
{
	IWDG_ReloadCounter();	// ι��
}
