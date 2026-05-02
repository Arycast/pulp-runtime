/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __SPI_FLASH_SIMULATION_POSIX_H__
#define __SPI_FLASH_SIMULATION_POSIX_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
	* spi flash size in bits
	*/
#ifndef SPI_FLASH_SIZE_BITS
#define SPI_FLASH_SIZE_BITS (64u * 1024u * 1024u) /* W25Q64JV: 64 Mib */
/*#define SPI_FLASH_SIZE_BITS (128 * 1024 * 1024)*/ /* W25Q128FV: 128 Mib */
#endif

#ifndef SPI_FLASH_SIZE_BYTES
#define SPI_FLASH_SIZE_BYTES ((SPI_FLASH_SIZE_BITS) / 8u)
#endif

/* default 4 kiB */
#ifndef SPI_FLASH_SECTOR_SIZE_BYTES
#define SPI_FLASH_SECTOR_SIZE_BYTES    (4 * 1024)
#endif

/**
	* simulation to implement JEDEC's JESD216
	*/
#define JESD216_PARAMETER1_VERSION_MAJOR                              1u
#define JESD216_PARAMETER1_VERSION_MINOR                              1u

#define JESD216_PARAMETER1_ADDRESS                                    0x30u /* macronix (mxic) */
/*#define JESD216_PARAMETER1_ADDRESS                                    0x80*/ /* winbond */

#if (((JESD216_PARAMETER1_ADDRESS) % 4) != 0)
	#error  JESD216_PARAMETER1_ADDRESS should be 4-byte aligned. \
		set JESD216_PARAMETER1_ADDRESS to a value divisible by 4
#endif

#if ((JESD216_PARAMETER1_ADDRESS) < 0x10)
	#error  JESD216_PARAMETER1_ADDRESS is too small. \
		set JESD216_PARAMETER1_ADDRESS a bigger or equal to 0x10 \
		or bigger value if many sfdp parameter are available
#endif

/**
	* from Macronix application notes: SFDP Introduction
	* Remark: *(x-y-z): Indicate the opcode (x), address (y), and data (z).
	*/


/**
	* ************************************************************
	* JEDEC-defined/standardized parameter:
	* ************************************************************
	*/


/**
	* ************************************************************
	* JEDEC Flash Parameters: 1st DWORD
	*/

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 31:23       Unused
	*             Contains 0x1ff and can never be changed.
	*/

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 22          Supports (1-1-4) Fast Read
	*             Device supports single input opcode & address and quad output data Fast Read.
	*             0b: (1-1-4) Fast Read NOT supported.
	*             1b: (1-1-4) Fast Read supported.
	*/
#define JESD216_PARAMETER1_VALUE1_SUPPORT_1_1_4_FAST_READ             1u

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 21          Supports (1-4-4) Fast Read
	*             Device supports single input opcode, quad input address, and quad output data Fast Read.
	*             0b: (1-4-4) Fast Read NOT supported.
	*             1b: (1-4-4) Fast Read supported.
	*/
#define JESD216_PARAMETER1_VALUE1_SUPPORT_1_4_4_FAST_READ             1u

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 20          Supports (1-2-2) Fast Read
	*             Device supports single input opcode, dual input address, and dual output data Fast Read.
	*             0b: (1-2-2) Fast Read NOT supported.
	*             1b: (1-2-2) Fast Read supported.
	*/
#define JESD216_PARAMETER1_VALUE1_SUPPORT_1_2_2_FAST_READ             1u

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 19          Supports Double Transfer Rate (DTR) Clocking
	*             Indicates the device supports some type of double transfer rate clocking.
	*             0b: DTR NOT supported
	*             1b: DTR Clocking supported
	*/
