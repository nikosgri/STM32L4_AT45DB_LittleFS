/*
 * GPIO.C
 *
 *  Created on: Apr 25, 2024
 *      Author: ngrigoriadis
 */


#include "gpio.h"


void USARTx_gpio_init(GPIO_TypeDef *GPIOx, int USARTx_GPIO_RX_PIN, int USARTx_GPIO_TX_PIN)
{
	if (GPIOx == GPIOA){
		/*Enable clock access to GPIO port A*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);
	} else if (GPIOx == GPIOB){
		/*Enable clock access to GPIO port B*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);
	} else if (GPIOx == GPIOC){
		/*Enable clock access to GPIO port C*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);
	} else if (GPIOx == GPIOD){
		/*Enable clock access to GPIO port D*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIODEN);
	}

					/**** RX pin ****/
	/*Set the pin as alternate function mode*/
	SET_BIT(GPIOx->MODER, (1U<<(USARTx_GPIO_RX_PIN*2+1)));
	CLEAR_BIT(GPIOx->MODER, (1U<<(USARTx_GPIO_RX_PIN*2)));

	/*Set the output type as push-pull*/
	CLEAR_BIT(GPIOx->OTYPER, (1U<<USARTx_GPIO_RX_PIN));

	/*No pull-up, no pull-down*/
	CLEAR_BIT(GPIOx->PUPDR, (1U<<(USARTx_GPIO_RX_PIN*2+1)));
	CLEAR_BIT(GPIOx->PUPDR, (1U<<(USARTx_GPIO_RX_PIN*2)));

	/*Every USARTx, x=1,2,3 have their AF=7*/
	if (USARTx_GPIO_RX_PIN>7){
		/*Configuring GPIO alternate function high register*/
		if (USARTx_GPIO_RX_PIN==8) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL8, (0x07 << GPIO_AFRH_AFSEL8_Pos));
		} else if (USARTx_GPIO_RX_PIN == 9) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL9, (0x07 << GPIO_AFRH_AFSEL9_Pos));
		} else if (USARTx_GPIO_RX_PIN == 10) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL10, (0x07 << GPIO_AFRH_AFSEL10_Pos));
		} else if (USARTx_GPIO_RX_PIN == 11) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL11, (0x07 << GPIO_AFRH_AFSEL11_Pos));
		} else if (USARTx_GPIO_RX_PIN == 12) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL12, (0x07 << GPIO_AFRH_AFSEL12_Pos));
		} else if (USARTx_GPIO_RX_PIN == 13) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL13, (0x07 << GPIO_AFRH_AFSEL13_Pos));
		} else if (USARTx_GPIO_RX_PIN == 14) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL14, (0x07 << GPIO_AFRH_AFSEL14_Pos));
		} else if (USARTx_GPIO_RX_PIN == 15) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL15, (0x07 << GPIO_AFRH_AFSEL15_Pos));
		}
	}
#if USARTx_GPIO_RX_PIN < 8
	else
	{
		SET_BIT(GPIOx->AFR[0], (1U<<(USARTx_GPIO_RX_PIN*4)));
		SET_BIT(GPIOx->AFR[0], (1U<<(USARTx_GPIO_RX_PIN*4+1)));
		SET_BIT(GPIOx->AFR[0], (1U<<(USARTx_GPIO_RX_PIN*4+2)));
		CLEAR_BIT(GPIOx->AFR[0], (1U<<(USARTx_GPIO_RX_PIN*4+3)));
	}
#endif
					/**** TX pin ****/
	/*Set the pin as alternate function mode*/
	SET_BIT(GPIOx->MODER, (1U<<(USARTx_GPIO_TX_PIN*2+1)));
	CLEAR_BIT(GPIOx->MODER, (1U<<(USARTx_GPIO_TX_PIN*2)));

	/*Set the output type as push-pull*/
	CLEAR_BIT(GPIOx->OTYPER, (1U<<USARTx_GPIO_TX_PIN));

	/*No pull-up, no pull down*/
	CLEAR_BIT(GPIOx->PUPDR, (1U<<(USARTx_GPIO_TX_PIN*2+1)));
	CLEAR_BIT(GPIOx->PUPDR, (1U<<(USARTx_GPIO_TX_PIN*2)));

	if (USARTx_GPIO_TX_PIN>7){
		/*Configuring GPIO alternate function high register*/
		if (USARTx_GPIO_TX_PIN==8) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL8, (0x07 << GPIO_AFRH_AFSEL8_Pos));
		} else if (USARTx_GPIO_TX_PIN == 9) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL9, (0x07 << GPIO_AFRH_AFSEL9_Pos));
		} else if (USARTx_GPIO_TX_PIN == 10) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL10, (0x07 << GPIO_AFRH_AFSEL10_Pos));
		} else if (USARTx_GPIO_TX_PIN == 11) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL11, (0x07 << GPIO_AFRH_AFSEL11_Pos));
		} else if (USARTx_GPIO_TX_PIN == 12) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL12, (0x07 << GPIO_AFRH_AFSEL12_Pos));
		} else if (USARTx_GPIO_TX_PIN == 13) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL13, (0x07 << GPIO_AFRH_AFSEL13_Pos));
		} else if (USARTx_GPIO_TX_PIN == 14) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL14, (0x07 << GPIO_AFRH_AFSEL14_Pos));
		} else if (USARTx_GPIO_TX_PIN == 15) {
			MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL15, (0x07 << GPIO_AFRH_AFSEL15_Pos));
		}
	}
