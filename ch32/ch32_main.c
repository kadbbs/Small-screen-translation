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
static uint8_t zh_data[DATA_MAX];


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


//void  ReceivePacket(USART_TypeDef *USARTx,char * data,uint16_t *len1,char *data_zh,uint16_t *len2,uint16_t size){
//    int dataStatu=0;
//    int dataStatu_=0;
//    memset(data,0,size);
//    memset(data_zh,0,size);
//    uint16_t len_ch=size;
//    uint16_t len_zh=size;
//    uint16_t size_=size;
//    while(1){
//
//        //USART_ClearFlag(USARTx, USART_FLAG_ORE);
//        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET){
//            uint16_t re=USART_ReceiveData(USARTx);
//
//                    if(re=='@'){
//                        dataStatu=1;
//                    }else if(re=='$'){
//                        dataStatu=0;
//                        *len1=len_ch-size;
//                        continue;
//                    }else if(dataStatu){
//
//                        if(size!=0){
//                           *data=(char)re;
//
//                            data++;
//                            size--;
//                         }else{
//                             dataStatu=0;
//                            *len1= len_ch;
//                            continue;
//                         }
//
//                    }
//
//
//                    if(re=='%'){
//                        dataStatu_=1;
//                      }else if(re=='$'){
//                          dataStatu_=0;
//                          *len2=len_zh-size_;
//                          return ;
//                      }else if(dataStatu_){
//                                if(size_!=0){
//                                   *data_zh=(char)re;
//
//                                    data_zh++;
//                                    size_--;
//                                 }else{
//                                     dataStatu_=0;
//                                    *len2=len_zh;
//                                    return ;
//                                 }
//
//                      }
//
//
//              }
//
//    }
//    dataStatu=0;
//}


uint16_t  sendPacket(USART_TypeDef *USARTx,char *data,int size){

            send_byte('@');
            for(int i=0;i<size;i++){
                send_byte(data[i]);
            }
            send_byte('$');
    return 1;

}

void  ReceivePacket(USART_TypeDef *USARTx,char * data,uint16_t *len1,char *data_zh,uint16_t *len2,uint16_t size){

    memset(data,0,*len1);
    memset(data_zh,0,*len2);
    int len_all=256;
    char data_all[len_all];
    memset(data_all,0,len_all);
    int i=0;
    int a,b=0,c,d;
    while(1){
        //USART_ClearFlag(USARTx, USART_FLAG_ORE);
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET){
            uint8_t re=USART_ReceiveData(USARTx);

            if(i<len_all){
                if(re=='@'){
                    a=i;
                    //send_byte(a);
                }else if(re=='$'){
                    if(b==0){
                        b=i;
                        //send_byte(b);
                    }else{
                        c=i;
                        //send_byte(c);
                    }

                }else if(re=='%'){
                    d=i;
                    data_all[i++]=re;

                    //send_byte(d);
                    break;
                }

                data_all[i++]=re;
            }else{
                send_byte('B');
                break;
            }
        }

    }

    //sendPacket(USART1, data_all, strlen(data_all));
    //send_byte(data_all[a+1]);
    i=0;
    for(int p=a+1;p<b;p++){

            //send_byte(data_all[p]);
            data[i++]=data_all[p];



    }
    //sendPacket(USART1, data, b-a-1);
    *len1=b-a-1;
    i=0;
    for(int p=c+1;p<d;p++){

            //send_byte(data_all[p]);
            data_zh[i++]=data_all[p];


    }
    //sendPacket(USART1, data_zh, d-c-1);
    *len2=d-c-1;

    return ;
}







int main(void)
{

//    ST7789_GPIO_init();
//    Delay_Init();
//    uint8_t i=0;
//  while(1)
//    {   i=!i;
//      LCD_LED(i);
//      Delay_Ms(2000);
//  }


    open_uart1(115200);

    lcd_init();
    lcd_clear(WHITE);

    USART_SendData(USART1,'U');
    uint16_t len_ch=0;
    uint16_t len_zh=0;
    lprintf("%s","trans powering!!");
    Delay_Ms(3000);
    tf_init();

    coord coord_lcd;


    uint8_t hanzi[2]={0};
        while(1)
        {
//            lcd_show_string(66, 0, 108, 12, "IoT Board", 24, 1);
////            lcd_show_chinese(72, 32, pandora_ch, 32, 3, 1);
//            delay_ms(1000);
//            lcd_clear(YELLOW);
//            delay_ms(1000);
            coord_lcd.x=0;
            coord_lcd.y=100;
            if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET){

                        ReceivePacket(USART1,re_data,&len_ch,zh_data,&len_zh,DATA_MAX);

                        lprintf("%s",re_data);
                        //temp not send to pc
                        //show chinese
                        for(int i=0;i<len_zh/2;i++){
                            hanzi[0]=zh_data[i*2];
                            hanzi[1]=zh_data[i*2+1];
                            Get_GBK_DZK(hanzi,0x88000,&coord_lcd);
                        }


//                        sendPacket(USART1, re_data, len_ch);
//                        sendPacket(USART1, zh_data, len_zh);


                        //void lcd_show_char(u16 x, u16 y, u8 num, u8 size, u8 mode)
                    }

        }
}

