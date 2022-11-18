/*! @file : leds.c
 * @author  Ernesto Jaraba
 * @version V0.02
 * @date    Nov 15, 2022
 * @brief   Driver para 
 * @details
 *
*/
/***************************
* Includes
 **********/
#include "leds.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*********
 * Definitions
 **********/
#define BOARD_LED_GPIO1     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN1 BOARD_LED_GREEN_GPIO_PIN

#define BOARD_LED_GPIO2     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN2 BOARD_LED_RED_GPIO_PIN

/*********
 * Private Prototypes
 **********/


/*********
 * External vars
 **********/


/*********
 * Local vars
 **********/


/*********
 * Private Source Code
 **********/


/*********
 * Public Source Code
 **********/
 
 void led_on_green(){
	 GPIO_PortClear(BOARD_LED_GPIO1, 1u << BOARD_LED_GPIO_PIN1);

 }

 void led_off_green(){
	 GPIO_PortSet(BOARD_LED_GPIO1, 1u << BOARD_LED_GPIO_PIN1);

  }

 void led_on_red(){
	 GPIO_PortClear(BOARD_LED_GPIO2, 1u << BOARD_LED_GPIO_PIN2);

  }


 void led_off_red(){
	 GPIO_PortSet(BOARD_LED_GPIO2, 1u << BOARD_LED_GPIO_PIN2);

   }
