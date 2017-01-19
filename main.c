/*
* main.c
* David Muriuki Karibe
*
* Licence: check LICENCE file
* Created July 2016
* Last updated January 2017
*/
#include"main.h"

void main(void){
	uint8_t test[1]={'\0'};

	init_system(); //initialize the KIT
	while(1){ //loop forever
		blink_RED_LED();
		uart_read(test,1);
		uart_write(test,1);
		uart_write('\r',1);
		uart_write('\n',1);
	}
}
