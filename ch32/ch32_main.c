/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include <string.h>
#include "../General_File/system.h"
#include "../General_File/driver/st7789.h"
#include "../General_File/driver/font.h"

/* Global typedef */

/* Global define */

/* Global Variable */


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
//void EXTI0_led(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//
//volatile uint16_t led_status=1;
//
//void EXTI0_led(){
//
//    EXTI_ClearITPendingBit(EXTI_Line0);
//    led_status=!led_status;
//
//    GPIO_WriteBit(GPIOE, GPIO_Pin_11, led_status);
//
//}
#define DATA_MAX 128
static char re_data[DATA_MAX];
static u_int8_t zh_data[DATA_MAX];


void send_byte(uint8_t data){
    USART_SendData(USART1,data);

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);


}



void open_uart1(uint32_t baudrate){
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);


        USART_InitStructure.USART_BaudRate = baudrate;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;


            USART_Init(USART1, &USART_InitStructure);
            USART_Cmd(USART1, ENABLE);

}


void  ReceivePacket(USART_TypeDef *USARTx,char * data,int *len1,char data_zh,int *len2,uint16_t size){
    int dataStatu=0;
    int dataStatu_=0;
    memset(data,0,size);
    memset(data_zh,size)
    int len_ch=size;
    int len_zh=size;
    int size_=size;
    while(1){

        //USART_ClearFlag(USARTx, USART_FLAG_ORE);
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET){
            uint16_t re=USART_ReceiveData(USARTx);

                    if(re=='@'){
                        dataStatu=1;
                    }else if(re=='$'){
                        dataStatu=0;
                        *len1=len_ch-size;
                        continue;
                    }else if(dataStatu){

                        if(size!=0){
                           *data=(char)re;

                            data++;
                            size--;
                         }else{
                             dataStatu=0;
                            *len1= len_ch;
                         }

                    }


                    if(re=='%'){
                        dataStatu_=1;
                      }else if(re=='$'){
                          dataStatu_=0;
                          *len2=len_zh-size_;
                          continue;
                            }else if(dataStatu_){
                                if(size_!=0){
                                   *data_zh=(char)re;

                                    data_zh++;
                                    size_--;
                                 }else{
                                     dataStatu_=0;
                                    *len2=len_zh;
                                 }

                            }


              }

    }
    dataStatu=0;
}

uint16_t  sendPacket(USART_TypeDef *USARTx,char *data,int size){

            send_byte('@');
            for(int i=0;i<size;i++){
                send_byte(data[i]);
                //USART_SendData(USARTx,data[i]);

                //lcd_show_char(i*32,0,(u8)re_data[i],32,0);


            }
            send_byte('$');

    return 1;

}


int main(void)
{

//    ST7789_GPIO_init();
//    Delay_Init();
//    uint8_t i=0;
//	while(1)
//    {   i=!i;
//	    LCD_LED(i);
//	    Delay_Ms(2000);
//	}


    open_uart1(115200);
//    u YELLOW=0xffff;
    lcd_init();
    lcd_clear(YELLOW);
    //lprintf("%c",'G');
    USART_SendData(USART1,'U');
    int len_ch=0;
    int len_zh=0;

        while(1)
        {
//            lcd_show_string(66, 0, 108, 12, "IoT Board", 24, 1);
////            lcd_show_chinese(72, 32, pandora_ch, 32, 3, 1);
//            delay_ms(1000);
//            lcd_clear(YELLOW);
//            delay_ms(1000);

            if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET){

                        uint16_t len=ReceivePacket(USART1,re_data,len_ch,&zh_data,&len_zh,DATA_MAX);

                        lprintf("%s",re_data);
                        //temp not send to pc
                        //show chinese



                        sendPacket(USART1, re_data, len);
                        sendPacket(USART1, zh_data, len);


                        //void lcd_show_char(u16 x, u16 y, u8 num, u8 size, u8 mode)
                    }

        }
}

