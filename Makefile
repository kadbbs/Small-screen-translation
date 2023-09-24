

all:get_xsel read_tty

get_xsel: get_xsel.cpp
	g++ get_xsel.cpp set_serial.h set_serial.cpp serial_form_data.h serial_form_data.cpp -o get_xsel -lpthread -g 
read_tty: read_tty.cpp
	g++ -o read_tty read_tty.cpp  -lpthread -g




#	-I /home/bbs/anaconda3/envs/tf/include/python3.9 -l python3

#	怎么查看

# <0xe6><0x80><0x8e><0xe4><0xb9><0x88><0xe6><0x9f><0xa5><0xe7><0x9c><0x8b>
#
#
#
#
#