#if USARTx_GPIO_TX_PIN <8
	else {
		SET_BIT(GPIOx->AFR[0], (1U<<(USARTx_GPIO_TX_PIN*4)));
		SET_BIT(GPIOx->AFR[0], (1U<<(USARTx_GPIO_TX_PIN*4+1)));
		SET_BIT(GPIOx->AFR[0], (1U<<(USARTx_GPIO_TX_PIN*4+2)));
		CLEAR_BIT(GPIOx->AFR[0], (1U<<(USARTx_GPIO_TX_PIN*4+3)));
	}
#endif
}

void SPIx_gpio_init(GPIO_TypeDef *GPIOx, uint16_t SPI_mode)
{

	if (GPIOx == GPIOA){
		/*Enable clock access to GPIO port A*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);
	} else if (GPIOx == GPIOB){
		/*Enable clock access to GPIO port B*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);
	} else if (GPIOx == GPIOC){
		/*Enable clock access to GPIO port C*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);
	} else if (GPIOx == GPIOD){
		/*Enable clock access to GPIO port D*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIODEN);
	} else if (GPIOx == GPIOE) {
		/*Enable clock access to GPIO port E*/
		SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOEEN);
	}

					/***** MOSI pin ******/
	/*Set the pin as alternate function mode*/
	SET_BIT(GPIOx->MODER, (1U<<(SPIx_GPIO_MOSI_PIN*2+1)));
	CLEAR_BIT(GPIOx->MODER, (1U<<(SPIx_GPIO_MOSI_PIN*2)));

	/*Set the output type as push-pull*/
	CLEAR_BIT(GPIOx->OTYPER, (1U<<SPIx_GPIO_MOSI_PIN));

	/*Enable very high speed to the peripheral pin*/
	SET_BIT(GPIOx->OSPEEDR, (1U<<(SPIx_GPIO_MOSI_PIN*2+1)));
	SET_BIT(GPIOx->OSPEEDR, (1U<<(SPIx_GPIO_MOSI_PIN*2)));

	/*No pull-down/up*/
	SET_BIT(GPIOx->PUPDR, (1U<<(SPIx_GPIO_MOSI_PIN*2+1)));
	CLEAR_BIT(GPIOx->PUPDR, (1U<<(SPIx_GPIO_MOSI_PIN*2)));

	/* Enable proper alternate function type
	 *  SPI1/2 is AF5 and SPI3 is AF6
	*/
	if (SPI_mode == 1 || SPI_mode == 2) {
		/*Enable alternate function 5 (AF5)*/
		if (SPIx_GPIO_MOSI_PIN > 7) {
			if (SPIx_GPIO_MOSI_PIN == 8) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL8, (0x05 << GPIO_AFRH_AFSEL8_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 9) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL9, (0x05 << GPIO_AFRH_AFSEL9_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 10) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL10, (0x05 << GPIO_AFRH_AFSEL10_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 11) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL11, (0x05 << GPIO_AFRH_AFSEL11_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 12) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL12, (0x05 << GPIO_AFRH_AFSEL12_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 13) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL13, (0x05 << GPIO_AFRH_AFSEL13_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 14) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL14, (0x05 << GPIO_AFRH_AFSEL14_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 15) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL15, (0x05 << GPIO_AFRH_AFSEL15_Pos));
			}
		}
#if SPIx_GPIO_MOSI_PIN<8
		else {
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MOSI_PIN*4+1)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MOSI_PIN*4+2)));
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MOSI_PIN*4+3)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MOSI_PIN*4+4)));
		}
