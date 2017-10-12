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

#ifndef __DS3231_H__
    #define __DS3231_H__    1
    
    //7 Bit Device ADDR
    #define DS_ADDR         0x68
    
    //Register Addresses
    //Timekeeper Registers
    #define DS_REG_SEC      0x00
    #define DS_REG_MIN      0x01
    #define DS_REG_HRS      0x02
    #define DS_REG_DAY      0x03
    #define DS_REG_DAT      0x04
    #define DS_REG_MON      0x05
    #define DS_REG_YRS      0x06
    
    //Alarm 1 Registers
    #define DS_REG_AL1S     0x07
    #define DS_REG_AL1M     0x08
    #define DS_REG_AL1H     0x09
    #define DS_REG_AL1D     0x0A
    
    //Alarm 2 Registers
    #define DS_REG_AL2M     0x0B
    #define DS_REG_AL2H     0x0C
    #define DS_REG_AL2D     0x0D
    
    //Management Registers
    #define DS_REG_CTRL     0x0E
    #define DS_REG_STAT     0x0F
    #define DS_REG_AOFST    0x10
    #define DS_REG_TEMPH    0x11
    #define DS_REG_TEMPL    0x12
    
    //Function Declarations
    void DSGetTimeTo(uint16 *recvTime, int timeOut);
    void DSSetTimeTo(uint16 *time, int timeOut);
    void DSGetSecsTo(uint16 *ss, int timeOut);
    void DSSetSecsTo(uint8 ss, int timeOut);
    void DSGetMinsTo(uint16 *mm, int timeOut);
    void DSSetMinsTo(uint8 mm, int timeOut);
    void DSGetHrsTo(uint16 *hh, int timeOut);
    void DSSetHrsTo(uint8 hh, int timeOut);
    void DSGetFullDateTo(uint16 *recvDate, int timeOut);
    void DSSetFullDateTo(uint16 *date, int timeOut);
    void DSGetDayTo(uint16 *d, int timeOut);
    void DSSetDayTo(uint8 d, int timeOut);
    void DSGetDateTo(uint16 *dd, int timeOut);
    void DSSetDateTo(uint8 dd, int timeOut);
    void DSGetMonthTo(uint16 *mm, int timeOut);
    void DSSetMonthTo(uint8 mm, int timeOut);
    void DSGetYearTo(uint16 *yy, int timeOut);
    void DSSetYearTo(uint8 yy, int timeOut);
    void DSEnAL1To(uint8 en, int timeOut);
    void DSEnAL2To(uint8 en, int timeOut);
    void DSSetCtrlRegTo(uint8 val, int timeOut);
    void DSGetCtrlRegTo(uint16 *val, int timeOut);
    void DSGetStatRegTo(uint16 *val, int timeOut);
    void DSGetTempRegTo(int16 *temp, int timeOut);
    void DSWriteByteTo(uint8 reg, uint8 data, int timeOut);
    void DSReadByteTo(uint8 reg, uint8 *recvData, int timeOut);
    
#endif

/* [] END OF FILE */
