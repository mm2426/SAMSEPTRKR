/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "CommInterface.h"
#include "PvTracker.h"

//MODBUS Register Values
uint16 mBusRegs[MBUS_MAX_REGS]={};
//MODBUS Slave Address
uint8 slaveAddr = 0x01;
//uint8 clrMbusBuff = 0;
//MODBUS Stack control variables
volatile rxstates uart0State = idle;
volatile uint8 uartRxData = 0;
volatile uint8 uartBuff[UART_BUFF_SIZE_MAX];
volatile uint8 buffInd = 0;

//Pv tracker variables
extern float lat, lon;
extern float timeZone, dist, width;
//Panels will track +/-pvAngleRng degrees. 
extern float pvAngleRng;

extern uint8 minCtr;

CY_ISR(Uart0RxIsr)
{
    ISRUart0Rx_ClearPending();
    //Read Uart data byte 
    uartRxData = UART0_GetByte();
    switch(uart0State)
    {
        case idle:
            uartBuff[0] = uartRxData;
            buffInd = 1;
            uart0State = dRx;
            //Timer0_Start();
            break;
        case dRx:
            uartBuff[buffInd++] = uartRxData;
            if(buffInd == UART_BUFF_SIZE_MAX)
            {
                buffInd = 0;
            }
            //Timer0_WriteCounter(0);
            break;
    }
}

CY_ISR(Timer0Isr)
{
    Timer0_ReadStatusRegister();
    ISRTimer0_ClearPending();
    Timer0_Stop();
    /*Disable Uart0 Interrupt*/
    ISRUart0Rx_Disable();
}

void vCommTask(void *pvParameters)
{
    uint8 respBuff[UART_BUFF_SIZE_MAX], respLen = 0;
    uint8 commState = 0;
    uint8 prevInd = 0;
    
    while(1)
    {
        /* Sleep for some time and chk if data recvd */
        vTaskDelay(pdMS_TO_TICKS(40));
        //if(!clrMbusBuff)
        {
            switch(commState)
            {
                case 0:
                    if(buffInd!=0)
                    {
                        commState = 1;
                        prevInd = buffInd;
                    }
                    break;
                case 1:
                    if(prevInd != buffInd)
                    {
                        prevInd = buffInd;
                    }
                    else
                    {
                        ISRUart0Rx_Disable();
                        
                        /* Parse Received MBUS Pkt, update actual variables */
                        parseMbusPkt((uint8*)uartBuff, (uint8)buffInd, respBuff, &respLen);
                        
                        /* Generate MBus Resp */
                        if(respLen)
                        {
                            sendRespPkt(respBuff, respLen);
                        }
                        
                        memset((uint8 *)uartBuff,0,sizeof(uartBuff));
                        memset(respBuff,0,sizeof(respBuff));
                        buffInd = 0;
                        prevInd = 0;
                        respLen = 0;
                        uart0State = idle;
                        UART0_ClearRxBuffer();
                        while(UART0_GetTxBufferSize());
                        ISRUart0Rx_Enable();
                        //clrMbusBuff = 0;
                        commState = 0;
                        /*buffInd = 0;
                        prevInd = 0;
                        uart0State = idle;
                        ISRUart0Rx_Enable();
                        commState = 0;*/
                    }
                    break;
            }
        }
        /*else
        {
            memset((uint8 *)uartBuff,0,sizeof(uartBuff));
            memset(respBuff,0,sizeof(respBuff));
            buffInd = 0;
            prevInd = 0;
            respLen = 0;
            uart0State = idle;
            UART0_ClearRxBuffer();
            UART0_ClearTxBuffer();
            ISRUart0Rx_Enable();
            clrMbusBuff = 0;
            commState = 0;
        }*/
    }
}

void vCommInit(void)
{
    /* Enable UART0 */
    UART0_Start();
    
    /* Init and Enable Timer & Rx Int */
    ISRTimer0_StartEx(Timer0Isr);
    ISRUart0Rx_StartEx(Uart0RxIsr);
    
}

