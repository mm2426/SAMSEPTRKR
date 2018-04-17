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

#ifndef __DS3231RTOS_H__
    #define __DS3231RTOS_H__    1
    #if defined(DS3231_USE_RTOS_API)
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
    
		#include <freertos_twi_master.h>

		//Function Declarations
		void DSGetTimeTo(freertos_twi_if port, uint16_t *recvTime, int timeOut);
		void DSSetTimeTo(freertos_twi_if port, uint16_t *time, int timeOut);
		void DSGetSecsTo(freertos_twi_if port, uint16_t *ss, int timeOut);
		void DSSetSecsTo(freertos_twi_if port, uint8_t ss, int timeOut);
		void DSGetMinsTo(freertos_twi_if port, uint16_t *mm, int timeOut);
		void DSSetMinsTo(freertos_twi_if port, uint8_t mm, int timeOut);
		void DSGetHrsTo(freertos_twi_if port, uint16_t *hh, int timeOut);
		void DSSetHrsTo(freertos_twi_if port, uint8_t hh, int timeOut);
		void DSGetFullDateTo(freertos_twi_if port, uint16_t *recvDate, int timeOut);
		void DSSetFullDateTo(freertos_twi_if port, uint16_t *date, int timeOut);
		void DSGetDayTo(freertos_twi_if port, uint16_t *d, int timeOut);
		void DSSetDayTo(freertos_twi_if port, uint8_t d, int timeOut);
		void DSGetDateTo(freertos_twi_if port, uint16_t *dd, int timeOut);
		void DSSetDateTo(freertos_twi_if port, uint8_t dd, int timeOut);
		void DSGetMonthTo(freertos_twi_if port, uint16_t *mm, int timeOut);
		void DSSetMonthTo(freertos_twi_if port, uint8_t mm, int timeOut);
		void DSGetYearTo(freertos_twi_if port, uint16_t *yy, int timeOut);
		void DSSetYearTo(freertos_twi_if port, uint8_t yy, int timeOut);
		void DSEnAL1To(freertos_twi_if port, uint8_t en, int timeOut);
		void DSEnAL2To(freertos_twi_if port, uint8_t en, int timeOut);
		void DSSetCtrlRegTo(freertos_twi_if port, uint8_t val, int timeOut);
		void DSGetCtrlRegTo(freertos_twi_if port, uint16_t *val, int timeOut);
		void DSGetStatRegTo(freertos_twi_if port, uint16_t *val, int timeOut);
		void DSGetTempRegTo(freertos_twi_if port, int16_t *temp, int timeOut);
		void DSWriteByteTo(freertos_twi_if port, uint8_t reg, uint8_t data, int timeOut);
		void DSReadByteTo(freertos_twi_if port, uint8_t reg, uint8_t *recvData, int timeOut);
    #endif
#endif

/* [] END OF FILE */
