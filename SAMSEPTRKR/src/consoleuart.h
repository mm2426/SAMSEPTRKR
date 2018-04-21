/*
 * consoleuart.h
 *
 * Created: 21-Apr-18 3:33:28 PM
 *  Author: Mahesh
 */ 


#ifndef __CONSOLEUART_H__
#define __CONSOLEUART_H__		1

#include <asf.h>
#include "conf_board.h"
#include <string.h>
#ifdef __FREERTOS__
	#include "FreeRTOS.h"
#endif

/* Console Port Definitions */
#define CNSL_UART				UART0
#define CNSL_UART_ID			ID_UART0
#define CNSL_BAUDRATE			115200

void ConsoleInit(void);
void ConsoleWrite(uint8_t *buff, uint16_t dataLen);

#endif /* __CONSOLEUART_H__ */