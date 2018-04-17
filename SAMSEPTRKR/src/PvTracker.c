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

#include "PvTracker.h"

freertos_twi_if twiPort;

//extern uint8_t ccEn, ccStatus;
extern uint16_t mBusRegs[MBUS_MAX_REGS];

//Pv tracker variables
float lat = 17.46608f, lon = 78.44009f;
float timeZone = 5.5f, dist = 5.0f, width = 2.0f;
//Panels will track +/-pvAngleRng degrees. 
float pvAngleRng = 43;
uint8_t bkTrkFlg = 0;
uint8_t minCtr = 0;

static void InitTWI(void);

/* RTC 1 Min ISR */
void RTCIntHandler(uint32_t ul_id, uint32_t ul_mask)
{
    if (PIN_RTC_INT_PIO_ID != ul_id || PIN_RTC_INT_MASK != ul_mask)
		return;
	
    /* If Auto Mode */
    if(!mBusRegs[MBUS_REG_OPMODE])
    {
        /* Increment Time Var */
        if(minCtr < TRACKING_INTERVAL)
            minCtr++;
    }
}

void vPvTrackerTask(void *pvParameters)
{
    uint8_t status = 0;
	uint16_t recvData;
    
    #ifdef LOG_EN
        Debug_PutString("Hrs,Mins,Secs,Tracking Mode,PvAngle,BkAngle\r\n");
    #endif
	
	/* These Init routines are shifted here because they should only be called after the scheduler has started */
	/* Init Accelerometer */
	#ifndef ICM20648_USE_RTOS_API
		ICMInitialize(BOARD_TWI, ICM_ADDR);
	#else
		ICMInitializeTo(twiPort, ICM_ADDR, 50);
	#endif

	/* Init RTC 1 min Alarm */
	#ifndef DS3231_USE_RTOS_API
		DSEnAL2(BOARD_TWI, 1);
	#else
		DSEnAL2To(twiPort, 1, 50);
	#endif

	while(1)
    {
        /* Clear RTC interrupt flag */
		#ifndef DS3231_USE_RTOS_API
			DSReadByte(BOARD_TWI, DS_REG_STAT,&status);
		#else
			DSReadByteTo(twiPort, DS_REG_STAT,&status,50);
		#endif
        if(status&0x02)
        {
            /* Clear Status Reg */
			#ifndef DS3231_USE_RTOS_API
				DSWriteByte(BOARD_TWI, DS_REG_STAT, 0x00);
			#else
				DSWriteByteTo(twiPort, DS_REG_STAT, 0x00, 50);
			#endif
        }
        
        /* In Tracking / Auto Mode */
        if(!mBusRegs[MBUS_REG_OPMODE])
        {
            /* If Tracking Time Expired */
            if(minCtr >= TRACKING_INTERVAL)
            {
                PVTrack();
                minCtr = 0;
            }
            /* LED1 Heart Beat Status (No Block) */
        }
        else
        {
            /* In Manual Mode */
            TestCode();
        }
        //vTaskDelay(pdMS_TO_TICKS(1000));
		vTaskDelay(1000);
    }
}

void vPvTrackerInit(void)
{
    /* Initialize all peripherals */
    /* Initialize TWI Port */
    //InitTwiRTOS();
	InitTWI();

	/* Initialize FLASH Controller for EMULATED EEPROM Access*/
	//nvm_init(INT_FLASH);
	/* Initialize Variables from EEPROM */
    //InitVars();
    
    /* Init Motor Controller */
	
}

