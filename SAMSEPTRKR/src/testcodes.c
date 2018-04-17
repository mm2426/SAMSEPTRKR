/*
 * testcodes.c
 *
 * Created: 17-Apr-18 4:15:29 PM
 *  Author: Mahesh
 */ 


 /************************** EEPROM TEST NON RTOS START *********************************/
 uint8_t buffTemp[4] = {0xAB, 0xCD, 0xEF, 0xBA};
 uint8_t buffRead[4] = {};

 WriteEEPROM(BOARD_TWI, 0x50, 0x12, buffTemp, 4);

 while(1)
 {
	 memset(buffRead, 0x00, sizeof(buffRead));
	 ReadEEPROM(BOARD_TWI, 0x50, 0x12, buffRead, 4);
	 if(memcmp_ram2ram(buffTemp, buffRead, 4))
	 {
		 gpio_set_pin_high(PIN_DEBUGLED_IDX);
	 }
	 else
	 {
		 gpio_set_pin_low(PIN_DEBUGLED_IDX);
	 }
	 delay_ms(200);
 }
 /************************** EEPROM TEST NON RTOS END *********************************/

 /************************** RTC TEST NON RTOS START *********************************/
 mBusRegs[MBUS_REG_SEC] = 0x45;
 mBusRegs[MBUS_REG_MIN] = 0x03;
 mBusRegs[MBUS_REG_HRS] = 0x08;
 DSSetTime(BOARD_TWI, (uint16_t *)&mBusRegs[MBUS_REG_SEC]);

 /* Init RTC 1 min Alarm */
 #ifndef DS3231_USE_RTOS_API
 //DSEnAL2(BOARD_TWI, 1);
 DSEnAL1(BOARD_TWI, 1);
 #else
 DSEnAL2To(twiPort, 1, 50);
 #endif

 while(1)
 {
	 /* Clear RTC interrupt flag */
	 #ifndef DS3231_USE_RTOS_API
	 DSReadByte(BOARD_TWI, DS_REG_STAT,&status);
	 #else
	 DSReadByteTo(twiPort, DS_REG_STAT,&status, 50);
	 #endif
	 
	 if(status&0x02)
	 {
		 /* Clear Status Reg */
		 #ifndef DS3231_USE_RTOS_API
		 DSWriteByte(BOARD_TWI, DS_REG_STAT, 0x00);
		 #else
		 DSWriteByteTo(twiPort, DS_REG_STAT, 0x00, 100);
		 #endif
	 }
	 #ifndef DS3231_USE_RTOS_API
	 DSGetTime(BOARD_TWI, (uint16_t *)&mBusRegs[MBUS_REG_SEC]);
	 #else
	 DSGetTimeTo(twiPort, (uint16_t *)&mBusRegs[MBUS_REG_SEC], 50);
	 #endif

	 gpio_toggle_pin(PIN_DEBUGLED_IDX);
	 vTaskDelay(200);
 }

 /************************** RTC TEST NON RTOS END *********************************/
 
 /************************** ACCELEROMETER TEST NON RTOS START *********************************/
 int16_t accVals[3] = {};
 float oriVals[3];
 
 /* Init Accelerometer */
 #ifndef ICM20648_USE_RTOS_API
	ICMInitialize(BOARD_TWI, ICM_ADDR);
 #else
	ICMInitializeTo(twiPort, ICM_ADDR, 50);
 #endif

 while(1)
 {
	 #ifndef ICM20648_USE_RTOS_API
	 ICMReadAccDataAll(BOARD_TWI, ICM_ADDR,(uint16_t*)accVals);
	 #else
	 ICMReadAccDataAllTo(twiPort, ICM_ADDR,(uint16_t*)accVals, 50);
	 #endif
	 GetOrientation(accVals, oriVals);
	 vTaskDelay(500);
 }

 /************************** ACCELEROMETER TEST NON RTOS END *********************************/