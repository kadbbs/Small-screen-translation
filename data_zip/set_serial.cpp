#include <iostream>
#include "set_serial.h"

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>

#include <string.h>


void set_arg(int fd){

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




}

