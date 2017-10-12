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
    /* Operating Mode (0 = AUTO / 1 = MAN) */
    #define MBUS_REG_OPMODE         24
    /* Motor Direction */
    #define MBUS_REG_MOTDR          25
    /* Motor Control (0 = OFF / 1 = ON) */
    #define MBUS_REG_MOTON          26
    /* NTC Thermistor */
    /* Bat Temperature (Flt) */
    #define MBUS_REG_TEMPL          28
    #define MBUS_REG_TEMPH          29
    /* Reference Resitance Val (Flt) */
    #define MBUS_REG_REFRL          30
    #define MBUS_REG_REFRH          31
    /* Distance B/W panels (Flt) */
    #define MBUS_REG_DISTL          32
    #define MBUS_REG_DISTH          33
    /* Width of Panels (Flt) */
    #define MBUS_REG_WIDTHL         34
    #define MBUS_REG_WIDTHH         35
    
    /* Maximum Modbus Registers */
    #define MBUS_MAX_REGS           36
    /* Modbus CRC Gen Poly */
    #define MBUS_CRC_POLY           0xA001
    
    /* Max UART Buffer Size*/
    #define UART_BUFF_SIZE_MAX      150
    
    /* To check whether bit is set / clr */
    #define bitIsSet(val, bitNo)    (val&(1<<bitNo))
    
    #include "project.h"
    #include "FreeRTOS.h"
    #include "task.h"
    #include "semphr.h"
    #include "DS3231.h"
    
    typedef enum rxstates
    {
        idle, dRx
    }rxstates;

    /* ISRs */
    CY_ISR_PROTO(Uart0RxIsr);
    CY_ISR_PROTO(Timer0Isr);
    
    /* RTOS Task Declarations */
    void vCommTask(void *pvParameters);
    
    /* General Function Declarations */
    /* Initialize Communication Variables */
    void vCommInit(void);
    /* MODBUS Functions */
    void parseMbusPkt(uint8 *buff, uint8 len, uint8 *respBuff, uint8 *respLen);
    void writeMbusRegs(uint16 *mbusBuff, uint8 regAddr, uint8 len);
    uint16 calcCRC(uint8 *buff, uint8 len);
    void sendRespPkt(uint8 *pkt, uint8 len);
    
#endif

/* [] END OF FILE */