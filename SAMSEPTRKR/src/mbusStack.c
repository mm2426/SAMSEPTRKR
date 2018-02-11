/*
 * mbusStack.c
 *
 * Created: 03-Nov-17 10:13:05 PM
 *  Author: Mahesh
 */ 

 #include "mbusStack.h"

//MODBUS Register Values
uint16_t mBusRegs[MBUS_MAX_REGS]={};
 
//Write function pointer
void (*writeFunc)(uint16_t *mbusBuff, uint8_t regAddr, uint8_t len);

/* Modbus stack parser function */
void ParseMbusPkt(uint8_t slaveAddr, uint8_t *buff, uint8_t len, uint8_t *respBuff, uint8_t *respLen)
{
	 uint16_t crc = 0, recvCrc = 0;
	 uint16_t regAddr = 0, numReg = 0;
	 uint16_t regVal = 0;
	 uint8_t fCode = 0, genErrRsp = 0;
	 uint8_t expFuncCode = 0, expCode = 0;
	 uint8_t i = 0, ind = 0;
	 
	 /* Self Addr recvd or Broadcast Addr = 0 recvd */
	 if(buff[0]==slaveAddr||buff[0]==0)
	 {
		 /* Calc CRC */
		 recvCrc = (((uint16_t)buff[len-1])<<8)|buff[len-2];
		 crc = calcCRC(buff,len-2);
		 /* If CRC Matched */
		 if(crc==recvCrc)
		 {
			 /* Function Code Extraction */
			 fCode = buff[1];
			 switch(fCode)
			 {
				 /* Read Analog Reg Func Code Recvd */
				 case 0x03:
				 numReg = (((uint16_t)buff[4])<<8)|buff[5];
				 /* 0x0001<= If Qty of Regs <= 0x007D */
				 if(numReg>=0x0001&&numReg<=0x007D)
				 {
					 regAddr = (((uint16_t)buff[2])<<8)|(buff[3]);
					 /* Starting Addr == OK && Starting Addr + Qty of Regs == OK */
					 if((regAddr+numReg) <= MBUS_MAX_REGS)
					 {
						 /* Process Read Reg Req */
						 /* Gen Valid Resp */
						 respBuff[0] = slaveAddr;
						 respBuff[1] = 0x03;
						 respBuff[2] = (numReg *2); //No of Data Bytes
						 
						 i = 3;
						 for(ind = 0; ind < numReg; ind++)
						 {
							 respBuff[i++] = (mBusRegs[regAddr+ind]>>8);
							 respBuff[i++] = (mBusRegs[regAddr+ind]&0x00FF);
						 }
						 crc = calcCRC(respBuff, i);
						 respBuff[i++] = (crc&0x00FF);
						 respBuff[i++] = (crc>>8);
						 *respLen = i;
					 }
					 else
					 {
						 /* Error code = 0x02 */
						 genErrRsp = 1;
						 expFuncCode = 0x83;
						 expCode = 0x02;
					 }
				 }
				 else
				 {
					 /* Error Code 0x03 */
					 genErrRsp = 1;
					 expFuncCode = 0x83;
					 expCode = 0x03;
				 }
				 break;
				 /* Write Single Analog Reg */
				 case 0x06:
				 /* Output Value = 0x0000 or 0xFF00 */
				 regVal = (((uint16_t)buff[4])<<8)|(buff[5]);
				 if(1)
				 {
					 /* Output Address = OK */
					 regAddr = (((uint16_t)buff[2])<<8)|(buff[3]);
					 if(regAddr <= MBUS_MAX_REGS)
					 {
						 /* Gen Valid Mbus Resp */
						 mBusRegs[regAddr] = regVal;
						 /* Success Error Code */
						 respBuff[0] = slaveAddr;
						 respBuff[1] = 0x06;
						 respBuff[2] = buff[2];
						 respBuff[3] = buff[3];
						 respBuff[4] = buff[4];
						 respBuff[5] = buff[5];
						 crc = calcCRC(respBuff, 6);
						 respBuff[6] = (crc&0x00FF);
						 respBuff[7] = (crc>>8);
						 *respLen = 8;
						 
						 /* Process Write Reg Req */
						 if(writeFunc!=NULL)
							writeFunc(mBusRegs,regAddr,1);
					 }
					 else
					 {
						 /* Illegal Data Address Error */
						 genErrRsp = 1;
						 expFuncCode = 0x86;
						 expCode = 0x02;
					 }
				 }
				 else
				 {
					 /* Illegal Data Value Error */
					 genErrRsp = 1;
					 expFuncCode = 0x86;
					 expCode = 0x03;
				 }
				 break;
				 case 0x10: /* Write Mul Register */
				 numReg = (((uint16_t)buff[4])<<8)|buff[5];
				 /* 0x0001<= If Qty of Regs <= 0x007D */
				 if(numReg>=0x0001&&numReg<=0x007D)
				 {
					 regAddr = (((uint16_t)buff[2])<<8)|(buff[3]);
					 /* Starting Addr == OK && Starting Addr + Qty of Regs == OK */
					 if((regAddr+numReg) <= MBUS_MAX_REGS)
					 {
						 /* Gen Valid Resp */
						 respBuff[0] = slaveAddr;
						 respBuff[1] = 0x10;
						 respBuff[2] = buff[2];
						 respBuff[3] = buff[3];
						 respBuff[4] = buff[4];
						 respBuff[5] = buff[5];
						 crc = calcCRC(respBuff, 6);
						 respBuff[6] = (crc&0x00FF);
						 respBuff[7] = (crc>>8);
						 *respLen = 8;
						 
						 /* Byte count */
						 numReg = 7 + buff[6];
						 ind = 0;
						 for(i = 7; i < numReg; i++)
						 {
							 if(i%2==1)
							 {
								 regVal = ((uint16_t)buff[i])<<8;
							 }
							 else
							 {
								 regVal |= buff[i];
								 mBusRegs[regAddr+ind] = regVal;
								 ind ++;
							 }
						 }
						 /* Process Write Reg Req */
						 writeFunc(mBusRegs,regAddr,ind);
					 }
					 else
					 {
						 /* Error code = 0x02 */
						 genErrRsp = 1;
						 expFuncCode = 0x90;
						 expCode = 0x02;
					 }
				 }
				 else
				 {
					 /* Error Code 0x03 */
					 genErrRsp = 1;
					 expFuncCode = 0x90;
					 expCode = 0x03;
				 }
				 break;
				 default:
				 /* Illegal Function Code Error */
				 /* Set MSB of recvd Function code */
				 /* Error Code 0x01 */
				 expFuncCode = fCode|(1<<7);
				 expCode = 0x01;
				 genErrRsp = 1;
				 break;
			 }
		 }
	 }
	 else
	 {
		 *respLen = 0;
	 }
	 
	 if(genErrRsp)
	 {
		 respBuff[0] = slaveAddr;
		 respBuff[1] = expFuncCode;
		 respBuff[2] = expCode;
		 crc = calcCRC(respBuff, 3);
		 respBuff[3] = (crc&0x00FF);
		 respBuff[4] = (crc>>8);
		 *respLen = 5;
	 }
 }

 uint16_t calcCRC(uint8_t *buff, uint8_t len)
 {
	 uint16_t crcReg = 0xFFFF;
	 uint8_t i = 0, j=0;

	 for(i=0;i<len;i++)
	 {
		 crcReg = crcReg ^ buff[i];
		 for(j=0;j<8;j++)
		 {

			 if(bitIsSet(crcReg,0))
			 {
				 crcReg = crcReg >> 1;
				 crcReg = crcReg ^ MBUS_CRC_POLY;
			 }
			 else
			 {
				 crcReg = crcReg >> 1;
			 }
		 }
	 }
	 return crcReg;
 }

