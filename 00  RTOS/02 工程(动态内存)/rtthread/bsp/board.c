/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 */
#include <rthw.h>  // 处理器相关
#include <rtthread.h>  // RTT 内核相关
#include "board.h"  // 固件库和 BSP 相关


#ifdef __CC_ARM
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024
//从内部SRAM里面分配一部分静态内存来作为rtt的堆空间，这里配置为4KB
static uint32_t rt_heap[RT_HEAP_SIZE];
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif
#endif

extern uint8_t OSRunning;



/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	Usart1_Init(115200);


	OSRunning=1;
	
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
    
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
	/* 进入中断 rt_interrupt_nest++ */
	rt_interrupt_enter();
	
	/* 更新时基 */
	rt_tick_increase();

	/* 离开中断 rt_interrupt_nest-- */
	rt_interrupt_leave();
}
