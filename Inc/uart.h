/*
 * uart.h
 *
 *  Created on: Apr 25, 2024
 *      Author: ngrigoriadis
 */

#ifndef UART_H_
#define UART_H_

#include "main.h" 	//Common headers
#include "gpio.h"   //For accessing the USART GPIOs



/**
 * @brief Initialize the USARTx peripheral (Use USART2 to print into the serial port).
 * @param USARTx : Define the peripheral that you want to initialize.
 * @param GPIOx  : Define the GPIO port of the desired pins
 * @param USARTx_GPIO_RX_PIN :  The pin number for RX.
 * @param USARTx_GPIO_TX_PIN :  The pin number for TX.
 * @retval None.
 */
void USARTx_init(USART_TypeDef * USARTx, GPIO_TypeDef *GPIOx, int USARTx_GPIO_RX_PIN, int USARTx_GPIO_TX_PIN);

/**
 * @brief Calculates the usart_div value based on oversampling by 16.
 * @param bd 	 : The desired baudrate. Change the value in the "main.h" file.
 * @retval A proper number for the BRR register to set the baudrate.
 */
uint32_t calculate_u_div(uint32_t bd);

/**
 * @brief Send a byte over USARTx peripheral.
 * @param USARTx : Define the peripheral that you want to initialize.
 * @param data   : The byte we need to send.
 */
void USARTx_write_byte(USART_TypeDef * USARTx, uint8_t data);

/**
 * @brief Receive data over RX line from USARTx peripheral.
 * @param USARTx : The desired peripheral of USART.
 * @param data   : The buffer that will hold the received data.
 * @param size   : The size of the received data.
 * @retval None.
 */
void USARTx_receive(USART_TypeDef * USARTx, char *data, uint16_t size);

/**
 * @brief Transmit data over TX line in USARTx peripheral.
 * @param USARTx : The desired peripheral of USART.
 * @param data   : The buffer that contains the data to transmit.
 * @param size   : The size of data that will be transmitted over the TX line.
 * @retval None.
 */
void USARTx_transmit(USART_TypeDef * USARTx, char *data, uint16_t size);

/**
 * @brief Deinitialize USARTx peripheral.
 * @param USARTx : The desired peripheral of USART.
 * @retval None.
 */
void USARTx_deinit(USART_TypeDef *USARTx);

#endif /* UART_H_ */