/* Initializes RAM variables from EERPOM */
void InitVars(void)
{
    uint16_t *ptr;
    //Read vars from Memory
//     mBusRegs[MBUS_REG_LATL] = EEPROM0_ReadByte(EE_REG_LAT1);
//     mBusRegs[MBUS_REG_LATL] = (mBusRegs[MBUS_REG_LATL]<<8) | EEPROM0_ReadByte(EE_REG_LAT0);
//     mBusRegs[MBUS_REG_LATH] = EEPROM0_ReadByte(EE_REG_LAT3);
//     mBusRegs[MBUS_REG_LATH] = (mBusRegs[MBUS_REG_LATH]<<8) | EEPROM0_ReadByte(EE_REG_LAT2);
	nvm_read(INT_FLASH, EE_REG_LAT0, (void *)&mBusRegs[MBUS_REG_LATL], 4);
    
//     mBusRegs[MBUS_REG_LONL] = EEPROM0_ReadByte(EE_REG_LON1);
//     mBusRegs[MBUS_REG_LONL] = (mBusRegs[MBUS_REG_LONL]<<8) | EEPROM0_ReadByte(EE_REG_LON0);
//     mBusRegs[MBUS_REG_LONH] = EEPROM0_ReadByte(EE_REG_LON3);
//     mBusRegs[MBUS_REG_LONH] = (mBusRegs[MBUS_REG_LONH]<<8) | EEPROM0_ReadByte(EE_REG_LON2);
    nvm_read(INT_FLASH, EE_REG_LON0, (void *)&mBusRegs[MBUS_REG_LONL], 4);

//     mBusRegs[MBUS_REG_TZL] = EEPROM0_ReadByte(EE_REG_TZ1);
//     mBusRegs[MBUS_REG_TZL] = (mBusRegs[MBUS_REG_TZL]<<8) | EEPROM0_ReadByte(EE_REG_TZ0);
//     mBusRegs[MBUS_REG_TZH] = EEPROM0_ReadByte(EE_REG_TZ3);
//     mBusRegs[MBUS_REG_TZH] = (mBusRegs[MBUS_REG_TZH]<<8) | EEPROM0_ReadByte(EE_REG_TZ2);
    nvm_read(INT_FLASH, EE_REG_TZ0, (void *)&mBusRegs[MBUS_REG_TZL], 4);

//     mBusRegs[MBUS_REG_DISTL] = EEPROM0_ReadByte(EE_REG_DIST1);
//     mBusRegs[MBUS_REG_DISTL] = (mBusRegs[MBUS_REG_DISTL]<<8) | EEPROM0_ReadByte(EE_REG_DIST0);
//     mBusRegs[MBUS_REG_DISTH] = EEPROM0_ReadByte(EE_REG_DIST3);
//     mBusRegs[MBUS_REG_DISTH] = (mBusRegs[MBUS_REG_DISTH]<<8) | EEPROM0_ReadByte(EE_REG_DIST2);
    nvm_read(INT_FLASH, EE_REG_DIST0, (void *)&mBusRegs[MBUS_REG_DISTL], 4);

//     mBusRegs[MBUS_REG_WIDTHL] = EEPROM0_ReadByte(EE_REG_WIDTH1);
//     mBusRegs[MBUS_REG_WIDTHL] = (mBusRegs[MBUS_REG_WIDTHL]<<8) | EEPROM0_ReadByte(EE_REG_WIDTH0);
//     mBusRegs[MBUS_REG_WIDTHH] = EEPROM0_ReadByte(EE_REG_WIDTH3);
//     mBusRegs[MBUS_REG_WIDTHH] = (mBusRegs[MBUS_REG_WIDTHH]<<8) | EEPROM0_ReadByte(EE_REG_WIDTH2);
    nvm_read(INT_FLASH, EE_REG_WIDTH0, (void *)&mBusRegs[MBUS_REG_WIDTHL], 4);

//     mBusRegs[MBUS_REG_PNLRNGL] = EEPROM0_ReadByte(EE_REG_PNLRNG1);
//     mBusRegs[MBUS_REG_PNLRNGL] = (mBusRegs[MBUS_REG_PNLRNGL]<<8) | EEPROM0_ReadByte(EE_REG_PNLRNG0);
//     mBusRegs[MBUS_REG_PNLRNGH] = EEPROM0_ReadByte(EE_REG_PNLRNG3);
//     mBusRegs[MBUS_REG_PNLRNGH] = (mBusRegs[MBUS_REG_PNLRNGH]<<8) | EEPROM0_ReadByte(EE_REG_PNLRNG2);
	nvm_read(INT_FLASH, EE_REG_PNLRNG0, (void *)&mBusRegs[MBUS_REG_PNLRNGL], 4);
    
    //Init local vars
    ptr = (uint16_t*)&lat;
    ptr[1] = mBusRegs[MBUS_REG_LATH];
    ptr[0] = mBusRegs[MBUS_REG_LATL];
    ptr = (uint16_t*)&lon;
    ptr[1] = mBusRegs[MBUS_REG_LONH];
    ptr[0] = mBusRegs[MBUS_REG_LONL];
    ptr = (uint16_t*)&timeZone;
    ptr[1] = mBusRegs[MBUS_REG_TZH];
    ptr[0] = mBusRegs[MBUS_REG_TZL];
    ptr = (uint16_t*)&width;
    ptr[1] = mBusRegs[MBUS_REG_WIDTHH];
    ptr[0] = mBusRegs[MBUS_REG_WIDTHL];
    ptr = (uint16_t*)&dist;
    ptr[1] = mBusRegs[MBUS_REG_DISTH];
    ptr[0] = mBusRegs[MBUS_REG_DISTL];
    ptr = (uint16_t*)&pvAngleRng;
    ptr[1] = mBusRegs[MBUS_REG_PNLRNGH];
    ptr[0] = mBusRegs[MBUS_REG_PNLRNGL];
    
    /* Manual Override values as EEPROM contains invalid data*/
    /*lat = 17.46608f; 
    lon = 78.44009f;
    timeZone = 5.5f;
    dist = 5.0f;
    width = 2.0f;
    //Panels will track +/-pvAngleRng degrees. 
    pvAngleRng = 43;*/
    
}

