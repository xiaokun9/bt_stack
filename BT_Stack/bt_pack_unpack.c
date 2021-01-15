#include "bt_pack_unpack.h"
#include "bsp_usart.h"

HCI_Event_Handle_Fun HCI_Event_Fun[100] = {NULL};

void Send_HCI_Command_Packet(HCI_Command_Packet_Struct data)
{
		Usart_SendByte( USART1, HCI_CMD_PACK);
		Usart_SendByte( USART1, (uint8_t)(data.OCF));
		Usart_SendByte( USART1, data.OGF<<2 | data.OCF>>8);
		Usart_SendByte( USART1, data.LEN);
		Usart_SendArray( USART1,data.DATA,data.LEN);
}

void HCI_Command_Complete_Event_Handle(uint8_t *buffer, uint32_t len)
{
	uint8_t Num_HCI_Command_Packets = *buffer++;
	uint16_t OGF = (*buffer) | ((*(buffer+1) &0x03) << 8);
	uint8_t OCF = *(buffer + 1) >> 2;
	buffer = buffer + 2;
	uint8_t Return_Parameter = *buffer;

	printf("%d,%d,%d,%d",Num_HCI_Command_Packets,OGF,Return_Parameter,OCF);
}

void bt_stack_init(void)
{
	HCI_Event_Fun[HCI_Command_Complete_Event] = HCI_Command_Complete_Event_Handle;
}

void HCI_Event_Handle_Index(uint8_t index,uint8_t *buffer, uint32_t len)
{
	if (HCI_Event_Fun[index] != NULL) {
		HCI_Event_Fun[index](buffer,len);
	}
}
