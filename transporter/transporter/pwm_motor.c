#include "common_definitions.h"
#include "pwm_motor.h"

static unsigned int PWMS = DEFAULT_PWMS;
static unsigned int PWMC = DEFAULT_PWMS;
static unsigned int TS = 10;
static char DIR = FORWARD;
static char ERR = 0;

static void _error()
{
	ERR++;
}

void motor_port_init(){
	DDRB = 0xff;
	PORTB = 0x00;
}

int get_TS(){return TS;}
char get_DIR(){return DIR;}
unsigned int get_PWMS(){return PWMC;}
void inc_TS(){TS++;}
void dec_TS(){TS--;}
void set_TS(unsigned int val){TS = val;}
void set_DIR(char dir){DIR = dir;}
void set_PWMS(unsigned int val){PWMS = val;}

void change_DIR() //here assume you have defined FORWARD and BACK as 0 or 1
{
	if(DIR)DIR = 0;
		else DIR = 1;
}

void reset_PWMC(){PWMC = PWMS;}

void motor_step()
{
	if (PWMC)
	{	
		motor_action();
		PWMC--;
	}
}

void motor_action()
{
	switch(DIR)
	{
		case FORWARD :
		motor_forward();
		break;
		case BACK :
		motor_back();
		break;
		default:
		_error();
		break;
	}
}

void motor_forward()
{
	int TC = TS;
	Set_Bit(PORTB, MPF);
	_delay_us(THC);
	Clr_Bit(PORTB, MPF);
	while(TC--)
	_delay_us(TLS);
}

void motor_back()
{
	int TC = TS;
	Set_Bit(PORTB, MPB);
	_delay_us(THC);
	Clr_Bit(PORTB, MPB);
	while(TC--)
	_delay_us(TLS);
}

void motor_stop()
{
	Clr_Bit(PORTB, MPF);
	Clr_Bit(PORTB, MPB);
}