#endif
	} else {
		/*Enable alternate function 6 (AF6)*/
		if (SPIx_GPIO_MOSI_PIN > 7) {
			if (SPIx_GPIO_MOSI_PIN == 8) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL8, (0x06 << GPIO_AFRH_AFSEL8_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 9) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL9, (0x06 << GPIO_AFRH_AFSEL9_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 10) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL10, (0x06 << GPIO_AFRH_AFSEL10_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 11) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL11, (0x06 << GPIO_AFRH_AFSEL11_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 12) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL12, (0x06 << GPIO_AFRH_AFSEL12_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 13) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL13, (0x06 << GPIO_AFRH_AFSEL13_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 14) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL14, (0x06 << GPIO_AFRH_AFSEL14_Pos));
			} else if (SPIx_GPIO_MOSI_PIN == 15) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL15, (0x06 << GPIO_AFRH_AFSEL15_Pos));
			}
		}
#if SPIx_GPIO_MOSI_PIN < 8
		else {
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MOSI_PIN*4+1)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MOSI_PIN*4+2)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MOSI_PIN*4+3)));
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MOSI_PIN*4+4)));
		}
#endif
	}
					/***** MISO pin ******/
	/*Enable alternate function mode for this pin*/
	SET_BIT(GPIOx->MODER, (1U<<(SPIx_GPIO_MISO_PIN*2+1)));
	CLEAR_BIT(GPIOx->MODER, (1U<<SPIx_GPIO_MISO_PIN*2));

	/*Set the output type as push-pull*/
	CLEAR_BIT(GPIOx->OTYPER, (1U<<SPIx_GPIO_MISO_PIN));

	/*Enable very high speed to the peripheral pin*/
	SET_BIT(GPIOx->OSPEEDR, (1U<<(SPIx_GPIO_MISO_PIN*2+1)));
	SET_BIT(GPIOx->OSPEEDR, (1U<<SPIx_GPIO_MISO_PIN*2));

	/*No push-pull, no pull up*/
	SET_BIT(GPIOx->PUPDR, (1U<<(SPIx_GPIO_MISO_PIN*2+1)));
	CLEAR_BIT(GPIOx->PUPDR, (1U<<SPIx_GPIO_MISO_PIN*2));

	/* Enable proper alternate function type
	 *  SPI1/2 is AF5 and SPI3 is AF6
	*/
	if (SPI_mode == 1 || SPI_mode == 2) {
		/*Enable alternate function 5 (AF5)*/
		if (SPIx_GPIO_MISO_PIN > 7) {
			if (SPIx_GPIO_MISO_PIN == 8) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL8, (0x05 << GPIO_AFRH_AFSEL8_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 9) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL9, (0x05 << GPIO_AFRH_AFSEL9_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 10) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL10, (0x05 << GPIO_AFRH_AFSEL10_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 11) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL11, (0x05 << GPIO_AFRH_AFSEL11_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 12) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL12, (0x05 << GPIO_AFRH_AFSEL12_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 13) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL13, (0x05 << GPIO_AFRH_AFSEL13_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 14) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL14, (0x05 << GPIO_AFRH_AFSEL14_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 15) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL15, (0x05 << GPIO_AFRH_AFSEL15_Pos));
			}
		}
#if SPIx_F
		else {
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MISO_PIN*4+1)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MISO_PIN*4+2)));
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MISO_PIN*4+3)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MISO_PIN*4+4)));
		}
#endif
	}
#if SPIx_GPIO_MISO_PIN > 8
	else {
		/*Enable alternate function 6 (AF6)*/
		if (SPIx_GPIO_MISO_PIN > 7) {
			if (SPIx_GPIO_MISO_PIN == 8) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL8, (0x06 << GPIO_AFRH_AFSEL8_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 9) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL9, (0x06 << GPIO_AFRH_AFSEL9_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 10) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL10, (0x06 << GPIO_AFRH_AFSEL10_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 11) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL11, (0x06 << GPIO_AFRH_AFSEL11_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 12) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL12, (0x06 << GPIO_AFRH_AFSEL12_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 13) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL13, (0x06 << GPIO_AFRH_AFSEL13_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 14) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL14, (0x06 << GPIO_AFRH_AFSEL14_Pos));
			} else if (SPIx_GPIO_MISO_PIN == 15) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL15, (0x06 << GPIO_AFRH_AFSEL15_Pos));
			}
		}
#endif

#if SPIx_GPIO_MISO_PIN < 8
		else {
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MISO_PIN*4+1)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MISO_PIN*4+2)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MISO_PIN*4+3)));
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_MISO_PIN*4+4)));
		}
