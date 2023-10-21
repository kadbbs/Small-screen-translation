#include "tf.h"

#include "../system.h"
#include "st7789.h"

void tf_init(void){

    spi3_init();
    set_cs(GPIOB,GPIO_Pin_6);


    u8 r1;      //
    u16 retry;  //
    u8 buf[4];
    u16 i;



    for(i=0;i<10;i++){
        SPI3_ReadWriteByte(0XFF);
    }

    retry=20;
    do
    {
        r1=SD_SendCmd(CMD0,0,0x95);
    }while((r1!=0X01) && retry--);

    TF_CS(1);

    Delay_Us(30);

    //tf_write_reg(CMD1);



    SD_SendCmd(CMD55,0,0X01);     //发送CMD55
    r1=SD_SendCmd(CMD41,0,0X01);  //发送CMD41
    if(r1<=1)
    {
        //SD_Type=SD_TYPE_V1;
        //  printf("SD_TYPE_V1");
        retry=0XFFFE;
        do //????IDLE??
        {
            SD_SendCmd(CMD55,0,0X01);   //??CMD55
            r1=SD_SendCmd(CMD41,0,0X01);//??CMD41
        }while(r1&&retry--);

    }
    uint8_t cid[16]={0};

    uint8_t csd[16]={0};
//    SD_GetCID(cid);
//
////    send_array(cid,200);
////    send_byte('\n');
//    SD_GetCSD(csd);
//    send_array(csd,16);
//    send_byte('\n');
//    lprintf("%s--%s",cid,csd);
//
//
//
//    Delay_Ms(100);
//
//
//    u32 size= SD_GetSectorCount();
//    send_byte('\n');
//    send_array((u8*)&size,4);
//    send_byte('\n');

    Delay_Ms(100);
   //SPI3_SetSpeed(SPI_BaudRatePrescaler_16);
    u8 bootdata[512];


//    SD_ReadSingleBlock(0x70000,bootdata);
//
//        if(r1==0){
//
//            send_array(bootdata,512);
//            //lprintf("%s",bootdata);
//            send_byte('\n');
//        }else{
//            send_array("error!",6);
//        }



}



u8 SD_GetCSD(u8 *csd_data)
{
  u8 r1;
  r1=SD_SendCmd(CMD9,0,0x01);
  if(r1==0)
  {
     r1=SD_RecvData(csd_data, 16);
   }
  SD_DisSelect();
  if(r1)return 1;
  else return 0;
}




u8 SD_GetCID(u8 *cid_data)
{
  u8 r1;
  r1=SD_SendCmd(CMD10,0,0x01);
  if(r1==0x00)
  {
    r1=SD_RecvData(cid_data,16);    //接收16个字节的数据
  }
  SD_DisSelect();
  if(r1)return 1;
  else return 0;
}


u32 SD_GetSectorCount(void)
{
    u8 csd[16];
    u32 Capacity_KB,Capacity_MB ;
    u8 n;
        u16 csize;
    //取CSD信息，如果期间出错，返回0
    if(SD_GetCSD(csd)!=0) return 0;
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;

        Capacity_KB= (u32)csize << (n - 10);//得到扇区数 ,这里的单位是KB
        Capacity_MB = Capacity_KB/1024;
    return Capacity_MB;
}


void tf_write_reg(u8 reg){
    TF_CS(0);



    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)!=SET);
    SPI_I2S_SendData(SPI3, (uint16_t)reg);
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)==SET);

    TF_CS(1);
}




void tf_read(u8 data){



}


void tf_write(u8 data){


}

