#include "stm32f10x.h"
#include <stdio.h>
#include "bt_l2cap_pack.h"
#include "bsp_usart.h"

typedef enum {
	HCI_CMD_PACK = 0x01,
	HCI_ACL_DATA_PACK = 0x02,
	HCI_SCO_DATA_PACK = 0x03,
	HCI_EVENT_PACK = 0x04,
	HCI_ISO_DATA_PACK = 0x05,
}HCI_PACK_TYPE;

typedef enum {
	HCI_LINK_CONTROL_COMMANDS = 0x01,
	HCI_LINK_POLICY_COMMANDS = 0x02,
	HCI_CONTROLLER_AND_BASEBAND_COMMANDS = 0x03,
	HCI_INFORMATIONAL_PARAMETERS_COMMANDS = 0x04,
	HCI_STATUS_PARAMETERS_COMMANDS = 0x05,
	HCI_TESTING_COMMANDS = 0x06,
	HCI_EVENTS_COMMANDS = 0x07,
	HCI_LE_CONTROLLER_COMMANDS = 0x08,
}HCI_OGF_TYPE;

typedef enum {
	HCI_Inquiry_Complete_Event = 0x01,
	HCI_Inquiry_Result_Event = 0x02,
	HCI_Connection_Complete_Event = 0x03,
	HCI_Connection_Request_Event = 0x04,
	HCI_Disconnection_Complete_Event = 0x05,
	HCI_Authentication_Complete_Event = 0x06,
	HCI_Remote_Name_Request_Complete_Event = 0x07,
	HCI_Encryption_Change_Event = 0x08,
	HCI_Change_Connection_Link_Key_Complete_Event = 0x09,
	HCI_Master_Link_Key_Complete_Event = 0x0A,
	HCI_Read_Remote_Supported_Features_Complete_Event = 0x0B,
	HCI_Read_Remote_Version_Information_Complete_Event = 0x0C,
	HCI_QoS_Setup_Complete_Event = 0x0D,
	HCI_Command_Complete_Event = 0x0E,
	HCI_Command_Status_Event = 0x0F,
	HCI_Hardware_Error_Event = 0x10,
	HCI_Flush_Occurred_Event = 0x11,
	HCI_Role_Change_Event = 0x12,
	HCI_Number_Of_Completed_Packets_Event = 0x13,
	HCI_Mode_Change_Event = 0x14,
	HCI_Return_Link_Keys_Event = 0x15,
	HCI_PIN_Code_Request_Event = 0x16,
	HCI_Link_Key_Request_Event = 0x17,
	HCI_Link_Key_Notification_Event = 0x18,
	HCI_Loopback_Command_Event = 0x19,
	HCI_Data_Buffer_Overflow_Event = 0x1A,
	HCI_Max_Slots_Change_Event = 0x1B,
	HCI_Read_Clock_Offset_Complete_Event = 0x1C,
	HCI_Connection_Packet_Type_Changed_Event = 0x1D,
	HCI_QoS_Violation_Event = 0x1E,
	HCI_Page_Scan_Repetition_Mode_Change_Event = 0x20,
	HCI_Flow_Specification_Complete_Event = 0x21,
	HCI_Inquiry_Result_with_RSSI_Event = 0x22,
	HCI_Read_Remote_Extended_Features_Complete_Event = 0x23,
	HCI_Synchronous_Connection_Complete_Event = 0x2C,
	HCI_Synchronous_Connection_Changed_Event = 0x2D,
	HCI_Sniff_Subrating_Event = 0x2E,
	HCI_Extended_Inquiry_Result_Event = 0x2F,
	HCI_Encryption_Key_Refresh_Complete_Event = 0x30,
	HCI_IO_Capability_Request_Event = 0x31,
	HCI_IO_Capability_Response_Event = 0x32,
	HCI_User_Confirmation_Request_Event = 0x33,
	HCI_User_Passkey_Request_Event = 0x34,
	HCI_Remote_OOB_Data_Request_Event = 0x35,
	HCI_Simple_Pairing_Complete_Event = 0x36,
	HCI_Link_Supervision_Timeout_Changed_Event = 0x38,
	HCI_Enhanced_Flush_Complete_Event = 0x39,
	HCI_User_Passkey_Notification_Event = 0x3B,
	HCI_Keypress_Notification_Event = 0x3C,
	HCI_Remote_Host_Supported_Features_Notification_Event = 0x3D,
	HCI_Physical_Link_Complete_Event = 0x40,
	HCI_Channel_Selected_Event = 0x41,
	HCI_Disconnection_Physical_Link_Complete_Event = 0x42,
	HCI_Physical_Link_Loss_Early_Warning_Event = 0x43,
	HCI_Physical_Link_Recovery_Event = 0x44,
	HCI_Logical_Link_Complete_Event = 0x45,
	HCI_Disconnection_Logical_Link_Complete_Event = 0x46,
	HCI_Flow_Spec_Modify_Complete_Event = 0x47,
	HCI_Number_Of_Completed_Data_Blocks_Event = 0x48,
	HCI_Short_Range_Mode_Change_Complete_Event = 0x4C,
	HCI_AMP_Status_Change_Event = 0x4D,
	HCI_AMP_Start_Test_Event = 0x49,
	HCI_AMP_Test_End_Event = 0x4A,
	HCI_AMP_Receiver_Report_Event = 0x4B,
	HCI_LE_Meta_Event_Event = 0x3E,
	HCI_Triggered_Clock_Capture_Event = 0x4E,
	HCI_Synchronization_Train_Complete_Event = 0x4F,
	HCI_Synchronization_Train_Received_Event = 0x50,
	HCI_Connectionless_Slave_Broadcast_Receive_Event = 0x51,
	HCI_Connectionless_Slave_Broadcast_Timeout_Event = 0x52,
	HCI_Truncated_Page_Complete_Event = 0x53,
	HCI_Slave_Page_Response_Timeout_Event = 0x54,
	HCI_Connectionless_Slave_Broadcast_Channel_Map_Change_Event = 0x55,
	HCI_Inquiry_Response_Notification_Event = 0x56,
	HCI_Authenticated_Payload_Timeout_Expired_Event = 0x57,
	HCI_SAM_Status_Change_Event = 0x58,
	
}HCI_Chip_Event_TYPE;

