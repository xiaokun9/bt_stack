#include "bt_l2cap_pack.h"


void HCI_ACL_DATA_TO_L2CAP(uint8_t* buffer)
{
    //1:0~11 bit :Handle
    //2:12~13 bit :PB Flag
    //3:14~15 bit :BC Flag
    //4:16~31 bit :Data Total Length
    //5:32~ bit :Data
    
}
