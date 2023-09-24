

all:get_xsel read_tty

get_xsel: get_xsel.cpp
	g++ get_xsel.cpp set_serial.h set_serial.cpp serial_form_data.h serial_form_data.cpp -o get_xsel -lpthread -g
read_tty: read_tty.cpp
	g++ -o read_tty read_tty.cpp  -lpthread -g


