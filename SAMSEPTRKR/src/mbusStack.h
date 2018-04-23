/*
 * mbusStack.h
 *
 * Created: 03-Nov-17 10:12:41 PM
 *  Author: Mahesh
 */ 


#ifndef __MBUSSTACK_H__
#define __MBUSSTACK_H__

#include <asf.h>
#include "user_board.h"

/* Maximum Modbus Registers */
#define MBUS_MAX_REGS           38

/* Maximum Modbus Registers */
#ifndef MBUS_MAX_REGS
	#error "MBUS_MAX_REGS Undefined"
#endif

/* Modbus CRC Gen Poly */
#define MBUS_CRC_POLY           0xA001

/* To check whether bit is set / clr */
#define bitIsSet(val, bitNo)    (val&(1<<bitNo))

//Write function pointer
extern void (*writeFunc)(uint16_t *mbusBuff, uint8_t regAddr, uint8_t len);

/* Modbus stack parser function */
void ParseMbusPkt(uint8_t slaveAddr, uint8_t *buff, uint8_t len, uint8_t *respBuff, uint8_t *respLen);

uint16_t calcCRC(uint8_t *buff, uint8_t len);

#endif /* __MBUSSTACK_H__ */