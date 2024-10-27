/*
 * spi.h
 *
 *  Created on: Apr 25, 2024
 *      Author: ngrigoriadis
 */

#ifndef SPI_H_
#define SPI_H_

#include "main.h" //Common headers
#include "gpio.h" //For SPI GPIO pins


/**
 * @brief Initialize SPIx peripheral.
 * @param sSPIx : Define the peripheral that you want to initialize.
 * @retval None.
 */
void SPIx_init(SPI_TypeDef *sSPIx, GPIO_TypeDef *GPIOx);

/**
 * @brief Disable the SPI peripheral.
 * @param sSPIx : Define the peripheral that you want to initialize.
 * @retval None.
 */
void SPIx_deinit(SPI_TypeDef *sSPIx);

/**
 * @brief Transmit data over MOSI line.
 * @param sSPIx : Define the peripheral that you want to initialize.
 * @param data : Data to be transmitted.
 * @param size : The size of data.
 * @retval None.
 */
void SPIx_transmit(SPI_TypeDef *sSPIx, uint8_t *data, uint32_t size);

/**
 * @brief Receive byte over MISO line.
 * @param sSPIx  : Define the peripheral that you want to initialize.
 */
uint8_t SPIx_receive_byte(SPI_TypeDef *sSPIx);

/**
 * @brief Receive data over MISO line.
 * @param sSPIx  : Define the peripheral that you want to initialize.
 * @param recBuf : The buffer that you want to store the received data.
 * @param size : The size of data.
 * @retval None.
 */
void SPIx_receive(SPI_TypeDef *sSPIx, uint8_t *recBuf, uint32_t size);

/**
 * @brief Enables the slave device.
 * @param GPIOx	: Select the GPIO port you need (GPIOA/GPIOB/GPIOC/GPIOD).
 */
void SPIx_enable_slave(GPIO_TypeDef *GPIOx);

/**
 * @brief Disables the slave device.
 * @param GPIOx	: Select the GPIO port you need (GPIOA/GPIOB/GPIOC/GPIOD).
 */
void SPIx_disable_slave(GPIO_TypeDef *GPIOx);

#endif /* SPI_H_ */