#define JESD216_PARAMETER1_VALUE1_SUPPORT_DOUBLE_TRANSFER_RATE_CLOCKING   1u

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 18:17       Address Bytes
	*             Number of bytes used in addressing flash array read, write and erase:
	*             00b: 3-Byte only addressing
	*             01b: 3- or 4-Byte addressing (e.g. defaults to 3-Byte mode; enters 4-Byte mode on command)
	*             10b: 4-Byte only addressing
	*             11b: Reserved
	*             NOTE: All flash devices under 128 megabits in size should use 00b for this value for 24 bit addressing.
	*                   This field refers to the number of address bits/bytes that are clocked in for any instruction requiring an address in the flash array.
	*                   This field does not pertain to SFDP Header or Table accesses; all SFDP accesses use 3-byte addressing.
	*             Examples: Read, Fast Read, Write, 4 kilobyte Erase.
	*/
#define JESD216_PARAMETER1_VALUE1_ADDRESS_BYTE                        0u /* 3-byte only addressing */

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 16          Supports (1-1-2) Fast Read
	*             Device supports single input opcode & address and dual output data Fast Read with 8 bits of wait states.
	*             0b: (1-1-2) Fast Read NOT supported.
	*             1b: (1-1-2) Fast Read supported.
	*/
#define JESD216_PARAMETER1_VALUE1_SUPPORT_1_1_2_FAST_READ             1u

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 15:8        4 Kilobyte Erase Opcode
	*             NOTE: If 4 kilobyte erase is not supported, then enter 0xff.
	*/
#define JESD216_PARAMETER1_VALUE1_4KB_ERASE_OPCODE                    0x20u

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 7:5         Unused
	*             Contains 111b and can never be changed.
	*/

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 4           Write Enable Opcode Select for Writing to Volatile Status Register
	*             0: 0x50 is the Opcode to enable a status register write when bit 3 is set to 1.
	*             1: 0x06 is the Opcode to enable a status register write when bit 3 is set to 1.
	*             NOTE: If target flash status register is nonvolatile, then bits 3 and 4 must be set to 00b.
	*/
#define JESD216_PARAMETER1_VALUE1_WRITE_ENABLE_OPCODE_SELECT_FOR_WRITING_TO_VOLATILE_STATUS_REGISTER  0u /* 0x05 */

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 3           Write Enable Instruction Required for Writing to Volatile Status Register
	*             0: Target flash has nonvolatile status bit and does not require status register to be written on every power on to allow writes and erases.
	*             1: Target flash requires a 0x00 to be written to the status register in order to allow writes and erases.
	*             NOTE: If target flash register is nonvolatile, then bits 3 and 4 must be set to 00b.
	*/
#define JESD216_PARAMETER1_VALUE1_WRITE_ENABLE_INSTRUCTION_REQUIRED_FOR_WRITING_TO_VOLATILE_STATUS_REGISTER  0u /* non-volatile status register */

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 2           Write Granularity
	*             0: 1 Byte - Use this setting for single byte programmable devices or buffer programmable devices when the buffer is less than 64 bytes (32 Words).
	*             1: Use this setting for buffer programmable devices when the buffer size is 64 bytes (32 Words) or larger.
	*/
#define JESD216_PARAMETER1_VALUE1_WRITE_GRANULARITY                   1u /* 1-byte accessible */

/**
	* JEDEC Flash Parameter: 1st DWORD
	* Bits        Description
	* 1:0         Block/Sector Erase Sizes
	*             Identifies the erase granularity for all Flash Components.
	*             00b: Reserved
	*             01b: 4 kilobyte Erase
	*             10b: Reserved
	*             11b: Use this setting only if the 4 kilobyte erase is unavailable.
	*/
#define JESD216_PARAMETER1_VALUE1_BLOCK_OR_SECTOR_ERASE_SIZES         1u /* 4-kiB erase */




/**
	* ************************************************************
	* JEDEC Flash Parameter: 2nd DWORD
	*
	* Bits        Description
	*
	* 31:0        Flash Memory Density
	*             For densities 2 gigabits or less, bit-31 is set to 0b.
	*             The field 30:0 defines the size in bits.
	*             Example: 0x00ffffff = 16 megabits
	*             For densities 4 gigabits and above, bit-31 is set to 1b.
	*             The field 30:0 defines 'N' where the density is computed as 2^N bits (N must be >= 32).
	*             Example: 0x80000021 = 2^33 = 8 gigabits
	*/
