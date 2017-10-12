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

#ifndef __CHARGE_CTRL_H__
    #define __CHARGE_CTRL_H__   1
    
    #define ADC0_CH_PV          0
    #define ADC0_CH_PI          1
    #define ADC0_CH_BV          2
    #define ADC0_CH_BI          3
    
    /* Charge Controller Params in mVolts */
    /* If Pv > threshold, then charge */
    #define CC_PV_THRESHOLD     28000
    /* Bat float V */
    #define CC_BV_SETPOINT      27200
    /* Bat bulk I in mA */
    #define CC_BI_SETPOINT      1000
    /* Bat full charge cutoff I in mA */
    #define CC_BI_CUTOFF        50
    /* If Bv < threshold, then charge */
    #define CC_BV_THRESHOLD     27000
    
    #include "project.h"
    #include "FreeRTOS.h"
    #include "semphr.h"
    #include "task.h"
    
    /* ADC0 EOC ISR */
    CY_ISR_PROTO(ADC0Isr);
    /* RTOS Task Declarations*/
    void vCCTask(void *pvParameters);
    /*General Function Declarations*/
    /* Initialize CC Variables */
    void vCCInit(void);
    
#endif

/* [] END OF FILE */
