/* @file    K32L2B31A_Project_lalb-diplomado-iot
 * @author Ernesto Jaraba
 * @version 0.00
 * @date 05/10/2022
 * @brief   Funcion principal main.
 * @details
  			v0.00 proyecto base
 */

#include <stdio.h>
#include <stdlib.h>
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
volatile uint32_t g_systickCounter;
enum {
	FSM_ESTADO_INICIO = 0,
	FSM_ESTADO_ESPERA_NUEVO_DATO_LPUART0,
	FSM_ESTADO_ANALIZA_NUEVO_DATO_LPUART0,
	FSM_ESTADO_ENVIAR_COMANDO_ATI,
	FSM_ESTADO_ENVIAR_COMANDO_GMI,
	FSM_ESTADO_ENVIAR_COMANDO_GMM,
	FSM_ESTADO_ENVIAR_COMANDO_GMR,
	FSM_ESTADO_ENVIAR_COMANDO_CGMI,
	FMS_ESTADO_ESPERA_BOOFER,
	FSM_ESTADO_ANALIZA_BOFFER,
	FSM_ESTADO_BORRAR_BOFFER
};

enum {
	CMD_AT_ATI_Display_Product_Identification_Information = 0,
	CMD_AT_AT_GMI_Request_Manufacturer_Identification,
	CMD_AT_AT_GMM_Request_TA_Model_Identification,
	CMD_AT_AT_GMR_Request_TA_Revision_Identification_of_Software_Release,
	CMD_AT_AT_CGMI_Request_Manufacturer_Identification,
};

/*******************************************************************************
 ******************************************************************************/
uint8_t fst_estado_actual = FSM_ESTADO_INICIO;

/* Force the counter to be placed into memory. */

const char *cmd_at[5] = { "ATI\r\n",      //q
		"AT+GMI\r\n",   //w
		"AT+GMM\r\n",   //e
		"AT+GMR\r\n",   //r
		"AT+CGMI\r\n",  //t
		};

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/

int main(void) {
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	while (1) {
		switch (fst_estado_actual) {
		case FSM_ESTADO_INICIO:
			fst_estado_actual = FSM_ESTADO_ESPERA_NUEVO_DATO_LPUART0;
			break;

		case FSM_ESTADO_ESPERA_NUEVO_DATO_LPUART0:
			if (lpuart0_leer_bandera_nuevo_dato() != 0) {
				lpuart0_escribir_bandera_nuevo_dato(0);
				fst_estado_actual = FSM_ESTADO_ANALIZA_NUEVO_DATO_LPUART0;
			}
			break;

		case FSM_ESTADO_ANALIZA_NUEVO_DATO_LPUART0:
			switch (lpuart0_leer_dato()) {
			case 'z':
				fst_estado_actual = FSM_ESTADO_ENVIAR_COMANDO_ATI;
				break;
			case 'O':
				fst_estado_actual = FSM_ESTADO_ENVIAR_COMANDO_GMI;
				break;
			case 'y':
				fst_estado_actual = FSM_ESTADO_ENVIAR_COMANDO_GMM;
				break;
			case 'w':
				fst_estado_actual = FSM_ESTADO_ENVIAR_COMANDO_GMR;
				break;
			case 'p':
				fst_estado_actual = FSM_ESTADO_ENVIAR_COMANDO_CGMI;
				break;
			case 'b':
				lpuart0_borrar_buffer();
				printf("BUFFER BORRADO");
				break;

			default:	//estado ilegal
				fst_estado_actual = FSM_ESTADO_INICIO;
				break;

			}
			break;
		case FSM_ESTADO_ENVIAR_COMANDO_ATI:
			lpuart0_borrar_buffer();
			PRINTF("%s",
					cmd_at[CMD_AT_ATI_Display_Product_Identification_Information]);
			fst_estado_actual = FMS_ESTADO_ESPERA_BOOFER;
			break;

		case FSM_ESTADO_ENVIAR_COMANDO_GMI:
			lpuart0_borrar_buffer();
			PRINTF("%s",
					cmd_at[CMD_AT_AT_GMI_Request_Manufacturer_Identification]);
			fst_estado_actual = FMS_ESTADO_ESPERA_BOOFER;
			break;

		case FSM_ESTADO_ENVIAR_COMANDO_GMM:
			lpuart0_borrar_buffer();
			PRINTF("%s", cmd_at[CMD_AT_AT_GMM_Request_TA_Model_Identification]);
			fst_estado_actual = FMS_ESTADO_ESPERA_BOOFER;
			break;

		case FSM_ESTADO_ENVIAR_COMANDO_GMR:
			lpuart0_borrar_buffer();
			PRINTF("%s",
					cmd_at[CMD_AT_AT_GMR_Request_TA_Revision_Identification_of_Software_Release]);
			fst_estado_actual = FMS_ESTADO_ESPERA_BOOFER;
			break;

		case FSM_ESTADO_ENVIAR_COMANDO_CGMI:
			lpuart0_borrar_buffer();
			PRINTF("%s",
					cmd_at[CMD_AT_AT_CGMI_Request_Manufacturer_Identification]);
			fst_estado_actual = FMS_ESTADO_ESPERA_BOOFER;
			break;

		case FMS_ESTADO_ESPERA_BOOFER:
			if (SysTick_Config(SystemCoreClock / 1000U))
				;
			{
				SysTick_DelayTicks(1500U);
			}
			fst_estado_actual = FSM_ESTADO_ANALIZA_BOFFER;
			break;

		case FSM_ESTADO_ANALIZA_BOFFER:
			if (lpuart0_leer_bandera_nuevo_dato() != 0) {
				lpuart0_escribir_bandera_nuevo_dato(1);
				analizar_buffer_sin_errores();
				fst_estado_actual = FSM_ESTADO_INICIO;
			}

			break;

		default:	//estado ilegal
			fst_estado_actual = FSM_ESTADO_INICIO;
			break;

		}
	}
	return 0;
}