#define JESD216_PARAMETER1_VALUE2_MEMORY_DENSITY_IN_BIT               (SPI_FLASH_SIZE_BITS - 1)




/**
	* ************************************************************
	* JEDEC Flash Parameters: 3rd DWORD
	*/

/**
	* JEDEC Flash Parameter: 3rd DWORD
	* Bits        Description
	* 31:24       (1-1-4) Fast Read Opcode
	*             Opcode for single input opcode & address and quad output data Fast Read.
	*/
#define JESD216_PARAMETER1_VALUE3_1_1_4_FAST_READ_OPCODE              0x6bu

/**
	* JEDEC Flash Parameter: 3rd DWORD
	* Bits        Description
	* 23:21       (1-1-4) Fast Read Number of Mode Bits
	*             This field will be 000b if Mode Bits are not supported,
	*             NOTE: This field should be counted in clocks not number of bits received by the serial flash.
	*                   The master drives the bus during "mode bits" cycles; the master tri-states the bus during "dummy" cycles.
	*             Example: If 4 mode bits are needed with a single input address phase instruction, this field would be 100b.
	*/
#define JESD216_PARAMETER1_VALUE3_1_1_4_FAST_READ_NUMBER_OF_MODE_BITS   0u /* not supported */

/**
	* JEDEC Flash Parameter: 3rd DWORD
	* Bits        Description
	* 20:16       (1-1-4) Fast Read Number of Wait states (dummy clocks) needed before valid output
	*             This field will be 00000b if wait states/dummy clocks are not supported.
	*             (The number of dummy clocks should be > 0 to avoid contention on bi-directional pins.)
	*             Example: If 8 bits are needed with a single input address phase instruction, this field would be 01000b.
	*/
#define JESD216_PARAMETER1_VALUE3_1_1_4_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT  8u

/**
	* JEDEC Flash Parameter: 3rd DWORD
	* Bits        Description
	* 15:8        (1-4-4) Fast Read Opcode
	*             Opcode for single input opcode, quad input address, and quad output data Fast Read.
	*/
#define JESD216_PARAMETER1_VALUE3_1_4_4_FAST_READ_OPCODE              0xebu

/**
	* JEDEC Flash Parameter: 3rd DWORD
	* Bits        Description
	* 7:5         Quad Input Address Quad Output (1-4-4) Fast Read Number of Mode Bits
	*             This field will be 000b if Mode bits are not supported,
	*             NOTE: This field should be counted in clocks not number of bits received by the serial flash.
	*                   The master drives the bus during "mode bits" cycles; the master tri-states the bus during "dummy" cycles.
	*             Example: If 8 mode bits are needed with a quad input address phase instruction, this field would be 010b.
	*/
#define JESD216_PARAMETER1_VALUE3_1_4_4_FAST_READ_NUMBER_OF_MODE_BITS   2u

/**
	* JEDEC Flash Parameter: 3rd DWORD
	* Bits        Description
	* 4:0         (1-4-4) Fast Read Number of Wait states (dummy clocks) needed before valid output
	*             This field will be 00000b if wait states/dummy clocks are not supported.
	*             (The number of dummy clocks should be > 0 to avoid contention on bi-directional pins.)
	*             Example: If 16 bits are needed with a quad input address phase instruction, this field would be 00100b.
	*/
#define JESD216_PARAMETER1_VALUE3_1_4_4_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT  4u



/**
	* ************************************************************
	* JEDEC Flash Parameters: 4th DWORD
	*/

/**
	* JEDEC Flash Parameter: 4th DWORD
	* Bits        Description
	* 31:24       (1-2-2) Fast Read Opcode
	*             Opcode for single input opcode, dual input address, and dual output data Fast Read.
	*/
