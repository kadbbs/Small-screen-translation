#include "serial_form_data.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#include "set_serial.h"

using namespace std;





int sendPacket(int fd,char *data,int size){
    write(fd,"@",1);

    write(fd,data,size);
    write(fd,"$",1);

    return 1;
}


int sendPacket_zh(int fd,const char *data,int size){
    write(fd,"$",1);

    write(fd,data,size);
    write(fd,"%",1);

    return 1;
}


int receivePacket(int fd,char *data, int size){

    fcntl(fd,F_SETFL,FNDELAY);
    while(1){
        int re=read(fd,data,size);
        if(re<0){
            perror("receivePacket is error!");
        }else if(re==0){
            return 1;
            
        }else{
            //目前
        }

    }
    fcntl(fd, F_SETFL, FNDELAY);
    return 1;

}
