/*
 * spi.c
 *
 *  Created on: Apr 25, 2024
 *      Author: ngrigoriadis
 */


#include "spi.h"


/*Buffer for SPI incoming messages*/
uint8_t SPI_REC_BUF[SPI_REC_BUFF_SIZE];

void SPIx_init(SPI_TypeDef *sSPIx, GPIO_TypeDef *GPIOx)
{

	/*Initialize the SPI GPIO pins*/
	SPIx_gpio_init(GPIOx, SPI_MODE);

	/*Enable clock access to the SPIx peripheral*/
	if (sSPIx == SPI1) {
		/*Enable clock access to SPI1*/
		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
	} else if (sSPIx == SPI2) {
		/*Enable clock access to SPI2*/
		SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_SPI2EN);
	} else if (sSPIx == SPI3) {
		/*Enable clock access to SPI3*/
		SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_SPI3EN);
	}

	/*Disable SPIx before make any changes (just in case).*/
	CLEAR_BIT(sSPIx->CR1, SPI_CR1_SPE);

	/**
	 * Since the peripheral that we need to communicate has 5MHz max speed we need
	 * to divide our clock to a valid number (16/4 = 4)
	*/
	MODIFY_REG(sSPIx->CR1, SPI_CR1_BR, (0x06 << SPI_CR1_BR_Pos));

	/*Set the idle state of the clock to be low (0)*/
	CLEAR_BIT(sSPIx->CR1, SPI_CR1_CPOL);

	/*Enable the data capture at rising edge of the clock*/
	CLEAR_BIT(sSPIx->CR1, SPI_CR1_CPHA);

	/*Enable master mode*/
	SET_BIT(sSPIx->CR1, SPI_CR1_MSTR);

	/*Set the MSB to transfer first*/
	CLEAR_BIT(sSPIx->CR1, SPI_CR1_LSBFIRST);

	/*Enable software slave management*/
	SET_BIT(sSPIx->CR1, SPI_CR1_SSM);
	SET_BIT(sSPIx->CR1, SPI_CR1_SSI);

	/*Enable full-duplex mode*/
	CLEAR_BIT(sSPIx->CR1, SPI_CR1_RXONLY);

	/*Set the RXNE event threshold to 1/2*/
	SET_BIT(sSPIx->CR2, SPI_CR2_FRXTH);

	/*Set 8-bit data size*/
	MODIFY_REG(sSPIx->CR2, SPI_CR2_DS, (0x07 << SPI_CR2_DS_Pos));

	/*Disable chip select pin*/
	SPIx_disable_slave(GPIO_SPIx);

	/*Enable the peripheral*/
	SET_BIT(sSPIx->CR1, SPI_CR1_SPE);

}

void SPIx_deinit(SPI_TypeDef *sSPIx)
{
	uint32_t i=0; //Iterations

	/*Wait until no more data is to be transmitted*/
	while ((READ_BIT(sSPIx->SR, SPI_SR_FTLVL)) != (0x00)) {}

	/*Wait until the last data frame is processed*/
	while (READ_BIT(sSPIx->SR, SPI_SR_BSY) != (0x00)) {}

	/*Disable the SPI peripheral*/
	CLEAR_BIT(sSPIx->CR1, SPI_CR1_SPE);

	/*Read any last data if its any*/
	while (READ_BIT(sSPIx->SR, SPI_SR_FRLVL) != (0x00)) {

		/*Read what data that has been stack iSnto the RX_BUFFER*/
		SPI_REC_BUF[i] = sSPIx->DR;

		i++;
	}

}


void SPIx_transmit(SPI_TypeDef *sSPIx, uint8_t *data, uint32_t size)
{
	int i=0; //Iterations

	while ( i < size) {

		/*Wait until TXE bit is set, which indicates TX_FIFO is empty*/
		while (!(READ_BIT(sSPIx->SR, SPI_SR_TXE))) {}

		/*Load the data to the TX_FIFO, in order to transmit data to shift register*/
		*((volatile uint8_t *) &sSPIx->DR) = data[i];

		i++;
	}

	/*Wait until TXE is set (TX_FIFO is empty)*/
	while (!(READ_BIT(sSPIx->SR, SPI_SR_TXE))) {}

	/*Wait for BSY to reset (Bus is free)*/
	while (READ_BIT(sSPIx->SR, SPI_SR_BSY)) {}

	/*	Clearing the OVRN = Overrun flag.
	 * 	An overrun condition occurs when data is received by a master or slave and the RXFIFO
	 *	has not enough space to store this received data. This can happen if the software
	 *	did not have enough time to read the previously received data.
	 */

	(void)(sSPIx->DR);
	(void)(sSPIx->SR);
}

uint8_t SPIx_receive_byte(SPI_TypeDef *sSPIx)
{
	uint8_t rec_data;

	/*Wait for the bus to be free*/
	while (READ_BIT(sSPIx->SR, SPI_SR_BSY)) {}

	/*Load dummy data to DR register*/
	*((volatile uint8_t *) &sSPIx->DR) = 0xFF;

	/*Wait for RXNE to become 1, means RX_FIFO has data to be read*/
	while (!(READ_BIT(sSPIx->SR, SPI_SR_RXNE))) {}

	rec_data = *((volatile uint8_t *) &sSPIx->DR);

	return rec_data;
}

void SPIx_receive(SPI_TypeDef *sSPIx, uint8_t *recBuf, uint32_t size)
{
	for (int i=0;i<size;i++)
	{
		recBuf[i] = SPIx_receive_byte(sSPIx);
	}
}


void SPIx_enable_slave(GPIO_TypeDef *GPIOx)
{
	/*High to low transaction of CS pin enables the slave devise*/
	CLEAR_BIT(GPIOx->ODR, (1U<<SPIx_GPIO_CS_PIN));
}

void SPIx_disable_slave(GPIO_TypeDef *GPIOx)
{
	/*Low to high transaction of CS pin disables the slave device*/
	SET_BIT(GPIOx->ODR, (1U<<SPIx_GPIO_CS_PIN));
}
