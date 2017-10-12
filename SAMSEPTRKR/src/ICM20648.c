/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "ICM20648.h"


void ICMInitializeTo(uint8 sla, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    //Auto Select Best Clock Source
    ICMWriteByteTo(sla,ICM_REG_PWR_MGMT1,0x01, timeOut);
    
    //Select USER Bank 2
    ICMWriteByteTo(sla,ICM_REG_BANK_SEL,0x20, timeOut);
    
    //Accel Full Scale set to 2G
    //ICMWriteByteTo(sla,ICM_REG_ACC_CONFIG,0x00, timeOut);
    //Full Scale = 2G, Enable DLP, Cutoff = 23Hz
    ICMWriteByteTo(sla,ICM_REG_ACC_CONFIG,0x31, timeOut);
    
    //Output Data Rate Set to 17.xx Hz
    ICMWriteByteTo(sla,ICM_REG_ACC_SMPLRT_DIV1,0x00, timeOut);
    ICMWriteByteTo(sla,ICM_REG_ACC_SMPLRT_DIV2,0x3F, timeOut);
    
}

void ICMReadAccDataXTo(uint8 sla, uint16 *recvData, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    //Read ACC data
    ICMReadByteTo(sla, ICM_REG_ACC_XOUTH, &((uint8 *)recvData)[1], timeOut);
    ICMReadByteTo(sla, ICM_REG_ACC_XOUTL, &((uint8 *)recvData)[0], timeOut);
}

void ICMReadAccDataYTo(uint8 sla, uint16 *recvData, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    ICMReadByteTo(sla, ICM_REG_ACC_YOUTH, &((uint8 *)recvData)[1], timeOut);
    ICMReadByteTo(sla, ICM_REG_ACC_YOUTL, &((uint8 *)recvData)[0], timeOut);
}

void ICMReadAccDataZTo(uint8 sla, uint16 *recvData, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    //Read ACC data
    ICMReadByteTo(sla, ICM_REG_ACC_ZOUTH, &((uint8 *)recvData)[1], timeOut);
    ICMReadByteTo(sla, ICM_REG_ACC_ZOUTL, &((uint8 *)recvData)[0], timeOut);
}

void ICMReadAccDataAllTo(uint8 sla, uint16 *recvData, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    //Read ACC data
    ICMReadByteTo(sla, ICM_REG_ACC_XOUTH, &((uint8 *)recvData)[1], timeOut);
    ICMReadByteTo(sla, ICM_REG_ACC_XOUTL, &((uint8 *)recvData)[0], timeOut);
    ICMReadByteTo(sla, ICM_REG_ACC_YOUTH, &((uint8 *)recvData)[3], timeOut);
    ICMReadByteTo(sla, ICM_REG_ACC_YOUTL, &((uint8 *)recvData)[2], timeOut);
    ICMReadByteTo(sla, ICM_REG_ACC_ZOUTH, &((uint8 *)recvData)[5], timeOut);
    ICMReadByteTo(sla, ICM_REG_ACC_ZOUTL, &((uint8 *)recvData)[4], timeOut);
}

void ICMWriteByteTo(uint8 sla, uint8 reg, uint8 data, int timeOut)
{
    I2C0_MasterSendStartTo(sla,0, timeOut);
    I2C0_MasterWriteByteTo(reg, timeOut);
    I2C0_MasterWriteByteTo(data, timeOut);
    I2C0_MasterSendStopTo(timeOut);
}

void ICMReadByteTo(uint8 sla, uint8 reg, uint8 *recvData, int timeOut)
{
    I2C0_MasterSendStartTo(sla,0, timeOut);
    I2C0_MasterWriteByteTo(reg, timeOut);
    I2C0_MasterSendRestartTo(sla,1, timeOut);
    *recvData = I2C0_MasterReadByteTo(I2C0_NAK_DATA, timeOut);
    I2C0_MasterSendStopTo(timeOut);
}

/* [] END OF FILE */