typedef enum {
	SCAN_UNDISCOVER_UNCONNECT = 0x00,
	SCAN_DISCOVER_UNCONNECT = 0x01,
	SCAN_UNDISCOVER_CONNECT = 0x02,
	SCAN_DISCOVER_CONNECT = 0x03,
}scan_mode;
typedef struct {
	uint16_t OCF:10;
	uint16_t OGF:6;
	uint8_t LEN:8;
	uint8_t *DATA;
}HCI_Command_Packet_Struct;

typedef struct {
	uint16_t HANDLE:12;
	uint16_t PB_FLAG:2;
	uint16_t BC_FLAG:2;
	uint16_t LEN:16;
	uint8_t DATA[0];
}HCI_ACL_Data_packet_Struct;

typedef struct {
	uint16_t Connection_Handle:12;
	uint16_t Packet_Status_Flag:2;
	uint16_t RFU:2;
	uint8_t LEN:8;
	uint8_t DATA[0];
}HCI_Synchronous_Data_packet_Struct;

typedef struct {
	uint8_t Event_Code:8;
	uint8_t LEN:8;
	uint8_t DATA[0];
}HCI_Event_packet_Struct;

typedef struct {
	uint16_t Connection_Handle:12;
	uint16_t PB_Flag:2;
	uint16_t TS_Flag:1;
	uint16_t RFU1:1;
	uint16_t LEN:14;
	uint16_t RFU2:2;
	uint8_t DATA[0];
}HCI_ISO_Data_packets_Struct;

typedef struct {
	uint8_t byte0;
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t byte7;
}HCI_Support_Feature;


typedef struct {
	uint8_t  loacl_addr[6];
	
	/*remote device info*/
	uint8_t  remote_addr[6];
	uint32_t remote_class_of_device;
	uint8_t  remote_link_type;
    uint8_t  remote_link_encryption_type;
	
	HCI_Support_Feature hci_support_feature;// 8 byte
	uint8_t  sco_data_packet_length;
	uint16_t acl_data_packet_length;
	uint16_t total_num_acl_data_packets;
	uint16_t total_num_sco_data_packets;
	
	uint8_t  host_sco_data_packet_length;
	uint16_t host_acl_data_packet_length;
	uint16_t host_total_num_acl_data_packets;
	uint16_t host_total_num_sco_data_packets;
	
	//version
	uint8_t HCI_Version;
	uint16_t HCI_Revision;
	uint8_t LMP_PAL_Version;
	uint16_t Manufacturer_Name;
	uint16_t LMP_PAL_Subversion;

    //ACL
    uint16_t ACL_Connection_Handle;
	
}HCI_STATUS_Struct;

typedef void (*HCI_Event_Handle_Fun)(uint8_t *buffer, uint32_t len);
typedef void (*HCI_Cmd_Callback)(void);

void HCI_Cmd_Exec_Next(void);
void bt_stack_init(void);
void Send_HCI_Command_Packet(HCI_Command_Packet_Struct data);
void HCI_Event_Handle_Index(uint8_t index,uint8_t *buffer, uint32_t len);
