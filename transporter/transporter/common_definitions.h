#define __AVR_LIBC_DEPRECATED_ENABLE__
#include <avr/io.h>

#define F_CPU 8000000UL

#include <avr/delay.h>
#include <avr/interrupt.h>

#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)    (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )

#define MPF 0	//motor ctrl pin forward
#define MPB 1	//motor ctrl pin forward

#define STOP 0
#define AUTO 1
#define MANI 2
#define STEP 3

#define TS_MIN -1
#define TS_MAX 10000

#define FACTOR 200 //For PWMS
#define CMD_MUL_TSX 10 //Set TSX

#define CMD_SLOW	0
#define CMD_ACC		1
#define CMD_STOP	2
#define CMD_AUTO	3
#define CMD_MANI	4
#define CMD_STEP	5
#define CMD_CHDIR	6
#define CMD_SET_DIR	7
#define CMD_SET_TSX	8
#define CMD_SET_STEP 9


#define BAUD_9600 9600

static void uart_init_9600(void){
	UBRRH = (F_CPU / BAUD_9600 / 16 - 1) / 256;
	UBRRL = (F_CPU / BAUD_9600 / 16 - 1) % 256;
	UCSRB |= (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);
}