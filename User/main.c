/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
/* 开发板硬件bsp头文件 */
#include "bsp_led.h"
#include "bsp_usart.h"

#include "semphr.h"
#include "bt_pack_unpack.h"
#include <stdio.h>
#include <string.h>
/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;
/* LED2任务句柄 */
static TaskHandle_t LED2_Task_Handle = NULL;
/* LED3任务句柄 */
static TaskHandle_t LED3_Task_Handle = NULL;
/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */


/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */

SemaphoreHandle_t xSemaphore = NULL;
extern uint8_t receivebuf[1024];
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void LED1_Task(void* pvParameters);/* LED1_Task任务实现 */
static void LED2_Task(void* pvParameters);/* LED2_Task任务实现 */
static void LED3_Task(void* pvParameters);/* LED3_Task任务实现 */
static void BSP_Init(void);/* 用于初始化板载相关资源 */

/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

  /* 开发板硬件初始化 */
  BSP_Init();
  //printf("FreeRTOS-start\r\n");
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
 // BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
  
  /* 创建LED_Task任务 */
  xTaskCreate((TaskFunction_t )LED1_Task, /* 任务入口函数 */
                        (const char*    )"LED1_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&LED1_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("创建LED1_Task任务成功!\r\n");
  
	/* 创建LED_Task任务 */
  xTaskCreate((TaskFunction_t )LED2_Task, /* 任务入口函数 */
                        (const char*    )"LED2_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&LED2_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("创建LED2_Task任务成功!\r\n");
	/* 创建LED_Task任务 */
  xTaskCreate((TaskFunction_t )LED3_Task, /* 任务入口函数 */
                        (const char*    )"LED3_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&LED3_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("创建LED3_Task任务成功!\r\n");  
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}



/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void LED1_Task(void* parameter)
{	
    while (1)
    {
        LED1_ON;
        vTaskDelay(500);   /* 延时500个tick */
        
        LED1_OFF;     
        vTaskDelay(500);   /* 延时500个tick */		 		
    }
}

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
uint8_t buf[4] = {0x01,0x03,0x0c,0x00};
static void LED2_Task(void* parameter)
{	
    while (1)
    {
        LED2_ON;
        vTaskDelay(1000);   /* 延时500个tick */
        
        LED2_OFF;     
        vTaskDelay(1000);   /* 延时500个tick */	
//				Usart_SendByte( USART1, 0x01);
//				Usart_SendByte( USART1, 0x03);
//				Usart_SendByte( USART1, 0x0c);
//				Usart_SendByte( USART1, 0x00);
//				Usart_SendArray(USART1,buf,4);

    }
}

static void LED3_Task(void* parameter)
{	
		bt_stack_init();
		printf("%s","BTH reset succeed\r\n");
    while (1)
    {
				if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE )
				{
        LED3_ON;
        //vTaskDelay(500);   /* 延时500个tick */
				uint8_t* temp_addr = receivebuf;
				uint8_t temp_char = *temp_addr;
				printf("UART RECV data type:%d\r\n",temp_char);
				if (temp_char == HCI_EVENT_PACK) { //HCI EVENT
					temp_addr =temp_addr + 1;
					uint8_t index = *(temp_addr);
					printf("UART RECV data index:0x%02x\r\n",index);
					temp_addr =temp_addr + 1;
					if (index >0 && index < 100) {
						
						HCI_Event_Handle_Index(index,temp_addr + 1, *(temp_addr));
					}
					//
					
					HCI_Cmd_Exec_Next();//vendor cmd
				} else if(temp_char == HCI_ACL_DATA_PACK) { // HCI_ACL_DATA_PACK
				    HCI_ACL_DATA_TO_L2CAP(temp_addr+1);
                    printf("HCI_ACL_DATA_PACK\r\n");
                } else if(temp_char == HCI_CMD_PACK) { // HCI_CMD_PACK
                    printf("HCI_CMD_PACK\r\n");
                } else if(temp_char == HCI_SCO_DATA_PACK) { // HCI_SCO_DATA_PACK
                    printf("HCI_SCO_DATA_PACK\r\n");
                } else if(temp_char == HCI_ISO_DATA_PACK) { // HCI_ISO_DATA_PACK
                    printf("HCI_ISO_DATA_PACK\r\n");
                }
        LED3_OFF;     
        //vTaskDelay(500);   /* 延时500个tick */	
				//receivebuf
				memset(receivebuf,0,1024);					
				}
    }
}

/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/

static void BSP_Init(void)
{
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED 初始化 */
	LED_GPIO_Config();

	/* UART1	send cmd to bt chip or recv bt chip message*/
	USART1_Config();
	NVIC_Configuration();
	/*UART2 debug*/
	USART_Config();
  xSemaphore = xSemaphoreCreateBinary();
//	bt_stack_init();
	//test cmd
//	HCI_Command_Packet_Struct data;
//	data.OCF = 0x03;
//	data.OGF = 0x03;
//	data.LEN = 0;
//	Send_HCI_Command_Packet(data);
}

/********************************END OF FILE****************************/
