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
extern uint16_t mBusRegs[MBUS_MAX_REGS];
extern Pdc *rs485PdcBase;
pdc_packet_t pdcPkt;

//MODBUS Slave Address
uint8_t slaveAddr = 1;

//Pv tracker variables
extern float lat, lon;
extern float timeZone, dist, width;
//Panels will track +/-pvAngleRng degrees. 
extern float pvAngleRng;
extern float bkTrkParam1, bkTrkParam2;
volatile extern uint8_t minCtr;

extern freertos_twi_if twiPort;

void WriteMbusRegs(uint16_t *mbusBuff, uint8_t regAddr, uint8_t len);
void SendRespPkt(uint8_t *pkt, uint8_t len);

void vCommTask(void *pvParameters)
{
    
	/* RS485 Rx buffer */
	uint8_t rs485RxBuffer[RS485_BUFFER_SIZE];
	/* RS485 Tx buffer */
    uint8_t respBuff[RS485_BUFFER_SIZE], respLen = 0;
    uint8_t commState = 0, recvdBytes = 0, prevRecvdBytes;
    /* Init Mbus writeFunc() ptr */
    writeFunc = WriteMbusRegs;
	
	/* Start Receiving Data */
	usart_disable_tx(RS485_USART);
	usart_enable_rx(RS485_USART);
	Rs485PdcStartRx((uint32_t)rs485RxBuffer,RS485_BUFFER_SIZE);

    while(1)
    {
        /* Sleep for some time and check if data recvd */
        vTaskDelay(40 / portTICK_RATE_MS);
        
		recvdBytes = Rs485PdcGetRxBytes();
		/* Check if data received on 485 */
		switch(commState)
		{
			case 0:
				if(recvdBytes!=0)
				{
					commState = 1;
					prevRecvdBytes = recvdBytes;
				}
				break;
			case 1:
				if(prevRecvdBytes != recvdBytes)
				{
					prevRecvdBytes = recvdBytes;
				}
				else
				{
					/* 1 Modbus Pkt received. Process it. */
					usart_disable_rx(RS485_USART);
					pdc_disable_transfer(rs485PdcBase, PERIPH_PTCR_RXTEN);

					/* Parse Received MBUS Pkt, update actual variables */
					ParseMbusPkt(slaveAddr, rs485RxBuffer, recvdBytes, respBuff, &respLen);
				
					/* Generate MBus Resp */
					if(respLen)
					{
						SendRespPkt(respBuff, respLen);
					}
					
					memset((uint8_t *)rs485RxBuffer,0,sizeof(rs485RxBuffer));
					memset(respBuff,0,sizeof(respBuff));
					respLen = 0;
					commState = 0;
					
					usart_enable_rx(RS485_USART);
					pdc_enable_transfer(rs485PdcBase, PERIPH_PTCR_RXTEN);
					Rs485PdcStartRx((uint32_t)rs485RxBuffer,RS485_BUFFER_SIZE);
				}
				break;
		}
    }
}

void vCommInit(void)
{
    /* Init RS485 port with PDC support */
    InitRs485Pdc();
}