#endif
	}

						/***** CLK pin ******/
	/*Set the pin as alternate function mode*/
	SET_BIT(GPIOx->MODER, (1U<<(SPIx_GPIO_CLK_PIN*2+1)));
	CLEAR_BIT(GPIOx->MODER, (1U<<(SPIx_GPIO_CLK_PIN*2)));

	/*Set the output type as push-pull*/
	CLEAR_BIT(GPIOx->MODER, (1U<<(SPIx_GPIO_CLK_PIN)));

	/*Enable very high speed to the peripheral pin*/
	SET_BIT(GPIOx->OSPEEDR, (1U<<(SPIx_GPIO_CLK_PIN*2+1)));
	SET_BIT(GPIOx->OSPEEDR, (1U<<(SPIx_GPIO_CLK_PIN*2)));

	/*No pull-down/up*/
	SET_BIT(GPIOx->PUPDR, (1U<<(SPIx_GPIO_CLK_PIN*2+1)));
	CLEAR_BIT(GPIOx->PUPDR, (1U<<(SPIx_GPIO_CLK_PIN*2)));

	/* Enable proper alternate function type
	 *  SPI1/2 is AF5 and SPI3 is AF6
	*/
	if (SPI_mode == 1 || SPI_mode == 2) {
		/*Enable alternate function 5 (AF5)*/
		if (SPIx_GPIO_CLK_PIN > 7) {
			if (SPIx_GPIO_CLK_PIN == 8) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL8, (0x05 << GPIO_AFRH_AFSEL8_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 9) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL9, (0x05 << GPIO_AFRH_AFSEL9_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 10) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL10, (0x05 << GPIO_AFRH_AFSEL10_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 11) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL11, (0x05 << GPIO_AFRH_AFSEL11_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 12) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL12, (0x05 << GPIO_AFRH_AFSEL12_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 13) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL13, (0x05 << GPIO_AFRH_AFSEL13_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 14) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL14, (0x05 << GPIO_AFRH_AFSEL14_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 15) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL15, (0x05 << GPIO_AFRH_AFSEL15_Pos));
			}
		}
#if SPIx_GPIO_CLK_PIN < 8
		else {
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_CLK_PIN*4+1)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_CLK_PIN*4+2)));
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_CLK_PIN*4+3)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_CLK_PIN*4+4)));
		}
#endif
	} else {
		/*Enable alternate function 6 (AF6)*/
		if (SPIx_GPIO_CLK_PIN > 7) {
			if (SPIx_GPIO_CLK_PIN == 8) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL8, (0x06 << GPIO_AFRH_AFSEL8_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 9) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL9, (0x06 << GPIO_AFRH_AFSEL9_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 10) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL10, (0x06 << GPIO_AFRH_AFSEL10_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 11) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL11, (0x06 << GPIO_AFRH_AFSEL11_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 12) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL12, (0x06 << GPIO_AFRH_AFSEL12_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 13) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL13, (0x06 << GPIO_AFRH_AFSEL13_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 14) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL14, (0x06 << GPIO_AFRH_AFSEL14_Pos));
			} else if (SPIx_GPIO_CLK_PIN == 15) {
				MODIFY_REG(GPIOx->AFR[1], GPIO_AFRH_AFSEL15, (0x06 << GPIO_AFRH_AFSEL15_Pos));
			}
		}
#if SPIx_GPIO_CLK_PIN < 8
		else {
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_CLK_PIN*4+1)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_CLK_PIN*4+2)));
			SET_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_CLK_PIN*4+3)));
			CLEAR_BIT(GPIOx->AFR[0], (1U<<(SPIx_GPIO_CLK_PIN*4+4)));
		}
#endif
	}

					/***** CS pin ******/
	/*Set the pin as general purpose output mode*/
	CLEAR_BIT(GPIOx->MODER, (1U<<(SPIx_GPIO_CS_PIN*2+1)));
	SET_BIT(GPIOx->MODER, (1U<<(SPIx_GPIO_CS_PIN*2)));

	/*Set the output type as push-pull*/
	CLEAR_BIT(GPIOx->OTYPER, (1U<<(SPIx_GPIO_CS_PIN)));

	/*Enable very high speed to the peripheral pin*/
	SET_BIT(GPIOx->OSPEEDR, (1U<<(SPIx_GPIO_CS_PIN*2+1)));
	SET_BIT(GPIOx->OSPEEDR, (1U<<(SPIx_GPIO_CS_PIN*2)));

	/*Enable internal pull up resistor*/
	CLEAR_BIT(GPIOx->PUPDR, (1U<<(SPIx_GPIO_CS_PIN*2+1)));
	SET_BIT(GPIOx->PUPDR, (1U<<(SPIx_GPIO_CS_PIN*2)));

}
