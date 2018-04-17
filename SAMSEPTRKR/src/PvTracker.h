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

#ifndef __PV_TRACKER_H__
    #define __PV_TRACKER_H__  1
    
    //#define LOG_EN                 1
    //#define DEBUG_EN               1
    
    /* Accelerometer Address */
    #define ICM_ADDR				0x69
    /* Tracking Interval in mins */
    #define TRACKING_INTERVAL		3
    
    /* EEPROM Addresses */
	/* Base address of the Emulated EEPROM (internal FLASH is used as EEPROM) */
	/* Last 1KBytes reserved for EEPROM Emulation */
	#define EE_REG_BASE				0x0043FC00
    /* Latitude (Flt) */
    #define EE_REG_LAT0				(EE_REG_BASE + 0)
    #define EE_REG_LAT1				(EE_REG_BASE + 1)
    #define EE_REG_LAT2				(EE_REG_BASE + 2)
    #define EE_REG_LAT3				(EE_REG_BASE + 3)
    /* Longitude (Flt) */
    #define EE_REG_LON0				(EE_REG_BASE + 4)
    #define EE_REG_LON1				(EE_REG_BASE + 5)
    #define EE_REG_LON2				(EE_REG_BASE + 6)
    #define EE_REG_LON3				(EE_REG_BASE + 7)	 
    /* TimeZone (Flt) */
    #define EE_REG_TZ0				(EE_REG_BASE + 8)
    #define EE_REG_TZ1				(EE_REG_BASE + 9)
    #define EE_REG_TZ2				(EE_REG_BASE + 10)
    #define EE_REG_TZ3				(EE_REG_BASE + 11)
    /* Motor Speed */
    #define EE_REG_MOTSP0			(EE_REG_BASE + 12)
    #define EE_REG_MOTSP1			(EE_REG_BASE + 13)
    /* Distance B/W panels (Flt) */
    #define EE_REG_DIST0			(EE_REG_BASE + 14)
    #define EE_REG_DIST1			(EE_REG_BASE + 15)
    #define EE_REG_DIST2			(EE_REG_BASE + 16)
    #define EE_REG_DIST3			(EE_REG_BASE + 17)
    /* Width of Panels (Flt) */
    #define EE_REG_WIDTH0			(EE_REG_BASE + 18)
    #define EE_REG_WIDTH1			(EE_REG_BASE + 19)
    #define EE_REG_WIDTH2			(EE_REG_BASE + 20)
    #define EE_REG_WIDTH3			(EE_REG_BASE + 21)
    /* Pnl Tracking Range (Flt) */
    #define EE_REG_PNLRNG0			(EE_REG_BASE + 22)
    #define EE_REG_PNLRNG1			(EE_REG_BASE + 23)
    #define EE_REG_PNLRNG2			(EE_REG_BASE + 24)
    #define EE_REG_PNLRNG3			(EE_REG_BASE + 25)
    
    #include "FreeRTOS.h"
	#ifndef DS3231_USE_RTOS_API
		#include "DS3231.h"
	#else
		#include "DS3231RTOS.h"
	#endif
	#ifndef ICM20648_USE_RTOS_API
		#include "ICM20648.h"
	#else
		#include "ICM20648RTOS.h"
	#endif
	#if defined(AT24C08_USE_RTOS_API)
		#include "AT24C08RTOS.h"
	#else
		#include "AT24C08.h"
	#endif
    #include "CommInterface.h"
    #include <stdio.h>
    #include <math.h>
    
    /* RTC 1 Min ISR */
    //CY_ISR_PROTO(RTCMinIsr);
    
    /* RTOS Task Declarations*/
    void vPvTrackerTask(void *pvParameters);
    
    /*General Function Declarations*/
    /* Initialize Tracker Variables */
    void vPvTrackerInit(void);
    /*Initializes RAM variables from EERPOM*/
    void InitVars(void);
	void InitTwiRTOS(void);
    void PVTrack(void);
    void GotoAngle(float pvAngle);
    void TestCode(void);
    #if defined(DEBUG_EN) || defined(LOG_EN)
        void PrintFlt(float val);
        void PrintInt(int val);
    #endif
    void GetOrientation(int16_t *acc, float *orientation);
    float GetPvAngle(void);
    float GetPvBackTrackAngle(float pvAngle);
    float Rad(float deg);
    float Deg(float rad);
    
#endif

/* [] END OF FILE */
