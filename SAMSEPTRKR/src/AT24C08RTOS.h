/*
 * AT28C08.h
 *
 * Created: 16-Apr-18 11:58:15 AM
 *  Author: Mahesh
 */ 


#ifndef __AT24C08_H__
#define __AT24C08_H__		1

#include <freertos_twi_master.h>

#if defined(AT24C08_USE_RTOS_API)
	void WriteEEPROM(freertos_twi_if port, uint8_t chipAddr, uint8_t memPage, uint8_t *dPkt, uint16_t dLen, int timeOut);
	void ReadEEPROM(freertos_twi_if port, uint8_t chipAddr, uint8_t memPage, uint8_t *dPkt, uint16_t dLen, int timeOut);
#endif

#endif /* __AT24C08_H__ */