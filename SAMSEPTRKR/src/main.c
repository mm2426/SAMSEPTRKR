
//#define BOARD USER_BOARD

#define LED_PIO		PIOC

#include <asf.h>
// /* RTOS includes. */
#include "task.h"
// /* App Includes */
// #include "ChargeCtrl.h"
// #include "PvTracker.h"
// #include "CommInterface.h"

/* Function Declarations */
/* Installs the RTOS interrupt handlers and starts the peripherals. */
void vInitPeripherals(void);

/* RTOS Task Declarations*/
void vBlinkTask(void *pvParameters);

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	/* There is nothing in board_init() as it is a custom board, hence we call this*/
	vInitPeripherals();
    
    /* Enable global interrupts. */   
    
//     xTaskCreate(vCCTask, "Cc", 100, NULL, 2, NULL);
//     xTaskCreate(vPvTrackerTask, "Pv", 300, NULL, 1, NULL);
//     xTaskCreate(vCommTask, "Comm", 512, NULL, 1, NULL);
	   xTaskCreate(vBlinkTask, "Blinky", 100, NULL, 1, NULL);
    
    /* Start Watchdog Timer */
        
    /*HB_RST_Write(1);
    PWM0_Start();
    PWM0_WriteCompare(1023);
    PwmSelReg_Write(0);
    while(1);*/
    
    vTaskStartScheduler();
    
	for( ;; );
	{
		/*gpio_set_pin_high(PIO_PC23_IDX);
		delay_ms(1000);
		gpio_set_pin_low(PIO_PC23_IDX);
		delay_ms(1000);*/
	}
	
}

// void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
// {
// 	/* The stack space has been execeeded for a task, considering allocating more. */
// 	taskDISABLE_INTERRUPTS();
// 	for( ;; );
// }
// 
void vApplicationMallocFailedHook( void )
{
	/* The heap space has been execeeded. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vInitPeripherals( void )
{
// 	#if defined(DEBUG_EN) || defined(LOG_EN)
// 		Debug_Start();
// 	#endif
	
	//CyDelay(1000);
	
	/* Initialize all RTOS vars */
// 	vCCInit();
// 	vPvTrackerInit();
// 	vCommInit();

	gpio_configure_pin(PIO_PC23_IDX, (PIO_OUTPUT_1 | PIO_DEFAULT));

}

void vBlinkTask( void *pvParameters )
{
	while(1)
	{
		//LED1_Write(1);
		gpio_set_pin_high(PIO_PC23_IDX);
		//UART0_PutChar('A');
		vTaskDelay(500);
		//LED1_Write(0);
		gpio_set_pin_low(PIO_PC23_IDX);
		//Debug_PutChar('A');
		vTaskDelay(500);
	}
}

