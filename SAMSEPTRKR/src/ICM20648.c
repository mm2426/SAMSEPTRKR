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

#ifndef ICM20648_USE_RTOS_API
	void ICMInitialize(Twi *port, uint8_t sla)
	{
		//Select USER Bank 0
		ICMWriteByte(port, sla,ICM_REG_BANK_SEL,0x00);
    
		//Auto Select Best Clock Source
		ICMWriteByte(port, sla,ICM_REG_PWR_MGMT1,0x01);
    
		//Select USER Bank 2
		ICMWriteByte(port, sla,ICM_REG_BANK_SEL,0x20);
    
		//Accel Full Scale set to 2G
		//ICMWriteByte(sla,ICM_REG_ACC_CONFIG,0x00);
		//Full Scale = 2G, Enable DLP, Cutoff = 23Hz
		ICMWriteByte(port, sla,ICM_REG_ACC_CONFIG,0x31);
    
		//Output Data Rate Set to 17.xx Hz
		ICMWriteByte(port, sla,ICM_REG_ACC_SMPLRT_DIV1,0x00);
		ICMWriteByte(port, sla,ICM_REG_ACC_SMPLRT_DIV2,0x3F);
    
	}

	void ICMReadAccDataX(Twi *port, uint8_t sla, uint16_t *recvData)
	{
		//Select USER Bank 0
		ICMWriteByte(port, sla,ICM_REG_BANK_SEL,0x00);
    
		//Read ACC data
		ICMReadByte(port, sla, ICM_REG_ACC_XOUTH, &((uint8_t *)recvData)[1]);
		ICMReadByte(port, sla, ICM_REG_ACC_XOUTL, &((uint8_t *)recvData)[0]);
	}

	void ICMReadAccDataY(Twi *port, uint8_t sla, uint16_t *recvData)
	{
		//Select USER Bank 0
		ICMWriteByte(port, sla,ICM_REG_BANK_SEL,0x00);
    
		ICMReadByte(port, sla, ICM_REG_ACC_YOUTH, &((uint8_t *)recvData)[1]);
		ICMReadByte(port, sla, ICM_REG_ACC_YOUTL, &((uint8_t *)recvData)[0]);
	}

	void ICMReadAccDataZ(Twi *port, uint8_t sla, uint16_t *recvData)
	{
		//Select USER Bank 0
		ICMWriteByte(port, sla,ICM_REG_BANK_SEL,0x00);
    
		//Read ACC data
		ICMReadByte(port, sla, ICM_REG_ACC_ZOUTH, &((uint8_t *)recvData)[1]);
		ICMReadByte(port, sla, ICM_REG_ACC_ZOUTL, &((uint8_t *)recvData)[0]);
	}

	void ICMReadAccDataAll(Twi *port, uint8_t sla, uint16_t *recvData)
	{
		//Select USER Bank 0
		ICMWriteByte(port, sla,ICM_REG_BANK_SEL,0x00);
    
		//Read ACC data
		ICMReadByte(port, sla, ICM_REG_ACC_XOUTH, &((uint8_t *)recvData)[1]);
		ICMReadByte(port, sla, ICM_REG_ACC_XOUTL, &((uint8_t *)recvData)[0]);
		ICMReadByte(port, sla, ICM_REG_ACC_YOUTH, &((uint8_t *)recvData)[3]);
		ICMReadByte(port, sla, ICM_REG_ACC_YOUTL, &((uint8_t *)recvData)[2]);
		ICMReadByte(port, sla, ICM_REG_ACC_ZOUTH, &((uint8_t *)recvData)[5]);
		ICMReadByte(port, sla, ICM_REG_ACC_ZOUTL, &((uint8_t *)recvData)[4]);
	}

	void ICMWriteByte(Twi *port, uint8_t sla, uint8_t reg, uint8_t data)
	{
		twi_packet_t pkt;
    
		pkt.chip = sla;
		pkt.addr[0] = reg;
		pkt.addr_length = 1;
		pkt.buffer = &data;
		pkt.length = 1;
    
		twi_master_write(port, &pkt);
	}

	void ICMReadByte(Twi *port, uint8_t sla, uint8_t reg, uint8_t *recvData)
	{
		twi_packet_t pkt;
    
		pkt.chip = sla;
		pkt.addr[0] = reg;
		pkt.addr_length = 1;
		pkt.buffer = recvData;
		pkt.length = 1;
    
		twi_master_read(port, &pkt);
	}
#endif
/* [] END OF FILE */