static void InitTWI(void)
{
	twi_options_t twiSettings = {
		sysclk_get_peripheral_hz(),
		100000,
		0,
		0
	};
	/* Enable the peripheral clock in the PMC. */
	sysclk_enable_peripheral_clock(BOARD_TWI_ID);

	/* Enable TWI master mode */
	twi_enable_master_mode(BOARD_TWI);

	/* Initialize TWI peripheral */
	twi_master_init(BOARD_TWI, &twiSettings);
}

void InitTwiRTOS(void)
{
	const freertos_peripheral_options_t settings = {
		NULL,
		0,
		configLIBRARY_LOWEST_INTERRUPT_PRIORITY,
		TWI_I2C_MASTER,
		USE_TX_ACCESS_SEM | USE_RX_ACCESS_MUTEX | WAIT_TX_COMPLETE | WAIT_RX_COMPLETE
	};

	/* Enable the peripheral clock in the PMC. */
	sysclk_enable_peripheral_clock(BOARD_TWI_ID);
	//InitTWI();
	twiPort = freertos_twi_master_init(BOARD_TWI, &settings);
}

void PVTrack(void)
{
    float pvAngle, bkTrkAngle;
    uint16_t *ptr;
    
    /* Disable Charge Ctrlr */
    //ccEn = 0;
    /* Wait until charge ctrlr is disabled */
//     while(ccStatus)
//     {
//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
    
    taskENTER_CRITICAL();
    
    /* Read RTC */
	#ifndef DS3231_USE_RTOS_API
		DSGetTime(BOARD_TWI, (uint16_t *)&mBusRegs[MBUS_REG_SEC]);
		DSGetFullDate(BOARD_TWI, (uint16_t *)&mBusRegs[MBUS_REG_DAY]);
	#else
		DSGetTimeTo(twiPort, (uint16_t *)&mBusRegs[MBUS_REG_SEC], 50);
		DSGetFullDateTo(twiPort, (uint16_t *)&mBusRegs[MBUS_REG_DAY], 50);
	#endif
    
    /* Clacluate PV Angle from time */
    pvAngle = GetPvAngle();
    ptr = (uint16_t*)&pvAngle;
    mBusRegs[MBUS_REG_PVANGLEH] = ptr[1];
    mBusRegs[MBUS_REG_PVANGLEL] = ptr[0];
        
    #ifdef DEBUG_EN
        Debug_PutString("Targ Ang = ");
        PrintFlt(pvAngle);
        Debug_PutString("\r\n");
    #endif
    
    #ifdef LOG_EN
        PrintInt(bkTrkFlg);
        Debug_PutString(",");
        PrintFlt(pvAngle);
        Debug_PutString(",");
    #endif
    
    /* If not backtracking */
    if(!bkTrkFlg)
    {
        #ifdef LOG_EN
            Debug_PutString("NA\r\n");
        #endif
        /* If Angle between +/- pvAngleRng deg */
        if(pvAngle>= -pvAngleRng && pvAngle <= pvAngleRng)
        {
            /* Rotate Motor */
            #ifndef LOG_EN
                GotoAngle(pvAngle);
            #endif
        }
        else
        {
            bkTrkFlg = 1;
        }
    }
    else
    {
        if(pvAngle>= -90 && pvAngle <= 90)
        {
            bkTrkAngle = GetPvBackTrackAngle(pvAngle);
            
            /* If bkTrkAngle is within +/- given range */ 
            if(bkTrkAngle>= -pvAngleRng && bkTrkAngle <= pvAngleRng)
            {
                #ifdef LOG_EN
                    PrintFlt(bkTrkAngle);
                    Debug_PutString("\r\n");
                #endif
                
                /* Rotate Motor */
                #ifndef LOG_EN
                    GotoAngle(bkTrkAngle);
                #endif
            }
            #ifdef LOG_EN
                else
                {
                    Debug_PutString("NA\r\n");
                }
            #endif
        }
        #ifdef LOG_EN
            else
            {
                Debug_PutString("NA\r\n");
            }
        #endif
        if(pvAngle>= -pvAngleRng && pvAngle <= pvAngleRng)
        {
            bkTrkFlg = 0;
        }
    }
          
    /* Clear any required flags */
    minCtr = 0;
    taskEXIT_CRITICAL();
    
    /* Enable Charge Ctrlr */
    //ccEn = 1;
} 

