#include "bt_pack_unpack.h"
#include "bsp_usart.h"


void Send_HCI_Command_Packet(HCI_Command_Packet_Struct data)
{
		Usart_SendByte( USART1, HCI_CMD_PACK);
		Usart_SendByte( USART1, (uint8_t)(data.OCF));
		Usart_SendByte( USART1, data.OGF<<2 | data.OCF>>8);
		Usart_SendByte( USART1, data.LEN);
		Usart_SendArray( USART1,data.DATA,data.LEN);
}
