/* 关闭JTAG功能，不影响程序下载 */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;  //选择你要设置的IO口
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
GPIO_Init(GPIOA,&GPIO_InitStructure); 	   /* 初始化GPIO */