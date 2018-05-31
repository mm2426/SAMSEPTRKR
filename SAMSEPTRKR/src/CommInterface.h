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

#ifndef __COMM_INTF_H__
    #define __COMM_INTF_H__         1
    
    /* MODBUS Register Indexes */
    /* RTC Regs */
    #define MBUS_REG_SEC            0
    #define MBUS_REG_MIN            1
    #define MBUS_REG_HRS            2
    #define MBUS_REG_DAY            3
    #define MBUS_REG_DD             4
    #define MBUS_REG_MM             5
    #define MBUS_REG_YY             6
    /* Latitude (Flt) */
    #define MBUS_REG_LATL           8
    #define MBUS_REG_LATH           9
    /* Longitude (Flt) */
    #define MBUS_REG_LONL           10
    #define MBUS_REG_LONH           11 
    /* TimeZone (Flt) */
    #define MBUS_REG_TZL            12
    #define MBUS_REG_TZH            13
    /* Motion Snsr Regs */
    /* Orientation X (Flt) */
    #define MBUS_REG_ANXL           14
    #define MBUS_REG_ANXH           15
    /* Panel Angle Val (Flt) */
    #define MBUS_REG_PVANGLEL       16
    #define MBUS_REG_PVANGLEH       17
    /* Panel Tracking Range (Flt) */
    #define MBUS_REG_PNLRNGL        18
    #define MBUS_REG_PNLRNGH        19
    /* Charge Ctrlr */
    /* Panel Voltage (Flt) */
    #define MBUS_REG_PVVL           20
    #define MBUS_REG_PVVH           21
    /* Bat Voltage (Flt) */
    #define MBUS_REG_BATVL          22
    #define MBUS_REG_BATVH          23
    /* Operating Mode (0 = AUTO / 1 = MAN / 2 = WINDSPD / 3 = CLEAN) */
    #define MBUS_REG_OPMODE         24
    /* Motor Direction */
    #define MBUS_REG_MOTDR          25
    /* Motor Control (0 = OFF / 1 = ON) */
    #define MBUS_REG_MOTON          26
    /* NTC Thermistor */
    /* Bat Temperature (Flt) */
    #define MBUS_REG_TEMPL          28
    #define MBUS_REG_TEMPH          29
    /* Distance B/W panels (Flt) */
    #define MBUS_REG_DISTL          30
    #define MBUS_REG_DISTH          31
    /* Width of Panels (Flt) */
    #define MBUS_REG_WIDTHL         32
    #define MBUS_REG_WIDTHH         33
	/* Backtracking Param 1 (Flt) */
	#define MBUS_REG_BKPARAM1L      34
	#define MBUS_REG_BKPARAM1H      35
	/* Backtracking Param 2 (Flt) */
	#define MBUS_REG_BKPARAM2L		36
	#define MBUS_REG_BKPARAM2H		37
	/* Cleaning Mode Direction (0 = +Max-Rng / 1 = -Max-Rng) */
	#define MBUS_REG_CLMODEDIR		38
	/* MODBUS Slave Address Register */
	#define MBUS_REG_SLA			39
	/* MODBUS Comm BuadRate 1 = 4800, 2 = 9600, 3 = 19200 */
	#define MBUS_REG_BAUDRATE		40
	/* MODBUS Comm Parity 0 = None, 1 = Even, 2 = Odd */
	#define MBUS_REG_PARITY			41
	
	/* Tracker Operation Modes */
	#define TRKR_OPMODE_AUTO		0
	#define TRKR_OPMODE_MAN			1
	#define TRKR_OPMODE_WINDSPD		2
	#define TRKR_OPMODE_CLEAN		3

    #include <asf.h>
	#include <string.h>
	#include "FreeRTOS.h"
    #include "task.h"
    #include "semphr.h"
	#ifndef DS3231_USE_RTOS_API
		#include "DS3231.h"
	#else
		#include "DS3231RTOS.h"
	#endif
	#include "rs485pdc.h"
    #include "mbusStack.h"

    /* RTOS Task Declarations */
    void vCommTask(void *pvParameters);
    
    /* General Function Declarations */
    /* Initialize Communication Port */
    void vCommInit(void);
	/* Initialize Communication Variables from EEPROM */
	void UpdateRs485Params(void);
    
#endif

/* [] END OF FILE */
