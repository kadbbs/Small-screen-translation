#ifndef __tf_h__
#define __tf_h__

#include <stdlib.h>
#include "debug.h"

#define TF_CS(n)   (n ? GPIO_WriteBit(GPIOB, GPIO_Pin_6, SET) : GPIO_WriteBit(GPIOB, GPIO_Pin_6, RESET))





/* 命令响应定义 define command's response */
#define R1 1
#define R1B 2
#define R2 3
#define R3 4


/******************************** 基本命令集 Basic command set **************************/
/* 复位SD 卡 Reset cards to idle state */
#define CMD0 0
#define CMD0_R R1

/* 读OCR寄存器 Read the OCR (MMC mode, do not use for SD cards) */
#define CMD1 1
#define CMD1_R R1

/* 读CSD寄存器 Card sends the CSD */
#define CMD9 9
#define CMD9_R R1

/* 读CID寄存器 Card sends CID */
#define CMD10 10
#define CMD10_R R1

/* 停止读多块时的数据传输 Stop a multiple block (stream) read/write operation */
#define CMD12 12
#define CMD12_R R1B

/* 读 Card_Status 寄存器 Get the addressed card's status register */
#define CMD13 13
#define CMD13_R R2

/***************************** 块读命令集 Block read commands **************************/

/* 设置块的长度 Set the block length */
#define CMD16 16
#define CMD16_R R1

/* 读单块 Read a single block */
#define CMD17 17
#define CMD17_R R1

/* 读多块,直至主机发送CMD12为止 Read multiple blocks until a CMD12 */
#define CMD18 18
#define CMD18_R R1

/***************************** 块写命令集 Block write commands *************************/
/* 写单块 Write a block of the size selected with CMD16 */
#define CMD24 24
#define CMD24_R R1

/* 写多块 Multiple block write until a CMD12 */
#define CMD25 25
#define CMD25_R R1

/* 写CSD寄存器 Program the programmable bits of the CSD */
#define CMD27 27
#define CMD27_R R1

/***************************** 写保护 Write protection *****************************/
/* Set the write protection bit of the addressed group */
#define CMD28 28
#define CMD28_R R1B

/* Clear the write protection bit of the addressed group */
#define CMD29 29
#define CMD29_R R1B

/* Ask the card for the status of the write protection bits */
#define CMD30 30
#define CMD30_R R1

/***************************** 擦除命令 Erase commands *******************************/
/* 设置擦除块的起始地址(只用于SD卡) Set the address of the first write block to be erased(only for SD) */
#define CMD32 32
#define CMD32_R R1

/* 设置擦除块的终止地址(只用于SD卡) Set the address of the last write block to be erased(only for SD) */
#define CMD33 33
#define CMD33_R R1

/* 设置擦除块的起始地址(只用于MMC卡) Set the address of the first write block to be erased(only for MMC) */
#define CMD35 35
#define CMD35_R R1

/* 设置擦除块的终止地址(只用于MMC卡) Set the address of the last write block to be erased(only for MMC) */
#define CMD36 36
#define CMD36_R R1

/* 擦除所选择的块 Erase the selected write blocks */
#define CMD38 38
#define CMD38_R R1B



#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标志字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF










#define CMD41   41








/***************************** 锁卡命令 Lock Card commands ***************************/
/* 设置/复位密码或上锁/解锁卡 Set/reset the password or lock/unlock the card */
#define CMD42 42
#define CMD42_R        R1B
/* Commands from 42 to 54, not defined here */

/***************************** 应用命令 Application-specific commands ****************/
/* 禁止下一个命令为应用命令  Flag that the next command is application-specific */
#define CMD55 55
#define CMD55_R R1

/* 应用命令的通用I/O  General purpose I/O for application-specific commands */
#define CMD56 56
#define CMD56_R R1

/* 读OCR寄存器  Read the OCR (SPI mode only) */
#define CMD58 58
#define CMD58_R R3

/* 使能或禁止 CRC Turn CRC on or off */
#define CMD59 59
#define CMD59_R R1

/***************************** 应用命令 Application-specific commands ***************/
/* 获取 SD Status寄存器 Get the SD card's status */
#define ACMD13 13
#define ACMD13_R R2

/* 得到已写入卡中的块的个数 Get the number of written write blocks (Minus errors ) */
#define ACMD22 22
#define ACMD22_R R1

/* 在写之前,设置预先擦除的块的个数 Set the number of write blocks to be pre-erased before writing */
#define ACMD23 23
#define ACMD23_R R1

/* 读取OCR寄存器 Get the card's OCR (SD mode) */
#define ACMD41 41
#define ACMD41_R R1

/* 连接/断开CD/DATA[3]引脚上的上拉电阻 Connect or disconnect the 50kOhm internal pull-up on CD/DAT[3] */
#define ACMD42 42
#define ACMD42_R R1

/* 读取SCR寄存器 Get the SD configuration register */
#define ACMD51 51
#define ACMD51_R R1
















u32 SD_GetSectorCount(void);
void tf_init(void);
void tf_write_reg(u8 reg);

void tf_read(u8 data);

void tf_write(u8 data);
u8 SD_GetCID(u8 *cid_data);
u8 SD_GetCSD(u8 *csd_data);
#endif
