/*
 * AT24C08.h
 *
 * Created: 16-Apr-18 12:26:55 PM
 *  Author: Mahesh
 */ 


#ifndef __AT24C08_H__
#define __AT24C08_H__

#include <asf.h>
#include "conf_board.h"

#ifndef AT24C08_USE_RTOS_API
	void WriteEEPROM(Twi * Port, uint8_t chipAddr, uint16_t memAddr, uint8_t *dPkt, uint8_t dLen);
	void ReadEEPROM(Twi * Port, uint8_t chipAddr, uint16_t memAddr, uint8_t *dPkt, uint16_t dLen);
#endif

#endif /* __AT24C08_H__ */