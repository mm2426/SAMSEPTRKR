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
    #define ICM_ADDR               0x69
    /* Tracking Interval in mins */
    #define TRACKING_INTERVAL      3
    
    /* EEPROM Addresses */
    /* Latitude (Flt) */
    #define EE_REG_LAT0           0
    #define EE_REG_LAT1           1
    #define EE_REG_LAT2           2
    #define EE_REG_LAT3           3
    /* Longitude (Flt) */
    #define EE_REG_LON0           4
    #define EE_REG_LON1           5
    #define EE_REG_LON2           6
    #define EE_REG_LON3           7 
    /* TimeZone (Flt) */
    #define EE_REG_TZ0            8
    #define EE_REG_TZ1            9
    #define EE_REG_TZ2            10
    #define EE_REG_TZ3            11
    /* Motor Speed */
    #define EE_REG_MOTSP0         12
    #define EE_REG_MOTSP1         13
    /* Distance B/W panels (Flt) */
    #define EE_REG_DIST0          14
    #define EE_REG_DIST1          15
    #define EE_REG_DIST2          16
    #define EE_REG_DIST3          17
    /* Width of Panels (Flt) */
    #define EE_REG_WIDTH0         18
    #define EE_REG_WIDTH1         19
    #define EE_REG_WIDTH2         20
    #define EE_REG_WIDTH3         21
    /* Pnl Tracking Range (Flt) */
    #define EE_REG_PNLRNG0        22
    #define EE_REG_PNLRNG1        23
    #define EE_REG_PNLRNG2        24
    #define EE_REG_PNLRNG3        25
    
    #include "FreeRTOS.h"
    #include "DS3231.h"
    #include "ICM20648.h"
    #include "CommInterface.h"
    #include <stdio.h>
    #include <math.h>
    
    /* RTC 1 Min ISR */
    CY_ISR_PROTO(RTCMinIsr);
    
    /* RTOS Task Declarations*/
    void vPvTrackerTask(void *pvParameters);
    
    /*General Function Declarations*/
    /* Initialize Tracker Variables */
    void vPvTrackerInit(void);
    /*Initializes RAM variables from EERPOM*/
    void InitVars(void);
    void PVTrack(void);
    void GotoAngle(float pvAngle);
    void TestCode(void);
    #if defined(DEBUG_EN) || defined(LOG_EN)
        void PrintFlt(float val);
        void PrintInt(int val);
    #endif
    void ParseSPIPkt(uint16 *buff3, uint8 regAddr, uint8 len);
    void GetOrientation(int16 *acc, float *orientation);
    float GetPvAngle(void);
    float GetPvBackTrackAngle(float pvAngle);
    float Rad(float deg);
    float Deg(float rad);
    
#endif

/* [] END OF FILE */