#define JESD216_PARAMETER1_VALUE4_1_2_2_FAST_READ_OPCODE              0xbbu

/**
	* JEDEC Flash Parameter: 4th DWORD
	* Bits        Description
	* 23:21       (1-2-2) Fast Read Number of Mode Bits
	*             This field will be 000b if Mode bits are not supported,
	*             NOTE: This field should be counted in clocks not number of bits received by the serial flash.
	*                   The master drives the bus during "mode bits" cycles; the master tri-states the bus during "dummy" cycles.
	*             Example: If 8 mode bits are needed with a dual input address phase instruction, this field would be 100b.
	*/
#define JESD216_PARAMETER1_VALUE4_1_2_2_FAST_READ_NUMBER_OF_MODE_BITS   2u

/**
	* JEDEC Flash Parameter: 4th DWORD
	* Bits        Description
	* 20:16       (1-2-2) Fast Read Number of Wait states (dummy clocks) needed before valid output
	*             This field will be 00000b if wait states/dummy clocks are not supported.
	*             (The number of dummy clocks should be > 0 to avoid contention on bi-directional pins.)
	*             Example: If 8 bits are needed with a dual input address phase instruction, this field would be 00100b.
	*/
#define JESD216_PARAMETER1_VALUE4_1_2_2_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT  2u

/**
	* JEDEC Flash Parameter: 4th DWORD
	* Bits        Description
	* 15:8        (1-1-2) Fast Read Opcode
	*             Opcode for single input opcode & address and dual output data Fast Read.
	*             Note: The industry standard is 0x3b
	*/
#define JESD216_PARAMETER1_VALUE4_1_1_2_FAST_READ_OPCODE              0x3bu

/**
	* JEDEC Flash Parameter: 4th DWORD
	* Bits        Description
	* 7:5         (1-1-2) Fast Read Number of Mode Bits
	*             This field will be 000b if Mode bits are not supported,
	*             NOTE: This field should be counted in clocks not number of bits received by the serial flash.
	*             Example: If 4 mode bits are needed with a single input address phase instruction, this field would be 100b.
	*/
#define JESD216_PARAMETER1_VALUE4_1_1_2_FAST_READ_NUMBER_OF_MODE_BITS   0u

/**
	* JEDEC Flash Parameter: 4th DWORD
	* Bits        Description
	* 4:0         (1-1-2) Fast Read Number of Wait states (dummy clocks) needed before valid output
	*             This field should be programmed with 01000b for 8 clocks of dummy cycle.
	*             (The number of dummy clocks should be > 0 to avoid contention on bi-directional pins.)
	*             NOTE: For legacy reasons, if dummy clocks for this opcode is not 01000b,
	*                   then bit 16 of Flash Basic Properties offset 0 (Supports (1-1-2) Fast Read) must NOT be set to '1'.
	*/
#define JESD216_PARAMETER1_VALUE4_1_1_2_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT  8u



/**
	* ************************************************************
	* JEDEC Flash Parameters: 5th DWORD
	*/

/**
	* JEDEC Flash Parameter: 5th DWORD
	* Bits        Description
	* 31:5        Reserved. These bits default to all 1's
	*/

/**
	* JEDEC Flash Parameter: 5th DWORD
	* Bits        Description
	* 4           Supports (4-4-4) Fast Read
	*             Device supports Quad input opcode & address and quad output data Fast Read.
	*             0b: (4-4-4) Fast Read NOT supported.
	*             1b: (4-4-4) Fast Read supported.
	*/
#define JESD216_PARAMETER1_VALUE5_SUPPORT_4_4_4_FAST_READ             1u

/**
	* JEDEC Flash Parameter: 5th DWORD
	* Bits        Description
	* 3:1         Reserved. These bits default to all 1's
	*/

/**
	* JEDEC Flash Parameter: 5th DWORD
	* Bits        Description
	* 0           Supports (2-2-2) Fast Read
	*             Device supports dual input opcode & address and dual output data Fast Read.
	*             0b: (2-2-2) Fast Read NOT supported.
	*             1b: (2-2-2) Fast Read supported.
	*/
