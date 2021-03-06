#include "bt_pack_unpack.h"
#include "bsp_usart.h"
#include "string.h"

HCI_Event_Handle_Fun HCI_Event_Fun[100] = {NULL};
HCI_STATUS_Struct g_Hci_Status;
static uint16_t g_HCI_Cmd_Exec_Next_Num = 0;

void Send_HCI_Command_Packet(HCI_Command_Packet_Struct data)
{
		Usart_SendByte( USART1, HCI_CMD_PACK);
		Usart_SendByte( USART1, (uint8_t)(data.OCF));
		Usart_SendByte( USART1, data.OGF<<2 | data.OCF>>8);
		Usart_SendByte( USART1, data.LEN);
		Usart_SendArray( USART1,data.DATA,data.LEN);
		//printf("%c%c",0xFF,0xFF);
}

// HCI_Accept_Connection_Request
static void Send_HCI_Command_Accept_Connection_Request(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x09;
	data.OGF = 0x01;
	data.LEN = 0x07;
	uint8_t array[0x07] = {};
	memcpy(array,g_Hci_Status.remote_addr,6);
	array[0x06] = 0x01; // role:slave
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

void HCI_Command_Status_Event_Handle(uint8_t *buffer, uint32_t len)
{
	uint8_t Status = *buffer++;
	uint8_t Num_HCI_Command_Packets = *buffer++;
	uint16_t Command_Opcode = *(buffer) | (*(buffer + 1)) << 8;
	printf("Status:0x%02x,Num_HCI_Command_Packets:0x%02x,Command_Opcode:0x%04x\r\n",Status,Num_HCI_Command_Packets,Command_Opcode);
}

void HCI_PIN_Code_Request_Event_Handle(uint8_t *buffer, uint32_t len)
{
	printf("HCI_PIN_Code_Request_Event\r\n");
}

void HCI_IO_Capability_Request_Event_Handle(uint8_t *buffer, uint32_t len)
{
    //printf("HCI_IO_Capability_Request_Event_Handle:%d\r\n",*buffer);
	HCI_Command_Packet_Struct data;
	data.OCF = 0x2b;
	data.OGF = 0x01;
	data.LEN = 0x09;
	uint8_t array[0x09] = {};
	memcpy(array,buffer,6);
    
	array[0x06] = 0x01; //IO_Capability,NoInputNoOutput
	array[0x07] = 0x00; //OOB_Data_Present,
    array[0x08] = 0x03; //Authentication_Requirements,
	data.DATA = array;
	Send_HCI_Command_Packet(data);    
}

void HCI_User_Confirmation_Request_Event_Handle(uint8_t *buffer, uint32_t len)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x2c;
	data.OGF = 0x01;
	data.LEN = 0x06;
	uint8_t array[0x06] = {};
	memcpy(array,buffer,6);

	data.DATA = array;
	Send_HCI_Command_Packet(data);
    printf("HCI_User_Confirmation_Request succeed\r\n");
}

void HCI_Link_Key_Notification_Event_Handle(uint8_t *buffer, uint32_t len)
{
    printf("Link key succeed,type:%d\r\n",*(buffer+22));
}

void HCI_Simple_Pairing_Complete_Event_Handle(uint8_t *buffer, uint32_t len)
{
    uint8_t Status = *buffer++;
	printf("HCI_Simple_Pairing_Complete_Event_Handle,Status:0x%02x\r\n",Status);
}

void HCI_Disconnection_Complete_Event_Handle(uint8_t *buffer, uint32_t len)
{
    printf("HCI_Disconnection_Complete_Event_Handle\r\n");
}

void HCI_IO_Capability_Response_Event_Handle(uint8_t *buffer, uint32_t len)
{
    printf("HCI_IO_Capability_Response_Event\r\n");
}

