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
#include "serial_form_data.h"
#include "set_serial.h"



int main (int argc, char *argv[])
{

    int fd = open("/dev/ttyACM0",O_RDWR|O_NOCTTY|O_NDELAY);
    sendPacket(fd,"hello",5);
    return 0;
}
