//Edit this file to set value for your MCU
#include "common_definitions.h"
#include "pwm_motor.h"
#include "command.h"

extern char status;

void pcint_init(void)
{
	GIMSK |= (1 << INT0);
	GIMSK |= (1 << INT1);
}

int main(void)
{
	uart_init_9600(); 	//usart 9600 buad
	pcint_init();		//set up interrupt
	UDR = 35;			//acknowledge
	sei();				//enable interrupt
	motor_port_init();		//io port initial
	while(1)
	{
		//motor_forward();
		switch(status){
			case STOP :
				motor_stop();
			break;
			case AUTO :
				motor_action();
			break;
			case MANI :
				motor_action();
			break;
			case STEP:
				motor_step();
				if (get_PWMS() == 0)
				{
					status = STOP;
				}
			break;
		}
	}
}

unsigned char parac = 0;
unsigned char urc = 0;
unsigned char cmd_stat= 0;
unsigned char para_buf[16];
unsigned char *pbuf;

ISR(SIG_UART_RECV){
	urc = UDR;
	if (!parac)
	{
		switch(urc){
			case CMD_SLOW : cmd_slow();
			break;
			case CMD_ACC : cmd_acc();
			break;
			case CMD_STOP : cmd_STOP();
			break;
			case CMD_AUTO : cmd_AUTO();
			break;
			case CMD_MANI : cmd_MANI();
			break;
			case CMD_STEP : cmd_STEP();
			break;
			case CMD_CHDIR : cmd_CHDIR();
			break;
			case CMD_SET_DIR :
				parac = 1;
				pbuf = para_buf;
				cmd_stat = CMD_SET_DIR;
			break;
			case CMD_SET_TSX :
				parac = 1;
				pbuf = para_buf;
				cmd_stat = CMD_SET_TSX;
			break;
			case CMD_SET_STEP : 
				parac = 1;
				pbuf = para_buf;
				cmd_stat = CMD_SET_STEP;
			break;
			default:
			UDR = 'E';
		}
	}
	else
	{
		*pbuf++ = urc;
		parac--;
		if (!parac)
		{
			switch(cmd_stat)
			{
				case CMD_SET_STEP :
					_set_STEP(para_buf[0]); 
				break;
				case CMD_SET_DIR :
					_set_DIR(para_buf[0]);
				break;
				case CMD_SET_TSX :
					_set_TSX(para_buf[0]);
				break;
			}
		}
	}
}

ISR(SIG_INTERRUPT0)
{
	set_DIR(FORWARD);
	if (status != AUTO)
			status = STOP;
}

ISR(SIG_INTERRUPT1)
{
	set_DIR(BACK);
	if (status != AUTO)
			status = STOP;
}