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
void * get_word(void *arg){

    int fd=open("selection.txt",O_RDWR);

    while(1){
        memset(word_buf,0,30);
        int len=read(fd,word_buf,30);
        string tmp_word_str=word_buf;
        lseek(fd,0,SEEK_SET);

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


        sleep(1);
    }
    return NULL;

}


int main (int argc, char *argv[])
{
//    execl("/use/bin/bash","bash","/usr/bin/xclip","-o",">","selection.txt",NULL);
  // execl("/usr/bin/xclip", "xclip -o > selection.txt",NULL); 
   //execl("/home/bbs/code/cpp/learn/little_pro/test", "test","-o",">","selection.txt",NULL); 
   
    std::cout<<word_buf<<std::endl;
   //得到word
    pthread_t pd[10];

    pthread_create(&pd[1], NULL, get_word, NULL);
    pthread_create(&pd[0],NULL,select_xclip,NULL);
    //get_word(fd_word;word_buf,sizeof(word_buf));
    //
    //
    
    std::cout<<"get word is"<<word_buf<<std::endl;
   //打开串口
    int fd=open_port();
    int n;
    //设置serial参数
    set_arg(fd);
    //向串口写
    string word="";
    while(1){

        //cout<<"word="<<word<<endl;
       // cout<<"tmp="<<tmp_word_str<<endl;

        
        if(word.compare(tmp_word_str)!=0){

        cout<<"tmp="<<tmp_word_str<<endl;
            word.assign(tmp_word_str);
           // n=write(fd,word_buf,strlen(word_buf));
           cout<<"sendPacket doing"<<endl;
            sendPacket(fd,word_buf, strlen(word_buf));
            if(n<0){
                perror("write is errno!");
                exit(0);
            }

            std::cout<<"write size is"<<n<<std::endl;
        }

    }

    if(!n){

        std::cout<<"write to ttyf1 is error"<<n<<std::endl;
    }


    close(fd);

    std::cout<<"succes"<<std::endl;
    return 0;
}
