/*
 * rs485pdc.c
 *
 * Created: 15-Oct-17 11:36:37 AM
 *  Author: Mahesh
 */ 

 #include "rs485pdc.h"

 /* PDC data packet. */
 pdc_packet_t rs485PdcPkt;
 /* Pointer to PDC register base. */
 Pdc *rs485PdcBase;

 /* This variable is modified in other files to match the required baudrate */
 uint32_t rs485BaudRate = 9600;
 /* This variable is modified in other files to match the required parity */
 uint32_t rs485Parity = US_MR_PAR_NO;
 

 void InitRs485Pdc(void)
 {
	 sam_usart_opt_t usart_console_settings = {
		 rs485BaudRate,
		 US_MR_CHRL_8_BIT,
		 rs485Parity,
		 US_MR_NBSTOP_1_BIT,
		 US_MR_CHMODE_NORMAL,
		 /* This field is only used in IrDA mode. */
		 0
	 };

	 /* Enable the peripheral clock in the PMC. */
	 sysclk_enable_peripheral_clock(RS485_USART_ID);

	 /* Configure USART in RS485 mode. */
	 usart_init_rs485(RS485_USART, &usart_console_settings,
	 sysclk_get_peripheral_hz());


	 /* Enable TX & RX function. */
	 usart_enable_tx(RS485_USART);
	 usart_enable_rx(RS485_USART);
 	 
	 /* Get board USART PDC base address and enable receiver and transmitter. */
	 rs485PdcBase = usart_get_pdc_base(RS485_USART);
	 pdc_enable_transfer(rs485PdcBase, PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN);

 }

 /* Set pointers and start transmitting */
 void Rs485PdcStartTx(uint32_t buff, uint16_t nBytes)
 {
	rs485PdcPkt.ul_addr = buff;
	rs485PdcPkt.ul_size = nBytes;
	pdc_tx_init(rs485PdcBase, &rs485PdcPkt, NULL);
 }

 /* Set pointers and start receiving */
 void Rs485PdcStartRx(uint32_t buff, uint16_t nBytes)
 {
	rs485PdcPkt.ul_addr = buff;
	rs485PdcPkt.ul_size = nBytes;
	pdc_rx_init(rs485PdcBase, &rs485PdcPkt, NULL);
 }
 
 /* Returns number of bytes in Rx buffer */
 uint32_t Rs485PdcGetRxBytes(void)
 {
	uint32_t recvdBytes = RS485_BUFFER_SIZE;
	recvdBytes -= pdc_read_rx_counter(rs485PdcBase);
	if(recvdBytes>RS485_BUFFER_SIZE)//If overflow occurred. 
	{
		recvdBytes = 0;
	}
	return recvdBytes;
 }

 /* Ret Val = 1 If Rx complete int received
	Else Returns 0 */
 uint8_t Rs485PdcGetRxStatus(void)
 {
	return (usart_get_status(RS485_USART)&US_CSR_ENDRX);
 }

 /* Returns 1 if Tx Buffer is empty else 0 */
 uint32_t Rs485PdcGetTxStatus(void)
 {
	return (usart_get_status(RS485_USART)&US_CSR_ENDTX);
 }
