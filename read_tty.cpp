#include <iostream>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>

#include <string.h>


#include <pthread.h>

using namespace std;
int open_port(void){


    int fd;


    fd = open("/dev/ttyACM0",O_RDWR|O_NOCTTY|O_NDELAY);

    if(fd==-1){
        perror("open_port:unable to open /dev/ttyACM0");
    }else{
        fcntl(fd,F_SETFL,0);
        std::cout<<"setfl"<<std::endl;
    }

    return fd;

}

void* send_serial(void * fd){
    char str[100]="write date";
    while(1){

//        int re=write(*(int *)fd, str, 10);
            int re=10;
        if(re<0){

            perror("write is errno");

        }
//        cout<<"write date"<<endl; 
        sleep(1);
    }

}



int main (int argc, char *argv[])
{
    
    int fd=open_port();
    struct termios options;

    //设置串口参数
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(fd, TCSANOW, &options);
    options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    options.c_cflag |= CS8;    /* Select 8 data bits */
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    //options.c_cflag &= ~CNEW_RTSCTS;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    pthread_t id;
//    int res=pthread_create(&id, NULL, send_serial, &fd);
    char buf[1024];
    memset(buf,0,sizeof(buf));

    while(1){

        int len=read(fd, buf, sizeof(buf));


        std::cout<<buf<<std::endl;

    
    }
    
    

   close(fd);

    return 0;
}