void GotoAngle(float pvAngle)
{
    uint16_t *ptr;
    int16_t accVals[3] = {};
    float oriVals[3], error = 0;
    uint8_t p = 0;
    float oriX, prevOri = 0;
    
	#ifndef ICM20648_USE_RTOS_API
		ICMReadAccDataAll(BOARD_TWI, ICM_ADDR,(uint16_t*)accVals);
	#else
		ICMReadAccDataAllTo(twiPort, ICM_ADDR,(uint16_t*)accVals, 50);
	#endif

    GetOrientation(accVals, oriVals);
    
    #ifdef DEBUG_EN
        Debug_PutString("In Goto Angle:\r\n");
        Debug_PutString("Init X = ");
        PrintFlt(oriVals[0]);
        Debug_PutString("\r\n");
    #endif
    
    error = pvAngle - oriVals[0];
    
    #ifdef DEBUG_EN
        Debug_PutString("Error = ");
        PrintFlt(error);
        Debug_PutString("\r\n");
    #endif
    if(error > 0)
    {
        //Set Anti Clockwise Direction
        #ifdef DEBUG_EN
            Debug_PutString("ACLK\r\n");
        #endif
        gpio_set_pin_low(PIN_MOTOR_A_IDX);
		gpio_set_pin_high(PIN_MOTOR_B_IDX);
    }
    else
    {
        //Set Clockwise Direction
        #ifdef DEBUG_EN
            Debug_PutString("CLK\r\n");
        #endif
        gpio_set_pin_high(PIN_MOTOR_A_IDX);
        gpio_set_pin_low(PIN_MOTOR_B_IDX);
    }
    
    //If error greater than +/- 1.0f
    if(!((error >=-1.0f)&&(error<1.0f)))
    {
        //Turn Motor On
        //gpio_set_pin_high(PIN_MOTOR_RST_IDX);

        #ifdef DEBUG_EN
            Debug_PutString("ON\r\n");    
        #endif
    }
    
    while(!((error >=-1.0f)&&(error<1.0f)))
    {
        prevOri = 0;
        for(p = 0; p < 8; p++)
        {
            #ifndef ICM20648_USE_RTOS_API
				ICMReadAccDataAll(BOARD_TWI, ICM_ADDR,(uint16_t*)accVals);
			#else
				ICMReadAccDataAllTo(twiPort, ICM_ADDR,(uint16_t*)accVals, 50);
			#endif
            GetOrientation(accVals, oriVals);
            oriX = prevOri + ((oriVals[0] - prevOri)/((float)(p+1)));
        	prevOri = oriX;
            delay_ms(40);
        }
        oriVals[0] = oriX;
        
        #ifdef DEBUG_EN
            Debug_PutString("X = ");
            PrintFlt(oriVals[0]);
            Debug_PutString("\r\n");
        #endif
        
        error = pvAngle - oriVals[0];
        
        if(error > 0)
        {
            //Set Anti Clockwise Direction
            #ifdef DEBUG_EN
                Debug_PutString("ACLK\r\n");
            #endif
			gpio_set_pin_low(PIN_MOTOR_A_IDX);
			gpio_set_pin_high(PIN_MOTOR_B_IDX);
        }
        else
        {
            //Set Clockwise Direction
            #ifdef DEBUG_EN
                Debug_PutString("CLK\r\n");
            #endif
			gpio_set_pin_high(PIN_MOTOR_A_IDX);
			gpio_set_pin_low(PIN_MOTOR_B_IDX);
        }
    }
    //Turn Motor Off
    //gpio_set_pin_low(PIN_MOTOR_RST_IDX);
    
    #ifdef DEBUG_EN
        Debug_PutString("X = ");
        PrintFlt(oriVals[0]);
        Debug_PutString("\r\n");
    #endif
    
    //PWM0_Stop();
    #ifdef DEBUG_EN
        Debug_PutString("OFF\r\n");
    #endif
    
    ptr = (uint16_t*)&oriVals[0];
    mBusRegs[MBUS_REG_ANXH] = ptr[1];
    mBusRegs[MBUS_REG_ANXL] = ptr[0];
}

