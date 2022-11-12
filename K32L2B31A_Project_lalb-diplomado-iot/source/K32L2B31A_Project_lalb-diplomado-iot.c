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
#include "fsl_common.h"
#include "K32L2B31A.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "fsl_adc16.h"

#define BOARD_LED_GPIO_1     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN_1 BOARD_LED_GREEN_GPIO_PIN

#define BOARD_LED_GPIO_2     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN_2 BOARD_LED_RED_GPIO_PIN


volatile uint32_t g_systickCounter;

    volatile static uint8_t i = 0 ;
    volatile static float voltaje = 12.5;

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


int main(void) {


    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL

    BOARD_InitDebugConsole();
#endif





    if (SysTick_Config(SystemCoreClock / 1500U))
        {

        }


    	while(1) {


    				SysTick_DelayTicks(500U);
    		    	GPIO_PortToggle(BOARD_LED_GPIO_1, 1u << BOARD_LED_GPIO_PIN_1);
    		    	SysTick_DelayTicks(250U);
    		    	GPIO_PortToggle(BOARD_LED_GPIO_2, 1u << BOARD_LED_GPIO_PIN_2);

    			ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[0]);

    	        SysTick_DelayTicks(2000U);
    	        i++ ;

    	        while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP)))
    	                	            {
    	                	            }

    	        float Vout = (ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP) * (3.3 / 4096));

    	        //float RDLR = (10000 * (5-Vout))/Vout;
    	        float Iout = (Vout / 10000.0);


    	        float LUX =  (2 * ( 3.3 -( 3.3 / ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP) )))*100;


            	PRINTF("ADC Value: %d", ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP));
               	PRINTF("LUX Value: %f", LUX);
               	PRINTF("Vout Value: %f", Vout);
               	PRINTF("Iout Value: %f\r\n", Iout);

    	        __asm volatile ("nop");
    	    }


    return 0 ;
}
