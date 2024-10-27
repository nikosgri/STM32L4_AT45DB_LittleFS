/*
 * uart.c
 *
 *  Created on: Oct 27, 2024
 *      Author: Nikolaos Grigoriadis
 *      Email : n.grigoriadis09@gmail.com
 *      Title : Embedded software engineer
 *      Degree: BSc and MSc in computer science, university of Ioannina
 */


#include "uart.h"

/*
 * USART1-3 have all the features included, so their configuration is common. If you want,
 * to use UART4|UART5|LPUART1, you have to check the reference manual of the micro-controller,
 * and modify the function based on your needs.
 */
void USARTx_init(USART_TypeDef * USARTx, GPIO_TypeDef *GPIOx, int USARTx_GPIO_RX_PIN, int USARTx_GPIO_TX_PIN)
{
	uint32_t usart_div;

	/*Initialize USART GPIO pins*/
	USARTx_gpio_init(GPIOx, USARTx_GPIO_RX_PIN, USARTx_GPIO_TX_PIN);

	/*Enable clock access to the USARTx peripheral*/
	if (USARTx == USART1){
		/*Enable clock access to USART1*/
		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
	} else if (USARTx == USART2) {
		/*Enable clock access to USART2*/
		SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_USART2EN);
	} else if (USARTx == USART3) {
		/*Enable clock access to USART3*/
		SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_USART3EN);
	}

	/*Make sure that the USART peripheral is disabled before any configurations*/
	CLEAR_BIT(USARTx->CR1, USART_CR1_UE);

	/*Define the word length (8 data bits).*/
	CLEAR_BIT(USARTx->CR1, USART_CR1_M1);
	CLEAR_BIT(USARTx->CR1, USART_CR1_M0);

	/*Send data with LSB first*/
	CLEAR_BIT(USARTx->CR2, USART_CR2_MSBFIRST);

	/*Define the number of stop bits. 1 stop bit in our case.*/
	CLEAR_BIT(USARTx->CR2, USART_CR2_STOP_0);
	CLEAR_BIT(USARTx->CR2, USART_CR2_STOP_1);

	/*Enable the oversampling by 16*/
	CLEAR_BIT(USARTx->CR1, USART_CR1_OVER8);

	/*Set the BAUDRATE*/
	usart_div = calculate_u_div(BAUDRATE);
	WRITE_REG(USARTx->BRR, usart_div);

	/*Enable the transmitter*/
	SET_BIT(USARTx->CR1, USART_CR1_TE);

	/*Enable the receiver*/
	SET_BIT(USARTx->CR1, USART_CR1_RE);

	/*Enable the peripheral*/
	SET_BIT(USARTx->CR1, USART_CR1_UE);
}

uint32_t calculate_u_div(uint32_t bd)
{
	return (SYSTEM_CLK/bd);
}

void USARTx_write_byte(USART_TypeDef * USARTx, uint8_t data)
{
	/*Wait until the TX_BUFFER is empty, to prevent data overwrite*/
	while (!READ_BIT(USARTx->ISR, USART_ISR_TXE)) {}

	/*Load the data to TX_BUFFER*/
	WRITE_REG(USARTx->TDR, (data & 0xFF));

}

void USARTx_receive(USART_TypeDef * USARTx, char *data, uint16_t size)
{
	/*Buffer index*/
	int idx = 0;

	/*Loop until the size of data is zero*/
	while (size) {

		/*Wait for RXNE bit to set. This indicates that data is available in DR register*/
		while (!(READ_BIT(USARTx->ISR, USART_ISR_RXNE))) {}

		/*Receive the data from DR register*/
		data[idx] = USARTx->RDR;

		/*Decrease the size of the payload to be received*/
		size--;

		/*Increase the index size*/
		idx++;
	}
}

void USARTx_transmit(USART_TypeDef * USARTx, char *data, uint16_t size)
{
	/*Buffer index*/
	int idx = 0;

	/*Loop until the size of data becomes zero*/
	while (size) {

		/*Wait until TX buffer is empty. TXE bit is set when TX_BUFFER is empty*/
		while (!READ_BIT(USARTx->ISR, USART_ISR_TXE)) {}

		/*Load data to transmit data register (TDR)*/
		USARTx->TDR = (data[idx] & 0xFF);

		/*Decrease the size of the data*/
		size--;

		/*Increase the size of the index*/
		idx++;
	}
}

void USARTx_deinit(USART_TypeDef *USARTx)
{
	/*Disable USARTx peripheral*/
	CLEAR_BIT(USARTx->CR1, USART_CR1_UE);

	/*Disable clock access to USART peripheral*/
	if (USARTx == USART1){
		/*Disabe clock access to USART1*/
		CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
	} else if (USARTx == USART2) {
		/*Disabe clock access to USART2*/
		CLEAR_BIT(RCC->APB1ENR1, RCC_APB1ENR1_USART2EN);
	} else if (USARTx == USART3) {
		/*Disabe clock access to USART3*/
		CLEAR_BIT(RCC->APB1ENR1, RCC_APB1ENR1_USART3EN);
	}
}