void TestCode(void)
{
    int16_t accVals[3] = {0, 0, 0};
    float oriVals[3], pvAngle=0;
    uint16_t *ptr;
    #ifndef DS3231_USE_RTOS_API
		DSGetTime(BOARD_TWI, (uint16_t *)&mBusRegs[MBUS_REG_SEC]);
		DSGetFullDate(BOARD_TWI, (uint16_t *)&mBusRegs[MBUS_REG_DAY]);
	#else
		DSGetTimeTo(twiPort, (uint16_t *)&mBusRegs[MBUS_REG_SEC], 50);
		DSGetFullDateTo(twiPort, (uint16_t *)&mBusRegs[MBUS_REG_DAY], 50);
	#endif
    #ifdef DEBUG_EN
        Debug_PutString("Time: ");
        PrintInt(((mBusRegs[MBUS_REG_HRS]>>4)*10) + (mBusRegs[MBUS_REG_HRS]&0x000F));
        Debug_PutString(":");
        PrintInt(((mBusRegs[MBUS_REG_MIN]>>4)*10) + (mBusRegs[MBUS_REG_MIN]&0x000F));
        Debug_PutString(":");
        PrintInt(((mBusRegs[MBUS_REG_SEC]>>4)*10) + (mBusRegs[MBUS_REG_SEC]&0x000F));
        Debug_PutString("\r\n");
    #endif

	#ifndef ICM20648_USE_RTOS_API
		ICMReadAccDataAll(BOARD_TWI, ICM_ADDR,(uint16_t*)accVals);
	#else
		ICMReadAccDataAllTo(twiPort, ICM_ADDR,(uint16_t*)accVals, 50);
	#endif
    GetOrientation(accVals, oriVals);
    #ifdef DEBUG_EN
        Debug_PutString("X = ");
        PrintFlt(oriVals[0]);
        Debug_PutString("\r\n");
    #endif
    ptr = (uint16_t*)&oriVals[0];
    mBusRegs[MBUS_REG_ANXH] = ptr[1];
    mBusRegs[MBUS_REG_ANXL] = ptr[0];
    pvAngle = GetPvAngle();
    ptr = (uint16_t*)&pvAngle;
    mBusRegs[MBUS_REG_PVANGLEH] = ptr[1];
    mBusRegs[MBUS_REG_PVANGLEL] = ptr[0];
    
	if(mBusRegs[MBUS_REG_MOTDR])
	{
		gpio_set_pin_low(PIN_MOTOR_A_IDX);
		gpio_set_pin_high(PIN_MOTOR_B_IDX);
	}
	else
	{
		gpio_set_pin_high(PIN_MOTOR_A_IDX);
		gpio_set_pin_low(PIN_MOTOR_B_IDX);
	}

    if(mBusRegs[MBUS_REG_MOTON])
    {
        //Disable Charge Ctrlr
		//ccEn = 0;
        //Turn Motor On
		//gpio_set_pin_high(PIN_MOTOR_RST_IDX);
    }
    else
    {
        //Enable Charge Ctrlr
        //ccEn = 1;
        //Turn Motor Off
        //gpio_set_pin_low(PIN_MOTOR_RST_IDX);
    }
}

