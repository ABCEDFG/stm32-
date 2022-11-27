#ifndef __RT_DEF_H__
#define __RT_DEF_H__

/*
*************************************************************************
*                               ��������
*************************************************************************
*/
/* RT-Thread �����������Ͷ��� */
typedef signed   char                   rt_int8_t;      /**<  8bit ���� */
typedef signed   short                  rt_int16_t;     /**< 16bit ���� */
typedef signed   long                   rt_int32_t;     /**< 32bit ���� */
typedef unsigned char                   rt_uint8_t;     /**<  8bit �޷����������� */
typedef unsigned short                  rt_uint16_t;    /**< 16bit �޷����������� */
typedef unsigned long                   rt_uint32_t;    /**< 32bit �޷����������� */
typedef int                             rt_bool_t;      /**< ������ */

/* 32bit CPU */
typedef long                            rt_base_t;      /**< Nbit CPU ����������� */
typedef unsigned long                   rt_ubase_t;     /**< Nbit unsigned CPU ����������� */

typedef rt_base_t                       rt_err_t;       /**< �������� */
typedef rt_uint32_t                     rt_time_t;      /**< ʱ��������� */
typedef rt_uint32_t                     rt_tick_t;      /**< ʱ���������� */
typedef rt_base_t                       rt_flag_t;      /**< Ϊ��־���� */
typedef rt_ubase_t                      rt_size_t;      /**< ����ߴ��� */
typedef rt_ubase_t                      rt_dev_t;       /**< �豸���� */
typedef rt_base_t                       rt_off_t;       /**< �������� */

/* �������Ͷ��� */
#define RT_TRUE                         1               /**< ������  */
#define RT_FALSE                        0               /**< ������ */

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
*                               �����붨��
*************************************************************************
*/
/* RT-Thread �������ض��� */
#define RT_EOK                          0               /**< û�д��� */
#define RT_ERROR                        1               /**< ����һ���Դ��� */
#define RT_ETIMEOUT                     2               /**< ��ʱ */
#define RT_EFULL                        3               /**< ��Դ���� */
#define RT_EEMPTY                       4               /**< ��ԴΪ�� */
#define RT_ENOMEM                       5               /**< û�м��� */
#define RT_ENOSYS                       6               /**< ��ϵͳ */
#define RT_EBUSY                        7               /**< Busy */
#define RT_EIO                          8               /**< IO ���� */
#define RT_EINTR                        9               /**< �ж�ϵͳ���� */
#define RT_EINVAL                       10              /**< ��Ч���� */

/*
*************************************************************************
*                               ˫������ṹ��
*************************************************************************
*/
struct rt_list_node
{
    struct rt_list_node *next;              /* ָ���һ���ڵ� */
    struct rt_list_node *prev;              /* ָ��ǰһ���ڵ� */
};
typedef struct rt_list_node rt_list_t;                  


/*
*************************************************************************
*                               �߳̿��ƿ�ṹ��
*************************************************************************
*/

struct rt_thread
{
	void        *sp;	          /* �߳�ջָ�� */
	void        *entry;	          /* �߳���ڵ�ַ */
	void        *parameter;	      /* �߳��β� */	
	void        *stack_addr;      /* �߳���ʼ��ַ */
	rt_uint32_t stack_size;       /* �߳�ջ��С����λΪ�ֽ� */
	
	rt_list_t   tlist;            /* �߳�����ڵ� */
};
typedef struct rt_thread *rt_thread_t;


#endif  /* __RT_DEF_H__ */
