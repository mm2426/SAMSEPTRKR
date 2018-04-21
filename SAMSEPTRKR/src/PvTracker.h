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
    
    #define LOG_EN                 1
    //#define DEBUG_EN               1
    
    /* Accelerometer TWI Address */
    #define ICM_ADDR				0x69
	/* EEPROM TWI Address*/
	#define AT24C08_ADDR			0x50
    /* Tracking Interval in mins */
    #define TRACKING_INTERVAL		3
	/* Inclination Error */
	#define INCLINATION_ERROR		0.5f

    /* EEPROM Addresses */
	/* Base address of the Emulated EEPROM (internal FLASH is used as EEPROM) */
	/* Last 1KBytes reserved for EEPROM Emulation */
	//#define EE_REG_BASE				0x0043FC00
	/* External EEPROM used to store parameters */
	#define EE_REG_BASE				0x0000
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
    /* Distance B/W panels (Flt) */
    #define EE_REG_DIST0			(EE_REG_BASE + 12)
    #define EE_REG_DIST1			(EE_REG_BASE + 13)
    #define EE_REG_DIST2			(EE_REG_BASE + 14)
    #define EE_REG_DIST3			(EE_REG_BASE + 15)
    /* Width of Panels (Flt) */
    #define EE_REG_WIDTH0			(EE_REG_BASE + 16)
    #define EE_REG_WIDTH1			(EE_REG_BASE + 17)
    #define EE_REG_WIDTH2			(EE_REG_BASE + 18)
    #define EE_REG_WIDTH3			(EE_REG_BASE + 19)
    /* Pnl Tracking Range (Flt) */
    #define EE_REG_PNLRNG0			(EE_REG_BASE + 20)
    #define EE_REG_PNLRNG1			(EE_REG_BASE + 21)
    #define EE_REG_PNLRNG2			(EE_REG_BASE + 22)
    #define EE_REG_PNLRNG3			(EE_REG_BASE + 23)
	/* Backtracking Parameter 1 (Flt) */
	#define EE_REG_BKPARAM10		(EE_REG_BASE + 24)
	#define EE_REG_BKPARAM11		(EE_REG_BASE + 25)
	#define EE_REG_BKPARAM12		(EE_REG_BASE + 26)
	#define EE_REG_BKPARAM13		(EE_REG_BASE + 27)
	/* Backtracking Parameter 2 (Flt) */
	#define EE_REG_BKPARAM20		(EE_REG_BASE + 28)
	#define EE_REG_BKPARAM21		(EE_REG_BASE + 29)
	#define EE_REG_BKPARAM22		(EE_REG_BASE + 30)
	#define EE_REG_BKPARAM23		(EE_REG_BASE + 31)

	/* Detect whether to load default configuration or not. 
	*  Make sure that this is the last variable to be stored in the memory. 
	*/
	#define EE_REG_DEFCONFIG		(EE_REG_BASE + 32)
    
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
	#include "consoleuart.h"
    #include <stdio.h>
    #include <math.h>
    
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
    #endif
    void GetOrientation(int16_t *acc, float *orientation);
    float GetPvAngle(void);
    float GetPvBackTrackAngle(float pvAngle);
    float Rad(float deg);
    float Deg(float rad);
    
#endif

/* [] END OF FILE */
