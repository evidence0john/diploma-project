#include "common_definitions.h"
#include "pwm_motor.h"
#include "command.h"

char status = AUTO;;

void cmd_slow()
{
	inc_TS();
	if(get_TS() == TS_MAX)dec_TS();
	UDR = get_TS();
}

void cmd_acc()
{
	dec_TS();
	if(get_TS() == TS_MIN)inc_TS();
	UDR = get_TS();
}

void cmd_STOP()
{
	status = STOP;
}

void cmd_AUTO()
{
	status = AUTO;
}

void cmd_CHDIR()
{
	change_DIR();
}

void cmd_MANI()
{
	status = MANI;
}

void cmd_STEP()
{
	status = STEP;
	if (get_PWMS() == 0){
			reset_PWMC();
	}
}

void _set_STEP(unsigned char para)
{
	unsigned int val = para * FACTOR;
	set_PWMS(val);
}

void _set_DIR(unsigned char para)
{
	if(para > 1)UDR = 'E';
	else
	{
		set_DIR(para);
	}
}
void _set_TSX(unsigned char para)
{
	set_TS(CMD_MUL_TSX * para);
}