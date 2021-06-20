#include "bt_l2cap_pack.h"


void L2CAP_CONNECTION_REQ_HANDER(L2CAP_HEADER_INFO *info,uint8_t Identifier,uint8_t* data)
{
	uint16_t PSM = *data | *(data + 1)<<8;
	data += sizeof(uint16_t);
	uint16_t Source_CID = *data | *(data + 1)<<8;

	L2CAP_HEADER_B l2cap_info;
	l2cap_info.Identifier = Identifier;
	l2cap_info.l2cap_PSM = PSM;
	l2cap_info.Identifier = Source_CID;
	
//	L2CAP_CONNECTION_RSP_STR send_data;
//	send_data.Code = L2CAP_CONNECTION_RSP;
//	send_data.Identifier = Identifier;
//	send_data.Source_CID = Source_CID;
//	send_data.Length = 0x0008;
//	send_data.Destination_CID = 0x0040;//to do
//	send_data.Result = 0x0000;
//	send_data.Status = 0x0000;
}

void l2cap_Signaling_data_handler(L2CAP_HEADER_INFO *info,uint8_t* data)
{
		uint8_t *buffer = data;
		uint8_t Code = *buffer++;
		uint8_t Identifier = *buffer++;
		//uint16_t Len = *buffer | *(buffer + 1)<<8;
		buffer += sizeof(uint16_t);
		switch(Code) {
			case L2CAP_CONNECTION_REQ:
				L2CAP_CONNECTION_REQ_HANDER(info,Identifier,buffer);
				break;
		}
}

void l2cap_data_handle(L2CAP_HEADER_INFO *info,uint8_t* buffer)
{
		uint16_t l2cap_parameter_len = *buffer |*(buffer+1)<<8;
		info->l2cap_parameter_len = l2cap_parameter_len;
		buffer+=sizeof(uint16_t);
		uint16_t cid = *buffer | *(buffer + 1)<<8;
		info->cid = cid;
		buffer += sizeof(uint16_t);
		if(cid == l2cap_channel_Signaling) {
			l2cap_Signaling_data_handler(info,buffer);
		}	
}

void HCI_ACL_DATA_TO_L2CAP(uint8_t* buffer)
{
    //1:0~11 bit :Handle
    //2:12~13 bit :PB Flag
    //3:14~15 bit :BC Flag
    //4:16~31 bit :Data Total Length
    //5:32~ bit :Data
		L2CAP_HEADER_INFO info;
	
		uint16_t l2cap_handle = *buffer | (*(buffer + 1)&0x0F)<<8;
		info.l2cap_handle = l2cap_handle;
		buffer++;
		uint8_t l2cap_acl_data_pb_flag = *buffer&0x30>>4;
		info.l2cap_acl_data_pb_flag = l2cap_acl_data_pb_flag;
		uint8_t l2cap_acl_data_bc_flag = *buffer&0xc0>>6;
		info.l2cap_acl_data_bc_flag = l2cap_acl_data_bc_flag;
		buffer++;
	
    uint16_t l2cap_len = *buffer | *(buffer + 1)<<8;
		info.l2cap_len = l2cap_len;
		buffer += sizeof(uint16_t);
		l2cap_data_handle(&info,buffer);
}

void l2cap_data_to_hci_acl_data(L2CAP_HEADER_INFO *info,uint16_t len,uint8_t buffer)
{
	//1:0~11 bit :Handle
    //2:12~13 bit :PB Flag
    //3:14~15 bit :BC Flag
    //4:16~31 bit :Data Total Length
    //5:32~ bit :Data
		uint8_t buff[len];
		//Usart_SendArray( USART1,data.DATA,data.LEN);
}
