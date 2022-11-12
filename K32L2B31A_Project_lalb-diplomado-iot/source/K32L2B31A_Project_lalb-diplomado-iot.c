/* @file    K32L2B31A_Project_lalb-diplomado-iot
 * @author Ernesto Jaraba
 * @version 0.00
 * @date 05/10/2022
 * @brief   Funcion principal main.
 * @details
  			v0.00 proyecto base
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"


/// fls ////////////////////
#include "fsl_debug_console.h"
#include "fsl_adc16.h"


//// personales ///////////////

#include "lpuart0.h"


/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 *
 *
 *
 */

#define ADC16_BASE          ADC0
#define ADC16_CHANNEL_GROUP 0U
#define ADC16_USER_CHANNEL  3U

#define BOARD_LED_GPIO1     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN1 BOARD_LED_GREEN_GPIO_PIN

#define BOARD_LED_GPIO2     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN2 BOARD_LED_RED_GPIO_PIN


volatile uint32_t g_systickCounter;


///////////////////// funciones ////////////////////////
///////////////////////////////////////////////////////

void led_encendidos();

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

////////////////// definition /////////////////////////////7

typedef struct _iot_nodo_data{
	uint32_t preamble;
	uint16_t frame_sync;
	//------------------------------------
	uint32_t end_divice_ID;
	uint16_t payload;
	uint16_t auth;
	//------------------------------------
	uint16_t FCS;
	//------------------------------------
} iot_nodo_data_t;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
void ec25_print_data_raw(uint8_t *data_ptr, uint32_t data_size) {
	for (uint32_t i = 0; i < data_size; i++) {
		PRINTF("%c", *(data_ptr + i));
	}
}


int main(void) {

uint8_t	aux_recepcion = "H";

iot_nodo_data_t nuevo_formato;

nuevo_formato.preamble = 60;
nuevo_formato.frame_sync = 30;
nuevo_formato.end_divice_ID = 56;
nuevo_formato.payload = 89;
nuevo_formato.auth = 123;
nuevo_formato.FCS = 83;



    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitLEDsPins();


#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();


#endif



    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }

    while(1) {
    	if(leer_bandera_nuevo_dato()!=0){
    	escribir_bandera_nuevo_dato(0);

    	aux_recepcion = leer_dato();
    	PRINTF("%c\r\n",aux_recepcion);

    	if (aux_recepcion == 71){
    	  GPIO_PortClear(BOARD_LED_GPIO1, 1u << BOARD_LED_GPIO_PIN1); //Prender el LED
    	}

    	if (aux_recepcion == 103){
    		  GPIO_PortSet(BOARD_LED_GPIO1, 1u << BOARD_LED_GPIO_PIN1); //Apagar el LED
    	}


    	if (aux_recepcion == 82){
    	  GPIO_PortClear(BOARD_LED_GPIO2, 1u << BOARD_LED_GPIO_PIN2);
    	}

    	if (aux_recepcion == 114){
    	  GPIO_PortSet(BOARD_LED_GPIO2, 1u << BOARD_LED_GPIO_PIN2);
    	}

    	if(aux_recepcion == 76){
    		ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[0]);

    		    	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP)))
    		    	    	                	            {
    		    	    	                	            }
    		    	PRINTF("ADC Value: %d", ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP));
    	}



       }



        __asm volatile ("nop");
    }
    return 0 ;
}


void led_encendidos(){


}