void parseMbusPkt(uint8 *buff, uint8 len, uint8 *respBuff, uint8 *respLen)
{
    uint16 crc = 0, recvCrc = 0;
    uint16 regAddr = 0, numReg = 0;
    uint16 regVal = 0;
    uint8 fCode = 0, genErrRsp = 0;
    uint8 expFuncCode = 0, expCode = 0;
    uint8 i = 0, ind = 0;
    
    /* Self Addr recvd or Broadcast Addr = 0 recvd */
    if(buff[0]==slaveAddr||buff[0]==0)
    {
        /* Calc CRC */
        recvCrc = (((uint16)buff[len-1])<<8)|buff[len-2];
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
                    numReg = (((uint16)buff[4])<<8)|buff[5];
                    /* 0x0001<= If Qty of Regs <= 0x007D */
                    if(numReg>=0x0001&&numReg<=0x007D)
                    {
                        regAddr = (((uint16)buff[2])<<8)|(buff[3]);
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
                    regVal = (((uint16)buff[4])<<8)|(buff[5]);
                    if(1)
                    {
                        /* Output Address = OK */
                        regAddr = (((uint16)buff[2])<<8)|(buff[3]);
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
                            writeMbusRegs(mBusRegs,regAddr,1);
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
                    numReg = (((uint16)buff[4])<<8)|buff[5];
                    /* 0x0001<= If Qty of Regs <= 0x007D */
                    if(numReg>=0x0001&&numReg<=0x007D)
                    {
                        regAddr = (((uint16)buff[2])<<8)|(buff[3]);
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
                                    regVal = ((uint16)buff[i])<<8;
                                }
                                else
                                {
                                    regVal |= buff[i];
                                    mBusRegs[regAddr+ind] = regVal;
                                    ind ++;
                                }
                            }
                            /* Process Write Reg Req */
                            writeMbusRegs(mBusRegs,regAddr,ind);
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

void writeMbusRegs(uint16 *mbusBuff, uint8 regAddr, uint8 len)
{
    uint16 *ptr;
    
    while(len)
    {
        switch(regAddr)
        {
            case MBUS_REG_SEC:
                DSSetSecsTo(mbusBuff[regAddr],50);
                break;
            case MBUS_REG_MIN:
                DSSetMinsTo(mbusBuff[regAddr],50);
                break;
            case MBUS_REG_HRS:
                DSSetHrsTo(mbusBuff[regAddr],50);
                break;
            case MBUS_REG_DAY:
                DSSetDayTo(mbusBuff[regAddr],50);
                break;
            case MBUS_REG_DD:
                DSSetDateTo(mbusBuff[regAddr],50);
                break;
            case MBUS_REG_MM:
                DSSetMonthTo(mbusBuff[regAddr],50);
                break;
            case MBUS_REG_YY:
                DSSetYearTo(mbusBuff[regAddr],50);
                break;
            case MBUS_REG_LATH:
                ptr = (uint16*)&lat;
                ptr[1] = mBusRegs[MBUS_REG_LATH];
                ptr[0] = mBusRegs[MBUS_REG_LATL];
                //Write to EEPROM
                EEPROM0_UpdateTemperature();
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_LATL], EE_REG_LAT0);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_LATL]>>8), EE_REG_LAT1);
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_LATH], EE_REG_LAT2);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_LATH]>>8), EE_REG_LAT3);
                break;
            case MBUS_REG_LONH:
                ptr = (uint16*)&lon;
                ptr[1] = mBusRegs[MBUS_REG_LONH];
                ptr[0] = mBusRegs[MBUS_REG_LONL];
                //Write to EEPROM
                EEPROM0_UpdateTemperature();
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_LONL], EE_REG_LON0);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_LONL]>>8), EE_REG_LON1);
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_LONH], EE_REG_LON2);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_LONH]>>8), EE_REG_LON3);
                break;
            case MBUS_REG_TZH:
                ptr = (uint16*)&timeZone;
                ptr[1] = mBusRegs[MBUS_REG_TZH];
                ptr[0] = mBusRegs[MBUS_REG_TZL];
                //Write to EEPROM
                EEPROM0_UpdateTemperature();
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_TZL], EE_REG_TZ0);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_TZL]>>8), EE_REG_TZ1);
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_TZH], EE_REG_TZ2);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_TZH]>>8), EE_REG_TZ3);
                break;
            case MBUS_REG_REFRH:
                break;
            case MBUS_REG_PNLRNGH:
                ptr = (uint16*)&pvAngleRng;
                ptr[1] = mBusRegs[MBUS_REG_PNLRNGH];
                ptr[0] = mBusRegs[MBUS_REG_PNLRNGL];
                //Write to EEPROM
                EEPROM0_UpdateTemperature();
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_PNLRNGL], EE_REG_PNLRNG0);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_PNLRNGL]>>8), EE_REG_PNLRNG1);
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_PNLRNGH], EE_REG_PNLRNG2);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_PNLRNGH]>>8), EE_REG_PNLRNG3);
                break;
            case MBUS_REG_DISTH:
                ptr = (uint16*)&dist;
                ptr[1] = mBusRegs[MBUS_REG_DISTH];
                ptr[0] = mBusRegs[MBUS_REG_DISTL];
                //Write to EEPROM
                EEPROM0_UpdateTemperature();
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_DISTL], EE_REG_DIST0);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_DISTL]>>8), EE_REG_DIST1);
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_DISTH], EE_REG_DIST2);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_DISTH]>>8), EE_REG_DIST3);
                break;
            case MBUS_REG_WIDTHH:
                ptr = (uint16*)&width;
                ptr[1] = mBusRegs[MBUS_REG_WIDTHH];
                ptr[0] = mBusRegs[MBUS_REG_WIDTHL];
                //Write to EEPROM
                EEPROM0_UpdateTemperature();
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_WIDTHL], EE_REG_WIDTH0);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_WIDTHL]>>8), EE_REG_WIDTH1);
                EEPROM0_WriteByte(mBusRegs[MBUS_REG_WIDTHH], EE_REG_WIDTH2);
                EEPROM0_WriteByte((mBusRegs[MBUS_REG_WIDTHH]>>8), EE_REG_WIDTH3);
                break;
            case MBUS_REG_OPMODE:
                mBusRegs[MBUS_REG_MOTON] = 0;
                minCtr = 0;
                //Turn Motor Off
                HB_RST_Write(0);
                PWM0_Stop();
                break;
            default:
                break;
        }    
        regAddr++;
        len--;
    }
}

uint16 calcCRC(uint8 *buff, uint8 len)
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

void sendRespPkt(uint8 *pkt, uint8 len)
{
    //3.5 Char Wait Time using Timer.
    //Transmit Data
    UART0_PutArray(pkt,len);
    //3.5 Char Wait Time using Timer.
}

/* [] END OF FILE */
