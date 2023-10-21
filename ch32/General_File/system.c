#include <ch32v30x_gpio.h>
#include <ch32v30x_rcc.h>
#include <core_riscv.h>
#include "system.h"
#include "driver/st7789.h"
#include "debug.h"
#include "driver/tf.h"


////SPI1速度设置函数
////SpeedSet:0~7
////SPI速度=fAPB2/2^(SpeedSet+1)
////fAPB2时钟一般为84Mhz
//void SPI1_SetSpeed(u8 SpeedSet)
//{
//    SpeedSet&=0X07;         //限制范围
//    SPI1->CR1&=0XFFC7;
//    SPI1->CR1|=SpeedSet<<3; //设置SPI1速度
//    SPI1->CR1|=1<<6;        //SPI设备使能
//}



void spi1_init(void){
    //start rcc
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


    GPIO_InitTypeDef GPIO_InitStructure={0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitTypeDef spi_initStructure;
    spi_initStructure.SPI_Mode=SPI_Mode_Master;
    spi_initStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;
    spi_initStructure.SPI_CPHA=SPI_CPHA_1Edge;
    spi_initStructure.SPI_CPOL=SPI_CPOL_Low;
    spi_initStructure.SPI_CRCPolynomial=7;
    spi_initStructure.SPI_DataSize=SPI_DataSize_8b;
    spi_initStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    spi_initStructure.SPI_FirstBit=SPI_FirstBit_MSB;
    spi_initStructure.SPI_NSS=SPI_NSS_Soft;

    SPI_Init(SPI1, &spi_initStructure);
        LCD_CS(1);
    SPI_Cmd(SPI1,ENABLE);
//    LCD_CS(1);

}
void SPI1_SetSpeed(u8 SpeedSet)
{
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1,ENABLE);
}

void SPI3_SetSpeed(u16 SpeedSet)
{
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
    SPI_Init(SPI3, &SPI_InitStructure);
    SPI_Cmd(SPI3,ENABLE);
}


void spi3_init(void){
    //start rcc
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


    GPIO_InitTypeDef GPIO_InitStructure={0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);




    SPI_InitTypeDef spi_initStructure;
    spi_initStructure.SPI_Mode=SPI_Mode_Master;
    spi_initStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
    spi_initStructure.SPI_CPHA=SPI_CPHA_1Edge;
    spi_initStructure.SPI_CPOL=SPI_CPOL_Low;
    spi_initStructure.SPI_CRCPolynomial=7;
    spi_initStructure.SPI_DataSize=SPI_DataSize_8b;
    spi_initStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    spi_initStructure.SPI_FirstBit=SPI_FirstBit_MSB;
    spi_initStructure.SPI_NSS=SPI_NSS_Soft;

    SPI_Init(SPI3, &spi_initStructure);

    SPI_Cmd(SPI3,ENABLE);

}


void set_cs(GPIO_TypeDef *GPIOx,u_int16_t pin_n){
    GPIO_InitTypeDef GPIO_InitStructure={0};
    GPIO_InitStructure.GPIO_Pin = pin_n;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}


u8 SPI1_ReadWriteByte(u8 byte)
{
    u8 retry=0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
        {
        retry++;
        if(retry>200)return 0;
        }
    SPI_I2S_SendData(SPI1, byte); //Í¨¹ýÍâÉèSPIx·¢ËÍÒ»¸öÊý¾Ý
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:½ÓÊÜ»º´æ·Ç¿Õ±êÖ¾Î»
        {
        retry++;
        if(retry>200)return 0;
        }
    return SPI_I2S_ReceiveData(SPI1); //·µ»ØÍ¨¹ýSPIx×î½ü½ÓÊÕµÄÊý¾Ý

}

