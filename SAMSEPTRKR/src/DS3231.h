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

#ifndef __DS3231_H__
    #define __DS3231_H__    1
    
	#ifndef DS3231_USE_RTOS_API
		//7 Bit Device ADDR
		#define DS_ADDR				0x68
    
		//Register Addresses
		//Timekeeper Registers
		#define DS_REG_SEC			0x00
		#define DS_REG_MIN			0x01
		#define DS_REG_HRS			0x02
		#define DS_REG_DAY			0x03
		#define DS_REG_DAT			0x04
		#define DS_REG_MON			0x05
		#define DS_REG_YRS			0x06
    
		//Alarm 1 Registers
		#define DS_REG_AL1S			0x07
		#define DS_REG_AL1M			0x08
		#define DS_REG_AL1H			0x09
		#define DS_REG_AL1D			0x0A
    
		//Alarm 2 Registers
		#define DS_REG_AL2M			0x0B
		#define DS_REG_AL2H			0x0C
		#define DS_REG_AL2D			0x0D
    
		//Management Registers
		#define DS_REG_CTRL			0x0E
		#define DS_REG_STAT			0x0F
		#define DS_REG_AOFST		0x10
		#define DS_REG_TEMPH		0x11
		#define DS_REG_TEMPL		0x12
    
		#include <asf.h>
		#include "conf_board.h"

		//Function Declarations
		void DSGetTime(Twi *port, uint16_t *recvTime);
		void DSSetTime(Twi *port, uint16_t *time);
		void DSGetSecs(Twi *port, uint16_t *ss);
		void DSSetSecs(Twi *port, uint8_t ss);
		void DSGetMins(Twi *port, uint16_t *mm);
		void DSSetMins(Twi *port, uint8_t mm);
		void DSGetHrs(Twi *port, uint16_t *hh);
		void DSSetHrs(Twi *port, uint8_t hh);
		void DSGetFullDate(Twi *port, uint16_t *recvDate);
		void DSSetFullDate(Twi *port, uint16_t *date);
		void DSGetDay(Twi *port, uint16_t *d);
		void DSSetDay(Twi *port, uint8_t d);
		void DSGetDate(Twi *port, uint16_t *dd);
		void DSSetDate(Twi *port, uint8_t dd);
		void DSGetMonth(Twi *port, uint16_t *mm);
		void DSSetMonth(Twi *port, uint8_t mm);
		void DSGetYear(Twi *port, uint16_t *yy);
		void DSSetYear(Twi *port, uint8_t yy);
		void DSEnAL1(Twi *port, uint8_t en);
		void DSEnAL2(Twi *port, uint8_t en);
		void DSSetCtrlReg(Twi *port, uint8_t val);
		void DSGetCtrlReg(Twi *port, uint16_t *val);
		void DSGetStatReg(Twi *port, uint16_t *val);
		void DSGetTempReg(Twi *port, int16_t *temp);
		void DSWriteByte(Twi *port, uint8_t reg, uint8_t data);
		void DSReadByte(Twi *port, uint8_t reg, uint8_t *recvData);
    #endif
#endif

/* [] END OF FILE */
