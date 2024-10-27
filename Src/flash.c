/*
 * flash.c
 *
 *  Created on: Apr 26, 2024
 *      Author: ngrigoriadis
 */


#include "flash.h"


/**
 * @brief Unlock the flash memory of the boot-loader.
 * @retval 1 if unlock operation success, 0 otherwise.
 */
uint16_t bl_flash_unlock(void)
{
	/*Check the status of flash memory*/
	if (READ_BIT(FLASH->CR, FLASH_CR_LOCK)) {
		/*Unlock sequence to KEYR register*/
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;

		return 1;
	}

	return 0;
}

/**
 * @brief Lock the flash memory of the boot-loader.
 */
void bl_flash_lock(void)
{
	/*Lock the flash memory by setting the LOCK bit in CR*/
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}

/**
 * @brief Clears all error status flags of a previous fault operation.
 * Subsequently, it informs the user about a previous error in flash programming,
 * when the user uses a function that performs erase operation.
 */
void bl_flash_clear_status_flags(void)
{
	/*Programming error check*/
	if (READ_BIT(FLASH->SR, FLASH_SR_PROGERR)) {
		printf("Warning, Previously word written is not erased...\n\r");
		/*Clear the error flag*/
		SET_BIT(FLASH->SR, FLASH_SR_PROGERR);
	}

	/*Size error check*/
	if (READ_BIT(FLASH->SR, FLASH_SR_SIZERR)){
		printf("Warning, previous attempt to write half/byte word error detected...\n\r");
		/*Clear the error flag*/
		SET_BIT(FLASH->SR, FLASH_SR_SIZERR);
	}

	/*Fast programming miss data error check*/
	if (READ_BIT(FLASH->SR, FLASH_SR_MISERR)) {
		printf("Warning, previous fast programming data miss error detected...\n\r");
		/*Clear the error flag*/
		SET_BIT(FLASH->SR, FLASH_SR_MISERR);
	}

	/*Programming alignment error check*/
	if (READ_BIT(FLASH->SR, FLASH_SR_PGAERR)) {
		printf("Warning, previous programming alignment error detected...\n\r");
		/*Clear the error flag*/
		SET_BIT(FLASH->SR, FLASH_SR_PGAERR);
	}

	/*Fast programming error check*/
	if (READ_BIT(FLASH->SR, FLASH_SR_FASTERR)) {
		printf("Warning, previous fast programming error detected...\n\r");
		/*Clear the error flag*/
		SET_BIT(FLASH->SR, FLASH_SR_FASTERR);
	}

}

/**
 * @brief This function erases a page of BL-Flash Memory.
 * @param page  : The page that needs to be erased.
 * @param bank  : Is the bank (1/2) of the flash memory.
 * @retval 1 if page erased successfully, 0 otherwise due to undefined bank definition.
 */
uint16_t bl_flash_page_erase(uint32_t page, uint16_t bank)
{
	/*Wait if any flash memory operation is ongoing*/
	while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {}

	/*Check if any error occurs due to previous programming*/
	bl_flash_clear_status_flags();

	/*Check the bank of flash memory, specified by the user*/
	if (bank == 1) {
		/*Set bank 1*/
		CLEAR_BIT(FLASH->CR, FLASH_CR_BKER);
	} else if (bank == 2) {
		/*Set bank 2*/
		SET_BIT(FLASH->CR, FLASH_CR_BKER);
	} else {
		/*Undefined bank space*/
		return 0;
	}

	/*Define the page that must be erased*/
	MODIFY_REG(FLASH->CR, FLASH_CR_PNB, (page << FLASH_CR_PNB_Pos));

	/*Enable page erase operation*/
	SET_BIT(FLASH->CR, FLASH_CR_PER);

	/*Start the procedure*/
	SET_BIT(FLASH->CR, FLASH_CR_STRT);

	/*Wait the flash to finish its ongoing operation*/
	while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {}

	/*Disable page operation*/
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);

	return 1;
}


/**
 * @brief Performs mass erase into the selected bank.
 * @param bank  :  Is the bank (1/2) of the flash memory.
 * @retval 1 if the erase operation completes successfully, 0 otherwise.
 */
uint16_t bl_flash_mass_erase_bank(uint16_t bank)
{
	/*Wait if any flash memory operation is ongoing*/
	while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {}

	/*Check if any error occurs due to previous programming*/
	bl_flash_clear_status_flags();

	/*Check which bank is defined by the user*/
	if (bank == 1) {
		/*Enable bank 1 mass erase*/
		SET_BIT(FLASH->CR, FLASH_CR_MER1);
	} else if (bank == 2) {
		/*Enable bank 2 mass erase*/
		SET_BIT(FLASH->CR, FLASH_CR_MER2);
	} else {
		/*Undefined bank space*/
		return 0;
	}

	/*Start the procedure*/
	SET_BIT(FLASH->CR, FLASH_CR_STRT);

	printf("Starting erase operation into bank%d\n\r", bank);

	/*Wait flash to finish its ongoing operation*/
	while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {}


	return 1;
}

