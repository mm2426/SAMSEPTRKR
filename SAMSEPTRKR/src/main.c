
//#define BOARD USER_BOARD

#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"
// /* RTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
// /* App Includes */
// #include "ChargeCtrl.h"
#include "PvTracker.h"
#include "CommInterface.h"
#include "consoleuart.h"

/* Function Declarations */
/* Installs the RTOS interrupt handlers and starts the peripherals. */
void vInitPeripherals(void);

/* RTOS Task Declarations*/
void vBlinkTask(void *pvParameters);

/*	*** Updates ***
	Modified MCK settings to:
		PLLA SRC = external XTAL.
		PLLA MUL = 20.
		PLLA DIV = 1.
		MCK SRC = PLLA Out = 240/2 = 120MHz.

	Added RS485, Modbus, TWI, NVM, DS3231RTOS and ICM20648RTOS libraries.
	Ported Tracker and Modbus Tasks.
	Code untested. 

*/

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	/* Initialize all peripherals */
	board_init();
	delay_init(sysclk_get_cpu_hz());
	
	vInitPeripherals();

	/* Enable global interrupts. */   
    
//     xTaskCreate(vCCTask, "Cc", 100, NULL, 2, NULL);
    xTaskCreate(vPvTrackerTask, (const signed char *)"Pv", 300, NULL, 1, NULL);
    xTaskCreate(vCommTask, (const signed char *)"Comm", 512, NULL, 1, NULL);
	xTaskCreate(vBlinkTask, (const signed char *)"Blinky", 100, NULL, 1, NULL);
    
    /* Start Watchdog Timer */
        
    vTaskStartScheduler();
    
	for( ;; );
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName )
{
	/* The stack space has been execeeded for a task, considering allocating more. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationMallocFailedHook( void )
{
	/* The heap space has been execeeded. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vInitPeripherals( void )
{
	#if defined(DEBUG_EN) || defined(LOG_EN)
		ConsoleInit();
	#endif
	
	gpio_set_pin_low(PIN_LDOEN_IDX);
	delay_ms(500);
	/* Enable LDO */
	gpio_set_pin_high(PIN_LDOEN_IDX);
	delay_ms(1000);

	/* Initialize all RTOS vars */
	//vCCInit();
	vPvTrackerInit();
	vCommInit();
}

void vBlinkTask( void *pvParameters )
{
	while(1)
	{
		gpio_set_pin_high(PIN_DEBUGLED_IDX);
		vTaskDelay(500 / portTICK_RATE_MS);
		gpio_set_pin_low(PIN_DEBUGLED_IDX);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

