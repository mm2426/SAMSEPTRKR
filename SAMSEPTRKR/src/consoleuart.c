/*
 * consoleuart.c
 *
 * Created: 21-Apr-18 3:33:08 PM
 *  Author: Mahesh
 */ 

 #include "consoleuart.h"

 /* Pointer to Console UART PDC register base */
 Pdc *cnslUartPdcBase;
 pdc_packet_t cnslPdcPkt;

 void ConsoleInit(void)
 {
	const usart_serial_options_t settings = {
		.baudrate = CNSL_BAUDRATE,
		.paritytype = UART_MR_PAR_NO
	};
	
	sysclk_enable_peripheral_clock(CNSL_UART_ID);
	usart_serial_init((Usart *)CNSL_UART,(usart_serial_options_t *)&settings);
	
	uart_enable_tx(CNSL_UART);
	uart_disable_rx(CNSL_UART);

	cnslUartPdcBase = uart_get_pdc_base(CNSL_UART);
	pdc_enable_transfer(cnslUartPdcBase, PERIPH_PTCR_TXTEN);
 }
 
 void ConsoleWrite(uint8_t *buff, uint16_t dataLen)
 {
	/* Initiate PDC Transfer for dataLen bytes */
	cnslPdcPkt.ul_addr = (uint32_t) buff;
	cnslPdcPkt.ul_size = dataLen;
	pdc_tx_init(cnslUartPdcBase, &cnslPdcPkt, NULL);
	/* If previous transfer not complete, wait */
	while (!(uart_get_status(CNSL_UART) & UART_SR_ENDTX))
	{
		#ifndef __FREERTOS__
			delay_ms(1);
		#else
			vTaskDelay(1 / portTICK_RATE_MS);
		#endif
	}
 }