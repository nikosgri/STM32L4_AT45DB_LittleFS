/*
 * at45db041.c
 *
 *  Created on: Apr 26, 2024
 *      Author: ngrigoriadis
 */


#include "at45db041.h"


/*Make the buffer visible for the external flash*/
extern uint8_t SPI_REC_BUF[SPI_REC_BUFF_SIZE];

/*Information structure*/
extern at45db_t AT45DB;


/**
 * @brief Initialize the external flash, with basic parameters and checks its availability.
 * @retval None.
 */
void at45db_init()
{
	uint16_t res = -1;
    uint8_t status_register[2] = {0};

    /*Check if the external flash memory is accessible*/
    res = at45db_read_manID();
    if (!res)
    {
        /*External flash memory is not accessible*/
        printf("Unable to communicate with external flash. Initiate wake up process!\r\n");

        /*Wake up process*/
        res = at45db_wake_up_from_deep_sleep(AT45DB);
        if (!res)
        {
            /*Device could be in ultra deep sleep*/
            res = at45db_wake_up_from_ultra_deep_sleep(AT45DB);
            if (!res)
            {
                 printf("Cannot wake up the external flash from ultra deep sleep\r\n");
            }
            else if (res)
            {
                 printf("External flash memory woke up from ultra deep sleep\r\n");
            }
        }
		else if (res)
        {
            printf("External flash memory wake up from power down mode\r\n");
        }
    }
    else if (res)
    {
         printf("Correct device ID, external flash memory is accessible\r\n");
    }

    delay_ms(100);

    /*Initializa information structure*/
#ifdef BINARY_PAGE_SIZE
    AT45DB.block_size = 2048;
    AT45DB.blocks     = 256;
    AT45DB.flash_Mbit = 4;
    AT45DB.page_size  = 256;
#endif

#ifdef DATAFLASH_PAGE_SIZE
    AT45DB->block_size = 2112;
    AT45DB->blocks     = 256;
    AT45DB->flash_Mbit = 4;
    AT45DB->page_size  = 264;
#endif

    /*Check the flash page size*/
    at45db_read_status(status_register);
    res = READ_BIT(status_register[0], PGS_BIT);
    if (!res)
    {
        printf("Device is configured for standard DataFlash page size (264 bytes)\r\n");

        memset(status_register, 0, sizeof(status_register));
        at45db_page_size_conf(2);
        at45db_read_status(status_register);
        res = READ_BIT(status_register[0], PGS_BIT);
        if (!res)
        {
             printf("Continuous in data flash page size 264 bytes\r\n");
        }
		else if (res)
        {
            printf("Changed to binary page size\r\n");
        }
    }
    else if (res)
    {
        printf("Device is configured for “power of 2” binary page size (256 bytes)\r\n");
    }

}

/**
 * @brief Read the manufacturer ID from external flush memory.
 * @retval 1 if the ID is OK, 0 otherwise.
 */
dev_id_t at45db_read_manID(void)
{
    uint8_t opcode = MAN_ID_OPCD;
    uint8_t rec_buf[5] = {0};

    /*Select the device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the proper opcode*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Start receiving data from the external flash*/
    SPIx_receive(SPI_PERIPH, rec_buf, 5);

    /*Release the device*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Check the data that has been arrived*/
    if (rec_buf[0] != 0x1F || rec_buf[1] != 0x24 ||
		rec_buf[2] != 0x00 || rec_buf[3] != 0x01 ||
		rec_buf[4] != 0x00) {

        /*Not proper MAN ID*/
        return DEV_ID_ERROR;
    }

    return DEV_ID_OK;

}

/**
 * @brief Perform chip erase. This command erase the entire main memory.
 * @retval None.
 */
void at45db_chip_erase(void)
{
    uint8_t cmd_buf[4] = {0};
    int res = -1;

    cmd_buf[0] = CHIP_ERASE_1;
    cmd_buf[1] = CHIP_ERASE_2;
    cmd_buf[2] = CHIP_ERASE_3;
    cmd_buf[3] = CHIP_ERASE_4;

    /*Select the device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the proper commands to the external FLASH memory*/
    SPIx_transmit(SPI_PERIPH, cmd_buf, 4);

    /*Release the device*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Check for fault operation*/
    res = at45db_fault_check();

	if (res == 1)
	{
        printf("There was a fault in erase operation...\n\r");
    }
	else if (res == 0) {
        printf("Successful, erase operation...\n\r");
    }
}

