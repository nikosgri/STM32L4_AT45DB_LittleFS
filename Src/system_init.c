/*
 * system_init.c
 *
 *  Created on: Apr 24, 2024
 *      Author: ngrigoriadis
 */


#include "system_init.h"


void rcc_init(void)
{
	/*Enable the HSI clock in RCC register*/
	SET_BIT(RCC->CR, RCC_CR_HSION);

	/*Wait until the clock is ready*/
	while (!READ_BIT(RCC->CR, RCC_CR_HSIRDY)) {}

	/*Select the HSI as system core clock*/
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, (0x01<<RCC_CFGR_SW_Pos));

	/*Wait for HSI to be the system clock*/
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {}

	/*Update the SystemCoreClock variable. This is for debugging.
	 * Copy the below parameter to live expressions and see the
	 * the MHz of the system clock*/
	SystemCoreClockUpdate();
}

/*
 * Our clock is HSI = 16MHz. It means that we have 16 000 000  cycles per seconds.
 * 1ms = 1000 seconds, that means we shall divide the FCLK / 16000 because 16 000 000 / 16 000 = 1000.
 * Loading that value to SysTick Reload register we are going to have an "interrupr" into the COUNTFLAG every 1 ms.
 */
void delay_ms(int delay)
{

	/*Load the starting value of SysTick counter*/
	SysTick->LOAD = 16000 - 1;

	/*Clear the previous value if any*/
	SysTick->VAL = 0;

	/*Select the internal clock source of the SysTick timer*/
	SET_BIT(SysTick->CTRL, SYSTICK_CLK_SRC);

	/*Enable the SysTick timer*/
	SET_BIT(SysTick->CTRL, SYSTICK_ENABLE);

	for (uint16_t i=0; i < delay; i++) {

		/*Wait for the COUNTFLAG to set*/
		while (!READ_BIT(SysTick->CTRL, SYSTICK_CTN_FLAG)) {}
	}
}
