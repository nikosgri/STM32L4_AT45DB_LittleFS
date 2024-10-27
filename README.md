# STM32L4 LittleFS with AT45DB041E

This repository contains code for using LittleFS on the AT45DB041E external flash memory with the STM32L4 Cortex-M4 microcontroller in a bare metal programming environment.

## Overview

The project demonstrates how to:

- Initialize the STM32L4 microcontroller.
- Set up the AT45DB041E external flash memory for read and write operations.
- Use the LittleFS file system to manage files on the external flash.

## Features

- Bare metal programming on STM32L4.
- Read and write files to the AT45DB041E external flash memory.
- Simple implementation of the LittleFS file system.

## Hardware Requirements

- STM32L4 microcontroller (e.g., STM32L4xx series)
- AT45DB041E external flash memory
- SPI interface for communication between the STM32L4 and the AT45DB041E

## Getting Started

### Prerequisites

- STM32CubeIDE environment for STM32.
- Datasheet of STM32L4 microcontroller, download [here](https://www.st.com/resource/en/datasheet/stm32l476je.pdf)
- Reference manual of STM32L4 microcontroller, download [here](https://www.st.com/resource/en/reference_manual/rm0351-stm32l47xxx-stm32l48xxx-stm32l49xxx-and-stm32l4axxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- Datasheet of external flash memory AT45DB, download [here](https://www.tme.eu/Document/f6c9d35da09cb9caa899d0d66aa11c4c/AT45DB161E-DTE.pdf)


### Installation

1. Clone this repository to your local machine:
   ```bash
   git clone https://github.com/yourusername/yourrepository.git
   cd yourrepository
2. Open the project in STM32CubeIDE.
3. Configure the SPI settings to match your hardware setup.
4. Compile the code and upload it to your STM32L4 microcontroller.

## Usage

* The code initializes the HSI clock, sets up the SPI interface, and mounts the LittleFS on the AT45DB041E.
* You can modify the main.c file to write and read data to/from the external flash memory.
* The example provided writes "Hello, LittleFS!" to a file named test.txt in the external flash.

## Example Code Snippet

```
/* Open the file for writing */
res = lfs_file_open(&lfs, &file, "test.txt", LFS_O_CREAT | LFS_O_RDWR);
if (res == LFS_ERR_OK) {
    const char *data = "Hello, LittleFS!";
    lfs_size_t bytes_written;
    
    /* Write data to the file */
    res = lfs_file_write(&lfs, &file, data, strlen(data), &bytes_written);
}
```

## Acknowledgements

* [STMicroelectronics](https://www.st.com/content/st_com/en.html) for providing the STM32L4 microcontrollers.
* [ittleFS](https://github.com/littlefs-project/littlefs) for the lightweight file system.