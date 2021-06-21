#include "bt_l2cap_pack.h"

#define local_cid_change_num 16

local_cid_str local_cid[local_cid_change_num]={0};

uint16_t l2cap_get_local_cid()
{
	for(int i = 0;i < local_cid_change_num;i++) {
			if(local_cid[i].used == 0) {
				local_cid[i].used = 1;
				return l2cap_base_cid + i;
			}
	}
	return -1;//fail
}

void l2cap_clear_local_cid(uint16_t cid)
{
	uint16_t temp_cid = cid - l2cap_base_cid;
	if(temp_cid < 0 || temp_cid >= local_cid_change_num) return;
	for(int i=0;i<local_cid_change_num;i++) {
		if(i == temp_cid && local_cid[i].used == 1) {
			local_cid[i].used =0;
		}
	}
}

void L2CAP_CONNECTION_REQ_HANDER(L2CAP_HEADER_INFO *info,uint8_t Identifier,uint8_t* data)
{
	uint16_t PSM = *data | *(data + 1)<<8;
	data += sizeof(uint16_t);
	uint16_t Source_CID = *data | *(data + 1)<<8;

	L2CAP_HEADER_B l2cap_info;
	l2cap_info.Identifier = Identifier;
	l2cap_info.l2cap_PSM = PSM;
	l2cap_info.Identifier = Source_CID;

		//L2CAP_CONNECTION_RSP
		uint8_t array[12]={0};
		array[0] = 0x03;//Code=0x03
		array[1] = Identifier;//Identifier
		array[2] = 0x08;
		array[3] = 0x00;//length
		array[4] = 0x00;//dcid
		array[5] = 0x00;
		array[6] = *data;//scid
		array[7] = *(data+1);
		array[8] = 0x00;//Result->succeed
		array[9] = 0x00;
		array[10] = 0x00;//Status->No further information available
		array[11] = 0x00;
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
