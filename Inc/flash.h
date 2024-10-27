/*
 * flash.h
 *
 *  Created on: Apr 26, 2024
 *      Author: ngrigoriadis
 *
 *      bl_... , stands for boot-loader.
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "main.h"  //Common headers

#define FLASH_BANK_SIZE                    (FLASH_SIZE >> 1U)
#define FLASH_BANK_2              		   ((uint32_t)0x02)
#define FLASH_BANK_1                       ((uint32_t)0x01)
#define FLASH_BASE                         (0x08000000UL)
#define FLASH_PAGE_SIZE                    ((uint32_t)0x800)
#define ADDR_FLASH_PAGE_64                 ((uint32_t)0x08020000) /* Base @ of Page 64, 2 Kbytes */
#define PACKET_SIZE					       512
#define PACKETS						       347
#define FLASH_PG_SIZE				       2048

/*Function definitions*/
uint16_t bl_flash_unlock(void);
void bl_flash_lock(void);
uint16_t bl_flash_page_erase(uint32_t page, uint16_t bank);
uint16_t bl_flash_mass_erase_bank(uint16_t bank);
void bl_flash_mass_erase(void);
void bl_flash_clear_status_flags(void);
void bl_flash_write64(uint32_t address, uint64_t data);
void bl_flash_write64_str(uint32_t start_addr, uint8_t *pBuf, uint16_t len);
void bl_flash_program(uint32_t start_addr, const uint8_t *str, uint16_t len);
uint32_t bl_flash_get_bank(uint32_t addr);
uint32_t bl_flash_get_page(uint32_t addr);
#endif /* FLASH_H_ */
