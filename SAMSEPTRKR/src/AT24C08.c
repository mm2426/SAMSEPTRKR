/*
 * AT24C08.c
 *
 * Created: 16-Apr-18 12:26:34 PM
 *  Author: Mahesh
 */ 

 #include "AT24C08.h"

 #ifndef AT24C08_USE_RTOS_API
	 void WriteEEPROM(Twi * Port, uint8_t chipAddr, uint8_t memPage, uint8_t *dPkt, uint16_t dLen)
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
		 twi_master_write(Port, &pkt);
	 }

	 void ReadEEPROM(Twi * Port, uint8_t chipAddr, uint8_t memPage, uint8_t *dPkt, uint16_t dLen)
	 {
		 uint32_t status;
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
		 status = twi_master_read(Port, &pkt);
	 }
#endif