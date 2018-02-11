/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

/* External XTAL related params */
#define BOARD_FREQ_MAINCK_XTAL			(12000000UL)
#define BOARD_FREQ_MAINCK_BYPASS		(12000000UL)
#define BOARD_OSC_STARTUP_US			(15625UL)
#define BOARD_FREQ_SLCK_XTAL			(32768UL)
#define BOARD_FREQ_SLCK_BYPASS			(32768UL)

#define BOARD_TWI						TWI0
#define BOARD_TWI_ID					ID_TWI0

#endif // CONF_BOARD_H