/**
 * @brief Configure page size. Careful you only have 10000 times to do that.
 * @param mode      : Set 1 for DataFlash (264 bytes), 2 for Binary (256 bytes).
 * @retval None.
 */
void at45db_page_size_conf(uint16_t mode)
{
    uint8_t cmd_buf[4] = {0};

    /*Enable the flash memory*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Set proper parameters*/
    cmd_buf[0] = 0x3D;
    cmd_buf[1] = 0x2A;
    cmd_buf[2] = 0x80;

    /*Check user input*/
    if (mode == 1)
    {
        /*Data flash page size*/
        cmd_buf[3] = 0xA7;

        /*Transmit the proper command*/
        SPIx_transmit(SPI_PERIPH, cmd_buf, sizeof(cmd_buf));

        /*Disable the slave device*/
        SPIx_disable_slave(GPIO_SPIx);
    }
    else if (mode == 2)
    {
        /*Binary page size*/
        cmd_buf[3] = 0xA6;

        /*Transmit the proper command*/
        SPIx_transmit(SPI_PERIPH, cmd_buf, sizeof(cmd_buf));

	    /*Disable the slave device*/
	    SPIx_disable_slave(GPIO_SPIx);
    }
}

/**
 * @brief This function erases a sector inside the external memory.
 * @param sector_num  :  This is the number of the sector you need to erase.
 * @retval None.
 */
void at45db_sector_erase(uint8_t sector_num)
{
    uint8_t opcode = SECTOR_ERASE;
    uint8_t address_byte[3];


    address_byte[0] = ((sector_num) << 3) & 0xF8;
    address_byte[1] = 0x00;
    address_byte[2] = 0x00;


    /*Enable slave device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the proper command*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Transmit the dummy bits with the proper sector selection*/
    SPIx_transmit(SPI_PERIPH, address_byte, 3);

    /*Disable slave device*/
    SPIx_disable_slave(GPIO_SPIx);
}

/**
 * @brief Erases a specific page inside the main memory.
 * @reval None.
 */
void at45db_page_erase(uint16_t page)
{
    uint8_t cmd_buf[3] = {0};
    uint8_t opcode = PAGE_ERASE_CMD;
    page = page << 9;

    /*Enable slave device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the proper opcode*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Modify the proper data bits*/
    cmd_buf[0] = (page >> 16) & 0xFF;
    cmd_buf[1] = (page >> 8) & 0xFF;
    cmd_buf[2] = page & 0xFF;

    /*Transmit the modifies data*/
    SPIx_transmit(SPI_PERIPH, cmd_buf, 3);

    /*Disable slave device*/
    SPIx_disable_slave(GPIO_SPIx);
}

/**
 * @brief Enter flash into deep sleep mode.
 * @param info : Structure information about the external flash operation.
 * @retval None.
 */
void at45db_deep_sleep(at45db_t *info)
{
    uint8_t opcode = DEEP_SLEEP;

    /*Select the device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the command the external FLASH memory*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Give some time to process*/
    delay_ms(100);

    /*Release the slave device*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Give some time to the system*/
    delay_ms(2);

    /*Make the MISO pin analog, for power saving*/
    SET_BIT(GPIO_SPIx->MODER, (1U<<(SPIx_GPIO_MISO_PIN*2+1)));
    SET_BIT(GPIO_SPIx->MODER, (1U<<(SPIx_GPIO_MISO_PIN*2)));

}

/**
 * @brief Wakes up external flash from deep sleep mode.
 * @param info : Structure information about the external flash operation.
 * @retval 1 for success, 0 on failure.
 */