#define JESD216_PARAMETER1_VALUE5_SUPPORT_2_2_2_FAST_READ             0u



/**
	* ************************************************************
	* JEDEC Flash Parameters: 6th DWORD
	*/

/**
	* JEDEC Flash Parameter: 6th DWORD
	* Bits        Description
	* 31:24       (2-2-2) Fast Read Opcode
	*             Opcode for dual input opcode & address and dual output data Fast Read.
	*/
#define JESD216_PARAMETER1_VALUE6_2_2_2_FAST_READ_OPCODE              0u

/**
	* JEDEC Flash Parameter: 6th DWORD
	* Bits        Description
	* 23:21       (2-2-2) Fast Read Number of Mode Bits
	*             This field will be 000b if Mode bits are not supported,
	*             NOTE: This field should be counted in clocks not number of bits received by the serial flash.
	*                   The master drives the bus during "mode bits" cycles; the master tri-states the bus during "dummy" cycles.
	*             Example: If 4 mode bits are needed with a (2-2-2) Fast Read instruction, this field would be 010b.
	*/
#define JESD216_PARAMETER1_VALUE6_2_2_2_FAST_READ_NUMBER_OF_MODE_BITS   0u

/**
	* JEDEC Flash Parameter: 6th DWORD
	* Bits        Description
	* 20:16       (2-2-2) Fast Read Number of Wait states (dummy clocks) needed before valid output
	*             This field will be 00000b if wait states/dummy clocks are not supported.
	*             (The number of dummy clocks should be > 0 to avoid contention on bi-directional pins.)
	*             Example: If 8 bits are needed with a (2-2-2) Fast Read instruction, this field would be 00100b.
	*/
#define JESD216_PARAMETER1_VALUE6_2_2_2_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT  0u

/**
	* JEDEC Flash Parameter: 6th DWORD
	* Bits        Description
	* 15:0        Reserved. These bits default to all 1's
	*/




/**
	* ************************************************************
	* JEDEC Flash Parameters: 7th DWORD
	*/

/**
	* JEDEC Flash Parameter: 7th DWORD
	* Bits        Description
	* 31:24       (4-4-4) Fast Read Opcode
	*             Opcode for quad input opcode/address, quad output data Fast Read.
	*/
#define JESD216_PARAMETER1_VALUE7_4_4_4_FAST_READ_OPCODE              0xebu

/**
	* JEDEC Flash Parameter: 7th DWORD
	* Bits        Description
	* 23:21       (4-4-4) Fast Read Number of Mode Bits
	*             This field will be 000b if Mode bits are not supported,
	*             NOTE: This field should be counted in clocks not number of bits received by the serial flash.
	*                   The master drives the bus during "mode bits" cycles; the master tri-states the bus during "dummy" cycles.
	*             Example: If 8 mode bits are needed with a (4-4-4) Fast Read phase instruction, this field would be 010b.
	*/
#define JESD216_PARAMETER1_VALUE7_4_4_4_FAST_READ_NUMBER_OF_MODE_BITS   2u

/**
	* JEDEC Flash Parameter: 7th DWORD
	* Bits        Description
	* 20:16       (4-4-4) Fast Read Number of Wait states (dummy clocks) needed before valid output
	*             This field will be 00000b if wait states/dummy clocks are not supported.
	*             (The number of dummy clocks should be > 0 to avoid contention on bi-directional pins.)
	*             Example: If 16 bits are needed with a (4-4-4) Fast Read phase instruction, this field would be 00100b.
	*/
#define JESD216_PARAMETER1_VALUE7_4_4_4_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT  0u

/**
	* JEDEC Flash Parameter: 7th DWORD
	* Bits        Description
	* 15:0        Reserved. These bits default to all 1's
	*/



/**
	* ************************************************************
	* JEDEC Flash Parameters: 8th DWORD
	*/

