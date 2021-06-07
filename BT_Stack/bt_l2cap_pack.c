#include "bt_l2cap_pack.h"


void L2CAP_CONNECTION_REQ_HANDER(uint8_t id,uint16_t len,uint8_t* data)
{
	uint16_t PSM = *data | *(data + 1)<<8;
	data += sizeof(uint16_t);
	uint16_t Source_CID = *data | *(data + 1)<<8;

	L2CAP_CONNECTION_RSP_STR send_data;
	send_data.Code = L2CAP_CONNECTION_RSP;
	send_data.Identifier = id;
	send_data.Source_CID = Source_CID;
	send_data.Length = 0x0008;
	send_data.Destination_CID = 0x0040;//to do
	send_data.Result = 0x0000;
	send_data.Status = 0x0000;
}

void l2cap_Signaling_data_handler(L2CAP_HEADER_B* data)
{
		uint8_t *buffer = data->data;
		uint8_t Code = *buffer++;
		uint8_t Identifier = *buffer++;
		uint16_t Len = *buffer | *(buffer + 1)<<8;
		buffer += sizeof(uint16_t);
		switch(Code) {
			case L2CAP_CONNECTION_REQ:
				L2CAP_CONNECTION_REQ_HANDER(Identifier,Len,buffer);
				break;
		}
}

void HCI_ACL_DATA_TO_L2CAP(uint8_t* buffer)
{
    //1:0~11 bit :Handle
    //2:12~13 bit :PB Flag
    //3:14~15 bit :BC Flag
    //4:16~31 bit :Data Total Length
    //5:32~ bit :Data

    uint16_t l2cap_len = *buffer | *(buffer + 1)<<8;
		buffer += sizeof(uint16_t);
		uint16_t cid = *buffer | *(buffer + 1)<<8;
		buffer += sizeof(uint16_t);
		if(cid == l2cap_channel_Signaling) {
			L2CAP_HEADER_B data;
			data.l2cap_channel_id = cid;
			data.l2cap_data_len = l2cap_len;
			data.data = buffer;
			l2cap_Signaling_data_handler(&data);
		}
}
