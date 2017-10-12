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

#include "ChargeCtrl.h"
#include "PvTracker.h"

enum dacStates
{
    readPanV, readBatV, readBatI, stop
};

enum ccState
{
    testPv, regCvcc, sleepCC, testBat
};

/* Charge controller shared variables */
volatile enum dacStates adc0State = readBatV;
volatile int32 PvMVolts = 0;
volatile int32 BiMVolts = 0, BvMVolts = 0;
SemaphoreHandle_t adcSem;

extern uint16 mBusRegs[MBUS_MAX_REGS];

uint8 ccEn = 1, ccStatus = 1;

CY_ISR(ADC0Isr)
{
    ISR_ADC0_ClearPending();
    switch(adc0State)
    {
        case readPanV:
            PvMVolts = ADC0_CountsTo_mVolts(ADC0_GetResult32());
            AMux0_Select(ADC0_CH_BV);
            adc0State = readBatV;
            ADC0_StartConvert();
            break;
        case readBatV:
            BvMVolts = ADC0_CountsTo_mVolts(ADC0_GetResult32());
            AMux0_Select(ADC0_CH_BI);
            adc0State = readBatI;
            ADC0_StartConvert();
            break;
       case readBatI:
            BiMVolts = ADC0_CountsTo_mVolts(ADC0_GetResult32());
            adc0State = stop;
            /* Signal BSem */
            xSemaphoreGiveFromISR(adcSem, NULL);
            break;
    }
}

void vCCTask(void *pvParameters)
{
    int32 Pv = 0;
    int32 Bi = 0, Bv = 0;
    int32 errV = 0, errI = 0;
    uint16 dacVal = 2048;
    enum ccState cState = testPv;
    uint8 ctr = 0;
    float val = 0;
    uint16 *ptr = (uint16 *)&val;
    
    OPRLY_Write(1);
    /*Enable_Write(1);
    ccEn = 1;
    adc0State = readBatV;
    AMux0_Select(ADC0_CH_PV);
    ADC0_StartConvert();*/
    
    while(1)
    {
        //Wait on BSem for signal
        if(xSemaphoreTake(adcSem, pdMS_TO_TICKS(1000)) == pdPASS)
        {
            ccStatus = 1;
            //Convert ADC vals into V & I
            Pv = (((float)PvMVolts)*22.2717f);
            Bv = (((float)BvMVolts)*22.2717f);
            Bi = (((float)BiMVolts/512.0f)*1000.0f);
            ctr++;
            if(ctr == 50)
            {
                val = Pv/1000.0f;
                mBusRegs[MBUS_REG_PVVH] = ptr[1];
                mBusRegs[MBUS_REG_PVVL] = ptr[0];
                val = Bv/1000.0f;
                mBusRegs[MBUS_REG_BATVH] = ptr[1];
                mBusRegs[MBUS_REG_BATVL] = ptr[0];
                ctr = 0;
            }
            
            switch(cState)
            {
                case testPv:
                    if(Pv >= CC_PV_THRESHOLD)
                    {
                        cState = regCvcc;
                        //Enable CC
                        Enable_Write(1);
                        //Set Initial DAC Value
                        dacVal = 2048;
                        DAC0_SetValue(dacVal);
                    }
                    break;
                case regCvcc:
                    if(Bi < CC_BI_SETPOINT)
                    {
                        errV = Bv - CC_BV_SETPOINT;
                        //Operate in Constant Voltage Mode
                        if((errV) > 10)
                        {
                            dacVal = dacVal + 10;
                            //Clamp DAC Output to MAX Val
                            if(dacVal>4000)
                                dacVal = 4000;
                            
                            DAC0_SetValue(dacVal);    
                        }
                        else if(errV < -10)
                        {
                            dacVal = dacVal - 10;
                            //Clamp DAC Output to Min Val, if ovf occorred
                            if(dacVal > 4000)
                                dacVal = 1;
                            
                            DAC0_SetValue(dacVal);
                        }
                    }
                    else
                    {
                        errI = Bi - CC_BI_SETPOINT;
                        //Operate in Constant Current Mode
                        if((errI) > 10)
                        {
                            dacVal = dacVal + 10;
                            //Clamp DAC Output to MAX Val
                            if(dacVal>4000)
                                dacVal = 4000;
                                
                            DAC0_SetValue(dacVal);    
                        }
                        else if(errI < - 10)
                        {
                            dacVal = dacVal - 10;
                            //Clamp DAC Output to Min Val, if ovf occorred
                            if(dacVal > 4000)
                                dacVal = 1;
                            
                            DAC0_SetValue(dacVal);
                        } 
                    }
                    
                    /*if(Bi < CC_BI_CUTOFF)
                    {
                        cState = testBat;
                        //Disable CC
                        Enable_Write(0);
                    }*/
                    if(Pv < CC_PV_THRESHOLD)
                    {
                        cState = sleepCC;
                        //Disable CC
                        Enable_Write(0);
                    }
                    break;
                case sleepCC:
                    //In sufficient Panel V, retry after delay
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    cState = testPv;
                    break;
                case testBat:
                    if(Bv < CC_BV_THRESHOLD)
                    {
                        cState = testPv;
                    }
                    break;
            }
            
            if(ccEn)
            {
                //Initiate ADC0 Conversion
                adc0State = readPanV;
                AMux0_Select(ADC0_CH_PV);
                ADC0_StartConvert();
            }
            else
            {
                Enable_Write(0);
            }
        }
        else
        {
            if(ccEn)
            {
                cState = testPv;
                adc0State = readPanV;
                //Initiate ADC0 Conversion
                AMux0_Select(ADC0_CH_PV);
                ADC0_StartConvert();
            }
            ccStatus = 0;
        }
    }
}

void vCCInit(void)
{
    /* ISR ADC0 EOC */
    ISR_ADC0_StartEx(ADC0Isr);    
    adcSem = xSemaphoreCreateBinary();
    
    /* Init All Peripherals */
    DAC0_Start();
    OpAmp0_Start();
    DAC0_SetValue(2500);
    ADC0_Start();
    
    //ADC1_Start();
    //Thermister0_Start();
}

/* [] END OF FILE */
