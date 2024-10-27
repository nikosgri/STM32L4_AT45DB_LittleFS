/*
 * at45db041.h
 *
 *  Created on: Apr 26, 2024
 *      Author: ngrigoriadis
 */

#ifndef AT45DB041_H_
#define AT45DB041_H_

#include "main.h"			//Common headers
#include "spi.h"			//Access SPI functions
#include "system_init.h"	//Access delay_ms function

/*Define the page size*/
#define BINARY_PAGE_SIZE
/*Define status register bits*/
#define RDY_BIT							0x80 /*Device is busy=0, Device is ready=1*/
#define RES_BIT					        0x40 /*Reserved for future use*/
#define EPE_BIT                         0x20 /*Success program/erase=0, Failure erase/program=1*/
#define SLE_BIT                         0x08 /*Sector LockDown disable=0, Sector LockDown enable=1*/
#define PS2_BIT                         0x04 /*Program suspend status (Buffer 2)*/
#define PS1_BIT                         0x02 /*Program suspend status (Buffer 1)*/
#define ES_BIT                          0x01 /*Erase suspend*/
#define PGS_BIT                         0x01 /*Page size configuration, 0=264, 1=256*/
#define PRCT_BIT                        0x02 /*Sector protection status*/
#define COMP_BIT                        0x40 /*Compare result*/
/*Define OPCODES for external flash memory*/
#define MAN_ID_OPCD						0x9F
#define SOFT_RST_CMD_1					0xF0
#define SOFT_RST_CMD_2					0x00
#define SOFT_RST_CMD_3					0x00
#define SOFT_RST_CMD_4					0x00
#define CHIP_ERASE_1					0xC7
#define CHIP_ERASE_2					0x94
#define CHIP_ERASE_3					0x80
#define CHIP_ERASE_4					0x9A
#define DEEP_SLEEP						0xB9
#define WAKE_UP_DEEP_SLEEP				0xAB
#define ULTRA_DEEP_SLEEP				0x79
#define STATUS_REGISTER					0xD7
#define CONTINUOUS_ARRAY_READ			0xE8
#define CONTINUOUS_ARRAY_READ_H_MODE1	0x1B
#define CONTINUOUS_ARRAY_READ_H_MODE2	0x0B
#define ENABLE_SECTOR_PROTECTION_1		0x3D
#define ENABLE_SECTOR_PROTECTION_2		0x2A
#define ENABLE_SECTOR_PROTECTION_3		0x7F
#define ENABLE_SECTOR_PROTECTION_4		0x9A
#define DISABLE_SECTOR_PROTECTION_1		0X3D
#define DISABLE_SECTOR_PROTECTION_2		0x2A
#define DISABLE_SECTOR_PROTECTION_3		0x7F
#define DISABLE_SECTOR_PROTECTION_4		0x9A
#define INTERNAL_BUFFER_1				0x84
#define SECTOR_ERASE					0x7C
#define BUFFER_TO_MAIN_1				0x83
#define BUFFER_TO_MAIN_2				0x86
#define PAGE_ERASE_CMD					0x81
#define BIN_PG_SIZE_1					0x3D
#define BIN_PG_SIZE_2					0x2A
#define BIN_PG_SIZE_3					0x80
#define BIN_PG_SIZE_4					0xA6
#define BUFFER_2_READ_BINARY			0xD6

/**
 * @brief Enumerators about MAN ID.
 */
typedef enum {
	DEV_ID_ERROR = 0,
	DEV_ID_OK	 = 1,
}dev_id_t;

/**
 * @brief Information about the operation of the external flash.
 */
typedef enum {
	_DEEP_SLEEP   = 0,
	_U_DEEP_SLEEP = 1,
	_NORMAL_MODE  = 2,
}at45db_op_info_t;

/**
 * @brief Basic parameters for the external flash memory.
 */
typedef struct at45db_ {
	at45db_op_info_t state;
	int flash_Mbit;
	int page_size;
	int blocks;
	int block_size;
}at45db_t;

/*Function prototypes*/
void at45db_init(void);
dev_id_t at45db_read_manID(void);
void at45db_chip_erase(void);
void at45db_sector_erase(uint8_t sector_num);
void at45db_page_erase(uint16_t page);
void at45db_deep_sleep(at45db_t *info);
void at45db_page_size_conf(uint16_t mode);
uint16_t at45db_wake_up_from_deep_sleep(at45db_t info);
void at45db_ultra_deep_sleep(at45db_t info);
uint16_t at45db_wake_up_from_ultra_deep_sleep(at45db_t info);
void at45db_read_data(uint32_t address, uint8_t *data, uint16_t size);
int at45db_IsReady(void);
uint16_t at45db_fault_check(void);
void at45db_program(uint32_t addr, uint8_t *buffer, uint16_t size);
void at45db_read_status(uint8_t *status_reg);
void at45db_read_continuous(uint32_t addr, uint8_t *buffer, uint32_t size);
int is_device_busy(void);
#endif /* AT45DB041_H_ */
