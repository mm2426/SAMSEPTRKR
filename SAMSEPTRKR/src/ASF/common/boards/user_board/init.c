/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */

	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;

	/* Init IO Port service to enable configuring pins */
	ioport_init();

	/* Configure USART0 Pins (RS485, MODBUS) */
	gpio_configure_group(PINS_USART0_PIO, PINS_USART0, PINS_USART0_FLAGS);

	/* Configure USART1 Pins (XBee) */
	gpio_configure_group(PINS_USART1_PIO, PINS_USART1, PINS_USART1_FLAGS);

	/* Configure UART0 Pins (Console) */
	gpio_configure_group(PINS_UART0_PIO, PINS_UART0, PINS_UART0_FLAGS);
	
	/* Configure TWI0 Pins */
	gpio_configure_group(PINS_TWI0_PIO, PINS_TWI0, PINS_TWI0_FLAGS);

	/* Configure Motor Controller Pins */
	#ifndef MOTOR_CTRL_A4955
	#else
		gpio_configure_pin(PIN_MOTOR_IN1_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
		gpio_configure_pin(PIN_MOTOR_IN2_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
		gpio_configure_pin(PIN_MOTOR_SLP_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
		gpio_configure_pin(PIN_MOTOR_OCL_IDX, (PIO_INPUT | PIO_OPENDRAIN));
	#endif

	/* Configure LED Pins */
	gpio_configure_pin(PIN_DEBUGLED_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));

	/* Configure LDO EN Pin */
	gpio_configure_pin(PIN_LDOEN_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
	
	/* Configure RTC 1 min Interrupt Pin */
	gpio_configure_pin(PIN_RTC_INT_IDX, PIO_INPUT);

	/* Initialize PIOs interrupt handlers (see PIO definition in board.h). */
	pio_handler_set(PIN_RTC_INT_PIO, PIN_RTC_INT_PIO_ID, PIN_RTC_INT_MASK, (PIO_OPENDRAIN | PIO_IT_FALL_EDGE), RTCIntHandler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type) PIN_RTC_INT_PIO_ID);

	/* Set RTC Int Priority */
	pio_handler_set_priority(PIN_RTC_INT_PIO, (IRQn_Type) PIN_RTC_INT_PIO_ID, configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
	
	/* Enable PIO line interrupts. */
	pio_enable_interrupt(PIN_RTC_INT_PIO, PIN_RTC_INT_MASK);
}
