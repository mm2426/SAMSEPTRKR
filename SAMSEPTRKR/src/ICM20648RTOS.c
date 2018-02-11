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

#include "ICM20648RTOS.h"


void ICMInitializeTo(freertos_twi_if port, uint8_t sla, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(port, sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    //Auto Select Best Clock Source
    ICMWriteByteTo(port, sla,ICM_REG_PWR_MGMT1,0x01, timeOut);
    
    //Select USER Bank 2
    ICMWriteByteTo(port, sla,ICM_REG_BANK_SEL,0x20, timeOut);
    
    //Accel Full Scale set to 2G
    //ICMWriteByteTo(sla,ICM_REG_ACC_CONFIG,0x00, timeOut);
    //Full Scale = 2G, Enable DLP, Cutoff = 23Hz
    ICMWriteByteTo(port, sla,ICM_REG_ACC_CONFIG,0x31, timeOut);
    
    //Output Data Rate Set to 17.xx Hz
    ICMWriteByteTo(port, sla,ICM_REG_ACC_SMPLRT_DIV1,0x00, timeOut);
    ICMWriteByteTo(port, sla,ICM_REG_ACC_SMPLRT_DIV2,0x3F, timeOut);
    
}

void ICMReadAccDataXTo(freertos_twi_if port, uint8_t sla, uint16_t *recvData, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(port, sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    //Read ACC data
    ICMReadByteTo(port, sla, ICM_REG_ACC_XOUTH, &((uint8_t *)recvData)[1], timeOut);
    ICMReadByteTo(port, sla, ICM_REG_ACC_XOUTL, &((uint8_t *)recvData)[0], timeOut);
}

void ICMReadAccDataYTo(freertos_twi_if port, uint8_t sla, uint16_t *recvData, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(port, sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    ICMReadByteTo(port, sla, ICM_REG_ACC_YOUTH, &((uint8_t *)recvData)[1], timeOut);
    ICMReadByteTo(port, sla, ICM_REG_ACC_YOUTL, &((uint8_t *)recvData)[0], timeOut);
}

void ICMReadAccDataZTo(freertos_twi_if port, uint8_t sla, uint16_t *recvData, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(port, sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    //Read ACC data
    ICMReadByteTo(port, sla, ICM_REG_ACC_ZOUTH, &((uint8_t *)recvData)[1], timeOut);
    ICMReadByteTo(port, sla, ICM_REG_ACC_ZOUTL, &((uint8_t *)recvData)[0], timeOut);
}

void ICMReadAccDataAllTo(freertos_twi_if port, uint8_t sla, uint16_t *recvData, int timeOut)
{
    //Select USER Bank 0
    ICMWriteByteTo(port, sla,ICM_REG_BANK_SEL,0x00, timeOut);
    
    //Read ACC data
    ICMReadByteTo(port, sla, ICM_REG_ACC_XOUTH, &((uint8_t *)recvData)[1], timeOut);
    ICMReadByteTo(port, sla, ICM_REG_ACC_XOUTL, &((uint8_t *)recvData)[0], timeOut);
    ICMReadByteTo(port, sla, ICM_REG_ACC_YOUTH, &((uint8_t *)recvData)[3], timeOut);
    ICMReadByteTo(port, sla, ICM_REG_ACC_YOUTL, &((uint8_t *)recvData)[2], timeOut);
    ICMReadByteTo(port, sla, ICM_REG_ACC_ZOUTH, &((uint8_t *)recvData)[5], timeOut);
    ICMReadByteTo(port, sla, ICM_REG_ACC_ZOUTL, &((uint8_t *)recvData)[4], timeOut);
}

void ICMWriteByteTo(freertos_twi_if port, uint8_t sla, uint8_t reg, uint8_t data, int timeOut)
{
    twi_packet_t pkt;
    
    pkt.chip = sla;
    pkt.addr[0] = reg;
    pkt.addr_length = 1;
    pkt.buffer = &data;
    pkt.length = 1;
    
    freertos_twi_write_packet(port, &pkt, timeOut);
}

void ICMReadByteTo(freertos_twi_if port, uint8_t sla, uint8_t reg, uint8_t *recvData, int timeOut)
{
    twi_packet_t pkt;
    
    pkt.chip = sla;
    pkt.addr[0] = reg;
    pkt.addr_length = 1;
    pkt.buffer = recvData;
    pkt.length = 1;
    
    freertos_twi_read_packet(port, &pkt, timeOut);
}

/* [] END OF FILE */
