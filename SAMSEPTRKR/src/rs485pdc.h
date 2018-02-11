/*
 * rs485pdc.h
 *
 * Created: 15-Oct-17 11:35:39 AM
 *  Author: Mahesh
 */ 


#ifndef __RS485PDC_H__
#define __RS485PDC_H__

#include <asf.h>
#include "compiler.h"
#include "board.h"
#include "conf_board.h"
#include "gpio.h"
#include "ioport.h"

#define RS485_USART					USART0
#define RS485_USART_ID				ID_USART0
#define RS485_BAUDRATE				9600

/* Size of the receive buffer and transmit buffer. */
#define RS485_BUFFER_SIZE		150

#ifndef RS485_BUFFER_SIZE
	#error "RS485_BUFFER_SIZE Undefined"
#endif

/* Initialize the PDC Module */
void InitRs485Pdc(void);
/* Set pointers and start transmitting */
void Rs485PdcStartTx(uint32_t buff, uint16_t nBytes);
/* Set pointers and start receiving */
void Rs485PdcStartRx(uint32_t buff, uint16_t nBytes);
/* Returns number of bytes in Rx buffer */
uint32_t Rs485PdcGetRxBytes(void);
/* Returns 1 if Rx complete interrupt has occurred */
uint8_t Rs485PdcGetRxStatus(void);
/* Returns 1 if Tx Buffer is empty */
uint32_t Rs485PdcGetTxStatus(void);


#endif /* __RS485PDC_H__ */