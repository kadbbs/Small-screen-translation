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
#include "serial_form_data.h"
#include "set_serial.h"

#include <string>
using namespace std;



int open_port(void){
    int fd;
    fd = open("/dev/ttyACM0",O_RDWR|O_NOCTTY|O_NDELAY);

    if(fd==-1){
        perror("open_port:unable to open /dev/tty17");
    }else{
        fcntl(fd,F_SETFL,0);

    }

    return fd;

}


char word_buf[30];
string tmp_word_str="^";
char word_chinse[100];
void * get_word(void *arg){

    int fd=open("selection.txt",O_RDWR);
    int fd_zh=open("translation.txt",O_RDWR);
    while(1){
        memset(word_buf,0,30);
        memset(word_chinse,0,100);
        int len=read(fd,word_buf,30);
        int len_zh=read(fd_zh,word_chinse,100);
        // string tmp_word_str=word_buf;
        lseek(fd,0,SEEK_SET);
        lseek(fd_zh,0,SEEK_SET);

        cout<<"fd_word= "<<word_buf<<endl;
        cout<<"tmp_word_str"<<tmp_word_str<<endl;
        if(len<0){

            perror("get_word read!");

        }


        sleep(3);

    }

    return NULL;

}


void *select_xclip(void *arg){

    
    while(1){

        system("/usr/bin/xclip -o > selection.txt");
        system("python connext_api.py");
        sleep(1);
    }
    return NULL;

}


int main (int argc, char *argv[])
{
   
    std::cout<<word_buf<<std::endl;
   //得到word
    pthread_t pd[10];

    pthread_create(&pd[1], NULL, get_word, NULL);
    pthread_create(&pd[0],NULL,select_xclip,NULL);
    //get_word(fd_word;word_buf,sizeof(word_buf));
    std::cout<<"get word is"<<word_buf<<std::endl;
   //打开串口
    int fd=open_port();
    //设置serial参数
    set_arg(fd);
    //向串口写
    string word="";
    while(1){

        cout<<"sendPacket doing"<<endl;
        sendPacket(fd,word_buf, strlen(word_buf));
        sendPacket_zh(fd,word_chinse, strlen(word_chinse));
        sleep(3);
    }
    close(fd);

    std::cout<<"succes"<<std::endl;
    return 0;
}
