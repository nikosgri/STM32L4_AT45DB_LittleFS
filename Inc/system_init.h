/*
 * system_init.h
 *
 *  Created on: Apr 24, 2024
 *      Author: ngrigoriadis
 */

#ifndef SYSTEM_INIT_H_
#define SYSTEM_INIT_H_

#include "main.h"  //Common headers

/*SysTick CTRL registers*/
#define SYSTICK_CTN_FLAG	(1U<<16) //It is raised every time the counter reaches 0 from reload value.
#define SYSTICK_CLK_SRC		(1U<<2)  //Setting the clock source of the counter (internal=1, external=0).
#define SYSTICK_ENABLE		(1U<<0)  //Enables the counter.

/**
 * @brief Initialize the system clock at 16MHz.
 */
void rcc_init(void);

/**
 * @brief Use the SysTick timer to generate delay in milliseconds.
 * @param delay : The milliseconds.
 * @retval None.
 */
void delay_ms(int delay);

#endif /* SYSTEM_INIT_H_ */
