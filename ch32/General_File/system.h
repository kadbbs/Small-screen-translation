#ifndef __SYSTEM_H
#define __SYSTEM_H
#include "debug.h"
#include "driver/st7789.h"
void system_init();
void system_loop();
void GPIO_init();
void EXTI0_INT_INIT(void);
void led_loop(void);
void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM6_IRQHandler(void)   __attribute__((interrupt("WCH-Interrupt-fast")));
void spi1_init(void);
void spi3_init(void);
void set_cs(GPIO_TypeDef *GPIOx,u_int16_t pin_n);
u8 SPI3_ReadWriteByte(u8 byte);
u8 SPI1_ReadWriteByte(u8 byte);
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc);
u8 SD_RecvData(u8*buf,u16 len);
void open_uart1(uint32_t baudrate);
void send_byte(uint8_t data);
void send_array(uint8_t *data,int size);
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);
void Get_GBK_DZK(uint8_t *code,u32 file_t,coord *coord_lcd);
void SD_DisSelect(void);
//volatile uint16_t led_status=0;
#endif
