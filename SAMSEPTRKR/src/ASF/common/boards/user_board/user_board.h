/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

/* USART0 Definitions*/
#define PINS_USART0_PIO			PIOA
/** USART0 pins (RXD0, TXD0 & RTS0(TXEN)) definitions, PA5,6,7. */
#define PINS_USART0				(PIO_PA5A_RXD0 | PIO_PA6A_TXD0 | PIO_PA7A_RTS0)
#define PINS_USART0_FLAGS		(PIO_PERIPH_A | PIO_DEFAULT)

/* USART1 Definitions*/
#define PINS_USART1_PIO			PIOA
/** USART1 pins (RXD1, TXD1 & RTS1(TXEN)) definitions, PA21,22,24. */
#define PINS_USART1				(PIO_PA21A_RXD1 | PIO_PA22A_TXD1 | PIO_PA24A_RTS1)
#define PINS_USART1_FLAGS		(PIO_PERIPH_A | PIO_DEFAULT)

/* TWI0 Definitions*/
#define PINS_TWI0_PIO			PIOA
/* TWI0 pins (TWCK0 & TWD0) definitions, PA4, 3. */
#define PINS_TWI0				(PIO_PA4A_TWCK0 | PIO_PA3A_TWD0)
#define PINS_TWI0_FLAGS			(PIO_PERIPH_A | PIO_DEFAULT)

/* Motor Controller Pins */
#define PIN_MOTOR_RST_IDX		PIO_PB0_IDX
#define PIN_MOTOR_A_IDX			PIO_PB1_IDX
#define PIN_MOTOR_B_IDX			PIO_PB2_IDX

/* Debug LED Pin */
#define PIN_DEBUGLED_IDX		PIO_PA16_IDX

/* RTC Interrupt Pin */
#define PIN_RTC_INT_PIO			PIOA
#define PIN_RTC_INT_PIO_ID		ID_PIOA
#define PIN_RTC_INT_MASK		PIO_PA20
#define PIN_RTC_INT_IDX			PIO_PA20_IDX

void RTCIntHandler(uint32_t ul_id, uint32_t ul_mask);

#endif // USER_BOARD_H
