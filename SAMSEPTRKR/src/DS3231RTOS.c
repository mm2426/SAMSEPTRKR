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

#include "DS3231RTOS.h"

void DSGetTimeTo(freertos_twi_if port, uint16_t *recvTime, int timeOut)
{
    uint8_t buff[3];
	twi_packet_t pkt;
	
	pkt.chip = DS_ADDR;
	pkt.addr[0] = DS_REG_SEC;
	pkt.addr_length = 1;
	pkt.buffer = buff;
	pkt.length = 3;
	freertos_twi_read_packet(port, &pkt,timeOut);

	//SS
	recvTime[0] = buff[0];
	//MM
	recvTime[1] = buff[1];
	//HH
	recvTime[2] = buff[2];
}

void DSSetTimeTo(freertos_twi_if port, uint16_t *time, int timeOut)
{
	uint8_t buff[3];
	twi_packet_t pkt;
	
	pkt.chip = DS_ADDR;
	pkt.addr[0] = DS_REG_SEC;
	pkt.addr_length = 1;
	pkt.buffer = buff;
	pkt.length = 3;

	//SS
	buff[0] = time[0];
	//MM
	buff[1] = time[1];
	//HH
	buff[2] = time[2];

	freertos_twi_write_packet(port, &pkt, timeOut);
}

void DSGetSecsTo(freertos_twi_if port, uint16_t *ss, int timeOut)
{
    DSReadByteTo(port, DS_REG_SEC, (uint8_t *)ss, timeOut);
}

void DSSetSecsTo(freertos_twi_if port, uint8_t ss, int timeOut)
{
    DSWriteByteTo(port, DS_REG_SEC, ss, timeOut);
}

void DSGetMinsTo(freertos_twi_if port, uint16_t *mm, int timeOut)
{
    DSReadByteTo(port, DS_REG_MIN, (uint8_t *)mm, timeOut);
}

void DSSetMinsTo(freertos_twi_if port, uint8_t mm, int timeOut)
{
    DSWriteByteTo(port, DS_REG_MIN, mm, timeOut);
}

void DSGetHrsTo(freertos_twi_if port, uint16_t *hh, int timeOut)
{
    DSReadByteTo(port, DS_REG_HRS, (uint8_t *)hh, timeOut);
}

void DSSetHrsTo(freertos_twi_if port, uint8_t hh, int timeOut)
{
    DSWriteByteTo(port, DS_REG_HRS, hh, timeOut);
}

void DSGetFullDateTo(freertos_twi_if port, uint16_t *recvDate, int timeOut)
{
    uint8_t buff[4];
    twi_packet_t pkt;
    
    pkt.chip = DS_ADDR;
    pkt.addr[0] = DS_REG_DAY;
    pkt.addr_length = 1;
    pkt.buffer = buff;
    pkt.length = 4;
    freertos_twi_read_packet(port, &pkt,timeOut);
	//DAY
    recvDate[0] = buff[0];
    //DD
	recvDate[1] = buff[1];
    //MM
	recvDate[2] = buff[2];
	//YY
	recvDate[3] = buff[3];
}

void DSSetFullDateTo(freertos_twi_if port, uint16_t *date, int timeOut)
{
	uint8_t buff[4];
	twi_packet_t pkt;
	
	pkt.chip = DS_ADDR;
	pkt.addr[0] = DS_REG_DAY;
	pkt.addr_length = 1;
	pkt.buffer = buff;
	pkt.length = 4;

	//DAY
	buff[0] = date[0];
	//DD
	buff[1] = date[1];
	//MM
	buff[2] = date[2];
	//YY
	buff[3] = date[3];

	freertos_twi_write_packet(port, &pkt, timeOut);
}

void DSGetDayTo(freertos_twi_if port, uint16_t *d, int timeOut)
{
    DSReadByteTo(port, DS_REG_DAY, (uint8_t *)d, timeOut);
}

void DSSetDayTo(freertos_twi_if port, uint8_t d, int timeOut)
{
    DSWriteByteTo(port, DS_REG_DAY, d, timeOut);
}

void DSGetDateTo(freertos_twi_if port, uint16_t *dd, int timeOut)
{
    DSReadByteTo(port, DS_REG_DAT, (uint8_t *)dd, timeOut);
}

void DSSetDateTo(freertos_twi_if port, uint8_t dd, int timeOut)
{
    DSWriteByteTo(port, DS_REG_DAT, dd, timeOut);
}

void DSGetMonthTo(freertos_twi_if port, uint16_t *mm, int timeOut)
{
    DSReadByteTo(port, DS_REG_MON, (uint8_t *)mm, timeOut);
}