u8 SPI3_ReadWriteByte(u8 byte)
{
    u8 retry=0;
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
        {
        retry++;
        if(retry>200)return 0;
        }
    SPI_I2S_SendData(SPI3, byte); //Í¨¹ýÍâÉèSPIx·¢ËÍÒ»¸öÊý¾Ý
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)//¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:½ÓÊÜ»º´æ·Ç¿Õ±êÖ¾Î»
        {
        retry++;
        if(retry>200)return 0;
        }
    return SPI_I2S_ReceiveData(SPI3); //·µ»ØÍ¨¹ýSPIx×î½ü½ÓÊÕµÄÊý¾Ý

}

u8 SD_GetResponse(u8 Response)
{
  u16 Count=0xFFFF;         //等待次数
  while ((SPI3_ReadWriteByte(0xff)!=Response)&&Count)Count--;
  if (Count==0)return MSD_RESPONSE_FAILURE;
  else return MSD_RESPONSE_NO_ERROR;
}


u8 SD_RecvData(u8*buf,u16 len)
{
  if(SD_GetResponse(0xFE))return 1;
  while(len--)
  {
    *buf=SPI3_ReadWriteByte(0xff);
     buf++;
  }
  SPI3_ReadWriteByte(0xff);
  SPI3_ReadWriteByte(0xff);
  return 0;
}

void SD_DisSelect(void)
{
    TF_CS(1);
    SPI3_ReadWriteByte(0xff);
}
//等待卡准备好
//准备好了返回值0；其他错误代码
u8 SD_WaitReady(void)
{
  u32 t=0;
  do
  {
    if(SPI3_ReadWriteByte(0xff)==0XFF)return 0;//OK
    t++;
  }while(t<0XFFFFFF);
  return 1;
}
//选择SD卡并等待卡准备好
//返回值 0成功，1失败
u8 SD_Select(void)
{
    TF_CS(0);
  if(SD_WaitReady()==0)return 0;
  SD_DisSelect();
  return 1;
}



//向SD卡发送一个命令
//u8 cmd  命令
//u32 arg 命令参数
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
  u8 r1;
  u8 Retry=0;
  SD_DisSelect();
  if(SD_Select())return 0XFF;
    SPI3_ReadWriteByte(cmd | 0x40);
    SPI3_ReadWriteByte(arg >> 24);
    SPI3_ReadWriteByte(arg >> 16);
    SPI3_ReadWriteByte(arg >> 8);
    SPI3_ReadWriteByte(arg);
    SPI3_ReadWriteByte(crc);
  if(cmd==CMD12) SPI3_ReadWriteByte(0xff);//Skip a stuff byte when stop reading
    Retry=0X1F;
  do
   {
     r1=SPI3_ReadWriteByte(0xFF);
   }while((r1&0X80) && Retry--);
  return r1;
}


u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)
{
    u8 r1;
//    if(SD_Type!=SD_TYPE_V2HC)
    sector <<= 9;//转换为字节地址
    if(cnt==1)
    {
        r1=SD_SendCmd(CMD17,sector,0X01);//读命令
        if(r1==0)//指令发送成功
        {
            r1=SD_RecvData(buf,512);//接收512个字节
        }
    }else
    {
        r1=SD_SendCmd(CMD18,sector,0X01);//连续读命令
        do
        {
            r1=SD_RecvData(buf,512);//接收512个字节
            buf+=512;
        }while(--cnt && r1==0);
        SD_SendCmd(CMD12,0,0X01);   //发送停止命令
    }
    SD_DisSelect();//取消片选
    return r1;//
}


u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
  u8 r1;
  u32 offset=0x10000;
  //高速模式
  //SPI_SetSpeed(SPI_SPEED_HIGH);
  //if(SD_Type!=SD_TYPE_V2HC) //如果不是SDHC卡
  //{
    sector = (offset+sector); //512*sector即物理扇区的边界对其地址
  //}
   r1 = SD_SendCmd(CMD17, sector, 0x01);   //发送CMD17 读命令
   if(r1 != 0x00){
       send_byte('*');
       return r1;
   }

   r1 = SD_RecvData(buffer, 512);   //一个扇区为512字节
   if(r1 != 0)
     return r1;   //读数据出错
   else
     return 0;      //读取正确，返回0
}

