#include "bt_pack_unpack.h"
#include "bsp_usart.h"
#include "string.h"

HCI_Event_Handle_Fun HCI_Event_Fun[100] = {NULL};
HCI_STATUS_Struct g_Hci_Status;


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
	buffer++;
	if (OGF == 0x03){ // CONTROLLER & BASEBAND COMMANDS
		switch(OCF) {
			case 0x03://HCI Reset
				if ( Return_Parameter != 0x00) {
					printf("HCI Cmd Exec hci Reset\r\n");
				}
				break;
			case 0x04:
				
				break;
		}
	} else if (OGF == 0x04) {
		switch(OCF) {
			case 0x05:
			if ( Return_Parameter == 0x00) {
					g_Hci_Status.acl_data_packet_length = *(buffer + 1)<<8 | (*buffer);// Buffer size
					buffer = buffer + sizeof(uint16_t);
					g_Hci_Status.sco_data_packet_length = *buffer;
					buffer++;
					g_Hci_Status.total_num_acl_data_packets = *(buffer + 1)<<8 | (*buffer);
					buffer = buffer + sizeof(uint16_t);
					g_Hci_Status.total_num_sco_data_packets = *(buffer + 1)<<8 | (*buffer);
				} else {
					printf("HCI Cmd Read Buffer Size Fail\r\n");
				}
			break;
			case 0x09:
			if ( Return_Parameter == 0x00) {
					memcpy(g_Hci_Status.loacl_addr,buffer,6);// Local Addr
				} else {
					printf("HCI Cmd Read Local Addr Fail\r\n");
				}
			break;
	}
	
}

}
/**********************************/
static void Send_HCI_vendor_Set_Freq_Cmd(void)
{
	//  Set ANA_Freq to 26MHz
	// 01 00 FC 13 C2 02 00 09 00 1E 00 03 70 00 00 FE 01 01 00 00 00 90 65
	HCI_Command_Packet_Struct data;
	data.OCF = 0x00;
	data.OGF = 0x3F;
	data.LEN = 0x13;
	uint8_t array[] = {0xC2,0x02,0x00,0x09,0x00,0x1E,0x00,0x03,0x70,0x00,0x00,0xFE,0x01,0x01,0x00,0x00,0x00,0x90,0x65};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
static void Send_HCI_vendor_HOST_H4_Cmd(void)
{
	//  PSKEY_HOST_INTERFACE H4
	// 01 00 FC 13 C2 02 00 09 00 04 00 03 70 00 00 F9 01 01 00 00 00 03 00
	HCI_Command_Packet_Struct data;
	data.OCF = 0x00;
	data.OGF = 0x3F;
	data.LEN = 0x13;
	uint8_t array[] = {0xC2,0x02,0x00,0x09,0x00,0x04,0x00,0x03,0x70,0x00,0x00,0xF9,0x01,0x01,0x00,0x00,0x00,0x03,0x00};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
static void Send_HCI_vendor_UART_CONFIG_Cmd(void)
{
	//  PSKEY PSKEY_UART_CONFIG_H4
	// 01 00 FC 13 C2 02 00 09 00 05 00 03 70 00 00 C0 01 01 00 00 00 A8 08
	HCI_Command_Packet_Struct data;
	data.OCF = 0x00;
	data.OGF = 0x3F;
	data.LEN = 0x13;
	uint8_t array[] = {0xC2,0x02,0x00,0x09,0x00,0x05,0x00,0x03,0x70,0x00,0x00,0xC0,0x01,0x01,0x00,0x00,0x00,0xA8,0x08};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
static void Send_HCI_vendor_PSKEY_BDADDR_Cmd(void)
{
	//  PSKEY_BDADDR
	// 01 00 FC 19 C2 02 00 0C 00 08 00 03 70 00 00 01 00 04 00 00 00 45 00 46 4F B8 00 64 DC
	HCI_Command_Packet_Struct data;
	data.OCF = 0x00;
	data.OGF = 0x3F;
	data.LEN = 0x19;
	uint8_t array[] = {0xC2,0x02,0x00,0x0C,0x00,0x08,0x00,0x03,0x70,0x00,0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x45,0x00,0x46,0x4F,0xB8,0x00,0x64,0xDC};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
static void Send_HCI_vendor_UART_BITRATE_Cmd(void)
{
	//  UART_BITRATE
	// 01 00 FC 15 C2 02 00 0A 00 09 00 03 70 00 00 EA 01 02 00 00 00 01 00 00 C2
	HCI_Command_Packet_Struct data;
	data.OCF = 0x00;
	data.OGF = 0x3F;
	data.LEN = 0x15;
	uint8_t array[] = {0xC2,0x02,0x00,0x0A,0x00,0x09,0x00,0x03,0x70,0x00,0x00,0xEA,0x01,0x02,0x00,0x00,0x00,0x01,0x00,0x00,0xC2};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
static void Send_HCI_vendor_Warm_Reset_Cmd(void)
{
	//  warm reset
	// 01 00 FC 13 C2 02 00 09 00 4D 0E 02 40 00 00 00 00 00 00 00 00 00 00
	HCI_Command_Packet_Struct data;
	data.OCF = 0x00;
	data.OGF = 0x3F;
	data.LEN = 0x13;
	uint8_t array[] = {0xC2,0x02,0x00,0x09,0x00,0x4D,0x0E,0x02,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
static void Send_HCI_vendor_Radio_test_Cmd(void)
{
	//  Radio test
	// 01 00 FC 13 C2 02 00 09 00 A0 00 04 50 00 00 1D 00 3A 00 00 00 00 00
	HCI_Command_Packet_Struct data;
	data.OCF = 0x00;
	data.OGF = 0x3F;
	data.LEN = 0x13;
	uint8_t array[] = {0x01,0x00,0xFC,0x13,0xC2,0x02,0x00,0x09,0x00,0xA0,0x00,0x04,0x50,0x00,0x00,0x1D,0x00,0x3A,0x00,0x00,0x00,0x00,0x00};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
/**********************************/



// HCI_Reset
static void Send_HCI_Command_Reset(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x03;
	data.OGF = 0x03;
	data.LEN = 0;
	Send_HCI_Command_Packet(data);
}
// vendor-specific
static void Send_HCI_vendor_Cmd(void)
{
	Send_HCI_vendor_Set_Freq_Cmd();
	Send_HCI_vendor_HOST_H4_Cmd();
	Send_HCI_vendor_UART_CONFIG_Cmd();
	Send_HCI_vendor_PSKEY_BDADDR_Cmd();
	Send_HCI_vendor_UART_BITRATE_Cmd();
	Send_HCI_vendor_Warm_Reset_Cmd();
	Send_HCI_vendor_Radio_test_Cmd();
}
// HCI_read local addr
static void Send_HCI_Command_Read_Local_Addr(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x09;
	data.OGF = 0x04;
	data.LEN = 0;
	Send_HCI_Command_Packet(data);
}
// HCI_read BTC buffer size
static void Send_HCI_Command_Read_Buffer_Size(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x05;
	data.OGF = 0x04;
	data.LEN = 0;
	Send_HCI_Command_Packet(data);
}
void bt_stack_init(void)
{
	HCI_Event_Fun[HCI_Command_Complete_Event] = HCI_Command_Complete_Event_Handle;
	
	Send_HCI_Command_Reset();//reset
	Send_HCI_vendor_Cmd();
	Send_HCI_Command_Read_Local_Addr();//read local addr
	Send_HCI_Command_Read_Buffer_Size();// read buffer size
	
}

void HCI_Event_Handle_Index(uint8_t index,uint8_t *buffer, uint32_t len)
{
	if (HCI_Event_Fun[index] != NULL) {
		HCI_Event_Fun[index](buffer,len);
	}
}