#if defined(DEBUG_EN) || defined(LOG_EN)
    void PrintFlt(float val)
    {
        char buff2[10];
        int iVal, fVal;

        if(val>0)
        {
            iVal = val;
            fVal = (val-iVal)*100;
            sprintf(buff2,"%d.%d",iVal,fVal);
        }
        else
        {
            val = fabs(val);
            iVal = val;
            fVal = (val-iVal)*100;
            sprintf(buff2,"-%d.%d",iVal,fVal);
        }   
        Debug_PutString(buff2);
    }

    void PrintInt(int val)
    {
        char buff2[10];

        sprintf(buff2,"%d",val);
        Debug_PutString(buff2);
    }
#endif

void GetOrientation(int16_t *acc, float *orientation)
{
    float accFlt[3];
    accFlt[0] = ((float)acc[0])/32768.0f;
    accFlt[1] = ((float)acc[1])/32768.0f;
    accFlt[2] = ((float)acc[2])/32768.0f;
    
    //Equation 25 (Rotate Across X Axis)
    orientation[0] = atan2(accFlt[1], accFlt[2]);
    //Radian to degrees conversion
    orientation[0] = (180.0f * orientation[0])/(float)M_PI;
    
    //Equation 26 (Rotate Across Y Axis)
    orientation[1] = sqrt(accFlt[1]*accFlt[1] + accFlt[2]*accFlt[2]);
    orientation[1] = atan2(-accFlt[0], orientation[1]); 
    //Radian to degrees conversion
    orientation[1] = (180.0f * orientation[1])/(float)M_PI;
}

