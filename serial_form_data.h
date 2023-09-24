#ifndef SERIAL_FROM_DATA_H

#define SERIAL_FROM_DATA_H

int sendPacket(int fd,char *data,int size);

int receivePacket(int fd,char *data, int size);

int sendPacket_zh(int fd,const char *data,int size);

#endif // !DEBUG
