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

#include "DS3231.h"
#include "I2CTO_custom.h"

void DSGetTimeTo(uint16 *recvTime, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
    I2C0_MasterWriteByteTo(DS_REG_SEC, timeOut);
    I2C0_MasterSendRestartTo(DS_ADDR, 1, timeOut);
    //SS
    recvTime[0] = I2C0_MasterReadByteTo(I2C0_ACK_DATA, timeOut);
    //MM
    recvTime[1] = I2C0_MasterReadByteTo(I2C0_ACK_DATA, timeOut);
    //HH
    recvTime[2] = I2C0_MasterReadByteTo(I2C0_NAK_DATA, timeOut);
    I2C0_MasterSendStopTo(timeOut);
}

void DSSetTimeTo(uint16 *time, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
    I2C0_MasterWriteByteTo(DS_REG_SEC, timeOut);
    I2C0_MasterWriteByteTo(time[0], timeOut);
    I2C0_MasterWriteByteTo(time[1], timeOut);
    I2C0_MasterWriteByteTo(time[2], timeOut);
    I2C0_MasterSendStopTo(timeOut);
}

void DSGetSecsTo(uint16 *ss, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_SEC, (uint8 *)ss, timeOut);
}

void DSSetSecsTo(uint8 ss, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSWriteByteTo(DS_REG_SEC, ss, timeOut);
}

void DSGetMinsTo(uint16 *mm, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_MIN, (uint8 *)mm, timeOut);
}

void DSSetMinsTo(uint8 mm, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSWriteByteTo(DS_REG_MIN, mm, timeOut);
}

void DSGetHrsTo(uint16 *hh, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_HRS, (uint8 *)hh, timeOut);
}

void DSSetHrsTo(uint8 hh, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSWriteByteTo(DS_REG_HRS, hh, timeOut);
}

void DSGetFullDateTo(uint16 *recvDate, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
    I2C0_MasterWriteByteTo(DS_REG_DAY, timeOut);
    I2C0_MasterSendRestartTo(DS_ADDR, 1, timeOut);
    //DAY
    recvDate[0] = I2C0_MasterReadByteTo(I2C0_ACK_DATA, timeOut);
    //DD
    recvDate[1] = I2C0_MasterReadByteTo(I2C0_ACK_DATA, timeOut);
    //MM
    recvDate[2] = I2C0_MasterReadByteTo(I2C0_ACK_DATA, timeOut);
    //YY
    recvDate[3] = I2C0_MasterReadByteTo(I2C0_NAK_DATA, timeOut);
    I2C0_MasterSendStopTo(timeOut);
}

void DSSetFullDateTo(uint16 *date, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
    I2C0_MasterWriteByteTo(DS_REG_DAY, timeOut);
    I2C0_MasterWriteByteTo(date[0], timeOut);
    I2C0_MasterWriteByteTo(date[1], timeOut);
    I2C0_MasterWriteByteTo(date[2], timeOut);
    I2C0_MasterWriteByteTo(date[3], timeOut);
    I2C0_MasterSendStopTo(timeOut);
}

void DSGetDayTo(uint16 *d, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_DAY, (uint8 *)d, timeOut);
}

void DSSetDayTo(uint8 d, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSWriteByteTo(DS_REG_DAY, d, timeOut);
}

void DSGetDateTo(uint16 *dd, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_DAT, (uint8 *)dd, timeOut);
}

void DSSetDateTo(uint8 dd, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSWriteByteTo(DS_REG_DAT, dd, timeOut);
}

void DSGetMonthTo(uint16 *mm, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_MON, (uint8 *)mm, timeOut);
}

void DSSetMonthTo(uint8 mm, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSWriteByteTo(DS_REG_MON, mm, timeOut);
}

void DSGetYearTo(uint16 *yy, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_YRS, (uint8 *)yy, timeOut);
}

void DSSetYearTo(uint8 yy, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSWriteByteTo(DS_REG_YRS, yy, timeOut);
}

void DSEnAL1To(uint8 en, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    if(en)
    {
        //Set Alarm 2 for 1 min interrupt
        I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
        I2C0_MasterWriteByteTo(DS_REG_AL1S, timeOut);
        I2C0_MasterWriteByteTo(0x80, timeOut);
        I2C0_MasterWriteByteTo(0x80, timeOut);
        I2C0_MasterWriteByteTo(0x80, timeOut);
        I2C0_MasterWriteByteTo(0x80, timeOut);
        I2C0_MasterSendStopTo(timeOut);
        //Write Enable Alarm Register
        DSWriteByteTo(DS_REG_CTRL, 0x05, timeOut);
        //Clear Status Reg
        DSWriteByteTo(DS_REG_STAT, 0x00, timeOut);
    }
    else
    {
        //Write Disable Alarm Register
        DSWriteByteTo(DS_REG_CTRL, 0x04, timeOut);
    }
}

void DSEnAL2To(uint8 en, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    if(en)
    {
        //Set Alarm 2 for 1 min interrupt
        I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
        I2C0_MasterWriteByteTo(DS_REG_AL2M, timeOut);
        I2C0_MasterWriteByteTo(0x80, timeOut);
        I2C0_MasterWriteByteTo(0x80, timeOut);
        I2C0_MasterWriteByteTo(0x80, timeOut);
        I2C0_MasterSendStopTo(timeOut);
        //Write Enable Alarm Register
        DSWriteByteTo(DS_REG_CTRL, 0x06, timeOut);
        //Clear Status Reg
        DSWriteByteTo(DS_REG_STAT, 0x80, timeOut);
    }
    else
    {
        //Write Disable Alarm Register
        DSWriteByteTo(DS_REG_CTRL, 0x04, timeOut);
    }
}

void DSSetCtrlRegTo(uint8 val, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSWriteByteTo(DS_REG_CTRL, val, timeOut);
}

void DSGetCtrlRegTo(uint16 *val, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_CTRL, (uint8 *)val, timeOut);
}

void DSGetStatRegTo(uint16 *val, int timeOut)
{
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    DSReadByteTo(DS_REG_STAT, (uint8 *)val, timeOut);
}

void DSGetTempRegTo(int16 *temp, int timeOut)
{
    uint8 rxData = 0;
    //Initialize I2C to bring peripheral to proper state
    I2C0_Init();
    I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
    I2C0_MasterWriteByteTo(DS_REG_TEMPH, timeOut);
    I2C0_MasterSendRestartTo(DS_ADDR, 1, timeOut);
    //TempH
    rxData = I2C0_MasterReadByteTo(I2C0_ACK_DATA, timeOut);
    //TempL
    *temp = ((rxData<<8) | I2C0_MasterReadByteTo(I2C0_NAK_DATA, timeOut));
    I2C0_MasterSendStopTo(timeOut);
}

void DSWriteByteTo(uint8 reg, uint8 data, int timeOut)
{
    I2C0_Init();
    I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
    I2C0_MasterWriteByteTo(reg, timeOut);
    I2C0_MasterWriteByteTo(data, timeOut);
    I2C0_MasterSendStopTo(timeOut);
}

void DSReadByteTo(uint8 reg, uint8 *recvData, int timeOut)
{
    I2C0_Init();
    I2C0_MasterSendStartTo(DS_ADDR, 0, timeOut);
    I2C0_MasterWriteByteTo(reg, timeOut);
    I2C0_MasterSendRestartTo(DS_ADDR, 1, timeOut);
    *recvData = I2C0_MasterReadByteTo(I2C0_NAK_DATA, timeOut);
    I2C0_MasterSendStopTo(timeOut);
}

/* [] END OF FILE */