void Get_GBK_DZK(uint8_t *code,u32 file_t,coord *coord_lcd){
    uint8_t GBKH,GBKL;
    uint32_t offset;
    uint8_t  character[32]={0};
    uint8_t* characterPtr;
    uint8_t disk[512];
    GBKH=*code;
    GBKL=*(code+1);

    if(GBKH>0XFE||GBKH<0X81){
        return;
    }
    GBKH-=0x81;
    GBKL-=0x40;
    //获取字符在bin文件中的偏移量
    offset=((uint32_t)192*GBKH+GBKL)*32;
    printf("offset=%x\n",offset);
    u32 num=offset%0x200;
    printf("num=%x\n",num);
    offset=offset-num;
    printf("offset-num=%x\n",offset);

//    if((-1 ==fseek(fd,offset,SEEK_SET))){
//        printf("Fseek error\r\n");
//    }
    //得到字符指针
    //fread(character,1,32,fd);
    //SD_ReadSingleBlock(,character);

    offset=file_t+offset;
    0xcf600;
    u8 r1;
    r1 = SD_SendCmd(CMD17, offset, 0x01);   //发送CMD17 读命令

    if(r1 != 0x00)  send_byte('*');

    r1 = SD_RecvData(disk, 512);   //read size 512

    //send_array(disk,512);

    characterPtr = disk+num;
    send_array(characterPtr,32);
    //send_byte(num);

    //显示字符


    u16 x=coord_lcd->x;
    u16 y=coord_lcd->y;


    int i,j;
    for(j=0; j<16; j++){
        x=coord_lcd->x;
        for(i=0; i<16; i++){
            if(*characterPtr &(0x80>>(i % 8))){
                //printf("*");
                lcd_draw_area(x, y, YELLOW,2);
                x=x+2;
                send_byte('1');
            }else{
                lcd_draw_area(x, y, BLUE,2);
                x=x+2;
                send_byte(' ');
            }
            if(i%8 == 7){
                characterPtr++;
            }
        }
        y=y+2;
        printf("\r\n");
    }



    if(x>320){
        coord_lcd->y=coord_lcd->y+32;
        coord_lcd->x=0;
    }else{
        coord_lcd->x=coord_lcd->x+32;
    }


}


#if 0
//打开uart1

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


void send_byte(uint8_t data){
    USART_SendData(USART1,data);

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);


}

#endif

void send_array(uint8_t *data,int size){
//    int i=0;
//    while(1){
//        if(data[i]!='\0'){
//            send_byte(data[i]);
//        }else{
//            send_byte('\n');
//            break;
//        }
//    }

    for(int i=0;i<size;i++){
        send_byte(data[i]);
    }


}
































void system_init(){

    //GPIO初始化

//    GPIO_SetBits(GPIOE, GPIO_Pin_0);
    //中断初始化
    EXTI0_INT_INIT();
    GPIO_init();
}

void GPIO_init(){
    GPIO_InitTypeDef GPIO_init_structure={0};

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
        GPIO_init_structure.GPIO_Pin=GPIO_Pin_11;
        GPIO_init_structure.GPIO_Mode=GPIO_Mode_Out_PP;
        GPIO_init_structure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOE,&GPIO_init_structure);
}

////中断初始化
//void EXTI0_INT_INIT(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure = {0};
//    EXTI_InitTypeDef EXTI_InitStructure = {0};
//    NVIC_InitTypeDef NVIC_InitStructure = {0};
//
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    /* GPIOA ----> EXTI_Line0 */
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
//
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}
//初始化定时器中断
void Interrupt_Init(void)
   {
      NVIC_InitTypeDef NVIC_InitStructure={0};
      NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
   }