/**
	* JEDEC Flash Parameter: 8th DWORD
	* Bits        Description
	* 31:24       Sector Type 2 Opcode
	*             Opcode used to erase the number of bytes specified by Sector Type 2 Size (bits 23-16).
	*/
#define JESD216_PARAMETER1_VALUE8_ERASE_SECTOR_TYPE_2_OPCODE          0x52u

/**
	* JEDEC Flash Parameter: 8th DWORD
	* Bits        Description
	* 23:16       Sector Type 2 Size:
	*             This field will be 0x00 if this sector type does not exist.
	*             NOTE: This field specifies 'N' and is used to calculate sector/block size = 2^N bytes
	*             Example: If the sector size is 32 kilobytes, this field would 0x0f.
	*/
#define JESD216_PARAMETER1_VALUE8_ERASE_SECTOR_TYPE_2_SIZE            0x0fu /* 32 kiB */

/**
	* JEDEC Flash Parameter: 8th DWORD
	* Bits        Description
	* 15:8        Sector Type 1 Opcode
	*             Opcode used to erase the number of bytes specified by Sector Type 1 Size (bits 7-0).
	*/
#define JESD216_PARAMETER1_VALUE8_ERASE_SECTOR_TYPE_1_OPCODE          0x20u

/**
	* JEDEC Flash Parameter: 8th DWORD
	* Bits        Description
	* 7:0         Sector Type 1 Size
	*             NOTE: This field specifies 'N' and is used to calculate sector/block size = 2^N bytes
	*             Example: If the sector size is 4 kilobytes, this field would 0x0c.
	*/
#define JESD216_PARAMETER1_VALUE8_ERASE_SECTOR_TYPE_1_SIZE            0x0cu /* 4 kiB */



/**
	* ************************************************************
	* JEDEC Flash Parameters: 9th DWORD
	*/

/**
	* JEDEC Flash Parameter: 9th DWORD
	* Bits        Description
	* 31:24       Sector Type 4 Opcode
	*             Opcode used to erase the number of bytes specified by Sector Type 4 Size (bits 23-16).
	*/
#define JESD216_PARAMETER1_VALUE9_ERASE_SECTOR_TYPE_4_OPCODE          0u

/**
	* JEDEC Flash Parameter: 9th DWORD
	* Bits        Description
	* 23:16       Sector Type 4 Size
	*             This field will be 0x00 if this sector type does not exist.
	*             NOTE: This field specifies 'N' and is used to calculate sector/block size = 2^N bytes
	*             Example: If the sector size is 256 kilobytes, this field would 0x12.
	*/
#define JESD216_PARAMETER1_VALUE9_ERASE_SECTOR_TYPE_4_SIZE            0u /* not supported */

/**
	* JEDEC Flash Parameter: 9th DWORD
	* Bits        Description
	* 15:8        Sector Type 3 Opcode
	*             Opcode used to erase the number of bytes specified by Sector Type 3 Size (bits 7-0).
	*/
#define JESD216_PARAMETER1_VALUE9_ERASE_SECTOR_TYPE_3_OPCODE          0xd8u

/**
	* JEDEC Flash Parameter: 9th DWORD
	* Bits        Description
	* 7:0         Sector Type 3 Size
	*             This field will be 0x00 if this sector type does not exist.
	*             NOTE: This field specifies 'N' and is used to calculate sector/block size = 2^N bytes
	*             Example: If the sector size is 64 kilobytes, this field would 0x10.
	*/
#define JESD216_PARAMETER1_VALUE9_ERASE_SECTOR_TYPE_3_SIZE            0x10u /* 64 kiB */




/**
	* initialize flash file
	* 1. create and allocate file if file don't exist
	* 2. truncate file if file size is smaller than SPI_FLASH_SIZE_BYTES
	*/
int  spi_flash_simulation_init(void);
void spi_flash_simulation_fini(void);

/* trx spi flash is handled here */
int spi_flash_trx_session(const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_stop);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(__SPI_FLASH_SIMULATION_POSIX_H__) */