void HCI_Connection_Complete_Event_Handle(uint8_t *buffer, uint32_t len)
{
    uint8_t Status = *buffer++;
    g_Hci_Status.ACL_Connection_Handle = *(buffer) | (*(buffer + 1)) << 8;
    buffer = buffer + 2;
    memcpy(g_Hci_Status.remote_addr,buffer,6);
    buffer += 6;
    g_Hci_Status.remote_link_type = *buffer;
    buffer++;
    g_Hci_Status.remote_link_encryption_type = *buffer;
	printf("HCI_Connection_Complete_Event,Status:0x%02x,type:0x%04x,type:0x%02x,encryption:0x%02x\r\n",\
        Status,g_Hci_Status.ACL_Connection_Handle,g_Hci_Status.remote_link_type,g_Hci_Status.remote_link_encryption_type);
    printf("addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",g_Hci_Status.remote_addr[0],g_Hci_Status.remote_addr[1],\
            g_Hci_Status.remote_addr[2],g_Hci_Status.remote_addr[3],g_Hci_Status.remote_addr[4],\
            g_Hci_Status.remote_addr[5]);
}

void HCI_Connection_Request_Event_Handle(uint8_t *buffer, uint32_t len)
{
	if(len != 10) {	
		return;
	}
//	for(int i = 0;i < 6;i++) {
//		g_Hci_Status.remote_addr[i] = *(buffer + 5 - i);
//	}
	memcpy(g_Hci_Status.remote_addr,buffer,6);
	
	buffer += 6;
	g_Hci_Status.remote_class_of_device = 0x00000000;
	g_Hci_Status.remote_class_of_device = *(buffer) | (*(buffer + 1)) << 8 | (*(buffer + 2)) << 16;
	buffer += 3;
	g_Hci_Status.remote_link_type = *(buffer);
	printf("Connection_Request_Event,addr:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\r\n",g_Hci_Status.remote_addr[0],\
		g_Hci_Status.remote_addr[1],g_Hci_Status.remote_addr[2],g_Hci_Status.remote_addr[3],g_Hci_Status.remote_addr[4],
		g_Hci_Status.remote_addr[5]);
	printf("Connection_Request_Event,remote_class_of_device:0x%06x,remote_link_type:%d\r\n",g_Hci_Status.remote_class_of_device,g_Hci_Status.remote_link_type);
	switch(*(buffer)) {
		case 0x00:  //SCO connection requested
			
			break;
		case 0x01:  //ACL connection requested
			Send_HCI_Command_Accept_Connection_Request();
			break;
		case 0x02:  //eSCO connection requested
			
			break;
		default:
			break;
	}
}
void HCI_Command_Complete_Event_Handle(uint8_t *buffer, uint32_t len)
{
	uint8_t Num_HCI_Command_Packets = *buffer++;
	uint16_t OCF = (*buffer) | ((*(buffer+1) &0x03) << 8);
	uint8_t OGF = *(buffer + 1) >> 2;
	buffer = buffer + 2;
	uint8_t Return_Parameter = *buffer;
	buffer++;
	printf("HCI_Command_Complete_Event_Handle,OGF:0x%02x,OCF:0x%02x\r\n",OGF,OCF);
	if (OGF == 0x03){ // CONTROLLER & BASEBAND COMMANDS
		switch(OCF) {
			case 0x01://HCI_Set_Event_Mask
				printf("HCI_Set_Event_Mask succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Set_Event_Mask fail\r\n");
				}
				break;
			case 0x03://HCI Reset
				if ( Return_Parameter == 0x00) {
					printf("HCI Cmd Exec hci Reset succeed\r\n");
				}
				break;
			case 0x13: // HCI_Write_Local_Name
				printf("HCI_Write_Local_Name succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Local_Name Fail\r\n");
				}
				break;
			case 0x18: // HCI_Write_Page_Timeout
				printf("HCI_Write_Page_Timeout succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Page_Timeout Fail\r\n");
				}
				break;	
			case 0x1A: // HCI_Write_Scan_Enable
				printf("HCI_Write_Scan_Enable succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Scan_Enable Fail\r\n");
				}
				break;
			case 0x1C: // HCI_Write_Page_Scan_Activity
				printf("HCI_Write_Page_Scan_Activity succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Page_Scan_Activity Fail\r\n");
				}
				break;
			case 0x1E: // HCI_Write_Inquiry_Scan_Activity
				printf("HCI_Write_Inquiry_Scan_Activity succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Inquiry_Scan_Activity Fail\r\n");
				}
				break;
			case 0x20: // HCI_Write_Authentication_Enable
				printf("HCI_Write_Authentication_Enable succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Authentication_Enable Fail\r\n");
				}
				break;
			case 0x24: // HCI_Write_Class_Of_Device
				printf("HCI_Write_Class_Of_Device succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Class_Of_Device Fail\r\n");
				}
				break;
			case 0x33: // HCI_Host_Buffer_Size
				printf("HCI_Host_Buffer_Size succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Host_Buffer_Size Fail\r\n");
				}
				break;				
			case 0x45: // HCI_Write_Inquiry_Mode
				printf("HCI_Write_Inquiry_Mode succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Inquiry_Mode Fail\r\n");
				}
				break;
			case 0x56: // HCI_Write_Simple_Pairing_Mode
				printf("HCI_Write_Simple_Pairing_Mode succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Simple_Pairing_Mode Fail\r\n");
				}
				break;
            case 0x7A: // HCI_Write_Secure_Connections_Host_Support
				printf("HCI_Write_Secure_Connections_Host_Support succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("HCI_Write_Secure_Connections_Host_Support Fail\r\n");
				}
				break;
		}
	} else if (OGF == 0x04) {
		switch(OCF) {
			case 0x01:
			if ( Return_Parameter == 0x00) { // HCI_Read_Local_Version_Information
					g_Hci_Status.HCI_Version = (uint8_t)(*buffer);
					buffer++;
					g_Hci_Status.HCI_Revision = (uint8_t)(*buffer) | (uint8_t)(*(buffer + 1)) << 8;
					buffer = buffer + 2;
					g_Hci_Status.LMP_PAL_Version = (uint8_t)(*buffer);
					buffer++;
					g_Hci_Status.Manufacturer_Name = (uint8_t)(*buffer) | (uint8_t)(*(buffer + 1)) << 8;
					buffer = buffer + 2;
					g_Hci_Status.LMP_PAL_Subversion = (uint8_t)(*buffer) | (uint8_t)(*(buffer + 1)) << 8;
				printf("HCI_Read_Local_Version_Information,HCI_Version:%02x,HCI_Revision:%04x,\
					LMP_PAL_Version:%02x,Manufacturer_Name:%04x,LMP_PAL_Subversion:%04x\r\n",\
					g_Hci_Status.HCI_Version,g_Hci_Status.HCI_Revision,g_Hci_Status.LMP_PAL_Version,\
					g_Hci_Status.Manufacturer_Name,g_Hci_Status.LMP_PAL_Subversion);
				} else {
					printf("HCI_Read_Local_Version_Information Fail\r\n");
				}
			break;
			case 0x03:
			if ( Return_Parameter == 0x00) {
					g_Hci_Status.hci_support_feature.byte0 = *buffer;// HCI_Read_Local_Supported_Features
					buffer++;
					g_Hci_Status.hci_support_feature.byte1 = *buffer;
					buffer++;
					g_Hci_Status.hci_support_feature.byte2 = *buffer;
					buffer++;
					g_Hci_Status.hci_support_feature.byte3 = *buffer;
					buffer++;
					g_Hci_Status.hci_support_feature.byte4 = *buffer;
					buffer++;
					g_Hci_Status.hci_support_feature.byte5 = *buffer;
					buffer++;
					g_Hci_Status.hci_support_feature.byte6 = *buffer;
					buffer++;
					g_Hci_Status.hci_support_feature.byte7 = *buffer;
					printf("HCI_Read_Local_Supported_Features succeed\r\n");
				} else {
					printf("HCI_Read_Local_Supported_Features Fail\r\n");
				}
			break;
			case 0x05:
			if ( Return_Parameter == 0x00) {
					g_Hci_Status.acl_data_packet_length = *(buffer + 1)<<8 | (*buffer);// Buffer size
					buffer = buffer + sizeof(uint16_t);
					g_Hci_Status.sco_data_packet_length = *buffer;
					buffer++;
					g_Hci_Status.total_num_acl_data_packets = *(buffer + 1)<<8 | (*buffer);
					buffer = buffer + sizeof(uint16_t);
					g_Hci_Status.total_num_sco_data_packets = *(buffer + 1)<<8 | (*buffer);
					printf("HCI Cmd Read Buffer Size,acl_len:%d,sco_len:%d,acl_total:%d,sco_total:%d\r\n",\
							g_Hci_Status.acl_data_packet_length,g_Hci_Status.sco_data_packet_length,\
							g_Hci_Status.total_num_acl_data_packets,g_Hci_Status.total_num_sco_data_packets);
				} else {
					printf("HCI Cmd Read Buffer Size Fail\r\n");
				}
			break;
			case 0x09:
			if ( Return_Parameter == 0x00) {
					memcpy(g_Hci_Status.loacl_addr,buffer,6);// Local Addr
				printf("local bt addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",buffer[0],\
						buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
				} else {
					printf("HCI Cmd Read Local Addr Fail\r\n");
				}
			break;
	    }
    }else if(OGF == 0x01) {
        switch(OCF) {
			case 0x2b://IO Capability Request
				printf("IO Capability Request succeed\r\n");
				if ( Return_Parameter != 0x00) {
					printf("IO Capability Request fail\r\n");
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
	printf("Vendor:Set ANA_Freq to 26MHz\r\n");
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
	printf("Vendor:PSKEY_HOST_INTERFACE H4\r\n");
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
	printf("Vendor:PSKEY PSKEY_UART_CONFIG_H4\r\n");
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
	printf("Vendor:PSKEY_BDADDR\r\n");
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
	printf("Vendor:PSKEY_BDADDR\r\n");
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
	printf("Vendor:warm reset\r\n");
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
	printf("Vendor:Radio test\r\n");
	HCI_Command_Packet_Struct data;
	data.OCF = 0x00;
	data.OGF = 0x3F;
	data.LEN = 0x13;
	uint8_t array[] = {0xC2,0x02,0x00,0x09,0x00,0xA0,0x00,0x04,0x50,0x00,0x00,0x1D,0x00,0x3A,0x00,0x00,0x00,0x00,0x00};
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
	data.LEN = 0x00;
	Send_HCI_Command_Packet(data);
}
// vendor-specific
//static void Send_HCI_vendor_Cmd(void)
//{
//	Send_HCI_vendor_Set_Freq_Cmd();
//	Send_HCI_vendor_HOST_H4_Cmd();
//	Send_HCI_vendor_UART_CONFIG_Cmd();
//	Send_HCI_vendor_PSKEY_BDADDR_Cmd();
//	Send_HCI_vendor_UART_BITRATE_Cmd();
//	Send_HCI_vendor_Warm_Reset_Cmd();
//	Send_HCI_vendor_Radio_test_Cmd();
//}
// HCI_read local addr
static void Send_HCI_Command_Read_Local_Addr(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x09;
	data.OGF = 0x04;
	data.LEN = 0x00;
	Send_HCI_Command_Packet(data);
}
// HCI_read BTC buffer size
static void Send_HCI_Command_Read_Buffer_Size(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x05;
	data.OGF = 0x04;
	data.LEN = 0x00;
	Send_HCI_Command_Packet(data);
}

// HCI_read Local Supported Feature
static void Send_HCI_Command_Read_Local_Supported_Feature(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x03;
	data.OGF = 0x04;
	data.LEN = 0x00;
	Send_HCI_Command_Packet(data);
}
//Set Event Mask command
static void Send_HCI_Command_Set_Event_Mask(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x01;
	data.OGF = 0x03;
	data.LEN = 0x08;
	uint8_t array[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0x3D};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
//HCI_Write_Page_Timeout
static void Send_HCI_Command_Write_Page_Timeout(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x18;
	data.OGF = 0x03;
	data.LEN = 0x02;
	uint8_t array[] = {0x00,0x20};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

static void Send_HCI_Command_Write_Default_Link_Policy_Settings()
{
    HCI_Command_Packet_Struct data;
	data.OCF = 0x0F;
	data.OGF = 0x02;
	data.LEN = 0x02;
	uint8_t array[] = {0x07,0x00};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

// HCI_Write_Class_Of_Device
static void Send_HCI_Command_Write_Class_Of_Device(void)
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x24;
	data.OGF = 0x03;
	data.LEN = 0x03;
	uint8_t array[] = {0x04,0x04,0x20};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
// HCI_Write_Authentication_Enable
static void Send_HCI_Command_Write_Authentication_Enable()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x20;
	data.OGF = 0x03;
	data.LEN = 0x01;
	uint8_t array[] = {0x00};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
// HCI_Write_Inquiry_Scan_Activity
static void Send_HCI_Command_Write_Inquiry_Scan_Activity()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x1E;
	data.OGF = 0x03;
	data.LEN = 0x04;
	uint8_t array[] = {0x00,0x10,0x12,0x00};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
// HCI_Write_Page_Scan_Activity
static void Send_HCI_Command_Write_Page_Scan_Activity()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x1C;
	data.OGF = 0x03;
	data.LEN = 0x04;
	uint8_t array[] = {0x00,0x08,0x12,0x00};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
// HCI_Write_Inquiry_Mode
static void Send_HCI_Command_Write_Inquiry_Mode()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x45;
	data.OGF = 0x03;
	data.LEN = 0x01;
	uint8_t array[] = {0x02};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
// HCI_Write_Page_Scan_Type
static void Send_HCI_Command_Write_Page_Scan_Type()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x47;
	data.OGF = 0x03;
	data.LEN = 0x01;
	uint8_t array[] = {0x01};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

static void Send_HCI_Command_Write_Inquiry_Scan_Type()
{
    HCI_Command_Packet_Struct data;
	data.OCF = 0x43;
	data.OGF = 0x03;
	data.LEN = 0x01;
	uint8_t array[] = {0x01};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

// HCI_Write_Local_Name
static void Send_HCI_Command_Write_Local_Name()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x13;
	data.OGF = 0x03;
	data.LEN = 0xF8;
	uint8_t array[248] = {"My Bt Stack"};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

// HCI_Host_Buffer_Size
static void Send_HCI_Command_Host_Buffer_Size()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x33;
	data.OGF = 0x03;
	data.LEN = 0x07;
	uint8_t array[0x07] = {};
	array[0] = (uint8_t)(g_Hci_Status.host_acl_data_packet_length);
	array[1] = (uint8_t)(g_Hci_Status.host_acl_data_packet_length >> 8);
	array[2] = (uint8_t)(g_Hci_Status.host_sco_data_packet_length);
	array[3] = (uint8_t)(g_Hci_Status.host_total_num_acl_data_packets);
	array[4] = (uint8_t)(g_Hci_Status.host_total_num_acl_data_packets >> 8);
	array[5] = (uint8_t)(g_Hci_Status.host_total_num_sco_data_packets);
	array[6] = (uint8_t)(g_Hci_Status.host_total_num_sco_data_packets >> 8);
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

// HCI_Read_Local_Version_Information
static void Send_HCI_Command_Read_Local_Version_Information()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x01;
	data.OGF = 0x04;
	data.LEN = 0x00;
	data.DATA = NULL;
	Send_HCI_Command_Packet(data);
}
// HCI_Write_Scan_Enable
static void Send_HCI_Command_Write_Scan_Enable()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x1A;
	data.OGF = 0x03;
	data.LEN = 0x01;
	uint8_t array[] = {SCAN_DISCOVER_CONNECT};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}
// Send_HCI_Write_Simple_Pairing_Mode
static void Send_HCI_Command_Write_Simple_Pairing_Mode()
{
	HCI_Command_Packet_Struct data;
	data.OCF = 0x56;
	data.OGF = 0x03;
	data.LEN = 0x01;
	uint8_t array[] = {0x01};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

static void Send_HCI_Command_Write_Secure_Connections_Host_Support()
{
    HCI_Command_Packet_Struct data;
	data.OCF = 0x7A;
	data.OGF = 0x03;
	data.LEN = 0x01;
	uint8_t array[] = {0x01};
	data.DATA = array;
	Send_HCI_Command_Packet(data);
}

void bt_stack_init(void)
{
	HCI_Event_Fun[HCI_Command_Complete_Event] = HCI_Command_Complete_Event_Handle;
	HCI_Event_Fun[HCI_Connection_Request_Event] = HCI_Connection_Request_Event_Handle;
	HCI_Event_Fun[HCI_Connection_Complete_Event] = HCI_Connection_Complete_Event_Handle;
	HCI_Event_Fun[HCI_Command_Status_Event] = HCI_Command_Status_Event_Handle;
	HCI_Event_Fun[HCI_PIN_Code_Request_Event] = HCI_PIN_Code_Request_Event_Handle;
    
    HCI_Event_Fun[HCI_Simple_Pairing_Complete_Event] = HCI_Simple_Pairing_Complete_Event_Handle;
    HCI_Event_Fun[HCI_Disconnection_Complete_Event] = HCI_Disconnection_Complete_Event_Handle;
    HCI_Event_Fun[HCI_IO_Capability_Response_Event] = HCI_IO_Capability_Response_Event_Handle;
    HCI_Event_Fun[HCI_IO_Capability_Request_Event] = HCI_IO_Capability_Request_Event_Handle;
    HCI_Event_Fun[HCI_User_Confirmation_Request_Event] = HCI_User_Confirmation_Request_Event_Handle;
    HCI_Event_Fun[HCI_Link_Key_Notification_Event] = HCI_Link_Key_Notification_Event_Handle;
    g_Hci_Status.host_sco_data_packet_length = 255;
	g_Hci_Status.host_acl_data_packet_length = 1024;
	g_Hci_Status.host_total_num_acl_data_packets = 20;
	g_Hci_Status.host_total_num_sco_data_packets = 10;

	Send_HCI_Command_Reset();//reset
}

void HCI_Event_Handle_Index(uint8_t index,uint8_t *buffer, uint32_t len)
{
	if (HCI_Event_Fun[index] != NULL) {
		HCI_Event_Fun[index](buffer,len);
	}
}

HCI_Cmd_Callback g_HCI_Cmd_Callback[] = {
	//Send_HCI_vendor_Cmd,
	// vendor-specific
	Send_HCI_vendor_Set_Freq_Cmd,
	Send_HCI_vendor_HOST_H4_Cmd,
	Send_HCI_vendor_UART_CONFIG_Cmd,
	Send_HCI_vendor_PSKEY_BDADDR_Cmd,
	Send_HCI_vendor_UART_BITRATE_Cmd,
	Send_HCI_vendor_Warm_Reset_Cmd,
	Send_HCI_vendor_Radio_test_Cmd,
	/********************************************/
	Send_HCI_Command_Read_Local_Addr,//read local addr
	Send_HCI_Command_Read_Buffer_Size,// read buffer size
	Send_HCI_Command_Host_Buffer_Size,// Host Buffer Size
	Send_HCI_Command_Read_Local_Version_Information,// HCI_Read_Local_Version_Information
	Send_HCI_Command_Read_Local_Supported_Feature,// HCI_Read_Local_Supported_Features
	Send_HCI_Command_Write_Simple_Pairing_Mode,//HCI_Write_Simple_Pairing_Mode
	Send_HCI_Command_Write_Secure_Connections_Host_Support,//HCI_Write_Secure_Connections_Host_Support
    Send_HCI_Command_Set_Event_Mask,// HCI_Set_Event_Mask
    Send_HCI_Command_Write_Inquiry_Mode,// HCI_Write_Inquiry_Mode
    Send_HCI_Command_Write_Page_Scan_Type,// HCI_Write_Page_Scan_Type
    Send_HCI_Command_Write_Inquiry_Scan_Type,//HCI_Command_Write_Inquiry_Scan_Type
    Send_HCI_Command_Write_Class_Of_Device, // HCI_Write_Class_Of_Device  0x200408
    Send_HCI_Command_Write_Page_Timeout,// HCI_Write_Page_Timeout   5.12s
    Send_HCI_Command_Write_Default_Link_Policy_Settings,//Command Code	Write Default Link Policy Settings
    Send_HCI_Command_Write_Local_Name, // HCI_Write_Local_Name
    Send_HCI_Command_Write_Inquiry_Scan_Activity,// HCI_Write_Inquiry_Scan_Activity
    Send_HCI_Command_Write_Scan_Enable,// Send_HCI_Command_Write_Scan_Enable

    
	//Send_HCI_Command_Write_Authentication_Enable,// HCI_Write_Authentication_Enable close
	
	//Send_HCI_Command_Write_Page_Scan_Activity, // HCI_Write_Page_Scan_Activity
	
	
};

void HCI_Cmd_Exec_Next(void)
{
	if(g_HCI_Cmd_Exec_Next_Num < sizeof(g_HCI_Cmd_Callback)/sizeof(HCI_Cmd_Callback)) {
		HCI_Cmd_Callback temp = g_HCI_Cmd_Callback[g_HCI_Cmd_Exec_Next_Num];
		if (temp != NULL) temp();
		g_HCI_Cmd_Exec_Next_Num++;
	}
}
