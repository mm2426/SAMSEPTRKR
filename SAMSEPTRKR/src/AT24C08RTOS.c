/*
 * AT24C08.c
 *
 * Created: 16-Apr-18 11:59:02 AM
 *  Author: Mahesh
 */ 

#include "AT24C08RTOS.h"

#if defined(AT24C08_USE_RTOS_API)
	void WriteEEPROM(freertos_twi_if port, uint8_t chipAddr, uint8_t memPage, uint8_t *dPkt, uint16_t dLen, int timeOut)
	{
		twi_packet_t pkt;
		/* Set Device Address */
		pkt.chip = chipAddr;
		/* Page Number */
		pkt.addr[0] = memPage;
		/* No Address Bytes to be clocked */
		pkt.addr_length = 1;
		/* Address of buffer where recvd data is to be stored */
		pkt.buffer = dPkt;
		/* No of bytes to read */
		pkt.length = dLen;
		//twi_master_write(Port, &pkt);
		freertos_twi_write_packet(port, &pkt, timeOut);
	}

	void ReadEEPROM(freertos_twi_if port, uint8_t chipAddr, uint8_t memPage, uint8_t *dPkt, uint16_t dLen, int timeOut)
	{
		twi_packet_t pkt;
		/* Set Device Address */
		pkt.chip = chipAddr;
		/* Page Number */
		pkt.addr[0] = memPage;
		/* No Address Bytes to be clocked */
		pkt.addr_length = 1;
		/* Address of buffer where recvd data is to be stored */
		pkt.buffer = dPkt;
		/* No of bytes to read */
		pkt.length = dLen;
		//twi_master_read(Port, &pkt);
		freertos_twi_write_packet(port, &pkt, timeOut);
	}
#endif