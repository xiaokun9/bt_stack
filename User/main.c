/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
#include "bsp_usart.h"

#include "semphr.h"
#include "bt_pack_unpack.h"
#include <stdio.h>
#include <string.h>
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1������ */
static TaskHandle_t LED1_Task_Handle = NULL;
/* LED2������ */
static TaskHandle_t LED2_Task_Handle = NULL;
/* LED3������ */
static TaskHandle_t LED3_Task_Handle = NULL;
/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */


/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */

SemaphoreHandle_t xSemaphore = NULL;
extern uint8_t receivebuf[1024];
/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void LED1_Task(void* pvParameters);/* LED1_Task����ʵ�� */
static void LED2_Task(void* pvParameters);/* LED2_Task����ʵ�� */
static void LED3_Task(void* pvParameters);/* LED3_Task����ʵ�� */
static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

  /* ������Ӳ����ʼ�� */
  BSP_Init();
  //printf("FreeRTOS-start\r\n");
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
 // BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* ����LED_Task���� */
  xTaskCreate((TaskFunction_t )LED1_Task, /* ������ں��� */
                        (const char*    )"LED1_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED1_Task_Handle);/* ������ƿ�ָ�� */
//  if(pdPASS == xReturn)
//    printf("����LED1_Task����ɹ�!\r\n");
  
	/* ����LED_Task���� */
  xTaskCreate((TaskFunction_t )LED2_Task, /* ������ں��� */
                        (const char*    )"LED2_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED2_Task_Handle);/* ������ƿ�ָ�� */
//  if(pdPASS == xReturn)
//    printf("����LED2_Task����ɹ�!\r\n");
	/* ����LED_Task���� */
  xTaskCreate((TaskFunction_t )LED3_Task, /* ������ں��� */
                        (const char*    )"LED3_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED3_Task_Handle);/* ������ƿ�ָ�� */
//  if(pdPASS == xReturn)
//    printf("����LED3_Task����ɹ�!\r\n");  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED1_Task(void* parameter)
{	
    while (1)
    {
        LED1_ON;
        vTaskDelay(500);   /* ��ʱ500��tick */
        
        LED1_OFF;     
        vTaskDelay(500);   /* ��ʱ500��tick */		 		
    }
}

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
uint8_t buf[4] = {0x01,0x03,0x0c,0x00};
static void LED2_Task(void* parameter)
{	
    while (1)
    {
        LED2_ON;
        vTaskDelay(1000);   /* ��ʱ500��tick */
        
        LED2_OFF;     
        vTaskDelay(1000);   /* ��ʱ500��tick */	
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
        //vTaskDelay(500);   /* ��ʱ500��tick */
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
        //vTaskDelay(500);   /* ��ʱ500��tick */	
				//receivebuf
				memset(receivebuf,0,1024);					
				}
    }
}

/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/

static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
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