uint16_t at45db_wake_up_from_deep_sleep(at45db_t info)
{
    uint16_t res = -1;
    uint8_t opcode = WAKE_UP_DEEP_SLEEP;

    /*Select the device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit proper command for waking up from stop mode*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Release the slave device*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Return the MISO pin to its initial state*/
    SET_BIT(GPIO_SPIx->MODER, (1U<<(SPIx_GPIO_MISO_PIN*2+1)));
    CLEAR_BIT(GPIO_SPIx->MODER, (1U<<SPIx_GPIO_MISO_PIN*2));

    /*Read MAN-ID*/
    res = at45db_read_manID();
    if (!res)
    {
         return 0;
    }

    info.state = _NORMAL_MODE;
    return 1;
}

/**
 * @brief Enter flash into ultra deep sleep.
 * @param info : Structure information about the external flash operation.
 * @retval None.
 */
void at45db_ultra_deep_sleep(at45db_t info)
{
    uint8_t opcode = ULTRA_DEEP_SLEEP;

    /*Select the slave device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the proper opcode*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Give some time to the system*/
    delay_ms(100);

    /*Release the device*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Make the MISO pin analog, for power saving*/
    SET_BIT(GPIO_SPIx->MODER, (1U<<(SPIx_GPIO_MISO_PIN*2+1)));
    SET_BIT(GPIO_SPIx->MODER, (1U<<(SPIx_GPIO_MISO_PIN*2)));

    info.state = _U_DEEP_SLEEP;

}

/**
 * @brief Exit from ultra deep sleep.
 * @param info : Structure information about the external flash operation.
 * @retval 1 for success, 0 on failure.
 */
uint16_t at45db_wake_up_from_ultra_deep_sleep(at45db_t info)
{
    uint16_t res = -1;

    /*Select the device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Delay*/
    delay_ms(2);

    /*Release the device*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Return the MISO pin to its initial state*/
    SET_BIT(GPIO_SPIx->MODER, (1U<<(SPIx_GPIO_MISO_PIN*2+1)));
    CLEAR_BIT(GPIO_SPIx->MODER, (1U<<SPIx_GPIO_MISO_PIN*2));

    /*Try to read MAD-ID*/
    res = at45db_read_manID();
    if (!res)
    {
        /*Failure*/
        return 0;
    }

    /*Success*/
    info.state = _NORMAL_MODE;

    return 1;
}

/**
 * @brief This functions performs a contineus array read inside the main memory of the external flash.
 * It starts from the specified address and continues to the next address if the size is long enough in order to take
 * information from the next addresses.
 * @param address : This is the starting address of the main memory you want to start reading from.
 * @param data    : The buffer that will contain the return data from the main memory of the flash.
 * @param size    : The size of data that you want to read.
 * @retval None.
 */
void at45db_read_data(uint32_t address, uint8_t *data, uint16_t size)
{
    uint8_t address_bytes[5];

    /*Set the address bytes*/
    address_bytes[0] = CONTINUOUS_ARRAY_READ_H_MODE2;
    address_bytes[1] = ((address >> 16) & 0xFF);
    address_bytes[2] = ((address >> 8) & 0xFF);
    address_bytes[3] = (address & 0xFF);
    address_bytes[4] = 0;

    /*Select the device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the address bytes*/
    SPIx_transmit(SPI_PERIPH, address_bytes, 4);

    /*Receive the data from the external flash memory*/
    SPIx_receive(SPI_PERIPH, data, size);

    /*Release the slave device*/
    SPIx_disable_slave(GPIO_SPIx);
}

/**
 * @brief Checks if the external flash is busy or not.
 * @retval 1 if it is not busy, 0 otherwise.
 */
int at45db_IsReady(void)
{
    uint8_t status_register[2];
    uint8_t opcode = STATUS_REGISTER;

    /*Enable slave device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit command to the external flash*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Receive the data from the external flash*/
    SPIx_receive(SPI_PERIPH, status_register, 2);

    /*Disable slave device*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Check the RDY/BSY bit*/
	if ((status_register[0] & 0x80) == 0x00)
	{
        /*External device is ready*/
        return 1;
    }
    else
	{
        /*External device is busy*/
        return 0;
    }
}

/**
 * @brief Checks if a fault erased after an erase or program operation.
 * @retval 1 if an error is detected, 0 otherwise.
 */