void DSSetMonthTo(freertos_twi_if port, uint8_t mm, int timeOut)
{
    DSWriteByteTo(port, DS_REG_MON, mm, timeOut);
}

void DSGetYearTo(freertos_twi_if port, uint16_t *yy, int timeOut)
{
    DSReadByteTo(port, DS_REG_YRS, (uint8_t *)yy, timeOut);
}

void DSSetYearTo(freertos_twi_if port, uint8_t yy, int timeOut)
{
    DSWriteByteTo(port, DS_REG_YRS, yy, timeOut);
}

void DSEnAL1To(freertos_twi_if port, uint8_t en, int timeOut)
{
    uint8_t buff[4];
    twi_packet_t pkt;
    
    if(en)
    {
        pkt.chip = DS_ADDR;
        pkt.addr[0] = DS_REG_AL1S;
        pkt.addr_length = 1;
        pkt.buffer = buff;
        pkt.length = 4;

        buff[0] = 0x80;
        buff[1] = 0x80;
        buff[2] = 0x80;
        buff[3] = 0x80;

		//Set Alarm 1 for 1 Sec interrupt
        freertos_twi_write_packet(port, &pkt, timeOut);
		//Write Enable Alarm Register
        DSWriteByteTo(port, DS_REG_CTRL, 0x05, timeOut);
        //Clear Status Reg
        DSWriteByteTo(port, DS_REG_STAT, 0x00, timeOut);
    }
    else
    {
        //Write Disable Alarm Register
        DSWriteByteTo(port, DS_REG_CTRL, 0x04, timeOut);
    }
}

void DSEnAL2To(freertos_twi_if port, uint8_t en, int timeOut)
{
    uint8_t buff[3];
    twi_packet_t pkt;

    if(en)
    {
        pkt.chip = DS_ADDR;
        pkt.addr[0] = DS_REG_AL2M;
        pkt.addr_length = 1;
        pkt.buffer = buff;
        pkt.length = 3;

        buff[0] = 0x80;
        buff[1] = 0x80;
        buff[2] = 0x80;

		//Set Alarm 2 for 1 min interrupt
        freertos_twi_write_packet(port, &pkt, timeOut);
        //Write Enable Alarm Register
        DSWriteByteTo(port, DS_REG_CTRL, 0x06, timeOut);
        //Clear Status Reg
        DSWriteByteTo(port, DS_REG_STAT, 0x80, timeOut);
    }
    else
    {
        //Write Disable Alarm Register
        DSWriteByteTo(port, DS_REG_CTRL, 0x04, timeOut);
    }
}

void DSSetCtrlRegTo(freertos_twi_if port, uint8_t val, int timeOut)
{
    DSWriteByteTo(port, DS_REG_CTRL, val, timeOut);
}

void DSGetCtrlRegTo(freertos_twi_if port, uint16_t *val, int timeOut)
{
    DSReadByteTo(port, DS_REG_CTRL, (uint8_t *)val, timeOut);
}

void DSGetStatRegTo(freertos_twi_if port, uint16_t *val, int timeOut)
{
    DSReadByteTo(port, DS_REG_STAT, (uint8_t *)val, timeOut);
}

void DSGetTempRegTo(freertos_twi_if port, int16_t *temp, int timeOut)
{
    uint8_t buff[2];
	twi_packet_t pkt;
	
	pkt.chip = DS_ADDR;
	pkt.addr[0] = DS_REG_TEMPH;
	pkt.addr_length = 1;
	pkt.buffer = buff;
	pkt.length = 2;
	freertos_twi_read_packet(port, &pkt,timeOut);
	*temp = (((uint16_t)buff[0])<<8) | buff[1];
}

void DSWriteByteTo(freertos_twi_if port, uint8_t reg, uint8_t data, int timeOut)
{
    twi_packet_t pkt;
    
    pkt.chip = DS_ADDR;
    pkt.addr[0] = reg;
    pkt.addr_length = 1;
    pkt.buffer = &data;
    pkt.length = 1;
	
	freertos_twi_write_packet(port, &pkt, timeOut);
}

void DSReadByteTo(freertos_twi_if port, uint8_t reg, uint8_t *recvData, int timeOut)
{
    twi_packet_t pkt;
    
    pkt.chip = DS_ADDR;
    pkt.addr[0] = reg;
    pkt.addr_length = 1;
    pkt.buffer = recvData;
    pkt.length = 1;
    
    freertos_twi_read_packet(port, &pkt, timeOut);
}

/* [] END OF FILE */
