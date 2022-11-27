#ifndef __RT_DEF_H__
#define __RT_DEF_H__

/*
*************************************************************************
*                               数据类型
*************************************************************************
*/
/* RT-Thread 基本数据类型定义 */
typedef signed   char                   rt_int8_t;      /**<  8bit 整型 */
typedef signed   short                  rt_int16_t;     /**< 16bit 整型 */
typedef signed   long                   rt_int32_t;     /**< 32bit 整型 */
typedef unsigned char                   rt_uint8_t;     /**<  8bit 无符号整数类型 */
typedef unsigned short                  rt_uint16_t;    /**< 16bit 无符号整数类型 */
typedef unsigned long                   rt_uint32_t;    /**< 32bit 无符号整数类型 */
typedef int                             rt_bool_t;      /**< 布尔型 */

/* 32bit CPU */
typedef long                            rt_base_t;      /**< Nbit CPU 相关日期类型 */
typedef unsigned long                   rt_ubase_t;     /**< Nbit unsigned CPU 相关数据类型 */

typedef rt_base_t                       rt_err_t;       /**< 输入错误号 */
typedef rt_uint32_t                     rt_time_t;      /**< 时间戳的类型 */
typedef rt_uint32_t                     rt_tick_t;      /**< 时钟数的类型 */
typedef rt_base_t                       rt_flag_t;      /**< 为标志键入 */
typedef rt_ubase_t                      rt_size_t;      /**< 输入尺寸编号 */
typedef rt_ubase_t                      rt_dev_t;       /**< 设备类型 */
typedef rt_base_t                       rt_off_t;       /**< 抵销类型 */

/* 布尔类型定义 */
#define RT_TRUE                         1               /**< 布尔真  */
#define RT_FALSE                        0               /**< 布尔假 */

#ifdef __CC_ARM
	#define rt_inline                   static __inline
	#define ALIGN(n)                    __attribute__((aligned(n)))

#elif defined (__IAR_SYSTEMS_ICC__)
    #define rt_inline                   static inline
	#define ALIGN(n)                    PRAGMA(data_alignment=n)
		
#elif defined (__GNUC__)
    #define rt_inline                   static __inline
	#define ALIGN(n)                    __attribute__((aligned(n)))
#else
    #error not supported tool chain			
#endif

		
#define RT_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))
#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

		
#define RT_NULL                         (0)

/*
*************************************************************************
*                               错误码定义
*************************************************************************
*/
/* RT-Thread 错误码重定义 */
#define RT_EOK                          0               /**< 没有错误 */
#define RT_ERROR                        1               /**< 发生一般性错误 */
#define RT_ETIMEOUT                     2               /**< 超时 */
#define RT_EFULL                        3               /**< 资源已满 */
#define RT_EEMPTY                       4               /**< 资源为空 */
#define RT_ENOMEM                       5               /**< 没有记忆 */
#define RT_ENOSYS                       6               /**< 无系统 */
#define RT_EBUSY                        7               /**< Busy */
#define RT_EIO                          8               /**< IO 错误 */
#define RT_EINTR                        9               /**< 中断系统调用 */
#define RT_EINVAL                       10              /**< 无效参数 */

/*
*************************************************************************
*                               双向链表结构体
*************************************************************************
*/
struct rt_list_node
{
    struct rt_list_node *next;              /* 指向后一个节点 */
    struct rt_list_node *prev;              /* 指向前一个节点 */
};
typedef struct rt_list_node rt_list_t;                  


/*
*************************************************************************
*                               线程控制块结构体
*************************************************************************
*/

struct rt_thread
{
	void        *sp;	          /* 线程栈指针 */
	void        *entry;	          /* 线程入口地址 */
	void        *parameter;	      /* 线程形参 */	
	void        *stack_addr;      /* 线程起始地址 */
	rt_uint32_t stack_size;       /* 线程栈大小，单位为字节 */
	
	rt_list_t   tlist;            /* 线程链表节点 */
};
typedef struct rt_thread *rt_thread_t;


#endif  /* __RT_DEF_H__ */
