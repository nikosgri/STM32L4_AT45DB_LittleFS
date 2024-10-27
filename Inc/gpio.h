/*
 * gpio.h
 *
 *  Created on: Apr 25, 2024
 *      Author: ngrigoriadis
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "main.h" //Common headers


/*Define the GPIO pin numbers for USART2 peripheral*/
#define _USART2_GPIO_RX_PIN		3U  //PA3 is USART2 RX pin.
#define _USART2_GPIO_TX_PIN		2U  //PA2 is USART2 TX pin.
/*Define the GPIO pin numbers for USART1 peripheral*/
#define _USART1_GPIO_RX_PIN		10U //PA10 is USART1 RX pin.
#define _USART1_GPIO_TX_PIN		9U  //PA9 is USART1 TX pin.
/*Define the GPIO pin numbers for SPIx peripheral*/
#define SPIx_GPIO_MISO_PIN		14U //PB14 is SPI2 MISO pin.
#define SPIx_GPIO_MOSI_PIN		15U //PB15 is SPI2 MOSI pin.
#define SPIx_GPIO_CLK_PIN		13U //PB13 is SPI2 CLK pin.
#define SPIx_GPIO_CS_PIN		9U  //PB9 is SPI2 CS pin.



/**
 * @brief Initialize GPIO pins for USART peripheral.
 * Select the USART peripheral you want to initialize, and
 * modify the above parameters that illustrates the pin numbers
 * of the USART peripheral. Set them properly based on your pin numbers.
 * @param GPIOx	: Select the GPIO port you need (GPIOA/GPIOB/GPIOC/GPIOD).
 * @param USARTx_GPIO_RX_PIN :  The pin number for RX.
 * @param USARTx_GPIO_TX_PIN :  The pin number for TX.
 * @retval None.
 */
void USARTx_gpio_init(GPIO_TypeDef *GPIOx, int USARTx_GPIO_RX_PIN, int USARTx_GPIO_TX_PIN);

/**
 * @brief Initialize GPIO pins for SPI peripheral.
 * @param GPIOx	   : Select the GPIO port you need (GPIOA/GPIOB/GPIOC/GPIOD).
 * @param SPI_mode : Set the SPI mode you want (1/2/3).
 * @retval None.
 */
void SPIx_gpio_init(GPIO_TypeDef *GPIOx, uint16_t SPI_mode);


#endif /* GPIO_H_ */
