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

#include "DS3231.h"

#ifndef DS3231_USE_RTOS_API
	void DSGetTime(Twi *port, uint16_t *recvTime)
	{
		uint8_t buff[3];
		twi_packet_t pkt;
	
		pkt.chip = DS_ADDR;
		pkt.addr[0] = DS_REG_SEC;
		pkt.addr_length = 1;
		pkt.buffer = buff;
		pkt.length = 3;
		twi_master_read(port, &pkt);

		//SS
		recvTime[0] = buff[0];
		//MM
		recvTime[1] = buff[1];
		//HH
		recvTime[2] = buff[2];
	}

	void DSSetTime(Twi *port, uint16_t *time)
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

		twi_master_write(port, &pkt);
	}

	void DSGetSecs(Twi *port, uint16_t *ss)
	{
		DSReadByte(port, DS_REG_SEC, (uint8_t *)ss);
	}

	void DSSetSecs(Twi *port, uint8_t ss)
	{
		DSWriteByte(port, DS_REG_SEC, ss);
	}

	void DSGetMins(Twi *port, uint16_t *mm)
	{
		DSReadByte(port, DS_REG_MIN, (uint8_t *)mm);
	}

	void DSSetMins(Twi *port, uint8_t mm)
	{
		DSWriteByte(port, DS_REG_MIN, mm);
	}

	void DSGetHrs(Twi *port, uint16_t *hh)
	{
		DSReadByte(port, DS_REG_HRS, (uint8_t *)hh);
	}

	void DSSetHrs(Twi *port, uint8_t hh)
	{
		DSWriteByte(port, DS_REG_HRS, hh);
	}

	void DSGetFullDate(Twi *port, uint16_t *recvDate)
	{
		uint8_t buff[4];
		twi_packet_t pkt;
    
		pkt.chip = DS_ADDR;
		pkt.addr[0] = DS_REG_DAY;
		pkt.addr_length = 1;
		pkt.buffer = buff;
		pkt.length = 4;
		twi_master_read(port, &pkt);
		//DAY
		recvDate[0] = buff[0];
		//DD
		recvDate[1] = buff[1];
		//MM
		recvDate[2] = buff[2];
		//YY
		recvDate[3] = buff[3];
	}

	void DSSetFullDate(Twi *port, uint16_t *date)
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

		twi_master_write(port, &pkt);
	}

	void DSGetDay(Twi *port, uint16_t *d)
	{
		DSReadByte(port, DS_REG_DAY, (uint8_t *)d);
	}

	void DSSetDay(Twi *port, uint8_t d)
	{
		DSWriteByte(port, DS_REG_DAY, d);
	}

	void DSGetDate(Twi *port, uint16_t *dd)
	{
		DSReadByte(port, DS_REG_DAT, (uint8_t *)dd);
	}

	void DSSetDate(Twi *port, uint8_t dd)
	{
		DSWriteByte(port, DS_REG_DAT, dd);
	}

	void DSGetMonth(Twi *port, uint16_t *mm)
	{
		DSReadByte(port, DS_REG_MON, (uint8_t *)mm);
	}

	void DSSetMonth(Twi *port, uint8_t mm)
	{
		DSWriteByte(port, DS_REG_MON, mm);
	}

	void DSGetYear(Twi *port, uint16_t *yy)
	{
		DSReadByte(port, DS_REG_YRS, (uint8_t *)yy);
	}

	void DSSetYear(Twi *port, uint8_t yy)
	{
		DSWriteByte(port, DS_REG_YRS, yy);
	}

	void DSEnAL1(Twi *port, uint8_t en)
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
			twi_master_write(port, &pkt);
			//Write Enable Alarm Register
			DSWriteByte(port, DS_REG_CTRL, 0x05);
			//Clear Status Reg
			DSWriteByte(port, DS_REG_STAT, 0x00);
		}
		else
		{
			//Write Disable Alarm Register
			DSWriteByte(port, DS_REG_CTRL, 0x04);
		}
	}

	void DSEnAL2(Twi *port, uint8_t en)
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
			twi_master_write(port, &pkt);
			//Write Enable Alarm Register
			DSWriteByte(port, DS_REG_CTRL, 0x06);
			//Clear Status Reg
			DSWriteByte(port, DS_REG_STAT, 0x80);
		}
		else
		{
			//Write Disable Alarm Register
			DSWriteByte(port, DS_REG_CTRL, 0x04);
		}
	}

	void DSSetCtrlReg(Twi *port, uint8_t val)
	{
		DSWriteByte(port, DS_REG_CTRL, val);
	}

	void DSGetCtrlReg(Twi *port, uint16_t *val)
	{
		DSReadByte(port, DS_REG_CTRL, (uint8_t *)val);
	}

	void DSGetStatReg(Twi *port, uint16_t *val)
	{
		DSReadByte(port, DS_REG_STAT, (uint8_t *)val);
	}

	void DSGetTempReg(Twi *port, int16_t *temp)
	{
		uint8_t buff[2];
		twi_packet_t pkt;
	
		pkt.chip = DS_ADDR;
		pkt.addr[0] = DS_REG_TEMPH;
		pkt.addr_length = 1;
		pkt.buffer = buff;
		pkt.length = 2;
		twi_master_read(port, &pkt);
		*temp = (((uint16_t)buff[0])<<8) | buff[1];
	}

	void DSWriteByte(Twi *port, uint8_t reg, uint8_t data)
	{
		twi_packet_t pkt;
    
		pkt.chip = DS_ADDR;
		pkt.addr[0] = reg;
		pkt.addr_length = 1;
		pkt.buffer = &data;
		pkt.length = 1;
	
		twi_master_write(port, &pkt);
	}

	void DSReadByte(Twi *port, uint8_t reg, uint8_t *recvData)
	{
		twi_packet_t pkt;
    
		pkt.chip = DS_ADDR;
		pkt.addr[0] = reg;
		pkt.addr_length = 1;
		pkt.buffer = recvData;
		pkt.length = 1;
    
		twi_master_read(port, &pkt);
	}
#endif

/* [] END OF FILE */
