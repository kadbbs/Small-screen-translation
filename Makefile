SRCS:=$(shell find ./* -type f |grep "\.cpp")

INCLUDE=.

main: main.o  set_serial.o serial_form_data.o 
	g++ -I${INCLUDE} $^ -o main -lpthread -g 
%.o:%.cpp
	g++ -c $^ -o $@
read_tty: read_tty.cpp
	g++ -o read_tty read_tty.cpp  -lpthread -g


clean:
	rm *.o


#	-I /home/bbs/anaconda3/envs/tf/include/python3.9 -l python3

#	怎么查看

# <0xe6><0x80><0x8e><0xe4><0xb9><0x88><0xe6><0x9f><0xa5><0xe7><0x9c><0x8b>
#
#
#
#
#