/**
 * @brief This function performs mass erase into both banks.
 * Use this function carefully, since it will clear all data, either the application's
 * or bootloader's.
 */
void bl_flash_mass_erase(void)
{
	/*Wait if any flash memory operation is ongoing*/
	while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {}

	/*Select both banks*/
	SET_BIT(FLASH->CR, FLASH_CR_MER1); /*Bank 1*/
	SET_BIT(FLASH->CR, FLASH_CR_MER2); /*Bank 2*/

	/*Start the erase operation*/
	SET_BIT(FLASH->CR, FLASH_CR_STRT);

	printf("Starting mass erase operation into both banks\n\r");

	/*Wait flash to finish its ongoing operation*/
	while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {}

	/*Stop the erase operation*/
	CLEAR_BIT(FLASH->CR, FLASH_CR_STRT);

}

/*
 * If this function returns 0, it means a previous programming operation had errors, which are going
 * to be specified in serial term as warnings. Possible error, if a data is going to be written in
 * a specific address and the there an erase operation to the specific address has not been done then
 * it will abort.
 *
 * @brief This function writes 64-bit (double word) data at a specified address to flash memory.
 * @param address : The address of the flash memory that needs to be programmed.
 * @param data	  : 64-bit data that is going to be written into the specified address.
 */
void bl_flash_write64(uint32_t address, uint64_t data)
{
	/*Wait if any flash memory operation is ongoing*/
	while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {}

	/*Check for previous programming errors, and clear them*/
	bl_flash_clear_status_flags();

	/*Enable flash programming operation*/
	SET_BIT(FLASH->CR, FLASH_CR_PG);

	/*Program double-word*/
	*(__IO uint32_t*) address = (uint32_t) data;
	*(__IO uint32_t*)(address + 4) = (uint32_t)(data >> 32);

	/*Wait if any flash memory operation is ongoing*/
	while (READ_BIT(FLASH->SR, FLASH_SR_BSY)) {}

	/*Stop the programming operation*/
	CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
}

/**
 * @brief Writes a string into the flash memory. It starts from a specific page address
 * 		  and keeps writing until its done.
 * @param start_addr  : Starting page address (0-255 = Bank 1, 256-511 = Bank 2) if the FLASH size defined in linker is 1MB.
 * 						For 512KB dual bank organization (0-127 = Bank 1, 256-383 = Bank 2).
 * 						For 256KB dual bank organization (0-63 = Bank 1, 256-319 = Bank 2).
 * @param pBuf		  : Pointer buffer, which contains the data that is going to be written into the flash memory.
 * @param len 		  : The length of the data (pBuf).
 * @retval 	None.
 */
void bl_flash_write64_str(uint32_t start_addr, uint8_t *pBuf, uint16_t len)
{
	uint32_t addr = start_addr;
	for (uint32_t i=0; i<len; i++) {
		bl_flash_write64(addr, pBuf[i]);
		addr = addr + 8;
	}
}

/**
 * @brief Writes a data block into flash memory.
 * @param start_addr : Starting address from which the words are going to be stored into.
 * @param str		 : The buffer where
 */
void bl_flash_program(uint32_t start_addr, const uint8_t *str, uint16_t len)
{
	for (uint32_t i = 0; i < len; i += 8) {
		bl_flash_write64(start_addr + i, *(uint64_t*)(str+i));
	}
}

/**
 * @brief Gets the page of a given address.
 * @param addr : Address of the FLASH memory.
 * @retval The page of a given address.
 */
uint32_t bl_flash_get_page(uint32_t addr)
{
	uint32_t page = 0;

	if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
	{
		/*Bank 1*/
		page = (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
	}
	else
	{
		/*Bank 2*/
		page = (addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
	}

	/*Return the page number to the user*/
	return page;
}

/**
 * @brief Gets the bank of a given address.
 * @param addr : Address of the FLASH memory.
 * @retval The bank of the given address.
 */
uint32_t bl_flash_get_bank(uint32_t addr)
{
	uint32_t bank = 0;

	/*Read the momory map register*/
	if ((SYSCFG->MEMRMP & SYSCFG_MEMRMP_FB_MODE) == 0)
	{
		/*No bank swap*/
		if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
		{
			/*Bank 1*/
			bank = FLASH_BANK_1;
		}
		else
		{
			/*Bank 2*/
			bank = FLASH_BANK_2;
		}
	}
	else
	{
		/*Bank swap*/
		if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
		{
			/*Bank 2*/
			bank = FLASH_BANK_2;
		}
		else
		{
			/*Bank 1*/
			bank = FLASH_BANK_1;
		}
	}

	/*Return the bank to the user*/
	return bank;
}

