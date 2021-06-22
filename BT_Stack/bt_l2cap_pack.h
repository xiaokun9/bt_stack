#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_usart.h"

void HCI_ACL_DATA_TO_L2CAP(uint8_t* buffer);

typedef enum{
	L2CAP_COMMAND_REJECT_RSP = 0x01,
	L2CAP_CONNECTION_REQ = 0x02,
	L2CAP_CONNECTION_RSP = 0x03,
	L2CAP_CONFIGURATION_REQ = 0x04,
	L2CAP_CONFIGURATION_RSP = 0x05,
	L2CAP_DISCONNECTION_REQ = 0x06,
	L2CAP_DISCONNECTION_RSP = 0x07,
	L2CAP_ECHO_REQ = 0x08,
	L2CAP_ECHO_RSP = 0x09,
	L2CAP_INFORMATION_REQ = 0x0A,
	L2CAP_INFORMATION_RSP = 0x0B,
	L2CAP_CREATE_CHANNEL_REQ = 0x0C,
	L2CAP_CREATE_CHANNEL_RSP = 0x0D,
	L2CAP_MOVE_CHANNEL_REQ = 0x0E,
	L2CAP_MOVE_CHANNEL_RSP = 0x0F,
	L2CAP_MOVE_CHANNEL_CONFIRMATION_REQ = 0x10,
	L2CAP_MOVE_CHANNEL_CONFIRMATION_RSP = 0x11,
	L2CAP_CONNECTION_PARAMETER_UPDATE_REQ = 0x12,
	L2CAP_CONNECTION_PARAMETER_UPDATE_RSP = 0x13,
	L2CAP_LE_CREDIT_BASED_CONNECTION_REQ = 0x14,
	L2CAP_LE_CREDIT_BASED_CONNECTION_RSP = 0x15,
	L2CAP_FLOW_CONTROL_CREDIT_IND = 0x16,
	L2CAP_CREDIT_BASED_CONNECTION_REQ = 0x17,  
	L2CAP_CREDIT_BASED_CONNECTION_RSP = 0x18,
	L2CAP_CREDIT_BASED_RECONFIGURE_REQ = 0x19,  
	L2CAP_CREDIT_BASED_RECONFIGURE_RSP = 0x1A,
}L2CAP_SIGNALING_PACKET_TYPE;

typedef enum{
	l2cap_channel_Null = 0x0000,
	l2cap_channel_Signaling = 0x0001,
	l2cap_channel_Connectionless = 0x0002,
	l2cap_channel_AMP_Manager = 0x0003,
	l2cap_channel_ER_EDR_Security = 0x0007,
	l2cap_channel_AMP_Test_Manager = 0x003F,
	
}l2cap_channel_id;

typedef struct {
	uint16_t l2cap_data_len;
	uint16_t l2cap_channel_id;
	uint8_t *data;
}L2CAP_HEADER_G;

typedef struct {
	uint8_t Identifier;
	uint16_t l2cap_PSM;
	uint16_t remote_cid;
}L2CAP_HEADER_B;


typedef struct {
	uint16_t l2cap_handle;
	uint8_t l2cap_acl_data_pb_flag;
	uint8_t l2cap_acl_data_bc_flag;
	uint16_t l2cap_len;
	uint16_t l2cap_parameter_len;
	uint16_t cid;
}L2CAP_HEADER_INFO;


typedef enum{
	l2cap_psm_sdp = 0x0001,
	l2cap_psm_udp = 0x0002,
	l2cap_psm_RFCOMM = 0x0003,
	l2cap_psm_tcp = 0x0004,
	l2cap_psm_tcs_bin,
	l2cap_psm_tcs_at,
	l2cap_psm_att = 0x0007,
	l2cap_psm_obex = 0x0008,
	l2cap_psm_ip,
	l2cap_psm_ftp = 0x000a,
	l2cap_psm_http,
	l2cap_psm_wsp,
	l2cap_psm_bnep = 0x000f,
	l2cap_psm_upnp = 0x0010,
	l2cap_psm_hidp = 0x0011,
	l2cap_psm_AVCTP = 0x0017,
	l2cap_psm_AVDTP = 0x0019,
	l2cap_psm_CMTP = 0x001b,
	l2cap_psm_L2CAP = 0x0100,
}l2cap_psm_type;

#define l2cap_base_cid 0x0040

typedef struct {
	uint16_t local_cid;
	uint16_t remote_cid;
	uint16_t psm;
	
}L2CAP_LINK_INFO_STR;

typedef struct {
	uint8_t used;
	L2CAP_LINK_INFO_STR info;
}local_cid_str;

