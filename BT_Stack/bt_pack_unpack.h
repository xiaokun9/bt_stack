#include "stm32f10x.h"
#include <stdio.h>

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


typedef struct {
	uint16_t OCF:10;
	uint16_t OGF:6;
	uint8_t LEN:8;
	uint8_t DATA[0];
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