//void EXTI0_INT_INIT(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure = {0};
//    EXTI_InitTypeDef EXTI_InitStructure = {0};
//    NVIC_InitTypeDef NVIC_InitStructure = {0};
//
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    /* GPIOA ----> EXTI_Line0 */
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
////    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
//
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}



void system_loop(){


//    GPIO_WriteBit(GPIOE,GPIO_Pin_11,1);
//    Delay_Ms(500);
            u8 i=0;
            u8 j=0;
            Delay_Init();

            Delay_Ms(250);
            GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i==0)?(i=Bit_SET):(i=Bit_RESET));
            Delay_Ms(250);
            GPIO_WriteBit(GPIOA, GPIO_Pin_1, (j==0)?(j=Bit_SET):(j=Bit_RESET));


}

void led_loop(){
    u16 i=0;
    u16 j=1;
            Delay_Ms(250);

            GPIO_WriteBit(GPIOE, GPIO_Pin_11, (i==0)?(i=Bit_SET):(i=Bit_RESET));
            Delay_Ms(250);
            GPIO_WriteBit(GPIOE, GPIO_Pin_12, (j==0)?(j=Bit_SET):(j=Bit_RESET));
}
volatile uint16_t LED_Status = 0; // 中断里使用的变量加 volatile 可当成全局变量

//外部中断处理函数
void EXTI0_IRQHandler(void)
{
    EXTI_ClearFlag(EXTI_Line0); // 置中断标志位为零
    LED_Status = !LED_Status ;  // 将 LED 状态值取反
    GPIO_WriteBit(GPIOE, GPIO_Pin_11, LED_Status); // 配置 PE11 (即 LED1) 状态
}

//定时器中断处理函数
//volatile uint16_t LED_Status = 0; // 中断里使用的变量加 volatile 可当成全局变量
   void TIM6_IRQHandler(void)
   {
      TIM_ClearFlag(TIM6, TIM_FLAG_Update);//清除标志位
      LED_Status = !LED_Status ;  // 将 LED 状态值取反
      GPIO_WriteBit(GPIOE, GPIO_Pin_11, LED_Status); // 配置 PE11 (即 LED1) 状态
   }

//定时器中断初始函数
//输    入        : arr：自动重装值，psc 预分频系数
void TIM6_Init( u16 arr, u16 psc)
  {
     TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

     RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM6, ENABLE );

     TIM_TimeBaseInitStructure.TIM_Period = arr;
     TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
     TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
     TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
     TIM_TimeBaseInit( TIM6, &TIM_TimeBaseInitStructure);

     TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
     TIM_ARRPreloadConfig( TIM6, ENABLE );
     TIM_Cmd( TIM6, ENABLE );
  }
uint32_t time =0;
typedef enum{
    key_up,
    key_downshake,
    key_down,
    key_upshake

}state_t;

typedef struct{
    state_t state;//四种按键状态
    uint8_t volatile cnt;//事件计时
    uint8_t auxiliary_flag;//状态机辅助位,判断双击，初始化为零
}value_t;

typedef enum{
    key_null,
    key_click,
    key_doucli,
    key_long

}key_event_all;


value_t value={key_up,0,0};
uint8_t key_event=key_null;//按键的事件

/*判断按键状态
 *
 * */
void key_scan(void){
    if(value.cnt>5){
        value.cnt=0;
        switch(value.state){
            case key_up:{
                if(value.auxiliary_flag){
                    if(value.cnt>500){
                        key_event=key_doucli;
                    }else{
                        key_event=key_click;
                    }

                    value.auxiliary_flag=0;

                }
                break;
            }
            case key_upshake:{
                break;
            }
            case key_down:{
                break;
            }
            case key_downshake:{
                break;
            }
        }

    }

}
/*按键形式不同，led 闪烁不同
 * 单击亮暗切换
 * 双击常量
 * 长按两灯（led1&led2）交替闪烁
 *
*/
void key_led_multi(){

    if(key_event==key_click){

    }else if(key_event==key_doucli){

    }else if(key_event==key_long){

    }
}

