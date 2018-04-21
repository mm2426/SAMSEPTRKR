/*
 * AT24C08.c
 *
 * Created: 16-Apr-18 12:26:34 PM
 *  Author: Mahesh
 */ 

 #include "AT24C08.h"

 #ifndef AT24C08_USE_RTOS_API
	 /*
	  * 2 LSBs of chipAddr (A1,A0) are the memory addresses (A9, A8).
	  * Write operation takes place in pages of 16Bytes only. Any data more than that is wrapped around.
	  */
	 void WriteEEPROM(Twi * Port, uint8_t chipAddr, uint16_t memAddr, uint8_t *dPkt, uint8_t dLen)
	 {
		 twi_packet_t pkt;
		 /* Set Device Address */
		 pkt.chip = (chipAddr&0xFC);
		 pkt.chip |= ((memAddr>>8)&0x03);
		 /* Page Number */
		 pkt.addr[0] = memAddr;
		 /* No Address Bytes to be clocked */
		 pkt.addr_length = 1;
		 /* Address of buffer where recvd data is to be stored */
		 pkt.buffer = dPkt;
		 /* No of bytes to read */
		 pkt.length = dLen;
		 twi_master_write(Port, &pkt);
		 delay_ms(5);
	 }

	 /*
	  * 2 LSBs of chipAddr (A1,A0) are the memory addresses (A9, A8).
	  * Read operation does not wrap around on page boundaries.
	  */
	 void ReadEEPROM(Twi * Port, uint8_t chipAddr, uint16_t memAddr, uint8_t *dPkt, uint16_t dLen)
	 {
		 twi_packet_t pkt;
		 /* Set Device Address */
		 pkt.chip = (chipAddr&0xFC);
		 pkt.chip |= ((memAddr>>8)&0x03);
		 /* Page Number */
		 pkt.addr[0] = memAddr;
		 /* No Address Bytes to be clocked */
		 pkt.addr_length = 1;
		 /* Address of buffer where recvd data is to be stored */
		 pkt.buffer = dPkt;
		 /* No of bytes to read */
		 pkt.length = dLen;
		 twi_master_read(Port, &pkt);
	 }
#endif