void WriteMbusRegs(uint16_t *mbusBuff, uint8_t regAddr, uint8_t len)
{
    uint16_t *ptr;
	uint8_t *ptr8;
    
    while(len)
    {
        switch(regAddr)
        {
            case MBUS_REG_SEC:
				#ifndef DS3231_USE_RTOS_API
					DSSetSecs(BOARD_TWI, mbusBuff[regAddr]);
				#else
					DSSetSecsTo(twiPort, mbusBuff[regAddr],50);
				#endif
                break;
            case MBUS_REG_MIN:
				#ifndef DS3231_USE_RTOS_API
					DSSetMins(BOARD_TWI, mbusBuff[regAddr]);
				#else
					DSSetMinsTo(twiPort, mbusBuff[regAddr],50);
				#endif
                break;
            case MBUS_REG_HRS:
				#ifndef DS3231_USE_RTOS_API
					DSSetHrs(BOARD_TWI, mbusBuff[regAddr]);
				#else
					DSSetHrsTo(twiPort, mbusBuff[regAddr],50);
				#endif
                break;
            case MBUS_REG_DAY:
				#ifndef DS3231_USE_RTOS_API
					DSSetDay(BOARD_TWI, mbusBuff[regAddr]);
				#else
					DSSetDayTo(twiPort, mbusBuff[regAddr],50);
				#endif
                break;
            case MBUS_REG_DD:
				#ifndef DS3231_USE_RTOS_API
					DSSetDate(BOARD_TWI, mbusBuff[regAddr]);
				#else
					DSSetDateTo(twiPort, mbusBuff[regAddr],50);
				#endif
                break;
            case MBUS_REG_MM:
				#ifndef DS3231_USE_RTOS_API
					DSSetMonth(BOARD_TWI, mbusBuff[regAddr]);
				#else
					DSSetMonthTo(twiPort, mbusBuff[regAddr],50);
				#endif
                break;
            case MBUS_REG_YY:
				#ifndef DS3231_USE_RTOS_API
					DSSetYear(BOARD_TWI, mbusBuff[regAddr]);
				#else
					DSSetYearTo(twiPort, mbusBuff[regAddr],50);
				#endif
                break;
            case MBUS_REG_LATH:
                ptr = (uint16_t*)&lat;
                ptr[1] = mBusRegs[MBUS_REG_LATH];
                ptr[0] = mBusRegs[MBUS_REG_LATL];
				/* Update EEPROM */
				ptr8 = (uint8_t *)&lat;
				WriteEEPROM(BOARD_TWI, AT24C08_ADDR, EE_REG_LAT0, ptr8, 4);
                break;
            case MBUS_REG_LONH:
                ptr = (uint16_t*)&lon;
                ptr[1] = mBusRegs[MBUS_REG_LONH];
                ptr[0] = mBusRegs[MBUS_REG_LONL];
				/* Update EEPROM */
                ptr8 = (uint8_t *)&lon;
                WriteEEPROM(BOARD_TWI, AT24C08_ADDR, EE_REG_LON0, ptr8, 4);
                break;
            case MBUS_REG_TZH:
                ptr = (uint16_t*)&timeZone;
                ptr[1] = mBusRegs[MBUS_REG_TZH];
                ptr[0] = mBusRegs[MBUS_REG_TZL];
                /* Update EEPROM */
				ptr8 = (uint8_t *)&timeZone;
				WriteEEPROM(BOARD_TWI, AT24C08_ADDR, EE_REG_TZ0, ptr8, 4);
                break;
            case MBUS_REG_PNLRNGH:
                ptr = (uint16_t*)&pvAngleRng;
                ptr[1] = mBusRegs[MBUS_REG_PNLRNGH];
                ptr[0] = mBusRegs[MBUS_REG_PNLRNGL];
                /* Update EEPROM */
				ptr8 = (uint8_t *)&pvAngleRng;
				WriteEEPROM(BOARD_TWI, AT24C08_ADDR, EE_REG_PNLRNG0, ptr8, 4);
                break;
            case MBUS_REG_DISTH:
                ptr = (uint16_t*)&dist;
                ptr[1] = mBusRegs[MBUS_REG_DISTH];
                ptr[0] = mBusRegs[MBUS_REG_DISTL];
                /* Update EEPROM */
				ptr8 = (uint8_t *)&dist;
				WriteEEPROM(BOARD_TWI, AT24C08_ADDR, EE_REG_DIST0, ptr8, 4);
                break;
            case MBUS_REG_WIDTHH:
                ptr = (uint16_t*)&width;
                ptr[1] = mBusRegs[MBUS_REG_WIDTHH];
                ptr[0] = mBusRegs[MBUS_REG_WIDTHL];
                /* Update EEPROM */
				ptr8 = (uint8_t *)&width;
                WriteEEPROM(BOARD_TWI, AT24C08_ADDR, EE_REG_WIDTH0, ptr8, 4);
                break;
			case MBUS_REG_BKPARAM1H:
				ptr = (uint16_t*)&bkTrkParam1;
				ptr[1] = mBusRegs[MBUS_REG_BKPARAM1H];
				ptr[0] = mBusRegs[MBUS_REG_BKPARAM1L];
				/* Update EEPROM */
				ptr8 = (uint8_t *)&bkTrkParam1;
				WriteEEPROM(BOARD_TWI, AT24C08_ADDR, EE_REG_BKPARAM10, ptr8, 4);
				break;
			case MBUS_REG_BKPARAM2H:
				ptr = (uint16_t*)&bkTrkParam2;
				ptr[1] = mBusRegs[MBUS_REG_BKPARAM2H];
				ptr[0] = mBusRegs[MBUS_REG_BKPARAM2L];
				/* Update EEPROM */
				ptr8 = (uint8_t *)&bkTrkParam2;
				WriteEEPROM(BOARD_TWI, AT24C08_ADDR, EE_REG_BKPARAM20, ptr8, 4);
				break;
            case MBUS_REG_OPMODE:
                mBusRegs[MBUS_REG_MOTON] = 0;
                minCtr = 0;
                /* Turn Motor Off */
				#ifndef MOTOR_CTRL_A4955
					gpio_set_pin_low(PIN_MOTOR_RST_IDX);
				#else
					gpio_set_pin_low(PIN_MOTOR_SLP_IDX);
					gpio_set_pin_low(PIN_MOTOR_IN1_IDX);
					gpio_set_pin_low(PIN_MOTOR_IN2_IDX);
				#endif
                break;
            default:
                break;
        }    
        regAddr++;
        len--;
    }
}

void SendRespPkt(uint8_t *pkt, uint8_t len)
{
	usart_enable_tx(RS485_USART);
	/* 3.5 Char Wait Time */
	
	/* Start PDC Transfer */
	Rs485PdcStartTx((uint32_t)pkt, len);

	/* Wait till tx completes */
	while(!Rs485PdcGetTxStatus())
		vTaskDelay(10 / portTICK_RATE_MS);

	/* 3.5 Char Wait Time */

	usart_disable_tx(RS485_USART);
}


/* [] END OF FILE */