uint16_t at45db_fault_check(void)
{
    uint8_t status_register[2];
    uint8_t opcode = STATUS_REGISTER;

    /*Select the slave device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the proper opcode*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Receive the status result*/
    SPIx_receive(SPI_PERIPH, status_register, 2);

    /*Disable the slave device*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Check the 5th bit of the second byte EPE bit*/
    if ((status_register[1] & EPE_BIT)) {

        /*Error detected*/
        return 1;
    }

    /*Success, in erasing external flash memory*/
    return 0;
}

/**
 * @brief Returns 2 bytes of data which represents the current status of the external flash memory.
 * Byte 1 is stored in status_reg[0] and byte 2 is stored in status_reg[1].
 * @retval None.
 */
void at45db_read_status(uint8_t *status_reg)
{
    uint8_t opcode = STATUS_REGISTER;

    /*Select the device*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Transmit the proper opcode*/
    SPIx_transmit(SPI_PERIPH, &opcode, 1);

    /*Receive the slave device*/
    SPIx_receive(SPI_PERIPH, status_reg, 2);

    /*Release the slave device*/
    SPIx_disable_slave(GPIO_SPIx);
}


/**
 * @brief This function performs a programming operation into the main memory of the external flash
 * using the internal buffer 1 with built in erase operation.
 * @param addr  : The starting address of the main memory.
 * @param buffer: The buffer that holds the data which will be stored inside the main memory.
 * @param size  : The size of the data.
 * @retval None.
 */
void at45db_program(uint32_t addr, uint8_t *buffer, uint16_t size)
{
    /*Load data into Buffer 1*/
    uint8_t loadCommand[4];
    loadCommand[0] = 0x84; // Buffer 1 Write opcode
    loadCommand[1] = 0x00;			// Dummy byte
    loadCommand[2] = 0x00;			// Dummy byte
    loadCommand[3] = 0x00;			// Buffer starting address

    /*Select the external flash memory*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Send the proper command & data to the internal buffer 1*/
    SPIx_transmit(SPI_PERIPH, loadCommand, 4);
    SPIx_transmit(SPI_PERIPH, (uint8_t *)buffer, size);

    /*Deselect the flash memory*/
    SPIx_disable_slave(GPIO_SPIx);

    /*Program Buffer 1 to Main Memory Page with Built-In Erase*/
    uint8_t programCommand[4];
    programCommand[0] = 0x83;	// Buffer 1 to Main Memory Page Program with Built-In Erase opcode
    programCommand[1] = (addr >> 16) & 0xFF;	// Address byte 1
    programCommand[2] = (addr >> 8) & 0xFF;		// Address byte 2
    programCommand[3] = addr & 0xFF;			// Address byte 3

    /*Select the external flash memory*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Program the main memory. Send contents of buffer 1 to main memory*/
    SPIx_transmit(SPI_PERIPH, programCommand, 4);

    /*Deselect the flash memory*/
    SPIx_disable_slave(GPIO_SPIx);


    bool isReady = at45db_IsReady();

    while (!isReady) {}

}

void at45db_read_continuous(uint32_t addr, uint8_t *buffer, uint32_t size)
{
    uint8_t cmd[8];

    /*Preparing the command*/
    cmd[0] = 0xE8;  //Opcode
    cmd[1] = (addr >> 16) & 0xFF;           // Address byte 1
    cmd[2] = (addr >> 8) & 0xFF;            // Address byte 2
    cmd[3] = addr & 0xFF;                   // Address byte 3
    cmd[4] = 0x00;                          // Dummy byte 1
    cmd[5] = 0x00;                          // Dummy byte 2
    cmd[6] = 0x00;                          // Dummy byte 3
    cmd[7] = 0x00;                          // Dummy byte 4

    /*Select the external flash memory*/
    SPIx_enable_slave(GPIO_SPIx);

    /*Send the proper commands to load data to the internal buffer 1*/
    SPIx_transmit(SPI_PERIPH, cmd, sizeof(cmd));

    /*Receive the data*/
    SPIx_receive(SPI_PERIPH, buffer, size);

    /*Deselect the external flash memory*/
    SPIx_disable_slave(GPIO_SPIx);
}
