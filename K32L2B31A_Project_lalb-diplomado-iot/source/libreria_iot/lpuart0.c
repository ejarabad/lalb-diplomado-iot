/*! @file : uard0.c
 * @author  Ernesto Jaraba
 * @version v0.02
 * @date    11/11/2022
 * @brief   Driver para 
 * @details
 *
*/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"

#include "lpuart0.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPUART0_BUFFER_SIZE_MAX	60
/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Local vars
 ******************************************************************************/
static uint8_t dato_lpuart0[LPUART0_BUFFER_SIZE_MAX];
static uint8_t dato_mierda[LPUART0_BUFFER_SIZE_MAX];
static uint8_t flag_nuevo_dato_lpuart0=0;
static uint8_t dato_lpuart0_index=0;
static uint32_t g_systickCounter;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
/* LPUART0_IRQn interrupt handler */
void LPUART0_SERIAL_RX_TX_IRQHANDLER(void) {
  uint32_t intStatus;

  /* captura banderas de estado para el puerto LPUART0
   * esta lectura tambien borra las banderas de estado*/
  intStatus = LPUART_GetStatusFlags(LPUART0_PERIPHERAL);

  /* Verifica que la IRQ es por llegada de nuevo dato por Rx*/
	if ((kLPUART_RxDataRegFullFlag) & intStatus) {
		dato_lpuart0[dato_lpuart0_index] = LPUART_ReadByte(LPUART0);
		dato_lpuart0_index++;
		if(dato_lpuart0_index>LPUART0_BUFFER_SIZE_MAX){
			dato_lpuart0_index=0;
		}
		flag_nuevo_dato_lpuart0=1;
	}

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

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

/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
uint8_t lpuart0_leer_dato(void) {
	if(dato_lpuart0_index!=0){
		return (dato_lpuart0[dato_lpuart0_index-1]);
	}else{
		return(0x00);
	}
}

uint8_t lpuart0_leer_bandera_nuevo_dato(void) {
	return (flag_nuevo_dato_lpuart0);

}

void lpuart0_escribir_bandera_nuevo_dato(uint8_t nuevo_valor) {
	flag_nuevo_dato_lpuart0 = nuevo_valor;
}

void lpuart0_borrar_buffer(void){
	for(uint8_t i; i<LPUART0_BUFFER_SIZE_MAX; i++){
		dato_lpuart0[i]=0x00;
	}
	dato_lpuart0_index=0;
}

void analizar_buffer_sin_errores(void){

	int i = 0;
	int j = 0;

		for( i=0; i<LPUART0_BUFFER_SIZE_MAX-1; i++){

					if(dato_lpuart0[i] == 79 && dato_lpuart0[i+1] == 75){
						printf("El mensaje no presenta errores\r\n");
						lpuart0_borrar_buffer();
						break;
				}
}


uint8_t leer_dato(void){

	return(dato_lpuart0);
}

uint8_t leer_bandera_nuevo_dato(void){
	 return(flag_nuevo_dato_lpuart0);
 }

 void  escribir_bandera_nuevo_dato(uint8_t nuevo_valor){
 	 flag_nuevo_dato_lpuart0 = nuevo_valor;
  }
}