float GetPvAngle(void)
{
	int32_t x1, x2, x3, x4, x5, x7;
	int32_t y0, y1, y4, y12;
    int16_t date, mon, year, hrs, min, sec;
    float y2, y3, y5, y6, y7, y8, y9, y10, y11, y13;
	float y14, y15, y16;
	float x9, x13;
	float e, f, g, h, i, j, k, l, m, n, o, p;
	float q, r, s, t;
	float timeInSecs;
    
    //BCD to Int Conversion
    date = ((mBusRegs[MBUS_REG_DD]>>4)*10) + (mBusRegs[MBUS_REG_DD]&0x000F); 
    mon = ((mBusRegs[MBUS_REG_MM]>>4)*10) + (mBusRegs[MBUS_REG_MM]&0x000F); 
    year = ((mBusRegs[MBUS_REG_YY]>>4)*10) + (mBusRegs[MBUS_REG_YY]&0x000F); 
    year += 2000;
    hrs = ((mBusRegs[MBUS_REG_HRS]>>4)*10) + (mBusRegs[MBUS_REG_HRS]&0x000F); 
    min = ((mBusRegs[MBUS_REG_MIN]>>4)*10) + (mBusRegs[MBUS_REG_MIN]&0x000F); 
    sec = ((mBusRegs[MBUS_REG_SEC]>>4)*10) + (mBusRegs[MBUS_REG_SEC]&0x000F); 
    
    #ifdef LOG_EN
        PrintInt(hrs);
        Debug_PutString(",");
        PrintInt(min);
        Debug_PutString(",");
        PrintInt(sec);
        Debug_PutString(",");
    #endif
    
    timeInSecs = (hrs*3600 + min*60 + sec)/86400.0f;

    //Angle Calcualation Algorithm
	if (mon <= 2)
	{
		x1 = 1;
	}
	else
	{
		x1 = 0;
	}
	x2 = 12 * x1;
	x3 = x2 - 3;
	x4 = mon + x3;
	x5 = 4800 - x1;
	x7 = x5 + year;

	y0 = 153*x4;
	y1 = y0 + 2;
	y2 = y1 / 5.0f;
	y3 = date + y2;
	y4 = 365 * x7;
	y5 = y3 + y4;
	y6 = x7 / 4.0f;
	y7 = y5 + y6;
	y8 = x7 / 100.0f;
	y9 = y7 - y8;
	y10 = x7 / 400.0f;
	y11 = y9 + y10;

	x9 = y11 - 32045;
	y12 = hrs - 12;
	y13 = y12 / 24.0f;
	y14 = min / 1440.0f;
	y15 = y13 + y14;
	y16 = sec / 86400.0f;
	x13 = y15 + y16;

	e = x9 + x13;
	f = (e - 2451545.0f) / 36525.0f;
	g = ((int)(280.46646f + f*(36000.76983f + f*0.0003032f))) % 360; //mod
	h = 357.52911f + f*(35999.05029f - 0.0001537f*f);
	i = 0.016708634f - f*(0.000042037f + 0.0000001267f*f);
	j = sin(Rad(h))*(1.914602f - f*(0.004817f + 0.000014f*f));
	j = j + sin(Rad(2 * h))*(0.019993f - 0.000101f*f);
	j = j + sin(Rad(3 * h))*0.000289f;
	k = g + j;
	l = k - 0.00569f - 0.00478f*sin(Rad(125.04f - 1934.136f*f));
	m = 23 + (26 + ((21.448f - f*(46.815f + f*(0.00059f - f*0.001813f)))) / 60.0f) / 60.0f;
	n = m + 0.00256f*cos(Rad(125.04f - 1934.136f*f));
	o = Deg(asin(sin(Rad(n))*sin(Rad(l))));
	p = tan(Rad(n / 2))*tan(Rad(n / 2));
	q = 4.0f * Deg(p*sin(2.0f * Rad(g)) - 2.0f * i*sin(Rad(h)) + 4.0f * i*p*sin(Rad(h))*cos(2.0f * Rad(g)) - 0.5f*p*p*sin(4.0f * Rad(g)) - 1.25f*i*i*sin(2.0f * Rad(h)));
	r = Deg(acos(cos(Rad(90.833f)) / (cos(Rad(lat))*cos(Rad(o))) - tan(Rad(lat))*tan(Rad(o))));
	s = ((int)(timeInSecs * 1440 + q + 4 * lon - 60 * timeZone)) % 1440;
	if ((s / 4.0f) < 0)
	{
		t = (s / 4.0f) + 180.0f;
	}
	else
	{
		t = (s / 4.0f) - 180.0f;
	}
	return (90.0f / r*t);
}

float GetPvBackTrackAngle(float pvAngle)
{
	float a0;
	
	a0 = pvAngle*0.0174603f;
	a0 += 1.575f;
	a0 -= 0.028989f;
	a0 = sin(a0);
	a0 = a0 * dist;
	a0 = a0 / width;
	/* If this val is < 1 enter back track calculation */
	/* Otherwise exit backtracking mode */
	if (a0 < 1)
	{
		a0 = asin(a0);
		a0 = a0 * 57.2958f;
		if ((pvAngle >= -90) && (pvAngle < 0))
		{
			a0 = -a0 / 2.0;
		}
		else if ((pvAngle >= 0) && (pvAngle <= 90))
		{
			a0 = a0 / 2.0;
		}
		return a0;
	}
	else
		return NAN;
}

float Rad(float deg)
{
	return ((float)M_PI*deg)/180.0f;
}

float Deg(float rad)
{
	return (180.0f *rad) / (float)M_PI;
}

/* [] END OF FILE */
