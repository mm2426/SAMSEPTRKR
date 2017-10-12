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

#include "project.h"
#include "I2CTO_custom.h"

#ifndef __ICM20648_H__
    #define __ICM20648_H__          1
    
    //I2C Register Definitions
    //User Bank 0 Regs
    #define ICM_REG_WHO_AM_I            0x00
    #define ICM_REG_USER_CTRL           0x03
    #define ICM_REG_LP_CONFIG           0x05
    #define ICM_REG_PWR_MGMT1           0x06
    #define ICM_REG_PWR_MGMT2           0x07
    #define ICM_REG_INT_PIN_CFG         0x0F
    #define ICM_REG_INT_ENABLE          0x10
    #define ICM_REG_INT_ENABLE1         0x11
    #define ICM_REG_INT_ENABLE2         0x12
    #define ICM_REG_INT_ENABLE3         0x13
    #define ICM_REG_DELAY_TIMEH         0x28
    #define ICM_REG_DELAY_TIMEL         0x29
    #define ICM_REG_ACC_XOUTH           0x2D
    #define ICM_REG_ACC_XOUTL           0x2E
    #define ICM_REG_ACC_YOUTH           0x2F
    #define ICM_REG_ACC_YOUTL           0x30
    #define ICM_REG_ACC_ZOUTH           0x31
    #define ICM_REG_ACC_ZOUTL           0x32
    #define ICM_REG_BANK_SEL            0x7F
    //User Bank 1 Regs
    #define ICM_REG_SLF_TST_XACC        0x0E
    #define ICM_REG_SLF_TST_YACC        0x0F
    #define ICM_REG_SLF_TST_ZACC        0x10
    #define ICM_REG_OFFSET_XAH          0x14
    #define ICM_REG_OFFSET_XAL          0x15
    #define ICM_REG_OFFSET_YAH          0x17
    #define ICM_REG_OFFSET_YAL          0x18
    #define ICM_REG_OFFSET_ZAH          0x1A
    #define ICM_REG_OFFSET_ZAL          0x1B
    //User Bank 2 Regs
    #define ICM_REG_ODR_ALIGN_EN        0x09
    #define ICM_REG_ACC_SMPLRT_DIV1     0x10
    #define ICM_REG_ACC_SMPLRT_DIV2     0x11
    #define ICM_REG_ACC_INTEL_CTRL      0x12
    #define ICM_REG_ACC_WOM_THR         0x13
    #define ICM_REG_ACC_CONFIG          0x14
    #define ICM_REG_ACC_CONFIG2         0x15
    #define ICM_REG_FSYNC_CONFIG        0x52
    #define ICM_REG_MOD_CTRL_USR        0x54
    //User Bank 3 Regs
    //Not required
        
    void ICMInitializeTo(uint8 sla, int timeOut);
    void ICMReadAccDataXTo(uint8 sla, uint16 *recvData, int timeOut);
    void ICMReadAccDataYTo(uint8 sla, uint16 *recvData, int timeOut);
    void ICMReadAccDataZTo(uint8 sla, uint16 *recvData, int timeOut);
    void ICMReadAccDataAllTo(uint8 sla, uint16 *recvData, int timeOut);
    
    void ICMWriteByteTo(uint8 sla, uint8 reg, uint8 data, int timeOut);
    void ICMReadByteTo(uint8 sla, uint8 reg, uint8 *recvData, int timeOut);
        
#endif

/* [] END OF FILE */
